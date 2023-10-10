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

void SEUpdaterShowConsole()
{
	if (!gUpdater) return;

	gUpdater->GetConfig().Debug = true;
	gUpdater->InitConsole();
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
	if (!gUpdater) {
		*length = 0;
		return;
	}

	*length = (uint32_t)gUpdater->GetErrorMessage().size();
	if (buf != nullptr) {
		std::copy(gUpdater->GetErrorMessage().begin(), gUpdater->GetErrorMessage().end(), buf);
	}
}

void SEUpdaterGetLog(char* buf, uint32_t* length)
{
	if (!gUpdater) {
		*length = 0;
		return;
	}

	*length = (uint32_t)gUpdater->GetErrorMessage().size();
	if (buf != nullptr) {
		std::copy(gUpdater->GetErrorMessage().begin(), gUpdater->GetErrorMessage().end(), buf);
	}
}

bool SEUpdaterGetResourceVersion(int32_t* major, int32_t* minor, int32_t* revision, int32_t* build)
{
	if (!gUpdater) return false;

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

void SEUpdaterShutdown()
{
	if (!gUpdater) return;

	gUpdater.reset();
	gGameHelpers.reset();
}

void SEUpdaterTest()
{
	SEUpdaterInitialize(nullptr);
	SEUpdaterShowConsole();
	SESetGameVersion(4, 37, 67, 641);
	SEUpdate();
	SEUpdaterShutdown();
}

END_SE()

extern "C"
{

void SEUpdaterInitialize(char const* configPath)
{
	bg3se::SEUpdaterInitialize(configPath);
}

void SEUpdaterShowConsole()
{
	bg3se::SEUpdaterShowConsole();
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

void SEUpdaterTest()
{
	bg3se::SEUpdaterTest();
}

void SEUpdaterShutdown()
{
	bg3se::SEUpdaterShutdown();
}

}
