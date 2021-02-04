#include "stdafx.h"
#include "DataLibraries.h"
#include "ExtensionState.h"
#include "OsirisProxy.h"
#include <GameDefinitions/Symbols.h>
#include <string>
#include <functional>
#include <psapi.h>
#include <DbgHelp.h>

namespace bg3se
{
	void InitPropertyMaps();

#define HOOK_DEFN(name, sym, defn, hookType) decltype(LibraryManager::name) * decltype(LibraryManager::name)::gHook;
#include <GameDefinitions/EngineHooks.inl>
#undef HOOK_DEFN

	uint8_t CharToByte(char c)
	{
		if (c >= '0' && c <= '9') {
			return c - '0';
		}
		else if (c >= 'A' && c <= 'F') {
			return c - 'A' + 0x0A;
		}
		else if (c >= 'a' && c <= 'f') {
			return c - 'a' + 0x0A;
		}
		else {
			Fail("Invalid hexadecimal character");
		}
	}

	uint8_t HexByteToByte(char c1, char c2)
	{
		uint8_t hi = CharToByte(c1);
		uint8_t lo = CharToByte(c2);
		return (hi << 4) | lo;
	}

	void Pattern::FromString(std::string_view s)
	{
		if (s.size() % 3) Fail("Invalid pattern length");
		auto len = s.size() / 3;
		if (!len) Fail("Zero-length patterns not allowed");

		pattern_.clear();
		pattern_.reserve(len);

		char const * c = s.data();
		for (auto i = 0; i < len; i++) {
			PatternByte b;
			if (c[2] != ' ') Fail("Bytes must be separated by space");
			if (c[0] == 'X' && c[1] == 'X') {
				b.pattern = 0;
				b.mask = 0;
			}
			else {
				b.pattern = HexByteToByte(c[0], c[1]);
				b.mask = 0xff;
			}

			pattern_.push_back(b);
			c += 3;
		}

		if (pattern_[0].mask != 0xff) Fail("First byte of pattern must be an exact match");
	}

	void Pattern::FromRaw(const char * s)
	{
		auto len = strlen(s) + 1;
		pattern_.resize(len);
		for (auto i = 0; i < len; i++) {
			pattern_[i].pattern = (uint8_t)s[i];
			pattern_[i].mask = 0xFF;
		}
	}

	bool Pattern::MatchPattern(uint8_t const * start)
	{
		auto p = start;
		for (auto const & pattern : pattern_) {
			if ((*p++ & pattern.mask) != pattern.pattern) {
				return false;
			}
		}

		return true;
	}

	void Pattern::ScanPrefix1(uint8_t const * start, uint8_t const * end, std::function<std::optional<bool> (uint8_t const *)> callback, bool multiple)
	{
		uint8_t initial = pattern_[0].pattern;

		for (auto p = start; p < end; p++) {
			if (*p == initial) {
				if (MatchPattern(p)) {
					auto matched = callback(p);
					if (!multiple || (matched && *matched)) return;
				}
			}
		}
	}

	void Pattern::ScanPrefix2(uint8_t const * start, uint8_t const * end, std::function<std::optional<bool> (uint8_t const *)> callback, bool multiple)
	{
		uint16_t initial = pattern_[0].pattern
			| (pattern_[1].pattern << 8);

		for (auto p = start; p < end; p++) {
			if (*reinterpret_cast<uint16_t const *>(p) == initial) {
				if (MatchPattern(p)) {
					auto matched = callback(p);
					if (!multiple || (matched && *matched)) return;
				}
			}
		}
	}

	void Pattern::ScanPrefix4(uint8_t const * start, uint8_t const * end, std::function<std::optional<bool> (uint8_t const *)> callback, bool multiple)
	{
		uint32_t initial = pattern_[0].pattern
			| (pattern_[1].pattern << 8)
			| (pattern_[2].pattern << 16)
			| (pattern_[3].pattern << 24);

		for (auto p = start; p < end; p++) {
			if (*reinterpret_cast<uint32_t const *>(p) == initial) {
				if (MatchPattern(p)) {
					auto matched = callback(p);
					if (!multiple || (matched && *matched)) return;
				}
			}
		}
	}

	void Pattern::Scan(uint8_t const * start, size_t length, std::function<std::optional<bool> (uint8_t const *)> callback, bool multiple)
	{
		// Check prefix length
		auto prefixLength = 0;
		for (auto i = 0; i < pattern_.size(); i++) {
			if (pattern_[i].mask == 0xff) {
				prefixLength++;
			} else {
				break;
			}
		}

		auto end = start + length - pattern_.size();
		if (prefixLength >= 4) {
			ScanPrefix4(start, end, callback, multiple);
		} else if (prefixLength >= 2) {
			ScanPrefix2(start, end, callback, multiple);
		} else {
			ScanPrefix1(start, end, callback, multiple);
		}
	}

	bool LibraryManager::IsConstStringRef(uint8_t const * ref, char const * str) const
	{
		return
			ref >= moduleStart_ 
			&& ref < moduleStart_ + moduleSize_
			&& strcmp((char const *)ref, str) == 0;
	}

	bool LibraryManager::IsFixedStringRef(uint8_t const * ref, char const * str) const
	{
		// FIXME!
		throw std::runtime_error("NOT IMPLEMENTED!");
		return false;
		/*if (ref >= moduleStart_ && ref < moduleStart_ + moduleSize_) {
			auto fsx = (FixedString const *)ref;
			if (*fsx && strcmp(fsx->Str, str) == 0) {
				return true;
			}
		}

		return false;*/
	}

	bool LibraryManager::EvaluateSymbolCondition(SymbolMappingCondition const & cond, uint8_t const * match)
	{
		uint8_t const * ptr{ nullptr };
		switch (cond.Type) {
		case SymbolMappingCondition::kString:
			ptr = AsmResolveInstructionRef(match + cond.Offset);
			return ptr != nullptr && IsConstStringRef(ptr, cond.String);

		case SymbolMappingCondition::kFixedString:
			ptr = AsmResolveInstructionRef(match + cond.Offset);
			return ptr != nullptr && IsFixedStringRef(ptr, cond.String);

		case SymbolMappingCondition::kNone:
		default:
			return true;
		}
	}

	SymbolMappingResult LibraryManager::ExecSymbolMappingAction(SymbolMappingTarget const & target, uint8_t const * match)
	{
		if (target.Type == SymbolMappingTarget::kNone) return SymbolMappingResult::Success;

		uint8_t const * ptr{ nullptr };
		switch (target.Type) {
		case SymbolMappingTarget::kAbsolute:
			ptr = match + target.Offset;
			break;

		case SymbolMappingTarget::kIndirect:
			ptr = AsmResolveInstructionRef(match + target.Offset);
			break;

		default:
			break;
		}

		if (ptr != nullptr) {
			auto targetPtr = target.Target.Get();
			if (targetPtr != nullptr) {
				*targetPtr = const_cast<uint8_t *>(ptr);
			}

			if (target.NextSymbol != nullptr) {
				if (!MapSymbol(*target.NextSymbol, ptr, target.NextSymbolSeekSize)) {
					return SymbolMappingResult::Fail;
				}
			}

			if (target.Handler != nullptr) {
				return target.Handler(ptr);
			} else {
				return SymbolMappingResult::Success;
			}
		} else {
			ERR("Could not map match to symbol address while resolving '%s'", target.Name);
			return SymbolMappingResult::Fail;
		}
	}

	bool LibraryManager::MapSymbol(SymbolMappingData const & mapping, uint8_t const * customStart, std::size_t customSize)
	{
		if (mapping.Version.Type != SymbolVersion::None) {
			bool passed;
			if (mapping.Version.Type == SymbolVersion::Below) {
				passed = gameRevision_ < mapping.Version.Revision;
			} else {
				passed = gameRevision_ >= mapping.Version.Revision;
			}

			if (!passed) {
				// Ignore mappings that aren't supported by the current game version
				return true;
			}
		}

		Pattern p;
		p.FromString(mapping.Matcher);

		uint8_t const * memStart;
		std::size_t memSize;

		switch (mapping.Scope) {
		case SymbolMappingData::kBinary:
			memStart = moduleStart_;
			memSize = moduleSize_;
			break;

		case SymbolMappingData::kText:
			memStart = moduleTextStart_;
			memSize = moduleTextSize_;
			break;

		case SymbolMappingData::kCustom:
			memStart = customStart;
			memSize = customSize;
			break;

		default:
			memStart = nullptr;
			memSize = 0;
			break;
		}

		bool mapped = false;
		p.Scan(memStart, memSize, [this, &mapping, &mapped](const uint8_t * match) -> std::optional<bool> {
			if (EvaluateSymbolCondition(mapping.Conditions, match)) {
				auto action1 = ExecSymbolMappingAction(mapping.Target1, match);
				auto action2 = ExecSymbolMappingAction(mapping.Target2, match);
				auto action3 = ExecSymbolMappingAction(mapping.Target3, match);
				mapped = action1 == SymbolMappingResult::Success 
					&& action2 == SymbolMappingResult::Success
					&& action3 == SymbolMappingResult::Success;
				return action1 != SymbolMappingResult::TryNext 
					&& action2 != SymbolMappingResult::TryNext
					&& action3 != SymbolMappingResult::TryNext;
			} else {
				return {};
			}
		});

		if (!mapped && !(mapping.Flag & SymbolMappingData::kAllowFail)) {
			ERR("No match found for mapping '%s'", mapping.Name);
			InitFailed = true;
			if (mapping.Flag & SymbolMappingData::kCritical) {
				CriticalInitFailed = true;
			}
		}

		return mapped;
	}


	// Fetch the address referenced by an assembly instruction
	uint8_t const * AsmResolveInstructionRef(uint8_t const * insn)
	{
		// Call (4b operand) instruction
		if (insn[0] == 0xE8) {
			int32_t rel = *(int32_t const *)(insn + 1);
			return insn + rel + 5;
		}

		// MOV/LEA (4b operand) instruction
		if ((insn[0] == 0x48 || insn[0] == 0x4C) && (insn[1] == 0x8D || insn[1] == 0x8B)) {
			int32_t rel = *(int32_t const *)(insn + 3);
			return insn + rel + 7;
		}


		ERR("AsmResolveInstructionRef(): Not a supported CALL, MOV or LEA instruction at %p", insn);
		return nullptr;
	}

	void LibraryManager::FindTextSegment()
	{
		IMAGE_NT_HEADERS * pNtHdr = ImageNtHeader(const_cast<uint8_t *>(moduleStart_));
		IMAGE_SECTION_HEADER * pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);

		for (std::size_t i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++) {
			if (memcmp(pSectionHdr->Name, ".text", 6) == 0) {
				moduleTextStart_ = moduleStart_ + pSectionHdr->VirtualAddress;
				moduleTextSize_ = pSectionHdr->SizeOfRawData;
				return;
			}
		}

		// Fallback, if .text segment was not found
		moduleTextStart_ = moduleStart_;
		moduleTextSize_ = moduleSize_;
	}

	bool LibraryManager::FindLibraries(uint32_t gameRevision)
	{
		gameRevision_ = gameRevision;
		//memset(&GetStaticSymbols().CharStatsGetters, 0, sizeof(GetStaticSymbols().CharStatsGetters));

		if (FindBG3(moduleStart_, moduleSize_)) {
			FindTextSegment();
			MapAllSymbols(false);

			HMODULE crtBase = GetModuleHandle(L"ucrtbase.dll");
			auto crtAllocProc = GetProcAddress(crtBase, "malloc");
			auto crtFreeProc = GetProcAddress(crtBase, "free");

			GetStaticSymbols().CrtAlloc = (CrtAllocFunc)crtAllocProc;
			GetStaticSymbols().CrtFree = (CrtFreeFunc)crtFreeProc;

			if (crtAllocProc == nullptr || crtFreeProc == nullptr) {
				ERR("Could not find memory management functions");
				CriticalInitFailed = true;
			}

			FindServerGlobalsBG3();
			FindEoCGlobalsBG3();
			FindGlobalStringTableBG3();

			return !CriticalInitFailed;
		} else {
			ERR("LibraryManager::FindLibraries(): Unable to locate BG3 module.");
			return false;
		}
	}

	bool LibraryManager::PostStartupFindLibraries()
	{
		if (PostLoaded) {
			return !CriticalInitFailed;
		}

		auto initStart = std::chrono::high_resolution_clock::now();

		MapAllSymbols(true);

		if (!CriticalInitFailed) {
			GFS.Initialize();
			InitializeEnumerations();
			InitPropertyMaps();

			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());

			auto& sym = GetStaticSymbols();

			/*if (sym.AppInstance != nullptr && *sym.AppInstance != nullptr) {
				sym.App__OnInputEvent = (*sym.AppInstance)->__vftable->OnInputEvent;
			}

			if (sym.StatusHealVMT != nullptr) {
				sym.esv__Status__GetEnterChance = sym.StatusHealVMT->GetEnterChance;
				sym.esv__StatusHeal__Enter = sym.StatusHealVMT->Enter;
			}

			if (sym.StatusHitVMT != nullptr) {
				sym.esv__StatusHit__Enter = sym.StatusHitVMT->Enter;
			}*/

#define HOOK_DEFN(name, sym, defn, hookType) if (GetStaticSymbols().sym != nullptr) { name.Wrap(GetStaticSymbols().sym); }
#include <GameDefinitions/EngineHooks.inl>
#undef HOOK_DEFN

			//sym.CharStatsGetters.WrapAll();

			DetourTransactionCommit();
		}

		auto initEnd = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(initEnd - initStart).count();
		DEBUG("LibraryManager::PostStartupFindLibraries() took %d ms", ms);

		PostLoaded = true;
		return !CriticalInitFailed;
	}

	void LibraryManager::Cleanup()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

#define HOOK_DEFN(name, sym, defn, hookType) name.Unwrap();
#include <GameDefinitions/EngineHooks.inl>
#undef HOOK_DEFN

		DetourTransactionCommit();
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

	void LibraryManager::ShowStartupError(STDWString const & msg, bool wait, bool exitGame)
	{
		ERR(L"STARTUP ERROR: %s", msg.c_str());

		if (GetStaticSymbols().ecl__EoCClient == nullptr
			|| GetStaticSymbols().ecl__EoCClient__HandleError == nullptr
			|| GetStaticSymbols().EoCAlloc == nullptr) {
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
					MessageBoxW(NULL, msg.c_str(), L"Script Extender Error", 
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

	void LibraryManager::ShowStartupError(STDWString const & msg, bool exitGame)
	{
		if (!CanShowMessages()) return;

		GetStaticSymbols().ecl__EoCClient__HandleError(*GetStaticSymbols().ecl__EoCClient, &msg, exitGame, &msg);
	}

	void LibraryManager::ShowStartupMessage(STDWString const & msg, bool exitGame)
	{
		// Don't show progress if we're already in a loaded state, as it'll show a message box instead
		if (CanShowError()) return;

		ShowStartupError(msg, exitGame);
	}

	bool LibraryManager::CanShowMessages()
	{
		return GetStaticSymbols().GetClientState()
			&& GetStaticSymbols().ecl__EoCClient__HandleError != nullptr
			&& GetStaticSymbols().EoCAlloc != nullptr;
	}

	bool LibraryManager::CanShowError()
	{
		return true;
		if (!CanShowMessages()) return false;

		auto state = GetStaticSymbols().GetClientState();
		return state == ecl::GameState::Running
			|| state == ecl::GameState::Paused
			|| state == ecl::GameState::GameMasterPause
			|| state == ecl::GameState::Menu
			|| state == ecl::GameState::Lobby;
	}
}
