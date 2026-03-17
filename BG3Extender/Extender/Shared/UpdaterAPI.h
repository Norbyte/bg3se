#pragma once

#include <BG3Updater/ExtenderAPI.h>

BEGIN_SE()

class UpdaterAPIHelpers
{
public:
	UpdaterAPIHelpers();

	void MarkReadyToDisplayErrors();
	std::optional<std::string> GetDisplayError();

private:
	UpdaterAPI api_;
	bool initialized_{ false };
};

END_SE()
