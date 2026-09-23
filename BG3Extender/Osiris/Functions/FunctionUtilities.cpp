#include <stdafx.h>
#include "FunctionLibrary.h"
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Symbols.h>

namespace bg3se
{
    ecl::EoCClient * GetEoCClient()
    {
        auto clientPtr = GetStaticSymbols().ecl__EoCClient;
        if (clientPtr == nullptr || *clientPtr == nullptr) {
            return nullptr;
        } else {
            return *clientPtr;
        }
    }
    
    esv::EoCServer * GetEoCServer()
    {
        auto serverPtr = GetStaticSymbols().esv__EoCServer;
        if (serverPtr == nullptr || *serverPtr == nullptr) {
            return nullptr;
        } else {
            return *serverPtr;
        }
    }

    ModManager * GetModManagerServer()
    {
        auto server = GetEoCServer();
        if (server == nullptr) {
            return nullptr;
        } else {
            return &server->ModManager;
        }
    }

    FixedString NameGuidToFixedString(char const * nameGuid)
    {
        if (nameGuid == nullptr) {
            return FixedString{};
        }

        if (!Guid::Parse(nameGuid)) {
            OsiError("GUID (" << nameGuid << ") malformed!");
            return FixedString{};
        }

        auto nameLen = strlen(nameGuid);
        auto guid = nameGuid + nameLen - 36;

        return FixedString(std::string_view(guid, 36));
    }

    namespace esv
    {
        /*IEoCServerObject* EntityWorld::GetGameObject(char const* nameGuid, bool logError)
        {
            if (nameGuid == nullptr) {
                OsiError("Attempted to look up object with null name!");
                return nullptr;
            }

            auto character = GetCharacter(nameGuid, false);
            if (character != nullptr) {
                return character;
            }

            auto item = GetItem(nameGuid, false);
            if (item != nullptr) {
                return item;
            }

            if (logError) {
                OsiError("No EoC server object found with GUID '" << nameGuid << "'");
            }

            return nullptr;
        }

        IEoCServerObject* EntityWorld::GetGameObject(ComponentHandle handle, bool logError)
        {
            if (!handle) {
                return nullptr;
            }

            switch ((ObjectType)handle.GetType()) {
            case ObjectType::ServerCharacter:
                return GetCharacter(handle, logError);

            case ObjectType::ServerItem:
                return GetItem(handle, logError);

            case ObjectType::ServerProjectile:
                return GetProjectile(handle, logError);

            default:
                OsiError("GameObjects with handle type " << handle.GetType() << " not supported!");
                return nullptr;
            }
        }*/

        ecs::EntityWorld* GetEntityWorld()
        {
            auto server = GetEoCServer();
            if (server == nullptr) {
                OsiErrorS("EoCServer not available!");
                return nullptr;
            } else {
                return server->EntityWorld;
            }
        }
    }
}
