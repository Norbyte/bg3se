#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Lua/LuaBinding.h>
#include <GameDefinitions/Stats/Stats.h>
#include <Lua/Shared/LuaStats.h>

namespace bg3se::lua::stats
{
    LegacyMap<FixedString, FixedString> GetModifierAttributes(FixedString const& modifierName)
    {
        auto stats = GetStaticSymbols().GetStats();
        if (!stats) {
            OsiError("Stats not available");
            return {};
        }

        auto modifierList = stats->ModifierLists.GetByName(modifierName);
        if (!modifierList) {
            OsiError("No such modifier list: " << modifierName);
            return {};
        }

        LegacyMap<FixedString, FixedString> modifiers;
        for (auto const& modifier : modifierList->Attributes.Values) {
            auto enumeration = GetStaticSymbols().GetStats()->ModifierValueLists.GetByHandle(modifier->EnumerationIndex);
            modifiers.insert(modifier->Name, enumeration->Name);
        }

        return modifiers;
    }

}
