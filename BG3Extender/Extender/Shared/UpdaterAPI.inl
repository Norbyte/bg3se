#include <Extender/Shared/UpdaterAPI.h>
#include <BG3Updater/ExtenderAPI.inl>

BEGIN_SE()

UpdaterAPIHelpers::UpdaterAPIHelpers()
{
	auto dWrite = GetModuleHandleW(L"DWrite.dll");
	if (dWrite != NULL && dWrite != gCoreLibPlatformInterface.ThisModule) {
		initialized_ = api_.Init(dWrite);
	}
}

void UpdaterAPIHelpers::MarkReadyToDisplayErrors()
{
	if (initialized_) {
		api_.SetCapabilities(CapErrorReporting);
	}
}

std::optional<std::string> UpdaterAPIHelpers::GetDisplayError()
{
	if (!initialized_) {
		return {};
	}

	auto displayFlags = api_.GetErrorFlags();
	if (!(displayFlags & ErrShowError)) {
		return {};
	}

	uint32_t length{ 0 };
	std::string error;

	api_.GetError(nullptr, &length);
	error.resize(length);
	api_.GetError(error.data(), &length);

	return error;
}

END_SE()
