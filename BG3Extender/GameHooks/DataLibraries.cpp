#include "stdafx.h"
#include <GameHooks/DataLibraries.h>
#include <Extender/Shared/ExtensionState.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Symbols.h>
#include <string>
#include <functional>
#include <psapi.h>
#include <DbgHelp.h>

namespace bg3se
{
	LibraryManager::LibraryManager()
		: symbolMapper_(mappings_)
	{}

	bool LibraryManager::FindLibraries(uint32_t gameRevision)
	{
		RegisterSymbols();

		SymbolMappingLoader loader(mappings_);
		PreRegisterLibraries(loader);
		if (!loader.LoadBuiltinMappings()) {
			ERR("Failed to load symbol mapping table");
			CriticalInitFailed = true;
		}

#if defined(_DEBUG)
		if (!CriticalInitFailed) {
			// Debug check for symbol xml -> engine bindings
			for (auto const& sym : mappings_.StaticSymbols) {
				if (!sym.second.Bound) {
					WARN("No mapping exists for engine symbol: %s", sym.first.c_str());
				}
			}
		}
#endif

		if (!BindApp()) {
			ERR("LibraryManager::FindLibraries(): Unable to bind BG3 module.");
			return false;
		}

		RegisterLibraries(symbolMapper_);
		symbolMapper_.MapAllSymbols(false);

		CriticalInitFailed = CriticalInitFailed || symbolMapper_.HasFailedCriticalMappings();
		InitFailed = InitFailed || symbolMapper_.HasFailedMappings();

		if (!CriticalInitFailed) {
			ApplyCodePatch("KillLauncher");
		}

		return !CriticalInitFailed;
	}

	bool LibraryManager::PostStartupFindLibraries()
	{
		if (PostLoaded) {
			return !CriticalInitFailed;
		}

		auto initStart = std::chrono::high_resolution_clock::now();

		symbolMapper_.MapAllSymbols(true);

		if (!CriticalInitFailed) {
			GFS.Initialize();
			InitializeEnumerations();
		}

		auto initEnd = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(initEnd - initStart).count();
		DEBUG("LibraryManager::PostStartupFindLibraries() took %d ms", ms);

		PostLoaded = true;
		return !CriticalInitFailed;
	}

	void LibraryManager::ApplyCodePatches()
	{
		if (gExtender->GetConfig().EnableAchievements) {
			if (ApplyCodePatch("ls::ModuleSettings::IsModded")) {
				DEBUG("Modded achievements enabled.");
			} else {
				ERR("Couldn't enable modded achievements (symbol not mapped)");
			}
		}

		if (gExtender->GetConfig().DisableStoryMerge) {
			ApplyCodePatch("esv::SavegamePostVisit_DisableStoryMerge");
		}

		if (gExtender->GetConfig().DisableStoryPatching && GetStaticSymbols().ls__GlobalSwitches) {
			(*GetStaticSymbols().ls__GlobalSwitches)->DisableStoryPatching = true;
		}
	}


	bool LibraryManager::ApplyCodePatch(std::string const& mapping)
	{
		auto it = mappings_.Mappings.find(mapping);
		if (it == mappings_.Mappings.end()) {
			ERR("Cannot apply patch - no such mapping: '%s'", mapping.c_str());
			return false;
		}

		if (it->second.Patches.empty()) {
			ERR("Cannot apply patch - mapping '%s' contains no patch entries", mapping.c_str());
			return false;
		}

		bool hasChanges = false;
		for (auto& patch : it->second.Patches) {
			hasChanges = ApplyPatch(patch) || hasChanges;
		}

		return hasChanges;
	}


	bool LibraryManager::UndoCodePatch(std::string const& mapping)
	{
		auto it = mappings_.Mappings.find(mapping);
		if (it == mappings_.Mappings.end()) {
			ERR("Cannot undo patch - no such mapping: '%s'", mapping.c_str());
			return false;
		}

		bool hasChanges = false;
		for (auto& patch : it->second.Patches) {
			hasChanges = UndoPatch(patch) || hasChanges;
		}

		return hasChanges;
	}


	bool LibraryManager::ApplyPatch(SymbolMappings::Patch& patch)
	{
		if (patch.WasApplied) {
			return true;
		}
		
		if (patch.ResolvedRef == nullptr) {
			return false;
		}

		WriteAnchor code(patch.ResolvedRef, patch.Bytes.size());
		patch.OriginalBytes.resize(patch.Bytes.size());
		memcpy(patch.OriginalBytes.data(), patch.ResolvedRef, patch.OriginalBytes.size());
		memcpy(const_cast<uint8_t*>(patch.ResolvedRef), patch.Bytes.data(), patch.Bytes.size());
		patch.WasApplied = true;
		return true;
	}


	bool LibraryManager::UndoPatch(SymbolMappings::Patch& patch)
	{
		if (!patch.WasApplied) return false;

		WriteAnchor code(patch.ResolvedRef, patch.OriginalBytes.size());
		memcpy(const_cast<uint8_t*>(patch.ResolvedRef), patch.OriginalBytes.data(), patch.OriginalBytes.size());
		patch.WasApplied = false;
		return true;
	}

	bool LibraryManager::GetGameVersion(GameVersionInfo & version)
	{
		HMODULE hGameModule = GetModuleHandleW(L"bg3.exe");
		if (hGameModule == NULL) {
			hGameModule = GetModuleHandleW(L"bg3_dx11.exe");
		}

		if (hGameModule == NULL) {
			return false;
		}

		auto hResource = FindResource(hGameModule, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
		if (hResource == NULL) return false;
		auto dwSize = SizeofResource(hGameModule, hResource);
		auto hData = LoadResource(hGameModule, hResource);
		if (hData == NULL) return false;
		auto pRes = LockResource(hData);
		if (pRes == NULL) return false;

		auto pResCopy = LocalAlloc(LMEM_FIXED, dwSize);
		CopyMemory(pResCopy, pRes, dwSize);

		UINT verLength;
		VS_FIXEDFILEINFO * fixedFileInfo;
		if (VerQueryValue(pResCopy, L"\\", (LPVOID*)&fixedFileInfo, &verLength) != TRUE) return false;

		version.Major = HIWORD(fixedFileInfo->dwFileVersionMS);
		version.Minor = LOWORD(fixedFileInfo->dwFileVersionMS);
		version.Revision = HIWORD(fixedFileInfo->dwFileVersionLS);
		version.Build = LOWORD(fixedFileInfo->dwFileVersionLS);

		LocalFree(pResCopy);
		FreeResource(hData);
		return true;
	}

	void LibraryManager::ShowStartupError(STDString const & msg, bool wait, bool exitGame)
	{
		ERR("STARTUP ERROR: %s", msg.c_str());

		if (GetStaticSymbols().ecl__EoCClient == nullptr
			|| GetStaticSymbols().ecl__EoCClient__HandleError == nullptr
			|| GetStaticSymbols().ls__GlobalAllocator__Alloc == nullptr) {
			return;
		}

		if (wait) {
			std::thread messageThread([this, msg, exitGame]() {
				unsigned retries{ 0 };
				while (!CanShowError() && retries < 1200) {
					Sleep(100);
					retries++;
				}

				if (retries < 1200) {
					ShowStartupError(msg, exitGame);
				} else {
					MessageBoxA(NULL, msg.c_str(), "Script Extender Error", 
						MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND);
				}
			});
			messageThread.detach();
		} else {
			if (CanShowError()) {
				ShowStartupError(msg, exitGame);
			} else {
				ShowStartupMessage(msg, exitGame);
			}
		}
	}

	void LibraryManager::ShowStartupError(STDString const & msg, bool exitGame)
	{
		if (!CanShowMessages()) return;

		// Abuse a deprecated string key for displaying custom text
		TranslatedString ts;
		ts.Handle.Handle = FixedString("h11018635g3003g46c6g8013g4630abe55cad");
		ts.Handle.Version = 1;
		ts.ArgumentString.Handle = FixedString("ls::TranslatedStringRepository::s_HandleUnknown");

		// Create a new entry in the string repository text pool
		auto& texts = (*GetStaticSymbols().ls__gTranslatedStringRepository)->TranslatedStrings[0];
		auto tskRef = texts->Texts.Find(ts.Handle);
		auto str = GameAlloc<STDString>(msg);
		texts->Strings.Add(str);

		// Update reference to new string
		**tskRef = StringView(*str);

		GetStaticSymbols().ecl__EoCClient__HandleError(*GetStaticSymbols().ecl__EoCClient, ts, exitGame, ts);
	}

	void LibraryManager::ShowStartupMessage(STDString const & msg, bool exitGame)
	{
		// Don't show progress if we're already in a loaded state, as it'll show a message box instead
		if (CanShowError()) return;

		ShowStartupError(msg, exitGame);
	}

	bool LibraryManager::CanShowMessages()
	{
		return GetStaticSymbols().GetClientState()
			&& GetStaticSymbols().ecl__EoCClient__HandleError != nullptr
			&& GetStaticSymbols().ls__GlobalAllocator__Alloc != nullptr;
	}

	bool LibraryManager::CanShowError()
	{
		if (!CanShowMessages()) return false;

		auto state = GetStaticSymbols().GetClientState();
		return state == ecl::GameState::Running
			|| state == ecl::GameState::Paused
			|| state == ecl::GameState::Menu
			|| state == ecl::GameState::Lobby;
	}
}
