#include "stdafx.h"
#include "Updater.h"

BEGIN_SE()

void SEUpdaterInitialize(char const* configPath)
{
	if (gUpdater) return;

	gGameHelpers = std::make_unique<GameHelpers>();
	gUpdater = std::make_unique<ScriptExtenderUpdater>();
	gUpdater->LoadConfig(configPath);
	gGameHelpers->Initialize();
}

void SESetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build)
{
	if (!gUpdater) return;

	gUpdater->SetGameVersion(major, minor, revision, build);
}

bool SEUpdate()
{
	if (!gUpdater) return false;

	return gUpdater->FetchUpdates();
}

void SEUpdaterGetError(char* buf, uint32_t* length)
{
	*length = (uint32_t)gUpdater->GetErrorMessage().size();
	if (buf != nullptr) {
		std::copy(gUpdater->GetErrorMessage().begin(), gUpdater->GetErrorMessage().end(), buf);
	}
}

void SEUpdaterGetLog(char* buf, uint32_t* length)
{
	*length = (uint32_t)gUpdater->GetErrorMessage().size();
	if (buf != nullptr) {
		std::copy(gUpdater->GetErrorMessage().begin(), gUpdater->GetErrorMessage().end(), buf);
	}
}

bool SEUpdaterGetResourceVersion(int32_t* major, int32_t* minor, int32_t* revision, int32_t* build)
{
	gUpdater->LoadCaches();
	auto ver = gUpdater->GetCache()->FindResourceVersion("ScriptExtender", gUpdater->GetCachedGameVersion());
	if (ver) {
		*major = ver->Version.Major;
		*minor = ver->Version.Minor;
		*revision = ver->Version.Revision;
		*build = ver->Version.Build;
		return true;
	} else {
		return false;
	}
}

END_SE()

extern "C"
{

void SEUpdaterInitialize(char const* configPath)
{
	bg3se::SEUpdaterInitialize(configPath);
}

void SESetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build)
{
	bg3se::SESetGameVersion(major, minor, revision, build);
}

bool SEUpdate()
{
	return bg3se::SEUpdate();
}

void SEUpdaterGetError(char* buf, uint32_t* length)
{
	bg3se::SEUpdaterGetError(buf, length);
}

void SEUpdaterGetLog(char* buf, uint32_t* length)
{
	bg3se::SEUpdaterGetLog(buf, length);
}

bool SEUpdaterGetResourceVersion(int32_t* major, int32_t* minor, int32_t* revision, int32_t* build)
{
	return bg3se::SEUpdaterGetResourceVersion(major, minor, revision, build);
}

}
