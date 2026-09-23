#include <GameDefinitions/Stats/Stats.h>
#include <GameDefinitions/Stats/Cache.h>

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
    return Functors->Values;
}

FixedString const* Object::GetFixedStringRef(StatModifierCache const& attribute) const
{
    auto index = IndexedProperties[attribute.Index];
    
    switch (attribute.Type) {
    case RPGEnumerationType::FixedString:
    {
        auto val = GetStaticSymbols().GetStats()->GetFixedString(index);
        if (val) {
            return *val;
        }
        break;
    }

    case RPGEnumerationType::Enumeration:
    {
        if (index >= 0 && index < (int32_t)attribute.Enum->Labels.size()) {
            auto const& enumLabel = attribute.Enum->Labels[index];
            if (enumLabel) {
                return &enumLabel;
            }
        }

        break;
    }

    case RPGEnumerationType::AIFlags:
    {
        return &AIFlags;
    }
    }

    return nullptr;
}

std::optional<FixedString> Object::GetFixedString(FixedString const& attributeName) const
{
    auto attrInfo = gStatStructureCache.GetCachedAttribute(ModifierListIndex, attributeName);
    if (attrInfo == nullptr) {
        return {};
    }

    auto fs = GetFixedStringRef(*attrInfo);
    return fs ? *fs : std::optional<FixedString>{};
}

std::optional<int> Object::GetInt(StatModifierCache const& attribute) const
{
    if (attribute.Type == RPGEnumerationType::Int
        || attribute.Type == RPGEnumerationType::Enumeration) {
        return IndexedProperties[attribute.Index];
    } else {
        return {};
    }
}

std::optional<float> Object::GetFloat(StatModifierCache const& attribute) const
{
    auto index = IndexedProperties[attribute.Index];
    if (attribute.Type == RPGEnumerationType::Float) {
        auto val = GetStaticSymbols().GetStats()->GetFloat(index);
        if (val) {
            return **val;
        }
    }

    return {};
}

std::optional<int64_t> Object::GetInt64(StatModifierCache const& attribute) const
{
    auto index = IndexedProperties[attribute.Index];
    if (attribute.Type == RPGEnumerationType::Flags) {
        auto val = GetStaticSymbols().GetStats()->GetInt64(index);
        if (val) {
            return **val;
        }
    }

    return {};
}

std::optional<Guid> Object::GetGuid(StatModifierCache const& attribute) const
{
    auto index = IndexedProperties[attribute.Index];
    if (attribute.Type == RPGEnumerationType::GUID) {
        auto val = GetStaticSymbols().GetStats()->GetGuid(index);
        if (val) {
            return **val;
        }
    }

    return {};
}

std::optional<TranslatedString> Object::GetTranslatedString(StatModifierCache const& attribute) const
{
    auto index = IndexedProperties[attribute.Index];
    if (attribute.Type == RPGEnumerationType::TranslatedString) {
        auto val = GetStaticSymbols().GetStats()->GetTranslatedString(index);
        if (val) {
            return **val;
        }
    }

    return {};
}

std::optional<Array<FixedString>> Object::GetFlags(StatModifierCache const& attribute) const
{
    auto index = IndexedProperties[attribute.Index];
    if (attribute.Type == RPGEnumerationType::Flags) {
        auto flags = GetStaticSymbols().GetStats()->GetInt64(index);
        Array<FixedString> flagSet;

        if (flags) {
            flagSet.reserve(std::popcount((uint64_t)**flags));
            for (uint32_t i = 1; i < attribute.Enum->Labels.size(); i++) {
                if (**flags & (1ull << (i - 1))) {
                    flagSet.push_back(attribute.Enum->Labels[i]);
                }
            }
        }

        return flagSet;
    }

    return {};
}

std::optional<Array<FunctorGroup> const*> Object::GetFunctors(StatModifierCache const& attribute) const
{
    if (attribute.Type != RPGEnumerationType::StatsFunctors) {
        return {};
    }

    auto functors = Functors.try_get(attribute.Name);
    if (functors) {
        return functors;
    } else {
        return {};
    }
}

std::optional<Array<FunctorGroup>*> Object::GetFunctors(StatModifierCache const& attribute)
{
    if (attribute.Type != RPGEnumerationType::StatsFunctors) {
        return {};
    }

    auto functors = Functors.try_get(attribute.Name);
    if (functors) {
        return functors;
    } else {
        return {};
    }
}

std::optional<Array<Object::RollCondition> const*> Object::GetRollConditions(StatModifierCache const& attribute) const
{
    if (attribute.Type != RPGEnumerationType::RollConditions) {
        return {};
    }

    return RollConditions.try_get(attribute.Name);
}

std::optional<Array<Object::RollCondition>*> Object::GetRollConditions(StatModifierCache const& attribute)
{
    if (attribute.Type != RPGEnumerationType::RollConditions) {
        return {};
    }

    return RollConditions.try_get(attribute.Name);
}

std::optional<StringView> Object::GetConditions(StatModifierCache const& attribute) const
{
    if (attribute.Type != RPGEnumerationType::Conditions) {
        return {};
    }

    auto index = IndexedProperties[attribute.Index];
    auto val = GetStaticSymbols().GetStats()->GetConditions(index);
    if (val) {
        return **val;
    } else {
        return {};
    }
}

void Object::SetString(int attributeIndex, FixedString const& value)
{
    int poolIdx{ -1 };
    auto fs = GetStaticSymbols().GetStats()->GetOrCreateFixedString(poolIdx);
    if (fs != nullptr) {
        *fs = value;
        IndexedProperties[attributeIndex] = poolIdx;
    }
}

bool Object::SetString(StatModifierCache const& attribute, const char * value)
{
    auto stats = GetStaticSymbols().GetStats();

    switch (attribute.Type) {
    case RPGEnumerationType::GUID:
    {
        auto guid = Guid::ParseGuidString(value);
        if (!guid) {
            OsiError("Couldn't set " << Name << "." << attribute.Name << ": Value (\"" << value << "\") is not a valid GUID");
            return false;
        }

        SetGuid(attribute.Index, guid);
        break;
    }
    
    case RPGEnumerationType::Conditions:
    {
        auto index = stats->GetOrCreateConditions(value);
        IndexedProperties[attribute.Index] = index;
        break;
    }
    
    case RPGEnumerationType::TranslatedString:
    {
        SetTranslatedString(attribute.Index, TranslatedString::FromString(value));
        break;
    }
    
    case RPGEnumerationType::RollConditions:
    {
        if (*value) {
            auto index = stats->GetOrCreateConditions(value);
            if (index >= 0) {
                RollCondition cond;
                cond.Name = GFS.strDefault;
                cond.Conditions.Id = index;
                Array<RollCondition> conditions;
                conditions.push_back(cond);
                SetRollConditions(attribute, conditions);
            } else {
                SetRollConditions(attribute, {});
            }
        } else {
            SetRollConditions(attribute, {});
        }
        break;
    }

    default:
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to string value: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    return true;
}

bool Object::SetFixedString(StatModifierCache const& attribute, FixedString const& value)
{
    switch (attribute.Type) {
    case RPGEnumerationType::AIFlags:
    {
        AIFlags = value;
        break;
    }
    
    case RPGEnumerationType::FixedString:
    {
        SetString(attribute.Index, value);
        break;
    }

    case RPGEnumerationType::Enumeration:
    case RPGEnumerationType::Flags:
    {
        auto enumIndex = attribute.Enum->LabelToIndex.try_get(value);
        if (enumIndex) {
            if (attribute.Type == RPGEnumerationType::Flags) {
                SetInt64Flags(attribute.Index, *enumIndex ? (1ll << (*enumIndex - 1)) : 0);
            } else {
                IndexedProperties[attribute.Index] = (int32_t)*enumIndex;
            }
        } else {
            OsiError("Couldn't set " << Name << "." << attribute.Name << ": Value (\"" << value << "\") is not a valid enum label");
            return false;
        }
        break;
    }

    default:
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to FixedString value: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    return true;
}

bool Object::SetInt(StatModifierCache const& attribute, int64_t value)
{
    if (attribute.Type == RPGEnumerationType::Int) {
        IndexedProperties[attribute.Index] = (int32_t)value;
    } else if (attribute.Type == RPGEnumerationType::Enumeration) {
        if (value >= 0 && value < (int)attribute.Enum->Labels.size()) {
            IndexedProperties[attribute.Index] = (int32_t)value;
        } else {
            OsiError("Couldn't set " << Name << "." << attribute.Name << ": Enum index (\"" << value << "\") out of range");
            return false;
        }
    } else if (attribute.Type == RPGEnumerationType::Flags) {
        SetInt64Flags(attribute.Index, value);
    } else {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to integer value: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    return true;
}

void Object::SetFloat(int attributeIndex, std::optional<float> value)
{
    if (value) {
        int poolIdx{ -1 };
        auto flt = GetStaticSymbols().GetStats()->GetOrCreateFloat(poolIdx);
        if (flt != nullptr) {
            *flt = *value;
            IndexedProperties[attributeIndex] = poolIdx;
        }
    } else {
        IndexedProperties[attributeIndex] = -1;
    }
}

bool Object::SetFloat(StatModifierCache const& attribute, std::optional<float> value)
{
    if (attribute.Type == RPGEnumerationType::Float) {
        SetFloat(attribute.Index, value);
    } else {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to float value: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    return true;
}

void Object::SetInt64Flags(int attributeIndex, int64_t value)
{
    int poolIdx{ -1 };
    auto i64 = GetStaticSymbols().GetStats()->GetOrCreateInt64(poolIdx);
    if (i64 != nullptr) {
        *i64 = value;
        IndexedProperties[attributeIndex] = poolIdx;
    }
}

bool Object::SetInt64(StatModifierCache const& attribute, int64_t value)
{
    if (attribute.Type == RPGEnumerationType::Flags) {
        SetInt64Flags(attribute.Index, value);
    } else {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to int64 value: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    return true;
}

void Object::SetGuid(int attributeIndex, std::optional<Guid> value)
{
    if (value) {
        int poolIdx{ -1 };
        auto guid = GetStaticSymbols().GetStats()->GetOrCreateGuid(poolIdx);
        if (guid != nullptr) {
            *guid = *value;
            IndexedProperties[attributeIndex] = poolIdx;
        }
    } else {
        IndexedProperties[attributeIndex] = -1;
    }
}

bool Object::SetGuid(StatModifierCache const& attribute, std::optional<Guid> value)
{
    if (attribute.Type == RPGEnumerationType::GUID) {
        SetGuid(attribute.Index, value);
    } else {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to GUID value: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    return true;
}

void Object::SetTranslatedString(int attributeIndex, std::optional<TranslatedString> value)
{
    if (value) {
        int poolIdx{ -1 };
        auto ts = GetStaticSymbols().GetStats()->GetOrCreateTranslatedString(poolIdx);
        if (ts != nullptr) {
            *ts = *value;
            IndexedProperties[attributeIndex] = poolIdx;
        }
    } else {
        IndexedProperties[attributeIndex] = -1;
    }
}

bool Object::SetTranslatedString(StatModifierCache const& attribute, std::optional<TranslatedString> value)
{
    if (attribute.Type == RPGEnumerationType::TranslatedString) {
        SetTranslatedString(attribute.Index, value);
    } else {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to TranslatedString value: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    return true;
}

bool Object::SetFlags(StatModifierCache const& attribute, Array<FixedString> const& value)
{
    if (attribute.Type != RPGEnumerationType::Flags) {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to flag array: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    int64_t flags{ 0 };
    for (auto const& flag : value) {
        auto flagValue = attribute.Enum->LabelToIndex.try_get(flag);
        if (!flagValue) {
            OsiError("Couldn't set " << Name << "." << attribute.Name << ": Value (\"" << flag << "\") is not a valid enum label");
            return false;
        }

        if (*flagValue) {
            flags |= (1ll << (*flagValue - 1));
        }
    }

    SetInt64Flags(attribute.Index, flags);
    return true;
}

bool Object::SetFunctors(StatModifierCache const& attribute, std::optional<Array<FunctorGroup>> const& value)
{
    if (attribute.Type != RPGEnumerationType::StatsFunctors) {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to stats functors: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    OsiErrorS("Temporarily disabled until functors are mapped");
    return false;
    if (value) {
        Functors.set(attribute.Name, *value);
    } else {
        // FIXME - clearing stats functors not implemented!
    }
    return true;
}

bool Object::SetRollConditions(StatModifierCache const& attribute, std::optional<Array<RollCondition>> const& value)
{
    if (attribute.Type != RPGEnumerationType::RollConditions) {
        OsiError("Couldn't set " << Name << "." << attribute.Name << " to roll conditions: Inappropriate type: " << attribute.TypeName);
        return false;
    }

    if (value) {
        RollConditions.set(attribute.Name, *value);
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
        auto objModifier = stats->ModifierLists.GetByHandle(ModifierListIndex);
        auto copyModifier = stats->ModifierLists.GetByHandle(source->ModifierListIndex);
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
