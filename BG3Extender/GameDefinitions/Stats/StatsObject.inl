#include "stdafx.h"

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/Enumerations.h>
#include <ScriptHelpers.h>
#include "OsirisProxy.h"
#include <PropertyMaps.h>

namespace bg3se
{
	RPGEnumeration* CRPGStats_Object::GetAttributeInfo(FixedString const& attributeName, int& attributeIndex)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto objModifiers = stats->ModifierLists.Find(ModifierListIndex);
		if (objModifiers == nullptr) {
			return nullptr;
		}

		auto modifierInfo = objModifiers->GetAttributeInfo(attributeName, &attributeIndex);
		if (modifierInfo == nullptr) {
			return nullptr;
		}

		return stats->ModifierValueLists.Find(modifierInfo->RPGEnumerationIndex);
	}

	std::optional<STDString> CRPGStats_Object::GetString(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		auto index = IndexedProperties[attributeIndex];
		if (typeInfo->Name == GFS.strFixedString
			|| typeInfo->Name == GFS.strStatusIDs) {
			auto val = GetStaticSymbols().GetStats()->GetFixedString(index);
			if (val) {
				return (*val)->GetString();
			}
		} else if (typeInfo->Values.Count() > 0) {
			auto enumLabel = typeInfo->Values.FindByValue(index);
			if (enumLabel) {
				return enumLabel->GetString();
			}
		}

		return {};
	}

	std::optional<int> CRPGStats_Object::GetInt(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		if (typeInfo->Name == GFS.strConstantInt
			|| typeInfo->Values.Count() > 0) {
			return IndexedProperties[attributeIndex];
		} else {
			return {};
		}
	}

	std::optional<int> CRPGStats_Object::GetIntScaled(FixedString const& attributeName, int level)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto objModifiers = stats->ModifierLists.Find(ModifierListIndex);
		if (objModifiers == nullptr) {
			return {};
		}

		int attributeIndex;
		auto modifierInfo = objModifiers->GetAttributeInfo(attributeName, &attributeIndex);
		if (modifierInfo == nullptr) {
			return {};
		}

		auto levelMap = stats->LevelMaps.Find(modifierInfo->LevelMapIndex);
		auto value = IndexedProperties[attributeIndex];
		if (levelMap) {
			return (int32_t)levelMap->GetScaledValue(value, level);
		} else {
			return value;
		}
	}

	std::optional<float> CRPGStats_Object::GetFloat(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		auto index = IndexedProperties[attributeIndex];
		if (typeInfo->Name == GFS.strConstantFloat) {
			auto val = GetStaticSymbols().GetStats()->GetFloat(index);
			if (val) {
				return **val;
			}
		}

		return {};
	}

	std::optional<int64_t> CRPGStats_Object::GetInt64(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		auto index = IndexedProperties[attributeIndex];
		if (RPGStats::IsFlagType(typeInfo->Name)) {
			auto val = GetStaticSymbols().GetStats()->GetInt64(index);
			if (val) {
				return **val;
			}
		}

		return {};
	}

	std::optional<UUID> CRPGStats_Object::GetGuid(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		auto index = IndexedProperties[attributeIndex];
		if (typeInfo->Name == GFS.strGuid) {
			auto val = GetStaticSymbols().GetStats()->GetGuid(index);
			if (val) {
				return **val;
			}
		}

		return {};
	}


	std::optional<ObjectSet<FixedString>> CRPGStats_Object::GetFlags(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		if (RPGStats::IsFlagType(typeInfo->Name)) {
			auto index = IndexedProperties[attributeIndex];
			auto flags = GetStaticSymbols().GetStats()->GetInt64(index);
			ObjectSet<FixedString> flagSet;

			if (flags) {
				for (auto const& kv : typeInfo->Values) {
					if (**flags & (1ull << kv.Value)) {
						flagSet.Add(kv.Key);
					}
				}
			}

			return flagSet;
		}

		return {};
	}

	bool CRPGStats_Object::SetString(FixedString const& attributeName, const char * value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		auto stats = GetStaticSymbols().GetStats();
		if (typeInfo->Name == GFS.strFixedString) {
			int poolIdx{ -1 };
			auto fs = stats->GetOrCreateFixedString(poolIdx);
			if (fs != nullptr) {
				*fs = FixedString(value);
				IndexedProperties[attributeIndex] = poolIdx;
			}
		} else if (typeInfo->Values.Count() > 0) {
			auto enumIndex = typeInfo->Values.Find(FixedString(value));
			if (enumIndex != nullptr) {
				IndexedProperties[attributeIndex] = *enumIndex;
			} else {
				OsiError("Couldn't set " << Name << "." << attributeName << ": Value (\"" << value << "\") is not a valid enum label");
				return false;
			}
		} else {
			OsiError("Couldn't set " << Name << "." << attributeName << " to string value: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		return true;
	}

	bool CRPGStats_Object::SetInt(FixedString const& attributeName, int32_t value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		if (typeInfo->Name == GFS.strConstantInt) {
			IndexedProperties[attributeIndex] = value;
		} else if (typeInfo->Values.Count() > 0) {
			if (value >= 0 && value < (int)typeInfo->Values.Count()) {
				IndexedProperties[attributeIndex] = value;
			} else {
				OsiError("Couldn't set " << Name << "." << attributeName << ": Enum index (\"" << value << "\") out of range");
				return false;
			}
		} else {
			OsiError("Couldn't set " << Name << "." << attributeName << " to integer value: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		return true;
	}

	bool CRPGStats_Object::SetFloat(FixedString const& attributeName, float value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		auto stats = GetStaticSymbols().GetStats();
		if (typeInfo->Name == GFS.strConstantFloat) {
			int poolIdx{ -1 };
			auto flt = stats->GetOrCreateFloat(poolIdx);
			if (flt != nullptr) {
				*flt = value;
				IndexedProperties[attributeIndex] = poolIdx;
			}
		} else {
			OsiError("Couldn't set " << Name << "." << attributeName << " to float value: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		return true;
	}

	bool CRPGStats_Object::SetInt64(FixedString const& attributeName, int64_t value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		auto stats = GetStaticSymbols().GetStats();
		if (RPGStats::IsFlagType(typeInfo->Name)) {
			int poolIdx{ -1 };
			auto i64 = stats->GetOrCreateInt64(poolIdx);
			if (i64 != nullptr) {
				*i64 = value;
				IndexedProperties[attributeIndex] = poolIdx;
			}
		} else {
			OsiError("Couldn't set " << Name << "." << attributeName << " to int64 value: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		return true;
	}

	bool CRPGStats_Object::SetGuid(FixedString const& attributeName, UUID const& value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		auto stats = GetStaticSymbols().GetStats();
		if (typeInfo->Name == GFS.strGuid) {
			int poolIdx{ -1 };
			auto guid = stats->GetOrCreateGuid(poolIdx);
			if (guid != nullptr) {
				*guid = value;
				IndexedProperties[attributeIndex] = poolIdx;
			}
		} else {
			OsiError("Couldn't set " << Name << "." << attributeName << " to GUID value: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		return true;
	}

	bool CRPGStats_Object::SetFlags(FixedString const& attributeName, ObjectSet<STDString> const& value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		if (!RPGStats::IsFlagType(typeInfo->Name)) {
			OsiError("Couldn't set " << Name << "." << attributeName << " to flag array: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		auto stats = GetStaticSymbols().GetStats();
		int64_t flags{ 0 };
		for (auto const& flag : value) {
			auto flagValue = stats->EnumLabelToIndex(typeInfo->Name, flag.c_str());
			if (!flagValue) {
				OsiError("Couldn't set " << Name << "." << attributeName << ": Value (\"" << flag << "\") is not a valid enum label");
				return false;
			}

			flags |= (1ll << *flagValue);
		}

		int poolIdx{ -1 };
		auto i64 = stats->GetOrCreateInt64(poolIdx);
		if (i64 != nullptr) {
			*i64 = flags;
			IndexedProperties[attributeIndex] = poolIdx;
		}

		return true;
	}

	// FIXME - ExtraProperties, ComboCategory
}
