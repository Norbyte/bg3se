#include <Lua/Shared/LuaEntityProxy.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>
#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Combat.h>
#include <GameDefinitions/Components/Passives.h>
#include <GameDefinitions/Components/Projectile.h>
#include <GameDefinitions/Components/Stats.h>
#include <GameDefinitions/Components/Boosts.h>

namespace bg3se::lua
{
	char const* const EntityProxy::MetatableName = "EntityProxy";

	EntityProxy::EntityProxy(EntityHandle const& handle, EntitySystemHelpersBase* entitySystem)
		: handle_(handle), entitySystem_(entitySystem)
	{}

	int EntityProxy::HasRawComponent(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		auto componentType = checked_get<char const*>(L, 2);
		auto componentIdx = self->entitySystem_->GetComponentIndex(componentType);
		if (componentIdx) {
			auto world = self->entitySystem_->GetEntityWorld();
			auto component = world->GetEntityComponentHandle(self->handle_, *componentIdx, false);
			push(L, (bool)component);
		}
		else {
			push(L, false);
		}

		return 1;
	}

	int EntityProxy::GetAllRawComponents(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);

		lua_newtable(L);
		int idx{ 1 };

		auto entity = self->entitySystem_->GetEntityWorld()->GetEntity(self->handle_);
		if (entity) {
			for (unsigned i = 0; i < entity->ComponentHandles.Size(); i++) {
				auto componentHandle = entity->ComponentHandles[i];
				if (componentHandle) {
					auto componentIdx = entity->SlotIndexToComponentIdMap[i];
					auto name = self->entitySystem_->GetComponentName((EntityWorldBase::ComponentTypeIndex)componentIdx);

					push(L, idx++);
					lua_newtable(L);
					settable(L, "Type", name);
					settable(L, "Handle", componentHandle);
					lua_settable(L, -3);
				}
			}
		}

		return 1;
	}

	template <class T>
	void PushComponentType(lua_State* L, EntitySystemHelpersBase* helpers, EntityHandle const& handle, 
		LifetimeHolder const& lifetime, bool logError)
	{
		auto component = helpers->GetEntityComponent<T>(handle, false);
		if (component) {
			ObjectProxy::MakeRef<T>(L, component, lifetime);
		} else {
			if (logError) {
				OsiError("Couldn't get component of type " << T::ComponentType);
			}

			push(L, nullptr);
		}
	}

#define T(cls) \
	case cls::ComponentType: \
	{ \
		PushComponentType<cls>(L, helpers, handle, lifetime, logError); \
		break; \
	}

	void PushComponent(lua_State* L, EntitySystemHelpersBase* helpers, EntityHandle const& handle, ExtComponentType componentType,
		LifetimeHolder const& lifetime, bool logError)
	{
		switch (componentType) {
			T(ActionResourcesComponent)
			T(ArmorComponent)
			T(BaseHpComponent)
			T(DataComponent)
			T(ExperienceComponent)
			T(HealthComponent)
			T(PassiveComponent)
			T(SenseComponent)
			T(SpellBookComponent)
			T(StatsComponent)
			T(StatusImmunitiesComponent)
			T(SurfacePathInfluencesComponent)
			T(UseComponent)
			T(ValueComponent)
			T(WeaponComponent)
			T(WieldingComponent)
			T(CustomStatsComponent)
			T(BoostConditionComponent)
			T(BoostsContainerComponent)
			// T(ActionResourceConsumeMultiplierBoostComponent)
			T(CombatParticipantComponent)
			T(GenderComponent)
			T(SpellContainerComponent)
			T(TagComponent)
			T(SpellBookPreparesComponent)

			T(CombatStateComponent)
			T(TurnBasedComponent)
			T(TurnOrderComponent)
			T(TransformComponent)
			T(PassiveContainerComponent)
			T(BoostInfoComponent)
			T(RelationComponent)
			T(CanInteractComponent)
			T(CanSpeakComponent)
			T(OriginComponent)
			T(LevelComponent)


			T(BackgroundComponent)
			T(GodComponent)
			T(LevelUpComponent)
			T(PlayerPrepareSpellComponent)
			T(CCPrepareSpellComponent)
			T(SpellCastComponent)
			T(FloatingComponent)
			T(VoiceComponent)
			T(CustomIconComponent)
			T(CharacterCreationStatsComponent)
			T(DisarmableComponent)
			T(ShortRestComponent)
			T(IsSummonComponent)
			T(SummonContainerComponent)
			T(StealthComponent)
			T(IsGlobalComponent)
			T(SavegameComponent)
			T(DisabledEquipmentComponent)
			T(LootingStateComponent)
			T(LootComponent)
			T(LockComponent)
			T(SummonLifetimeComponent)
			T(InvisibilityComponent)
			T(IconComponent)
			T(HotbarContainerComponent)
			T(OriginTagComponent)
			T(OriginPassivesComponent)
			T(GodTagComponent)
			T(ClassTagComponent)
			T(BackgroundTagComponent)
			T(BackgroundPassivesComponent)
			T(GlobalShortRestDisabledComponent)
			T(GlobalLongRestDisabledComponent)
			T(StoryShortRestDisabledComponent)
			T(FleeCapabilityComponent)
			T(CanDoRestComponent)
			T(ItemBoostsComponent)
			T(ActiveCharacterLightComponent)
			T(AnimationSetComponent)
			T(AnimationBlueprintComponent)
			T(CanModifyHealthComponent)
			T(AddedSpellsComponent)
			T(AvailableLevelComponent)
			T(CanBeLootedComponent)
			T(CanDoActionsComponent)
			T(CanMoveComponent)
			T(CanSenseComponent)
			T(ConcentrationComponent)
			T(DarknessComponent)
			T(DualWieldingComponent)
			T(GameObjectVisualComponent)
			T(InventorySlotComponent)
			T(SpellBookCooldowns)
			T(DisplayNameComponent)
			T(EquipableComponent)
			T(GameplayLightComponent)
			T(ProgressionContainerComponent)
			T(RaceComponent)
			T(SightComponent)
			T(CanTravelComponent)
			T(CanBeInInventoryComponent)
			T(MovementComponent)
			T(ObjectInteractionComponent)
			T(PathingComponent)
			T(SteeringComponent)
			T(CanDeflectProjectilesComponent)
			T(LearnedSpells)
			T(SpellAiConditions)
			T(ActiveSkeletonSlotsComponent)
			T(NetComponent)
			T(PhysicsComponent)


			T(RecruitedByComponent)
			T(GameTimerComponent)
			T(ExperienceGaveOutComponent)
			T(ReplicationDependencyComponent)
			T(IsUnsummoningComponent)
			T(FleeBlockedComponent)
			T(ActivationGroupContainerComponent)
			T(AnubisTagComponent)
			T(DialogTagComponent)
			T(DisplayNameListComponent)
			T(IconListComponent)
			T(PlanTagComponent)
			T(RaceTagComponent)
			T(TemplateTagComponent)
			T(ToggledPassivesComponent)
			T(BoostTagComponent)
			T(TriggerStateComponent)
			T(SafePositionComponent)
			T(AnubisExecutorComponent)
			T(DetachedBoostComponent)

			T(StaticPhysicsComponent)
			T(AnubisComponent)


			T(esv::Character)
			T(esv::Item)
			T(esv::Projectile)
			T(OsirisTagComponent)
			T(esv::ActiveComponent)

			/*
			TODO - client not supported yet!
			T(ecl::Character)
			T(ecl::Item)
			T(ecl::Projectile)*/

			// Boost components
			T(ArmorClassBoostComponent)
			T(AbilityBoostComponent)
			T(RollBonusBoostComponent)
			T(AdvantageBoostComponent)
			T(ActionResourceValueBoostComponent)
			T(CriticalHitBoostComponent)
			T(AbilityFailedSavingThrowBoostComponent)
			T(ResistanceBoostComponent)
			T(WeaponDamageResistanceBoostComponent)
			T(ProficiencyBonusOverrideBoostComponent)
			T(JumpMaxDistanceMultiplierBoostComponent)
			T(HalveWeaponDamageBoostComponent)
			T(UnlockSpellBoostComponent)
			T(SourceAdvantageBoostComponent)
			T(ProficiencyBonusBoostComponent)
			T(ProficiencyBoostComponent)
			T(IncreaseMaxHPBoostComponent)
			T(ActionResourceBlockBoostComponent)
			T(StatusImmunityBoostComponent)
			T(UseBoostsComponent)
			T(TemporaryHPBoostComponent)
			T(WeightBoostComponent)
			T(FactionOverrideBoostComponent)
			T(ActionResourceMultiplierBoostComponent)
			T(InitiativeBoostComponent)
			T(DarkvisionRangeBoostComponent)
			T(DarkvisionRangeMinBoostComponent)
			T(DarkvisionRangeOverrideBoostComponent)
			T(AddTagBoostComponent)
			T(IgnoreDamageThresholdMinBoostComponent)
			T(SkillBoostComponent)
			T(WeaponDamageBoostComponent)
			T(NullifyAbilityBoostComponent)
			T(RerollBoostComponent)
			T(DownedStatusBoostComponent)
			T(WeaponEnchantmentBoostComponent)
			T(GuaranteedChanceRollOutcomeBoostComponent)
			T(AttributeBoostComponent)
			T(GameplayLightBoostComponent)
			T(DualWieldingBoostComponent)
			T(SavantBoostComponent)
			T(MinimumRollResultBoostComponent)
			T(CharacterWeaponDamageBoostComponent)
			T(ProjectileDeflectBoostComponent)
			T(AbilityOverrideMinimumBoostComponent)
			T(ACOverrideMinimumBoostComponent)
			T(FallDamageMultiplierBoostComponent)

		default:
			OsiError("Don't know how to push component type: " << componentType);
			break;
		}
	}

#undef T

	int EntityProxy::GetComponent(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		auto componentType = checked_get<ExtComponentType>(L, 2);
		PushComponent(L, self->entitySystem_, self->handle_, componentType, GetCurrentLifetime(), false);
		return 1;
	}

	int EntityProxy::GetAllComponents(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);

		lua_newtable(L);

		auto entity = self->entitySystem_->GetEntityWorld()->GetEntity(self->handle_);
		if (entity) {
			for (unsigned i = 0; i < entity->ComponentHandles.Size(); i++) {
				auto componentHandle = entity->ComponentHandles[i];
				if (componentHandle) {
					auto componentIdx = entity->SlotIndexToComponentIdMap[i];
					auto type = self->entitySystem_->GetComponentType((EntityWorldBase::ComponentTypeIndex)componentIdx);

					if (type) {
						push(L, type);
						PushComponent(L, self->entitySystem_, self->handle_, *type, GetCurrentLifetime(), true);
						lua_settable(L, -3);
					}
				}
			}
		}

		return 1;
	}

	int EntityProxy::GetEntityType(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetType());
		return 1;
	}

	int EntityProxy::GetSalt(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetSalt());
		return 1;
	}

	int EntityProxy::GetIndex(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		push(L, self->handle_.GetIndex());
		return 1;
	}

	int EntityProxy::IsAlive(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<EntityProxy*>(L, 1);
		auto world = self->entitySystem_->GetEntityWorld();
		auto entity = world->GetEntity(self->handle_, false);
		push(L, entity != nullptr);
		return 1;
	}

	void EntityProxy::PopulateMetatable(lua_State* L)
	{
		lua_newtable(L);

		push(L, &EntityProxy::HasRawComponent);
		lua_setfield(L, -2, "HasRawComponent");

		push(L, &EntityProxy::GetAllRawComponents);
		lua_setfield(L, -2, "GetAllRawComponents");

		push(L, &EntityProxy::GetComponent);
		lua_setfield(L, -2, "GetComponent");

		push(L, &EntityProxy::GetAllComponents);
		lua_setfield(L, -2, "GetAllComponents");

		push(L, &EntityProxy::GetEntityType);
		lua_setfield(L, -2, "GetEntityType");

		push(L, &EntityProxy::GetSalt);
		lua_setfield(L, -2, "GetSalt");

		push(L, &EntityProxy::GetIndex);
		lua_setfield(L, -2, "GetIndex");

		push(L, &EntityProxy::IsAlive);
		lua_setfield(L, -2, "IsAlive");

		lua_setfield(L, -2, "__index");
	}

	int EntityProxy::Index(lua_State* L)
	{
		OsiError("Should not get here!");
		return 0;
	}

	int EntityProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[100];
		sprintf_s(entityName, "Entity (%016llx)", handle_.Handle);
		push(L, entityName);
		return 1;
	}


	char const* const ObjectHandleProxy::MetatableName = "ObjectHandleProxy";

	ObjectHandleProxy::ObjectHandleProxy(ObjectHandle const& handle, EntitySystemHelpersBase* entitySystem)
		: handle_(handle), entitySystem_(entitySystem)
	{}

	int ObjectHandleProxy::GetType(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		push(L, self->handle_.GetType());
		return 1;
	}

	int ObjectHandleProxy::GetTypeName(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		auto name = self->entitySystem_->GetComponentName(EntityWorldBase::HandleTypeIndex(self->handle_.GetType()));
		push(L, name);
		return 1;
	}

	int ObjectHandleProxy::GetSalt(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		push(L, self->handle_.GetSalt());
		return 1;
	}

	int ObjectHandleProxy::GetIndex(lua_State* L)
	{
		StackCheck _(L, 1);
		auto self = checked_get<ObjectHandleProxy*>(L, 1);
		push(L, self->handle_.GetIndex());
		return 1;
	}

	void ObjectHandleProxy::PopulateMetatable(lua_State* L)
	{
		lua_newtable(L);

		push(L, &ObjectHandleProxy::GetType);
		lua_setfield(L, -2, "GetType");

		push(L, &ObjectHandleProxy::GetTypeName);
		lua_setfield(L, -2, "GetTypeName");

		push(L, &ObjectHandleProxy::GetSalt);
		lua_setfield(L, -2, "GetSalt");

		push(L, &ObjectHandleProxy::GetIndex);
		lua_setfield(L, -2, "GetIndex");

		lua_setfield(L, -2, "__index");
	}

	int ObjectHandleProxy::Index(lua_State* L)
	{
		OsiError("Should not get here!");
		return 0;
	}

	int ObjectHandleProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[100];
		sprintf_s(entityName, "Object (%016llx)", handle_.Handle);
		push(L, entityName);
		return 1;
	}


	void RegisterEntityProxy(lua_State* L)
	{
		EntityProxy::RegisterMetatable(L);
		ObjectHandleProxy::RegisterMetatable(L);
	}
}
