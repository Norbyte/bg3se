#include <Extender/Shared/SavegameSerializer.h>
#include <Extender/Version.h>

BEGIN_SE()

void SavegameSerializer::SavegameVisit(ObjectVisitor* visitor)
{
    OPTICK_EVENT(Optick::Category::IO);
    if (visitor->EnterRegion(GFS.strScriptExtenderSave)) {
        uint32_t version = SavegameVersion;
        visitor->VisitUInt32(GFS.strExtenderVersion, version, 0);
        if (visitor->IsReading()) {
            if (version > SavegameVersion) {
                ERR("Savegame version too new! Extender version %d, savegame version %d; savegame data will not be loaded!", SavegameVersion, version);
                std::stringstream ss;
                ss << "Could not load Script Extender save data - savegame is newer than the currently installed extender!<br>";
                ss << "Extender version v" << SavegameVersion << ", savegame version v" << version;
                gExtender->GetLibraryManager().ShowStartupError(ss.str().c_str(), true, false);
            } else {
                Serialize(visitor, version);
            }
        } else {
            Serialize(visitor, SavegameVersion);
        }

        visitor->ExitRegion(GFS.strScriptExtenderSave);
    }
}


void SavegameSerializer::Serialize(ObjectVisitor* visitor, uint32_t version)
{
    SerializePersistentVariables(visitor, version);

    auto& state = gExtender->GetServer().GetExtensionState();

    if (version >= SavegameVerAddedUserVars) {
        state.GetUserVariables().SavegameVisit(visitor);
        state.GetModVariables().SavegameVisit(visitor);
    }

    if (version >= SavegameVerAddedTimers) {
        auto lua = state.GetLua();
        if (lua) {
            lua->GetTimers().SavegameVisit(visitor);
        }
    }
}


void SavegameSerializer::SerializePersistentVariables(ObjectVisitor* visitor, uint32_t version)
{
    OPTICK_EVENT(Optick::Category::IO);
    STDString nullStr;
    if (visitor->EnterNode(GFS.strLuaVariables, GFS.strEmpty)) {
        auto const& configs = gExtender->GetServer().GetExtensionState().GetConfigs();

        if (visitor->IsReading()) {
            std::unordered_map<FixedString, STDString> variables;
            uint32_t numMods{ 0 };
            visitor->VisitCount(GFS.strMod, &numMods);

            for (uint32_t i = 0; i < numMods; i++) {
                if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
                    FixedString modId;
                    visitor->VisitFixedString(GFS.strModId, modId, GFS.strEmpty);
                    STDString modVars;
                    visitor->VisitSTDString(GFS.strLuaVariables, modVars, nullStr);

                    variables.insert(std::make_pair(modId, modVars));
                    visitor->ExitNode(GFS.strMod);
                }
            }

            RestorePersistentVariables(variables);
        } else {
            auto& state = gExtender->GetServer().GetExtensionState();
            auto mods = state.GetPersistentVarMods();

            for (auto const& modId : mods) {
                auto vars = state.GetModPersistentVars(modId);
                if (vars) {
                    if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
                        FixedString modIdTemp = modId;
                        visitor->VisitFixedString(GFS.strModId, modIdTemp, GFS.strEmpty);
                        visitor->VisitSTDString(GFS.strLuaVariables, *vars, nullStr);
                        visitor->ExitNode(GFS.strMod);
                    }
                }
            }
        }

        visitor->ExitNode(GFS.strLuaVariables);
    }
}

void SavegameSerializer::RestorePersistentVariables(std::unordered_map<FixedString, STDString> const& variables)
{
    auto& state = gExtender->GetServer().GetExtensionState();
    for (auto const& var : variables) {
        state.RestoreModPersistentVars(var.first, var.second);
    }
}

END_SE()
