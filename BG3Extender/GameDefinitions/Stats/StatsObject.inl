#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/Stats/Stats.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(stats)

STDString* ConditionId::Get() const
{
	if (Id == -1) {
		return nullptr;
	}

	return &GetStaticSymbols().GetStats()->Conditions[Id];
}

Array<Functor*> FunctorGroup::GetFunctors() const
{
	return Functors->FunctorList;
}

RPGEnumeration* Object::GetAttributeInfo(FixedString const& attributeName, int& attributeIndex) const
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

	return stats->ModifierValueLists.Find(modifierInfo->EnumerationIndex);
}

std::optional<STDString> Object::GetString(FixedString const& attributeName) const
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
		if (rollConditions && (*rollConditions)->Size() == 1 && (**rollConditions)[0].Name == GFS.strDefault) {
			auto val = GetStaticSymbols().GetStats()->GetConditions((**rollConditions)[0].Conditions.Id);
			if (val) {
				return **val;
			}
		}
	} else if (typeInfo->Values.size() > 0) {
		auto enumLabel = typeInfo->Values.find_by_value(index);
		if (enumLabel != typeInfo->Values.end()) {
			return enumLabel.Key().GetString();
		}
	}

	return {};
}

std::optional<FixedString> Object::GetFixedString(FixedString const& attributeName) const
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
			return **val;
		}
	}

	return {};
}

std::optional<int> Object::GetInt(FixedString const& attributeName) const
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		return {};
	}

	if (typeInfo->Name == GFS.strConstantInt
		|| typeInfo->Values.size() > 0) {
		return IndexedProperties[attributeIndex];
	} else {
		return {};
	}
}

std::optional<float> Object::GetFloat(FixedString const& attributeName) const
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

std::optional<int64_t> Object::GetInt64(FixedString const& attributeName) const
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

std::optional<Guid> Object::GetGuid(FixedString const& attributeName) const
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

std::optional<TranslatedString> Object::GetTranslatedString(FixedString const& attributeName) const
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		return {};
	}

	auto index = IndexedProperties[attributeIndex];
	if (typeInfo->Name == GFS.strTranslatedString) {
		auto val = GetStaticSymbols().GetStats()->GetTranslatedString(index);
		if (val) {
			return **val;
		}
	}

	return {};
}

std::optional<Array<FixedString>> Object::GetFlags(FixedString const& attributeName) const
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		return {};
	}

	if (RPGEnumeration::IsFlagType(typeInfo->Name)) {
		auto index = IndexedProperties[attributeIndex];
		auto flags = GetStaticSymbols().GetStats()->GetInt64(index);
		Array<FixedString> flagSet;

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

std::optional<Array<FunctorGroup> const*> Object::GetFunctors(FixedString const& attributeName) const
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		return {};
	}

	if (typeInfo->Name != GFS.strStatsFunctors) {
		return {};
	}

	auto functors = Functors.try_get(attributeName);
	if (functors) {
		return functors;
	} else {
		return {};
	}
}

std::optional<Array<FunctorGroup>*> Object::GetFunctors(FixedString const& attributeName)
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		return {};
	}

	if (typeInfo->Name != GFS.strStatsFunctors) {
		return {};
	}

	auto functors = Functors.try_get(attributeName);
	if (functors) {
		return functors;
	} else {
		return {};
	}
}

std::optional<Array<Object::RollCondition> const*> Object::GetRollConditions(FixedString const& attributeName) const
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		return {};
	}

	if (typeInfo->Name != GFS.strRollConditions) {
		return {};
	}

	return RollConditions.try_get(attributeName);
}

std::optional<Array<Object::RollCondition>*> Object::GetRollConditions(FixedString const& attributeName)
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		return {};
	}

	if (typeInfo->Name != GFS.strRollConditions) {
		return {};
	}

	return RollConditions.try_get(attributeName);
}


bool Object::SetString(FixedString const& attributeName, const char * value)
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
		auto guid = Guid::ParseGuidString(value);
		if (!guid) {
			OsiError("Couldn't set " << Name << "." << attributeName << ": Value (\"" << value << "\") is not a valid GUID");
			return false;
		}

		return SetGuid(attributeName, *guid);

	} else if (typeInfo->Name == GFS.strConditions
		|| typeInfo->Name == GFS.strTargetConditions
		|| typeInfo->Name == GFS.strUseConditions) {
		auto index = stats->GetOrCreateConditions(value);
		IndexedProperties[attributeIndex] = index;

	} else if (typeInfo->Name == GFS.strTranslatedString) {
		int stringIdx{ -1 };
		auto ts = stats->GetOrCreateTranslatedString(stringIdx);
		*ts = TranslatedString::FromString(value);
		IndexedProperties[attributeIndex] = stringIdx;

	} else if (typeInfo->Name == GFS.strRollConditions) {
		if (*value) {
			auto index = stats->GetOrCreateConditions(value);
			if (index >= 0) {
				RollCondition cond;
				cond.Name = GFS.strDefault;
				cond.Conditions.Id = index;
				Array<RollCondition> conditions;
				conditions.Add(cond);
				SetRollConditions(attributeName, conditions);
			} else {
				SetRollConditions(attributeName, {});
			}
		} else {
			SetRollConditions(attributeName, {});
		}

	} else if (typeInfo->Values.size() > 0) {
		auto enumIndex = typeInfo->Values.find(FixedString(value));
		if (enumIndex != typeInfo->Values.end()) {
			IndexedProperties[attributeIndex] = enumIndex.Value();
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

bool Object::SetInt(FixedString const& attributeName, int32_t value)
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
		return false;
	}

	if (typeInfo->Name == GFS.strConstantInt) {
		IndexedProperties[attributeIndex] = value;
	} else if (typeInfo->Values.size() > 0) {
		if (value >= 0 && value < (int)typeInfo->Values.size()) {
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

bool Object::SetFloat(FixedString const& attributeName, std::optional<float> value)
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

bool Object::SetInt64(FixedString const& attributeName, int64_t value)
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

bool Object::SetGuid(FixedString const& attributeName, std::optional<Guid> value)
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

bool Object::SetTranslatedString(FixedString const& attributeName, std::optional<TranslatedString> value)
{
	int attributeIndex;
	auto typeInfo = GetAttributeInfo(attributeName, attributeIndex);
	if (typeInfo == nullptr) {
		OsiError("Couldn't fetch type info for " << Name << "." << attributeName);
		return false;
	}

	auto stats = GetStaticSymbols().GetStats();
	if (typeInfo->Name == GFS.strTranslatedString) {
		if (value) {
			int poolIdx{ -1 };
			auto ts = stats->GetOrCreateTranslatedString(poolIdx);
			if (ts != nullptr) {
				*ts = *value;
				IndexedProperties[attributeIndex] = poolIdx;
			}
		} else {
			IndexedProperties[attributeIndex] = -1;
		}
	} else {
		OsiError("Couldn't set " << Name << "." << attributeName << " to TranslatedString value: Inappropriate type: " << typeInfo->Name);
		return false;
	}

	return true;
}

bool Object::SetFlags(FixedString const& attributeName, Array<STDString> const& value)
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

bool Object::SetFunctors(FixedString const& attributeName, std::optional<Array<FunctorGroup>> const& value)
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

	OsiErrorS("Temporarily disabled until functors are mapped");
	return false;
	if (value) {
		Functors.set(attributeName, *value);
	} else {
		// FIXME - clearing stats functors not implemented!
	}
	return true;
}

bool Object::SetRollConditions(FixedString const& attributeName, std::optional<Array<RollCondition>> const& value)
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
		RollConditions.set(attributeName, *value);
	} else {
		// FIXME - clearing roll conditions not implemented!
	}

	return true;
}

// FIXME - ExtraProperties, ComboCategory

bool Object::CopyFrom(Object* source)
{
	if (ModifierListIndex != source->ModifierListIndex) {
		auto stats = GetStaticSymbols().GetStats();
		auto objModifier = stats->ModifierLists.Find(ModifierListIndex);
		auto copyModifier = stats->ModifierLists.Find(source->ModifierListIndex);
		OsiError("Cannot copy stats from object '" << source->Name << "' (a " << copyModifier->Name
			<< ") to an object of type " << objModifier->Name);
		return false;
	}

	AIFlags = source->AIFlags;

	for (uint32_t i = 0; i < IndexedProperties.size(); i++) {
		IndexedProperties[i] = source->IndexedProperties[i];
	}

	for (auto const& prop : source->Functors) {
		// TODO - is reusing property list objects allowed?
		Functors.set(prop.Key(), prop.Value());
	}

	for (auto const& cond : source->RollConditions) {
		// TODO - is reusing condition objects allowed?
		RollConditions.set(cond.Key(), cond.Value());
	}

	Requirements = source->Requirements;
	ComboProperties = source->ComboProperties;
	ComboCategories = source->ComboCategories;
	return true;
}

END_NS()
