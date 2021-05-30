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
			for (unsigned i = 0; i < entity->ComponentHandles.Size; i++) {
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

#define T(v, cls) \
	case ExtComponentType::v: \
	{ \
		PushComponentType<cls>(L, helpers, handle, lifetime, logError); \
		break; \
	}

	void PushComponent(lua_State* L, EntitySystemHelpersBase* helpers, EntityHandle const& handle, ExtComponentType componentType,
		LifetimeHolder const& lifetime, bool logError)
	{
		switch (componentType) {
		T(ActionResources, ActionResourcesComponent)
		T(Armor, ArmorComponent)
		T(BaseHp, BaseHpComponent)
		T(Data, DataComponent)
		T(Experience, ExperienceComponent)
		T(Health, HealthComponent)
		T(Passive, PassiveComponent)
		T(Sense, SenseComponent)
		T(SpellBook, SpellBookComponent)
		T(Stats, StatsComponent)
		T(StatusImmunities, StatusImmunitiesComponent)
		T(SurfacePathInfluences, SurfacePathInfluencesComponent)
		T(Use, UseComponent)
		T(Value, ValueComponent)
		T(Weapon, WeaponComponent)
		T(Wielding, WieldingComponent)
		T(CustomStats, CustomStatsComponent)
		T(BoostCondition, BoostConditionComponent)
		T(BoostsContainer, BoostsContainerComponent)
		// T(ActionResourceConsumeMultiplierBoost, ActionResourceConsumeMultiplierBoostComponent)
		T(CombatParticipant, CombatParticipantComponent)
		T(Gender, GenderComponent)
		T(SpellContainer, SpellContainerComponent)
		T(Tag, TagComponent)
		T(SpellBookPrepares, SpellBookPreparesComponent)

		T(CombatState, CombatStateComponent)
		T(TurnBased, TurnBasedComponent)
		T(TurnOrder, TurnOrderComponent)
		T(Transform, TransformComponent)
		T(PassiveContainer, PassiveContainerComponent)
		T(BoostInfo, BoostInfoComponent)
		T(Relation, RelationComponent)
		T(CanInteract, CanInteractComponent)
		T(CanSpeak, CanSpeakComponent)
		T(Origin, OriginComponent)
		T(Level, LevelComponent)

		T(ServerCharacter, esv::Character)
		T(ServerItem, esv::Item)
		T(ServerProjectile, esv::Projectile)
		T(ServerOsirisTag, OsirisTagComponent)
		T(ServerActive, esv::ActiveComponent)

		/*
		TODO - client not supported yet!
		T(ClientCharacter, ecl::Character)
		T(ClientItem, ecl::Item)
		T(ClientProjectile, ecl::Projectile)*/

		// Boost components
		T(ArmorClassBoost, ArmorClassBoostComponent)
		T(AbilityBoost, AbilityBoostComponent)
		T(RollBonusBoost, RollBonusBoostComponent)
		T(AdvantageBoost, AdvantageBoostComponent)
		T(ActionResourceValueBoost, ActionResourceValueBoostComponent)
		T(CriticalHitBoost, CriticalHitBoostComponent)
		T(AbilityFailedSavingThrowBoost, AbilityFailedSavingThrowBoostComponent)
		T(ResistanceBoost, ResistanceBoostComponent)
		T(WeaponDamageResistanceBoost, WeaponDamageResistanceBoostComponent)
		T(ProficiencyBonusOverrideBoost, ProficiencyBonusOverrideBoostComponent)
		T(JumpMaxDistanceMultiplierBoost, JumpMaxDistanceMultiplierBoostComponent)
		T(HalveWeaponDamageBoost, HalveWeaponDamageBoostComponent)
		T(UnlockSpellBoost, UnlockSpellBoostComponent)
		T(SourceAdvantageBoost, SourceAdvantageBoostComponent)
		T(ProficiencyBonusBoost, ProficiencyBonusBoostComponent)
		T(ProficiencyBoost, ProficiencyBoostComponent)
		T(IncreaseMaxHPBoost, IncreaseMaxHPBoostComponent)
		T(ActionResourceBlockBoost, ActionResourceBlockBoostComponent)
		T(StatusImmunityBoost, StatusImmunityBoostComponent)
		T(UseBoosts, UseBoostsComponent)
		T(TemporaryHPBoost, TemporaryHPBoostComponent)
		T(WeightBoost, WeightBoostComponent)
		T(FactionOverrideBoost, FactionOverrideBoostComponent)
		T(ActionResourceMultiplierBoost, ActionResourceMultiplierBoostComponent)
		T(InitiativeBoost, InitiativeBoostComponent)
		T(DarkvisionRangeBoost, DarkvisionRangeBoostComponent)
		T(DarkvisionRangeMinBoost, DarkvisionRangeMinBoostComponent)
		T(DarkvisionRangeOverrideBoost, DarkvisionRangeOverrideBoostComponent)
		T(AddTagBoost, AddTagBoostComponent)
		T(IgnoreDamageThresholdMinBoost, IgnoreDamageThresholdMinBoostComponent)
		T(SkillBoost, SkillBoostComponent)
		T(WeaponDamageBoost, WeaponDamageBoostComponent)
		T(NullifyAbilityBoost, NullifyAbilityBoostComponent)
		T(RerollBoost, RerollBoostComponent)
		T(DownedStatusBoost, DownedStatusBoostComponent)
		T(WeaponEnchantmentBoost, WeaponEnchantmentBoostComponent)
		T(GuaranteedChanceRollOutcomeBoost, GuaranteedChanceRollOutcomeBoostComponent)
		T(AttributeBoost, AttributeBoostComponent)
		T(GameplayLightBoost, GameplayLightBoostComponent)
		T(DualWieldingBoost, DualWieldingBoostComponent)
		T(SavantBoost, SavantBoostComponent)
		T(MinimumRollResultBoost, MinimumRollResultBoostComponent)
		T(CharacterWeaponDamageBoost, CharacterWeaponDamageBoostComponent)
		T(ProjectileDeflectBoost, ProjectileDeflectBoostComponent)
		T(AbilityOverrideMinimumBoost, AbilityOverrideMinimumBoostComponent)
		T(ACOverrideMinimumBoost, ACOverrideMinimumBoostComponent)
		T(FallDamageMultiplierBoost, FallDamageMultiplierBoostComponent)

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
			for (unsigned i = 0; i < entity->ComponentHandles.Size; i++) {
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
