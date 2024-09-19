#pragma once

#include <Extender/Shared/UseActions.h>

BEGIN_SE()

template <class T>
IActionData* CreateActionData()
{
	auto data = GameAlloc<T>();
	data->Type = T::StaticType;
	return data;
}

UseActionHelpers::UseActionHelpers()
{
	vftables_.resize((unsigned)ActionDataType::Unknown35 + 1);
	factories_.resize((unsigned)ActionDataType::Unknown35 + 1);
	factories_[(unsigned)ActionDataType::OpenClose] = &CreateActionData<OpenCloseActionData>;
	factories_[(unsigned)ActionDataType::Destroy] = &CreateActionData<DestroyActionData>;
	factories_[(unsigned)ActionDataType::Teleport] = &CreateActionData<TeleportActionData>;
	factories_[(unsigned)ActionDataType::CreateSurface] = &CreateActionData<CreateSurfaceActionData>;
	factories_[(unsigned)ActionDataType::DestroyParameters] = &CreateActionData<DestroyParametersActionData>;
	factories_[(unsigned)ActionDataType::Equip] = &CreateActionData<EquipActionData>;
	factories_[(unsigned)ActionDataType::Consume] = &CreateActionData<ConsumeActionData>;
	factories_[(unsigned)ActionDataType::StoryUse] = &CreateActionData<StoryUseActionData>;
	factories_[(unsigned)ActionDataType::Door] = &CreateActionData<DoorActionData>;
	factories_[(unsigned)ActionDataType::CreatePuddle] = &CreateActionData<CreatePuddleActionData>;
	factories_[(unsigned)ActionDataType::Book] = &CreateActionData<BookActionData>;
	factories_[(unsigned)ActionDataType::UseSpell] = &CreateActionData<UseSpellActionData>;
	factories_[(unsigned)ActionDataType::SpellBook] = &CreateActionData<SpellBookActionData>;
	factories_[(unsigned)ActionDataType::Sit] = &CreateActionData<SitActionData>;
	factories_[(unsigned)ActionDataType::Lie] = &CreateActionData<LieActionData>;
	factories_[(unsigned)ActionDataType::Insert] = &CreateActionData<InsertActionData>;
	factories_[(unsigned)ActionDataType::Stand] = &CreateActionData<LockpickActionData>;
	factories_[(unsigned)ActionDataType::StoryUseInInventory] = &CreateActionData<StoryUseInInventoryActionData>;
	factories_[(unsigned)ActionDataType::DisarmTrap] = &CreateActionData<DisarmTrapActionData>;
	factories_[(unsigned)ActionDataType::ShowStoryElementUI] = &CreateActionData<ShowStoryElementUIActionData>;
	factories_[(unsigned)ActionDataType::Combine] = &CreateActionData<CombineActionData>;
	factories_[(unsigned)ActionDataType::Ladder] = &CreateActionData<LadderActionData>;
	factories_[(unsigned)ActionDataType::PlaySound] = &CreateActionData<PlaySoundActionData>;
	factories_[(unsigned)ActionDataType::SpawnCharacter] = &CreateActionData<SpawnCharacterActionData>;
	factories_[(unsigned)ActionDataType::Constrain] = &CreateActionData<ConstrainActionData>;
	factories_[(unsigned)ActionDataType::Recipe] = &CreateActionData<RecipeActionData>;
	factories_[(unsigned)ActionDataType::Unknown31] = &CreateActionData<Unknown31ActionData>;
	factories_[(unsigned)ActionDataType::Throw] = &CreateActionData<ThrowActionData>;
	factories_[(unsigned)ActionDataType::LearnSpell] = &CreateActionData<LearnSpellActionData>;
	factories_[(unsigned)ActionDataType::Unknown34] = &CreateActionData<Unknown34ActionData>;
	factories_[(unsigned)ActionDataType::Unknown35] = &CreateActionData<Unknown35ActionData>;
}

IActionData* UseActionHelpers::Create(ActionDataType type)
{
	if (!mapped_) {
		DiscoverVftables();
		mapped_ = true;
	}

	if ((unsigned)type < vftables_.size() && (unsigned)type < factories_.size() && vftables_[(unsigned)type] != nullptr) {
		auto action = factories_[(unsigned)type]();
		action->vftable = vftables_[(unsigned)type];
		return action;
	} else {
		return nullptr;
	}
}

void UseActionHelpers::DiscoverVftables()
{
	auto const& templates = GetStaticSymbols().GetGlobalTemplateBank()->Templates;
	for (auto it : templates) {
		auto tmpl = it.Value;
		if (tmpl->GetTemplateType() == GFS.strcharacter) {
			auto ch = static_cast<CharacterTemplate*>(tmpl);
			DiscoverVftables(ch->OnDeathActions.Value);
		} else if (tmpl->GetTemplateType() == GFS.stritem) {
			auto item = static_cast<ItemTemplate*>(tmpl);
			DiscoverVftables(item->OnUsePeaceActions.Value);
			DiscoverVftables(item->OnDestroyActions.Value);
		}
	}
}

void UseActionHelpers::DiscoverVftables(Array<IActionData*> const& actions)
{
	for (auto action : actions) {
		if ((unsigned)action->Type + 1 < vftables_.size() && vftables_[(unsigned)action->Type] == nullptr) {
			vftables_[(unsigned)action->Type] = action->vftable;
		}
	}
}

END_SE()
