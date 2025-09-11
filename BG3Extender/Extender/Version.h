#pragma once

#include <cstdint>

#include "resource.h"
#include <Extender/BuildInfo.h>

namespace bg3se {
    static constexpr uint32_t CurrentVersion = RES_DLL_MAJOR_VERSION;

    // Version with user variables
    static constexpr uint32_t SavegameVerAddedUserVars = 9;
    // Persistent timers added
    static constexpr uint32_t SavegameVerAddedTimers = 10;
    // Boolean user vars added
    static constexpr uint32_t SavegameVerBoolUserVars = 11;
    // Binary user vars added
    static constexpr uint32_t SavegameVerBinaryUserVars = 12;
    // Last version with savegame changes
    static constexpr uint32_t SavegameVersion = 12;
}
