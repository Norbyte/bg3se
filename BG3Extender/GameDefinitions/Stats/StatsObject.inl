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
		} else if (typeInfo->Name == GFS.strConditions
			|| typeInfo->Name == GFS.strTargetConditions
			|| typeInfo->Name == GFS.strUseConditions) {
			auto val = GetStaticSymbols().GetStats()->GetConditions(index);
			if (val) {
				return **val;
			}
		} else if (typeInfo->Name == GFS.strRollConditions) {
			auto rollConditions = GetRollConditions(attributeName);
			if (rollConditions && (*rollConditions)->Size == 1 && (**rollConditions)[0].Name == GFS.strDefault) {
				auto val = GetStaticSymbols().GetStats()->GetConditions((**rollConditions)[0].ConditionsId);
				if (val) {
					return **val;
				}
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
		if (RPGEnumeration::IsFlagType(typeInfo->Name)) {
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

		if (RPGEnumeration::IsFlagType(typeInfo->Name)) {
			auto index = IndexedProperties[attributeIndex];
			auto flags = GetStaticSymbols().GetStats()->GetInt64(index);
			ObjectSet<FixedString> flagSet;

			if (flags) {
				for (auto const& kv : typeInfo->Values) {
					if (**flags & (1ull << (kv.Value - 1))) {
						flagSet.Add(kv.Key);
					}
				}
			}

			return flagSet;
		}

		return {};
	}

	std::optional<Array<CRPGStats_Object::StatsFunctorInfo>*> CRPGStats_Object::GetStatsFunctors(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		if (typeInfo->Name != GFS.strStatsFunctors) {
			return {};
		}

		auto functors = StatsFunctors.Find(attributeName);
		if (functors) {
			return *functors;
		} else {
			return {};
		}
	}

	std::optional<Array<CRPGStats_Object::RollConditionInfo>*> CRPGStats_Object::GetRollConditions(FixedString const& attributeName)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			return {};
		}

		if (typeInfo->Name != GFS.strRollConditions) {
			return {};
		}

		auto conditions = RollConditions.Find(attributeName);
		if (conditions) {
			return *conditions;
		} else {
			return {};
		}
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
		if (typeInfo->Name == GFS.strFixedString
			|| typeInfo->Name == GFS.strStatusIDs) {
			int poolIdx{ -1 };
			auto fs = stats->GetOrCreateFixedString(poolIdx);
			if (fs != nullptr) {
				*fs = FixedString(value);
				IndexedProperties[attributeIndex] = poolIdx;
			}
		} else if (typeInfo->Name == GFS.strGuid) {
			auto guid = ParseGuidString(value);
			if (!guid) {
				OsiError("Couldn't set " << Name << "." << attributeName << ": Value (\"" << value << "\") is not a valid GUID");
				return false;
			}

			return SetGuid(attributeName, *guid);
		} else if (typeInfo->Name == GFS.strConditions
			|| typeInfo->Name == GFS.strTargetConditions
			|| typeInfo->Name == GFS.strUseConditions) {
			int poolIdx{ -1 };
			auto str = stats->GetOrCreateConditions(poolIdx);
			if (str != nullptr) {
				*str = value;
				IndexedProperties[attributeIndex] = poolIdx;
			}
		} else if (typeInfo->Name == GFS.strRollConditions) {
			if (*value) {
				int poolIdx{ -1 };
				auto str = stats->GetOrCreateConditions(poolIdx);
				if (str != nullptr) {
					*str = value;
					RollConditionInfo cond;
					cond.Name = GFS.strDefault;
					cond.ConditionsId = poolIdx;
					Array<RollConditionInfo> conditions;
					conditions.Add(cond);
					SetRollConditions(attributeName, conditions);
				}
			} else {
				SetRollConditions(attributeName, {});
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

	bool CRPGStats_Object::SetFloat(FixedString const& attributeName, std::optional<float> value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		auto stats = GetStaticSymbols().GetStats();
		if (typeInfo->Name == GFS.strConstantFloat) {
			if (value) {
				int poolIdx{ -1 };
				auto flt = stats->GetOrCreateFloat(poolIdx);
				if (flt != nullptr) {
					*flt = *value;
					IndexedProperties[attributeIndex] = poolIdx;
				}
			} else {
				IndexedProperties[attributeIndex] = -1;
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
		if (RPGEnumeration::IsFlagType(typeInfo->Name)) {
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

	bool CRPGStats_Object::SetGuid(FixedString const& attributeName, std::optional<UUID> value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		auto stats = GetStaticSymbols().GetStats();
		if (typeInfo->Name == GFS.strGuid) {
			if (value) {
				int poolIdx{ -1 };
				auto guid = stats->GetOrCreateGuid(poolIdx);
				if (guid != nullptr) {
					*guid = *value;
					IndexedProperties[attributeIndex] = poolIdx;
				}
			} else {
				IndexedProperties[attributeIndex] = -1;
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

		if (!RPGEnumeration::IsFlagType(typeInfo->Name)) {
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

			flags |= (1ll << (*flagValue - 1));
		}

		int poolIdx{ -1 };
		auto i64 = stats->GetOrCreateInt64(poolIdx);
		if (i64 != nullptr) {
			*i64 = flags;
			IndexedProperties[attributeIndex] = poolIdx;
		}

		return true;
	}

	bool CRPGStats_Object::SetStatsFunctors(FixedString const& attributeName, std::optional<Array<StatsFunctorInfo>> const& value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		if (typeInfo->Name != GFS.strStatsFunctors) {
			OsiError("Couldn't set " << Name << "." << attributeName << " to stats functors: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		if (value) {
			StatsFunctors.Set(attributeName, *value);
		} else {
			// FIXME - clearing stats functors not implemented!
		}
		return true;
	}

	bool CRPGStats_Object::SetRollConditions(FixedString const& attributeName, std::optional<Array<RollConditionInfo>> const& value)
	{
		int attributeIndex;
		auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
		if (typeInfo == nullptr) {
			OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
			return false;
		}

		if (typeInfo->Name != GFS.strRollConditions) {
			OsiError("Couldn't set " << Name << "." << attributeName << " to stats functors: Inappropriate type: " << typeInfo->Name);
			return false;
		}

		if (value) {
			RollConditions.Set(attributeName, *value);
		} else {
			// FIXME - clearing roll conditions not implemented!
		}

		return true;
	}

	// FIXME - ExtraProperties, ComboCategory
}
