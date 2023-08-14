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

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>

BEGIN_SE()

[[noreturn]]
void Fail(char const * reason);

std::string ToUTF8(std::wstring const & s);
std::wstring FromUTF8(std::string const & s);

bool TryCreateDirectory(std::wstring const& path);
bool SaveFile(std::wstring const& path, std::vector<uint8_t> const& body);
bool SaveFile(std::wstring const& path, std::string const& body);
bool LoadFile(std::wstring const& path, std::vector<uint8_t>& body);
bool LoadFile(std::wstring const& path, std::string& body);

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

#define OsiError(msg) (void)0

extern HMODULE gThisModule;

END_SE()

#include <GameHelpers.h>
