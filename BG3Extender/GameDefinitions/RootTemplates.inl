#pragma once

#include <GameDefinitions/RootTemplates.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

FixedString GameObjectTemplate::GetTemplateType() const
{
	return *GetType();
}

IActionData* ItemTemplate::AddUseAction(ActionDataType type)
{
	auto action = gExtender->GetUseActionHelpers().Create(type);
	if (action) {
		OnUsePeaceActions.Value.push_back(action);
		return action;
	} else {
		return nullptr;
	}
}

void ItemTemplate::RemoveUseAction(int32_t index)
{
	if (index >= 1 && index <= (int)OnUsePeaceActions.Value.size()) {
		OnUsePeaceActions.Value.remove_at(index - 1);
	}
}

END_SE()
