#pragma once

#include <GameDefinitions/Stats/UseActions.h>

BEGIN_SE()

class UseActionHelpers
{
public:
	UseActionHelpers();

	IActionData* Create(ActionDataType type);

private:
	std::vector<IActionDataVMT*> vftables_;
	std::vector<IActionData* (*)()> factories_;
	bool mapped_{ false };

	void DiscoverVftables();
	void DiscoverVftables(Array<IActionData*> const& actions);
};

END_SE()
