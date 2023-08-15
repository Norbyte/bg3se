#include <stdafx.h>
#include <GameHooks/SymbolMapper.h>
#include <GameDefinitions/Symbols.h>
#include <string>
#include <functional>
#include <psapi.h>
#include <DbgHelp.h>
#include <Extender/Shared/tinyxml2.h>
#include "resource.h"

#undef DEBUG_MAPPINGS

BEGIN_SE()

void** StaticSymbolRef::Get() const
{
	if (Offset != -1) {
		return (void **)((uint8_t *)&GetStaticSymbols() + Offset);
	} else {
		return TargetPtr;
	}
}

std::optional<uint8_t> CharToByte(char c)
{
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 0x0A;
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 0x0A;
	} else {
		ERR("Invalid hexadecimal character: %c", c);
		return {};
	}
}

std::optional<uint8_t> HexByteToByte(char c1, char c2)
{
	auto hi = CharToByte(c1);
	auto lo = CharToByte(c2);
	if (hi && lo) {
		return (*hi << 4) | *lo;
	} else {
		return {};
	}
}

bool Pattern::FromString(std::string_view s)
{
	pattern_.clear();
	pattern_.reserve(100);

	char const * c = s.data();
	while (*c) {
		if (*c == ' ' || *c == '\t' || *c == '\r' || *c == '\n') {
			c++;
			continue;
		}

		if (*c == '/') {
			while (*c && *c != '\r' && *c != '\n') c++;
			continue;
		}

		if (*c == '@') {
			c++;
			auto anchorStart = c;
			while (*c && std::isalnum(*c)) c++;
			if (c != anchorStart) {
				anchors_.insert(std::make_pair(std::string(anchorStart, c - anchorStart), (unsigned)pattern_.size()));
				c++;
			} else {
				ERR("Empty anchor name found");
				return false;
			}

			continue;
		}

		PatternByte b;
		if (!c[1] || !c[2] || !std::isspace(c[2])) {
			ERR("Bytes must be separated by whitespace");
			return false;
		}

		if (c[0] == '?' && c[1] == '?') {
			b.pattern = 0;
			b.mask = 0;
		} else {
			auto patByte = HexByteToByte(c[0], c[1]);
			if (!patByte) {
				return false;
			}

			b.pattern = *patByte;
			b.mask = 0xff;
		}

		pattern_.push_back(b);
		c += 3;
	}

	if (pattern_.empty()) {
		ERR("Zero-length patterns not allowed");
		return false;
	}

	if (pattern_[0].mask != 0xff) {
		ERR("First byte of pattern must be an exact match");
		return false;
	}

	return true;
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

bool Pattern::MatchPattern(uint8_t const * start) const
{
	auto p = start;
	for (auto const & pattern : pattern_) {
		if ((*p++ & pattern.mask) != pattern.pattern) {
			return false;
		}
	}

	return true;
}

void Pattern::ScanPrefix1(uint8_t const * start, uint8_t const * end, std::function<ScanAction (uint8_t const *)> callback) const
{
	uint8_t initial = pattern_[0].pattern;

	for (auto p = start; p < end; p++) {
		if (*p == initial) {
			if (MatchPattern(p)) {
				auto action = callback(p);
				if (action == ScanAction::Finish) return;
			}
		}
	}
}

void Pattern::ScanPrefix2(uint8_t const * start, uint8_t const * end, std::function<ScanAction (uint8_t const *)> callback) const
{
	uint16_t initial = pattern_[0].pattern
		| (pattern_[1].pattern << 8);

	for (auto p = start; p < end; p++) {
		if (*reinterpret_cast<uint16_t const *>(p) == initial) {
			if (MatchPattern(p)) {
				auto action = callback(p);
				if (action == ScanAction::Finish) return;
			}
		}
	}
}

void Pattern::ScanPrefix4(uint8_t const * start, uint8_t const * end, std::function<ScanAction (uint8_t const *)> callback) const
{
	uint32_t initial = pattern_[0].pattern
		| (pattern_[1].pattern << 8)
		| (pattern_[2].pattern << 16)
		| (pattern_[3].pattern << 24);

	for (auto p = start; p < end; p++) {
		if (*reinterpret_cast<uint32_t const *>(p) == initial) {
			if (MatchPattern(p)) {
				auto action = callback(p);
				if (action == ScanAction::Finish) return;
			}
		}
	}
}

void Pattern::Scan(uint8_t const * start, size_t length, std::function<ScanAction (uint8_t const *)> callback) const
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
		ScanPrefix4(start, end, callback);
	} else if (prefixLength >= 2) {
		ScanPrefix2(start, end, callback);
	} else {
		ScanPrefix1(start, end, callback);
	}
}

std::optional<uint32_t> Pattern::GetAnchor(char const* anchor) const
{
	auto it = anchors_.find(anchor);
	if (it != anchors_.end()) {
		return it->second;
	} else {
		return {};
	}
}

std::optional<int> GetIntAttribute(tinyxml2::XMLElement* ele, char const* name)
{
	char const* value{ nullptr };
	auto e = ele->QueryStringAttribute(name, &value);
	if (e != tinyxml2::XML_SUCCESS) {
		return {};
	}

	std::size_t readSize{ 0 };
	int parsed;
	try {
		parsed = std::stoi(value, &readSize, 0);
	} catch (std::exception const& e) {
		ERR("Invalid int value in XML attribute '%s': '%s' (%s)", name, value, e.what());
		return {};
	}

	if (readSize != strlen(value)) {
		ERR("Invalid int value in XML attribute '%s': '%s' (garbage found at end of string)", name, value);
	}

	return parsed;
}

std::optional<int> GetOffsetAttribute(tinyxml2::XMLElement* ele, Pattern const& pattern, char const* name)
{
	char const* value{ nullptr };
	auto e = ele->QueryStringAttribute(name, &value);
	if (e != tinyxml2::XML_SUCCESS) {
		return {};
	}

	if (*value == '@') {
		auto offset = pattern.GetAnchor(value + 1);
		if (!offset) {
			ERR("Invalid anchor reference in XML attribute '%s': '%s'", name, value);
			return {};
		} else {
			return (int)*offset;
		}
	}

	std::size_t readSize{ 0 };
	int parsed;
	try {
		parsed = std::stoi(value, &readSize, 0);
	} catch (std::exception const& e) {
		ERR("Invalid offset value in XML attribute '%s': '%s' (%s)", name, value, e.what());
		return {};
	}

	if (readSize != strlen(value)) {
		ERR("Invalid offset value in XML attribute '%s': '%s' (garbage found at end of string)", name, value);
	}

	return parsed;
}

void SymbolMappingLoader::AddKnownModule(std::string const& name)
{
	knownModules_.insert(name);
}

bool SymbolMappingLoader::LoadBuiltinMappings()
{
	auto xml = GetExeResource(IDR_BINARY_MAPPINGS);

	if (!xml) {
		ERR("Couldn't load binary mappings resource");
		return false;
	}

	tinyxml2::XMLDocument doc;
	auto err = doc.Parse(xml->c_str(), xml->size());
	if (err != tinyxml2::XML_SUCCESS) {
		ERR("Couldn't parse binary mappings XML");
		return false;
	}

	return LoadMappings(&doc);
}

bool SymbolMappingLoader::LoadMappings(tinyxml2::XMLDocument* doc)
{
	// TODO - check version specific mappings
	auto ele = doc->RootElement()->FirstChildElement("Mappings");
	while (ele != nullptr) {
		if (ele->BoolAttribute("Default")) {
			return LoadMappingsNode(ele);
		}

		ele = ele->NextSiblingElement("Mappings");
	}

	return false;
}

bool SymbolMappingLoader::LoadMappingsNode(tinyxml2::XMLElement* mappingsNode)
{
	auto mapping = mappingsNode->FirstChildElement();
	while (mapping != nullptr) {
		if (strcmp(mapping->Name(), "Mapping") == 0) {
			SymbolMappings::Mapping sym;
			if (LoadMapping(mapping, sym)) {
				if (mappings_.Mappings.find(sym.Name) != mappings_.Mappings.end()) {
					ERR("Duplicate mapping name: %s", sym.Name.c_str());
				}

				mappings_.Mappings.insert(std::make_pair(sym.Name, sym));
			} else {
				ERR("Failed to parse mapping '%s'; mapping discarded", sym.Name.c_str());
			}
		} else if (strcmp(mapping->Name(), "DllImport") == 0) {
			SymbolMappings::DllImport imp;
			if (LoadDllImport(mapping, imp)) {
				mappings_.DllImports.insert(std::make_pair(imp.Symbol, imp));
			}
		} else {
			ERR("Unknown element in <Mappings>: '%s'", mapping->Name());
		}

		mapping = mapping->NextSiblingElement();
	}

	return true;
}

bool SymbolMappingLoader::LoadMapping(tinyxml2::XMLElement* mapping, SymbolMappings::Mapping& sym)
{
	auto name = mapping->Attribute("Name");
	if (!name) {
		ERR("Mapping must have a name");
		return false;
	}

	sym.Name = name;

	auto scope = mapping->Attribute("Scope");
	if (scope == nullptr || strcmp(scope, "Text") == 0) {
		sym.Scope = SymbolMappings::MatchScope::kText;
	} else if (strcmp(scope, "Binary") == 0) {
		sym.Scope = SymbolMappings::MatchScope::kBinary;
	} else if (strcmp(scope, "Custom") == 0) {
		sym.Scope = SymbolMappings::MatchScope::kCustom;
	} else {
		ERR("Mapping uses unsupported scope type: %s", scope);
		return false;
	}

	if (sym.Scope != SymbolMappings::MatchScope::kCustom) {
		auto mod = mapping->Attribute("Module");
		if (mod == nullptr) {
			mod = "Main";
		}

		auto moduleInfo = knownModules_.find(mod);
		if (moduleInfo == knownModules_.end()) {
			ERR("Mapping references unknown module: %s", mod);
			return false;
		}

		sym.Module = mod;
	} else {
		sym.Module = "";
	}

	if (mapping->BoolAttribute("Critical")) {
		sym.Flag |= SymbolMappings::Mapping::kCritical;
	}

	if (mapping->BoolAttribute("Deferred")) {
		sym.Flag |= SymbolMappings::Mapping::kDeferred;
	}

	if (mapping->BoolAttribute("AllowFail")) {
		sym.Flag |= SymbolMappings::Mapping::kAllowFail;
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
		ERR("Failed to parse mapping pattern:\n%s", pattern.c_str());
		return false;
	}

	auto targetNode = mapping->FirstChildElement("Target");
	while (targetNode != nullptr) {
		SymbolMappings::Target target;
		if (LoadTarget(targetNode, sym.Pattern, target)) {
			sym.Targets.push_back(target);
		}
		targetNode = targetNode->NextSiblingElement("Target");
	}

	auto patchNode = mapping->FirstChildElement("Patch");
	while (patchNode != nullptr) {
		SymbolMappings::Patch patch;
		if (LoadPatch(patchNode, sym.Pattern, patch)) {
			sym.Patches.push_back(patch);
		}
		patchNode = patchNode->NextSiblingElement("Patch");
	}

	auto conditionNode = mapping->FirstChildElement("Condition");
	while (conditionNode != nullptr) {
		SymbolMappings::Condition condition;
		if (!LoadCondition(conditionNode, sym.Pattern, condition)) {
			return false;
		}
		if (condition.Type == SymbolMappings::MatchType::kFixedString
			|| condition.Type == SymbolMappings::MatchType::kFixedStringIndirect) {
			sym.Flag |= SymbolMappings::Mapping::kDeferred;
		}
		sym.Conditions.push_back(condition);
		conditionNode = conditionNode->NextSiblingElement("Condition");
	}

	if (sym.Targets.empty() && sym.Patches.empty()) {
		ERR("Mapping '%s' has no valid targets or patches!", sym.Name.c_str());
		return false;
	}

	return true;
}

bool SymbolMappingLoader::LoadDllImport(tinyxml2::XMLElement* mapping, SymbolMappings::DllImport& imp)
{
	auto staticSymbol = mapping->Attribute("Symbol");
	if (!staticSymbol) {
		ERR("DllImport must have a symbol");
		return false;
	}

	auto symIt = mappings_.StaticSymbols.find(staticSymbol);
	if (symIt != mappings_.StaticSymbols.end()) {
		imp.TargetRef = StaticSymbolRef(symIt->second.Offset);
		symIt->second.Bound = true;
	} else {
		ERR("DllImport references nonexistent engine symbol: '%s'", staticSymbol);
		return false;
	}

	imp.Symbol = staticSymbol;

	auto mod = mapping->Attribute("Module");
	if (!mod) {
		ERR("DllImport must have a module");
		return false;
	}

	imp.Module = mod;

	auto proc = mapping->Attribute("Proc");
	if (!proc) {
		ERR("DllImport must have a proc name");
		return false;
	}

	imp.Proc = proc;

	return true;
}

bool SymbolMappingLoader::LoadTarget(tinyxml2::XMLElement* ele, Pattern const& pattern, SymbolMappings::Target& target)
{
	auto name = ele->Attribute("Name");
	if (name) target.Name = name;

	if (!LoadReference(ele, pattern, target.Ref)) return false;

	auto staticSymbol = ele->Attribute("Symbol");
	if (staticSymbol) {
		auto symIt = mappings_.StaticSymbols.find(staticSymbol);
		if (symIt != mappings_.StaticSymbols.end()) {
			target.TargetRef = StaticSymbolRef(symIt->second.Offset);
			symIt->second.Bound = true;
		} else {
			ERR("Mapping target references nonexistent engine symbol: '%s'", staticSymbol);
			return false;
		}
	}

	auto nextSymbol = ele->Attribute("NextSymbol");
	if (nextSymbol) {
		target.NextSymbol = nextSymbol;
		if (mappings_.Mappings.find(nextSymbol) == mappings_.Mappings.end()) {
			ERR("Mapping target references nonexistent symbol mapping: '%s'", nextSymbol);
			return false;
		}

		auto nextSymbolSeekSize = GetIntAttribute(ele, "NextSymbolSeekSize");
		if (!nextSymbolSeekSize) {
			ERR("Mapping target has invalid NextSymbolSeekSize value.");
			return false;
		}

		target.NextSymbolSeekSize = *nextSymbolSeekSize;
		if (target.NextSymbolSeekSize <= 0) {
			ERR("Mapping NextSymbolSeekSize must be greater than 0");
			return false;
		}
	}

	auto engineCallback = ele->Attribute("EngineCallback");
	if (engineCallback) {
		target.EngineCallback = engineCallback;
	}

	if (target.Name.empty()) {
		if (staticSymbol) {
			target.Name = staticSymbol;
		} else {
			target.Name = "(Unnamed)";
		}
	}

	if (target.NextSymbol.empty() && target.EngineCallback.empty() && !target.TargetRef) {
		ERR("Target doesn't specify any actions!");
		return false;
	}

	return true;
}

bool SymbolMappingLoader::LoadPatchText(std::string_view s, std::vector<uint8_t>& bytes)
{
	char const * c = s.data();
	while (*c) {
		if (*c == ' ' || *c == '\t' || *c == '\r' || *c == '\n') {
			c++;
			continue;
		}

		if (*c == '/') {
			while (*c && *c != '\r' && *c != '\n') c++;
			continue;
		}

		if (!c[1] || !c[2] || !std::isspace(c[2])) {
			ERR("Bytes must be separated by whitespace");
			return false;
		}

		auto patByte = HexByteToByte(c[0], c[1]);
		if (!patByte) {
			return false;
		}

		bytes.push_back(*patByte);
		c += 3;
	}

	return true;
}

bool SymbolMappingLoader::LoadPatch(tinyxml2::XMLElement* ele, Pattern const& pattern, SymbolMappings::Patch& patch)
{
	if (!LoadReference(ele, pattern, patch.Ref)) return false;

	std::string replacement;
	auto patternText = ele->FirstChild();
	while (patternText) {
		auto text = patternText->ToText();
		if (text) {
			replacement += text->Value();
		}

		patternText = patternText->NextSibling();
	}

	if (!LoadPatchText(replacement, patch.Bytes)) {
		ERR("Failed to parse patch replacement:\n%s", replacement.c_str());
		return false;
	}

	return true;
}


bool SymbolMappingLoader::LoadReference(tinyxml2::XMLElement* ele, Pattern const& pattern, SymbolMappings::Reference& ref)
{
	auto type = ele->Attribute("Type");
	if (!type) {
		ERR("Mapping reference must have a Type property.");
		return false;
	}

	if (strcmp(type, "Absolute") == 0) {
		ref.Type = SymbolMappings::ReferenceType::kAbsolute;
	} else if (strcmp(type, "Indirect") == 0) {
		ref.Type = SymbolMappings::ReferenceType::kIndirect;
	} else {
		ERR("Unsupported mapping reference type: %s", type);
		return false;
	}

	auto offset = GetOffsetAttribute(ele, pattern, "Offset");
	if (!offset) {
		ERR("Mapping reference has invalid Offset value.");
		return false;
	}

	ref.Offset = *offset;
	return true;
}

bool SymbolMappingLoader::LoadCondition(tinyxml2::XMLElement* ele, Pattern const& pattern, SymbolMappings::Condition& condition)
{
	auto type = ele->Attribute("Type");
	if (strcmp(type, "String") == 0) {
		condition.Type = SymbolMappings::MatchType::kString;
	} else if (strcmp(type, "WString") == 0) {
		condition.Type = SymbolMappings::MatchType::kWString;
	} else if (strcmp(type, "FixedString") == 0) {
		condition.Type = SymbolMappings::MatchType::kFixedString;
	} else if (strcmp(type, "FixedStringIndirect") == 0) {
		condition.Type = SymbolMappings::MatchType::kFixedStringIndirect;
	} else {
		ERR("Unsupported mapping condition type: %s", type);
		return false;
	}

	auto offset = GetOffsetAttribute(ele, pattern, "Offset");
	if (offset) {
		condition.Offset = *offset;
	} else {
		return false;
	}

	auto str = ele->Attribute("Value");
	if (!str) {
		ERR("String value missing from condition");
		return false;
	}
	condition.String = str;

	if (condition.Type == SymbolMappings::MatchType::kWString) {
		condition.WString = FromStdUTF8(str).c_str();
	}

	return true;
}


bool SymbolMapper::IsValidModulePtr(uint8_t const * ref) const
{
	for (auto const& mod : modules_) {
		if (ref >= mod.second.ModuleStart && ref <= mod.second.ModuleStart + mod.second.ModuleSize) {
			return true;
		}
	}

	return false;
}

bool SymbolMapper::IsConstStringRef(uint8_t const * ref, char const * str) const
{
	return
		IsValidModulePtr(ref)
		&& strcmp((char const *)ref, str) == 0;
}

bool SymbolMapper::IsConstWStringRef(uint8_t const * ref, wchar_t const * str) const
{
	return
		IsValidModulePtr(ref)
		&& wcscmp((wchar_t const *)ref, str) == 0;
}

bool SymbolMapper::IsFixedStringRef(uint8_t const * ref, char const * str) const
{
	if (IsValidModulePtr(ref)) {
		auto fsx = (FixedString const *)ref;
		if (*fsx && strcmp(fsx->GetString(), str) == 0) {
			return true;
		}
	}

	return false;
}

bool SymbolMapper::IsIndirectFixedStringRef(uint8_t const * ref, char const * str) const
{
	if (IsValidModulePtr(ref)) {
		auto fsx = (FixedString const * const *)ref;
		if (*fsx && IsValidModulePtr((uint8_t*)*fsx)) {
			if (**fsx && strcmp((*fsx)->GetString(), str) == 0) {
				return true;
			}
		}
	}

	return false;
}

bool SymbolMapper::EvaluateSymbolCondition(SymbolMappings::Condition const & cond, uint8_t const * match)
{
	uint8_t const * ptr{ nullptr };
	switch (cond.Type) {
	case SymbolMappings::MatchType::kString:
		ptr = AsmResolveInstructionRef(match + cond.Offset);
		return ptr != nullptr && IsConstStringRef(ptr, cond.String.c_str());
		
	case SymbolMappings::MatchType::kWString:
		ptr = AsmResolveInstructionRef(match + cond.Offset);
		return ptr != nullptr && IsConstWStringRef(ptr, cond.WString.c_str());

	case SymbolMappings::MatchType::kFixedString:
		ptr = AsmResolveInstructionRef(match + cond.Offset);
		return ptr != nullptr && IsFixedStringRef(ptr, cond.String.c_str());

	case SymbolMappings::MatchType::kFixedStringIndirect:
		ptr = AsmResolveInstructionRef(match + cond.Offset);
		return ptr != nullptr && IsIndirectFixedStringRef(ptr, cond.String.c_str());

	case SymbolMappings::MatchType::kNone:
	default:
		return true;
	}
}

std::optional<uint8_t const*> SymbolMapper::ResolveRef(SymbolMappings::Reference const& ref, uint8_t const* match)
{
	if (match == nullptr) return {};

	switch (ref.Type) {
	case SymbolMappings::ReferenceType::kAbsolute:
		return match + ref.Offset;

	case SymbolMappings::ReferenceType::kIndirect:
	{
		auto ptr = AsmResolveInstructionRef(match + ref.Offset);
		return (ptr != nullptr) ? ptr : std::optional<uint8_t const*>();
	}

	default:
		ERR("Unhandled reference type");
		return {};
	}
}

SymbolMapper::MappingResult SymbolMapper::ExecSymbolMappingAction(SymbolMappings::Target const & target, uint8_t const * match)
{
	if (target.Ref.Type == SymbolMappings::ReferenceType::kNone) return MappingResult::Success;

	auto pptr = ResolveRef(target.Ref, match);

	if (!pptr) {
		ERR("Could not map match to symbol address while resolving '%s'", target.Name.c_str());
		return MappingResult::Fail;
	}

	auto ptr = *pptr;
	auto targetPtr = target.TargetRef.Get();
	if (targetPtr != nullptr) {
		*targetPtr = const_cast<uint8_t *>(ptr);
	}

	if (!target.NextSymbol.empty()) {
		if (!MapSymbol(mappings_.Mappings[target.NextSymbol], ptr, target.NextSymbolSeekSize)) {
			return MappingResult::Fail;
		}
	}

	if (!target.EngineCallback.empty()) {
		auto cb = engineCallbacks_.find(target.EngineCallback);
		if (cb == engineCallbacks_.end()) {
			ERR("Target references nonexistent engine callback: '%s'", target.EngineCallback.c_str());
			return MappingResult::Fail;
		} else {
			return cb->second(ptr);
		}
	}

	return MappingResult::Success;
}

bool SymbolMapper::UpdatePatchReference(SymbolMappings::Patch& patch, uint8_t const* match)
{
	if (patch.ResolvedRef != nullptr) {
		ERR("Trying to resolve reference multiple times?");
		return true;
	}

	auto ptr = ResolveRef(patch.Ref, match);
	if (!ptr) {
		ERR("Could not map patch to symbol address");
		return false;
	}

	patch.ResolvedRef = *ptr;
	return true;
}

bool SymbolMapper::MapSymbol(std::string const& mappingName, uint8_t const* customStart, std::size_t customSize)
{
	auto mapping = mappings_.Mappings.find(mappingName);
	if (mapping == mappings_.Mappings.end()) {
		ERR("Can't execute nonexistent symbol mapping '%s'", mappingName.c_str());
		return false;
	}

	return MapSymbol(mapping->second, customStart, customSize);
}

bool SymbolMapper::MapSymbol(SymbolMappings::Mapping & mapping, uint8_t const * customStart, std::size_t customSize)
{
	if (mapping.Version.Type != SymbolMappings::SymbolVersion::None) {
		bool passed;
		if (mapping.Version.Type == SymbolMappings::SymbolVersion::Below) {
			passed = gameRevision_ < mapping.Version.Revision;
		} else {
			passed = gameRevision_ >= mapping.Version.Revision;
		}

		if (!passed) {
			// Ignore mappings that aren't supported by the current game version
			return true;
		}
	}

	uint8_t const * memStart;
	std::size_t memSize;

	if (mapping.Scope == SymbolMappings::MatchScope::kBinary || mapping.Scope == SymbolMappings::MatchScope::kText) {
		auto modIt = modules_.find(mapping.Module);
		if (modIt == modules_.end()) {
			ERR("Missing module data for module '%s'", mapping.Module.c_str());
			return false;
		}

		if (mapping.Scope == SymbolMappings::MatchScope::kBinary) {
			memStart = modIt->second.ModuleStart;
			memSize = modIt->second.ModuleSize;
		} else {
			memStart = modIt->second.ModuleTextStart;
			memSize = modIt->second.ModuleTextSize;
		}
	} else if (mapping.Scope == SymbolMappings::MatchScope::kCustom) {
		if (customStart == nullptr) {
			ERR("Tried to apply custom mapping '%s' with a null custom range!", mapping.Name.c_str());
			return false;
		}

		memStart = customStart;
		memSize = customSize;
	} else {
		ERR("Unknown mapping scope!");
		return false;
	}

#if defined(DEBUG_MAPPINGS)
	DEBUG("Try mapping: %s [%p -> %p]", mapping.Name.c_str(), memStart, memStart + memSize);
#endif

	bool mapped = false,
		hasMatches = false;
	mapping.Pattern.Scan(memStart, memSize, [this, &mapping, &mapped, &hasMatches](const uint8_t * match) -> Pattern::ScanAction {
		bool matching{ true };
		for (auto const& condition : mapping.Conditions) {
			if (!EvaluateSymbolCondition(condition, match)) {
				matching = false;
				break;
			}
		}

		if (matching) {
#if defined(DEBUG_MAPPINGS)
			DEBUG("\tMatch: [%p]", match);
#endif

			hasMatches = true;
			auto patternAction{ Pattern::ScanAction::Finish };
			for (auto const& target : mapping.Targets) {
				auto action = ExecSymbolMappingAction(target, match);
#if defined(DEBUG_MAPPINGS)
				DEBUG("\tAction: %s", (action == MappingResult::Success) ? "Success"
					: ((action == MappingResult::TryNext) ? "TryNext" : "Fail"));
#endif

				if (!mapped) {
					mapped = (action == MappingResult::Success);
				}
				if (action == MappingResult::TryNext) {
					patternAction = Pattern::ScanAction::Continue;
				}
			}

			for (auto& patch : mapping.Patches) {
				if (UpdatePatchReference(patch, match)) {
					mapped = true;
				}
			}

			return patternAction;
		} else {
			return {};
		}
	});

	if (!mapped && !(mapping.Flag & SymbolMappings::Mapping::kAllowFail)) {
		if (!hasMatches) {
			ERR("No match found for mapping '%s' %s", mapping.Name.c_str(),
				(mapping.Flag & SymbolMappings::Mapping::kCritical) ? "[CRITICAL]" : "");
		} else {
			ERR("Target mapping action did not succeed for mapping '%s' %s", mapping.Name.c_str(),
				(mapping.Flag & SymbolMappings::Mapping::kCritical) ? "[CRITICAL]" : "");
		}

		hasFailedMappings_ = true;
		if (mapping.Flag & SymbolMappings::Mapping::kCritical) {
			hasFailedCriticalMappings_ = true;
		}
	}

	return mapped;
}

bool SymbolMapper::MapDllImport(SymbolMappings::DllImport const & imp)
{
	auto hMod = GetModuleHandleA(imp.Module.c_str());
	if (!hMod) {
		ERR("Symbol import references module that is not loaded: '%s'", imp.Module.c_str());
		return false;
	}
	
	auto proc = GetProcAddress(hMod, imp.Proc.c_str());
	if (!proc) {
		ERR("Symbol import references proc that is not exported: '%s'.'%s'", imp.Module.c_str(), imp.Proc.c_str());
		return false;
	}

	auto targetPtr = imp.TargetRef.Get();
	if (targetPtr != nullptr) {
		*targetPtr = reinterpret_cast<uint8_t*>(proc);
	}

	return true;
}


// Fetch the address referenced by an assembly instruction
uint8_t const * AsmResolveInstructionRef(uint8_t const * insn)
{
	// Call (4b operand) instruction
	if (insn[0] == 0xE8 || insn[0] == 0xE9) {
		int32_t rel = *(int32_t const *)(insn + 1);
		return insn + rel + 5;
	}

	// MOV to 32-bit register (4b operand) instruction
	if (insn[0] == 0x8B && (insn[1] < 0x20)) {
		int32_t rel = *(int32_t const *)(insn + 2);
		return insn + rel + 6;
	}

	// MOV/LEA (4b operand) instruction
	if ((insn[0] == 0x48 || insn[0] == 0x4C) && (insn[1] == 0x8D || insn[1] == 0x8B || insn[1] == 0x89)) {
		int32_t rel = *(int32_t const *)(insn + 3);
		return insn + rel + 7;
	}

	// CMP reg, [rip+xx] (4b operand) instruction
	if (insn[0] == 0x48 && insn[1] == 0x3B && (insn[2] & 0x0F) == 0x0D) {
		int32_t rel = *(int32_t const *)(insn + 3);
		return insn + rel + 7;
	}

	ERR("AsmResolveInstructionRef(): Not a supported CALL, MOV, LEA or CMP instruction at %p", insn);
	return nullptr;
}

bool SymbolMapper::AddModule(std::string const& name, std::wstring const& modName)
{
	auto hLib = LoadLibraryW(modName.c_str());
	if (hLib == NULL) {
		ERR("SymbolMapper::AddModule(): Couldn't load module '%s'", ToUTF8(modName).c_str());
		return false;
	}

	MODULEINFO moduleInfo;
	if (!GetModuleInformation(GetCurrentProcess(), hLib, &moduleInfo, sizeof(moduleInfo))) {
		ERR("SymbolMapper::AddModule(): Couldn't get module info for '%s'", ToUTF8(modName).c_str());
		return false;
	}

	ModuleInfo modInfo;
	modInfo.ModuleStart = (uint8_t const*)moduleInfo.lpBaseOfDll;
	modInfo.ModuleSize = moduleInfo.SizeOfImage;

	// Fallback, if .text segment was not found
	modInfo.ModuleTextStart = modInfo.ModuleStart;
	modInfo.ModuleTextSize = modInfo.ModuleSize;

	auto pNtHdr = ImageNtHeader(const_cast<uint8_t*>(modInfo.ModuleStart));
	auto pSectionHdr = (IMAGE_SECTION_HEADER*)(pNtHdr + 1);

	for (std::size_t i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++) {
		if (memcmp(pSectionHdr->Name, ".text", 6) == 0) {
			modInfo.ModuleTextStart = modInfo.ModuleStart + pSectionHdr->VirtualAddress;
			modInfo.ModuleTextSize = pSectionHdr->SizeOfRawData;
			break;
		}
	}

	modules_.insert(std::make_pair(name, modInfo));
	return true;
}

void SymbolMapper::AddEngineCallback(std::string const& name, std::function<MappingResult(uint8_t const*)> const& cb)
{
	engineCallbacks_.insert(std::make_pair(name, cb));
}

void SymbolMapper::MapAllSymbols(bool deferred)
{
	for (auto& mapping : mappings_.Mappings) {
		if (mapping.second.Scope != SymbolMappings::MatchScope::kCustom
			&& deferred == ((mapping.second.Flag & SymbolMappings::Mapping::kDeferred) != 0)) {
			MapSymbol(mapping.second, nullptr, 0);
		}
	}

	if (!deferred) {
		for (auto const& imp : mappings_.DllImports) {
			MapDllImport(imp.second);
		}
	}
}

END_SE()
