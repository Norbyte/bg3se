#pragma once

#include <cstdint>

#include "resource.h"

namespace bg3se {
	static constexpr uint32_t CurrentVersion = RES_DLL_MAJOR_VERSION;
	// Last version with savegame changes
	static constexpr uint32_t SavegameVersion = 1;
}
