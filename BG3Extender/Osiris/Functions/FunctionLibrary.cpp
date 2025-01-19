#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Osiris/Functions/FunctionLibrary.h>
#include <Extender/Version.h>
#include <Extender/Shared/ScriptHelpers.h>
#include <fstream>
#include "json/json.h"

namespace bg3se::esv
{
    CustomFunctionLibrary::CustomFunctionLibrary(OsirisExtender & osiris)
        : osiris_(osiris)
    {}

    void CustomFunctionLibrary::Register()
    {
        auto & functionMgr = osiris_.GetCustomFunctionManager();
        functionMgr.BeginStaticRegistrationPhase();
        RegisterLuaFunctions();
        functionMgr.EndStaticRegistrationPhase();
    }
}
