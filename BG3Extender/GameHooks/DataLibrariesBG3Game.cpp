#include "stdafx.h"
#include "resource.h"
#include <GameHooks/DataLibraries.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Symbols.h>
#include <functional>
#include <psapi.h>
#include <Extender/Shared/tinyxml2.h>

#if defined(OSI_EOCAPP)
namespace bg3se
{
	bool LibraryManager::FindBG3(uint8_t const * & start, size_t & size)
	{
		HMODULE hEoCApp = GetModuleHandleW(L"bg3.exe");
		if (hEoCApp == NULL) {
			hEoCApp = GetModuleHandleW(L"bg3_dx11.exe");
		}

		if (hEoCApp == NULL) {
			return false;
		}

		MODULEINFO moduleInfo;
		if (!GetModuleInformation(GetCurrentProcess(), hEoCApp, &moduleInfo, sizeof(moduleInfo))) {
			Fail(L"Could not get module info of bg3.exe/bg3_dx11.exe");
		}

		start = (uint8_t const *)moduleInfo.lpBaseOfDll;
		size = moduleInfo.SizeOfImage;
		return true;
	}

#define SYM_OFF(name) staticSymbolOffsets_.insert(std::make_pair(#name, (int)offsetof(StaticSymbols, name)))

	void LibraryManager::LoadMappings()
	{
		SYM_OFF(ls__FixedString__Create);
		SYM_OFF(ls__FixedString__GetString);
		SYM_OFF(ls__FixedString__IncRef);
		SYM_OFF(ls__FixedString__DecRef);

		SYM_OFF(ls__FileReader__ctor);
		SYM_OFF(ls__FileReader__dtor);
		SYM_OFF(PathRoots);

		SYM_OFF(ecl__EoCClient);
		SYM_OFF(esv__EoCServer);
		SYM_OFF(ecl__EoCClient__HandleError);

		SYM_OFF(ecl__GameStateEventManager__ExecuteGameStateChangedEvent);
		SYM_OFF(esv__GameStateEventManager__ExecuteGameStateChangedEvent);
		SYM_OFF(ecl__GameStateThreaded__GameStateWorker__DoWork);
		SYM_OFF(esv__GameStateThreaded__GameStateWorker__DoWork);
		SYM_OFF(ecl__GameStateMachine__Update);
		SYM_OFF(esv__GameStateMachine__Update);

		SYM_OFF(esv__SurfaceActionFactory);
		SYM_OFF(esv__SurfaceActionFactory__CreateAction);
		SYM_OFF(esv__SurfaceManager__AddAction);

		SYM_OFF(esv__StatusMachine__CreateStatus);
		SYM_OFF(esv__StatusMachine__ApplyStatus);

		SYM_OFF(eoc__DealDamageFunctor__ApplyDamage);

		SYM_OFF(eoc__StatsFunctorSet__ExecuteType1);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType2);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType3);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType4);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType5);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType6);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType7);
		SYM_OFF(eoc__StatsFunctorSet__ExecuteType8);

		SYM_OFF(ModuleSettingsHasCustomMods);
		SYM_OFF(Stats);
		SYM_OFF(esv__SavegameManager);
		SYM_OFF(AppInstance);

		SYM_OFF(EoCAlloc);
		SYM_OFF(EoCFree);
		SYM_OFF(CrtAlloc);
		SYM_OFF(CrtFree);

		SYM_OFF(ResourceDefns);
		SYM_OFF(ResourceMgr);

		auto xml = GetResource(IDR_BINARY_MAPPINGS);
		if (!xml) {
			ERR("Couldn't load binary mappings resource");
			return;
		}

		tinyxml2::XMLDocument doc;
		auto err = doc.Parse(xml->c_str(), xml->size());
		if (err != tinyxml2::XML_SUCCESS) {
			ERR("Couldn't parse binary mappings XML");
			return;
		}

		// TODO - check version specific mappings
		auto ele = doc.RootElement()->FirstChildElement("Mappings");
		while (ele != nullptr) {
			if (ele->BoolAttribute("Default")) {
				LoadMappingsNode(ele);
			}

			ele = ele->NextSiblingElement("Mappings");
		}
	}

	void LibraryManager::LoadMappingsNode(tinyxml2::XMLElement* mappings)
	{
		auto mapping = mappings->FirstChildElement("Mapping");
		while (mapping != nullptr) {
			SymbolMappingData sym;
			if (LoadMapping(mapping, sym)) {
				if (mappings_.find(sym.Name) != mappings_.end()) {
					ERR("Duplicate mapping name: %s", sym.Name.c_str());
				}

				mappings_.insert(std::make_pair(sym.Name, sym));
			}

			mapping = mapping->NextSiblingElement("Mapping");
		}
	}

	bool LibraryManager::LoadMapping(tinyxml2::XMLElement* mapping, SymbolMappingData& sym)
	{
		auto name = mapping->Attribute("Name");
		if (!name) {
			ERR("Mapping must have a name");
			return false;
		}

		sym.Name = name;

		auto scope = mapping->Attribute("Scope");
		if (scope == nullptr || strcmp(scope, "Text") == 0) {
			sym.Scope = SymbolMappingData::kText;
		} else if (strcmp(scope, "Binary") == 0) {
			sym.Scope = SymbolMappingData::kBinary;
		} else if (strcmp(scope, "Custom") == 0) {
			sym.Scope = SymbolMappingData::kCustom;
		} else {
			ERR("Unsupported scope type: %s", scope);
			return false;
		}

		if (mapping->BoolAttribute("Critical")) {
			sym.Flag |= SymbolMappingData::kCritical;
		}

		if (mapping->BoolAttribute("Deferred")) {
			sym.Flag |= SymbolMappingData::kDeferred;
		}

		if (mapping->BoolAttribute("AllowFail")) {
			sym.Flag |= SymbolMappingData::kAllowFail;
		}

		std::string pattern;
		auto patternText = mapping->FirstChild();
		while (patternText) {
			auto text = patternText->ToText();
			if (text) {
				pattern += text->Value();
			}

			patternText = patternText->NextSibling();
		}

		if (!sym.Pattern.FromString(pattern)) {
			ERR("Failed to parse pattern:\n%s", pattern.c_str());
			return false;
		}

		auto targetNode = mapping->FirstChildElement("Target");
		while (targetNode != nullptr) {
			SymbolMappingTarget target;
			if (LoadTarget(targetNode, target)) {
				sym.Targets.push_back(target);
			}
			targetNode = targetNode->NextSiblingElement("Target");
		}

		auto conditionNode = mapping->FirstChildElement("Condition");
		while (conditionNode != nullptr) {
			SymbolMappingCondition condition;
			if (!LoadCondition(conditionNode, condition)) {
				return false;
			}
			if (condition.Type == SymbolMappingCondition::kFixedString) {
				sym.Flag |= SymbolMappingData::kDeferred;
			}
			sym.Conditions.push_back(condition);
			conditionNode = conditionNode->NextSiblingElement("Condition");
		}

		return true;
	}

	bool LibraryManager::LoadTarget(tinyxml2::XMLElement* ele, SymbolMappingTarget& target)
	{
		auto name = ele->Attribute("Name");
		if (!name) name = "(Unnamed)";
		target.Name = name;

		auto type = ele->Attribute("Type");
		if (strcmp(type, "Absolute") == 0) {
			target.Type = SymbolMappingTarget::kAbsolute;
		} else if (strcmp(type, "Indirect") == 0) {
			target.Type = SymbolMappingTarget::kIndirect;
		} else {
			ERR("Unsupported action type: %s", type);
			return false;
		}

		target.Offset = ele->IntAttribute("Offset");

		auto staticSymbol = ele->Attribute("Symbol");
		if (staticSymbol) {
			auto symIt = staticSymbolOffsets_.find(staticSymbol);
			if (symIt != staticSymbolOffsets_.end()) {
				target.Target = StaticSymbolRef(symIt->second);
			} else {
				ERR("Target references nonexistent engine symbol: '%s'", staticSymbol);
				return false;
			}
		}

		auto nextSymbol = ele->Attribute("NextSymbol");
		if (nextSymbol) {
			target.NextSymbol = nextSymbol;
			if (mappings_.find(nextSymbol) == mappings_.end()) {
				ERR("Target references nonexistent symbol mapping: '%s'", nextSymbol);
				return false;
			}

			target.NextSymbolSeekSize = ele->IntAttribute("NextSymbolSeekSize");
			if (target.NextSymbolSeekSize <= 0) {
				ERR("Mappings using NextSymbol must also specify a valid NextSymbolSeekSize");
				return false;
			}
		}
		return true;
	}

	bool LibraryManager::LoadCondition(tinyxml2::XMLElement* ele, SymbolMappingCondition& condition)
	{
		auto type = ele->Attribute("Type");
		if (strcmp(type, "String") == 0) {
			condition.Type = SymbolMappingCondition::kString;
		} else if (strcmp(type, "FixedString") == 0) {
			condition.Type = SymbolMappingCondition::kFixedString;
		} else {
			ERR("Unsupported condition type: %s", type);
			return false;
		}

		condition.Offset = ele->IntAttribute("Offset");

		auto str = ele->Attribute("Value");
		if (!str) {
			ERR("String value missing from condition");
			return false;
		}
		condition.String = str;
		return true;
	}

	void LibraryManager::MapAllSymbols(bool deferred)
	{
		for (auto const& mapping : mappings_) {
			if (mapping.second.Scope != SymbolMappingData::kCustom
				&& ((deferred && (mapping.second.Flag & SymbolMappingData::kDeferred))
				|| (!deferred && !(mapping.second.Flag & SymbolMappingData::kDeferred)))) {
				MapSymbol(mapping.second, nullptr, 0);
			}
		}

	}

	void LibraryManager::FindSymbolNameRegistrations()
	{
		static uint8_t const sig1[] = {
			0x48, 0x8B, 0x0C, 0xC8,
			0x8B, 0x04, 0x0A,
			0x39, 0x05
		};

		static uint8_t const sig2[] = {
			0xC7, 0x44, 0x24, 0x28
		};

		static uint8_t const sig3[] = {
			0x48, 0x89, 0x44, 0x24, 0x20
		};

		uint8_t const * p = (uint8_t const *)moduleStart_;
		uint8_t const * moduleEnd = p + moduleSize_;

		auto& maps = GetStaticSymbols().SymbolIdToNameMaps;
		for (; p < moduleEnd - 100; p++) {
			if (*(uint64_t*)p == *(uint64_t*)&sig1[0]
				&& *(uint32_t*)(p + 22) == *(uint32_t*)&sig2[0]
				&& *(uint32_t*)(p + 44) == *(uint32_t*)&sig3[0]
				&& memcmp(p, sig1, sizeof(sig1)) == 0) {

				int32_t rel = *(int32_t*)(p + 23 - 5);
				uint8_t const* registrantObj = p + rel + 20 + 7 - 5;

				int32_t relName = *(int32_t*)(p + 45 - 5);
				uint8_t const* relNamePtr = p + relName + 42 + 7 - 5;

				maps.insert(std::make_pair((int32_t*)registrantObj, (char*)relNamePtr));
			}
		}
	}
}
#endif
