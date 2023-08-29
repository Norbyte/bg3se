// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <string>
#include <memory>
#include <optional>
#include <sstream>
#include <vector>

#include <CoreLib/Base/Base.h>
#include <CoreLib/Utils.h>

BEGIN_SE()

struct UpdaterConfig
{
	std::string ManifestURL;
	std::string ManifestName;
	std::string UpdateChannel;
	std::string TargetVersion;
	std::string TargetResourceDigest;
	std::wstring CachePath;
	bool Debug;
	bool ValidateSignature;
	bool DisableUpdates;
};

END_SE()

