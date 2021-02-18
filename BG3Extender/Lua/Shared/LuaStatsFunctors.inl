#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaPropertyMapHelpers.h>

namespace bg3se::lua
{
	DECLARE_CLS(StatsFunctorBase);
	DECLARE_CLS(CustomDescriptionFunctor);
	DECLARE_CLS(ApplyStatusFunctor);
	DECLARE_CLS(SurfaceChangeFunctor);
	DECLARE_CLS(ResurrectFunctor);
	DECLARE_CLS(SabotageFunctor);
	DECLARE_CLS(SummonFunctor);
	DECLARE_CLS(ForceFunctor);
	DECLARE_CLS(DouseFunctor);
	DECLARE_CLS(SwapPlacesFunctor);
	DECLARE_CLS(EqualizeFunctor);
	DECLARE_CLS(PickupFunctor);
	DECLARE_CLS(CreateSurfaceFunctor);
	DECLARE_CLS(CreateConeSurfaceFunctor);
	DECLARE_CLS(RemoveStatusFunctor);
	DECLARE_CLS(DealDamageFunctor);
	DECLARE_CLS(ExecuteWeaponFunctorsFunctor);
	DECLARE_CLS(RegainHitPointsFunctor);
	DECLARE_CLS(TeleportSourceFunctor);
	DECLARE_CLS(SetStatusDurationFunctor);
	DECLARE_CLS(UseSpellFunctor);
	DECLARE_CLS(UseActionResourceFunctor);
	DECLARE_CLS(UseAttackFunctor);
	DECLARE_CLS(CreateExplosionFunctor);
	DECLARE_CLS(BreakConcentrationFunctor);
	DECLARE_CLS(ApplyEquipmentStatusFunctor);
	DECLARE_CLS(RestoreResourceFunctor);
	DECLARE_CLS(SpawnFunctor);
	DECLARE_CLS(StabilizeFunctor);
	DECLARE_CLS(UnlockFunctor);
	DECLARE_CLS(ResetCombatTurnFunctor);
	DECLARE_CLS(RemoveAuraByChildStatusFunctor);
	DECLARE_CLS(ExtenderFunctor);

	void RegisterStatsFunctors(lua_State* L)
	{
		REGISTER_CLS(StatsFunctorBase);
		REGISTER_CLS(CustomDescriptionFunctor);
		REGISTER_CLS(ApplyStatusFunctor);
		REGISTER_CLS(SurfaceChangeFunctor);
		REGISTER_CLS(ResurrectFunctor);
		REGISTER_CLS(SabotageFunctor);
		REGISTER_CLS(SummonFunctor);
		REGISTER_CLS(ForceFunctor);
		REGISTER_CLS(DouseFunctor);
		REGISTER_CLS(SwapPlacesFunctor);
		REGISTER_CLS(EqualizeFunctor);
		REGISTER_CLS(PickupFunctor);
		REGISTER_CLS(CreateSurfaceFunctor);
		REGISTER_CLS(CreateConeSurfaceFunctor);
		REGISTER_CLS(RemoveStatusFunctor);
		REGISTER_CLS(DealDamageFunctor);
		REGISTER_CLS(ExecuteWeaponFunctorsFunctor);
		REGISTER_CLS(RegainHitPointsFunctor);
		REGISTER_CLS(TeleportSourceFunctor);
		REGISTER_CLS(SetStatusDurationFunctor);
		REGISTER_CLS(UseSpellFunctor);
		REGISTER_CLS(UseActionResourceFunctor);
		REGISTER_CLS(UseAttackFunctor);
		REGISTER_CLS(CreateExplosionFunctor);
		REGISTER_CLS(BreakConcentrationFunctor);
		REGISTER_CLS(ApplyEquipmentStatusFunctor);
		REGISTER_CLS(RestoreResourceFunctor);
		REGISTER_CLS(SpawnFunctor);
		REGISTER_CLS(StabilizeFunctor);
		REGISTER_CLS(UnlockFunctor);
		REGISTER_CLS(ResetCombatTurnFunctor);
		REGISTER_CLS(RemoveAuraByChildStatusFunctor);
		REGISTER_CLS(ExtenderFunctor);

		{
			CLS(StatsFunctorBase);
			P_RO(UniqueName);
			P_RO(TypeId);
			P_RO(PropertyContext);
			P_RO(StatsConditionsId);
			// TODO - Conditions object access
			P(IsSelf);
			P_RO(StoryActionId);
		}

		{
			CLS(CustomDescriptionFunctor);
			INHERIT(StatsFunctorBase);
			P(Description);
		}

		{
			CLS(ApplyStatusFunctor);
			INHERIT(StatsFunctorBase);
			P(StatusId);
			P(StringParam);
			P_RO(StatsConditions); // TODO - later write support for Conditions + StatsConditionsId
			P(Duration);
			P(Param1);
			P(Param2);
		}

		{
			CLS(SurfaceChangeFunctor);
			INHERIT(StatsFunctorBase);
			P(Chance);
			P(field_24);
			P(field_28);
			P(field_2C);
			P(SurfaceChange);
		}

		{
			CLS(ResurrectFunctor);
			INHERIT(StatsFunctorBase);
			P(Probability);
			P(HealthPercentage);
		}

		{
			CLS(SabotageFunctor);
			INHERIT(StatsFunctorBase);
			P(field_20);
		}

		{
			CLS(SummonFunctor);
			INHERIT(StatsFunctorBase);
			P(MovingObject);
			P(field_24);
			P(SpawnLifetime);
			P(StatusesToApply);
			P(field_48);
		}

		{
			CLS(ForceFunctor);
			INHERIT(StatsFunctorBase);
			P(Arg0);
			P(Origin);
			P(Aggression);
		}

		{
			CLS(DouseFunctor);
			INHERIT(StatsFunctorBase);
			P(field_20);
			P(field_24);
		}

		{
			CLS(SwapPlacesFunctor);
			INHERIT(StatsFunctorBase);
			P(Arg0);
		}

		{
			CLS(EqualizeFunctor);
			INHERIT(StatsFunctorBase);
			P(HealType);
		}

		{
			CLS(PickupFunctor);
			INHERIT(StatsFunctorBase);
			P(Arg0);
		}

		{
			CLS(CreateSurfaceFunctor);
			INHERIT(StatsFunctorBase);
			P(SurfaceType);
			P(Radius);
			P(Arg4);
			P(Duration);
			P(IsControlledByConcentration);
		}

		{
			CLS(CreateConeSurfaceFunctor);
			INHERIT(StatsFunctorBase);
			P(Arg2);
			P(Arg0);
			P(Arg1);
			P(Arg3);
		}

		{
			CLS(RemoveStatusFunctor);
			INHERIT(StatsFunctorBase);
			P(StatusId);
		}

		{
			CLS(DealDamageFunctor);
			INHERIT(StatsFunctorBase);
			P(DamageType);
			P(WeaponType);
			P(WeaponDamageType);
			// TODO - Damage expression access
			P(Nonlethal);
			P(Magical);
		}

		{
			CLS(ExecuteWeaponFunctorsFunctor);
			INHERIT(StatsFunctorBase);
			P(Type);
		}

		{
			CLS(RegainHitPointsFunctor);
			INHERIT(StatsFunctorBase);
			// TODO - lua expr --  P(HitPoints);
		}

		{
			CLS(TeleportSourceFunctor);
			INHERIT(StatsFunctorBase);
		}

		{
			CLS(SetStatusDurationFunctor);
			INHERIT(StatsFunctorBase);
			P(StatusId);
			P(Duration);
			P(SetIfLonger);
		}

		{
			CLS(UseSpellFunctor);
			INHERIT(StatsFunctorBase);
			P(SpellId);
			P(IgnoreHasSpell);
			P(IgnoreChecks);
			P(Arg3);
		}

		{
			CLS(UseActionResourceFunctor);
			INHERIT(StatsFunctorBase);
			P(ActionResourceUUID);
			P(Amount);
			P(ResourceIndex);
			P(IsPercentage);
		}

		{
			CLS(UseAttackFunctor);
			INHERIT(StatsFunctorBase);
			P(IgnoreChecks);
		}

		{
			CLS(CreateExplosionFunctor);
			INHERIT(StatsFunctorBase);
			P(SpellId);
		}

		{
			CLS(BreakConcentrationFunctor);
			INHERIT(StatsFunctorBase);
		}

		{
			CLS(ApplyEquipmentStatusFunctor);
			INHERIT(ApplyStatusFunctor);
			P(EquipmentSlot);
		}

		{
			CLS(RestoreResourceFunctor);
			INHERIT(StatsFunctorBase);
			P(ActionResourceUUID);
			P(Hex);
			P(field_34);
			// TODO -- P(LuaAmount);
			P(Amount);
			P(IsPercentage);
		}

		{
			CLS(SpawnFunctor);
			INHERIT(StatsFunctorBase);
			P(TemplateId);
			P(Arg1);
			P(StatusesToApply);
		}

		{
			CLS(StabilizeFunctor);
			INHERIT(StatsFunctorBase);
		}

		{
			CLS(UnlockFunctor);
			INHERIT(StatsFunctorBase);
		}

		{
			CLS(ResetCombatTurnFunctor);
			INHERIT(StatsFunctorBase);
		}

		{
			CLS(RemoveAuraByChildStatusFunctor);
			INHERIT(StatsFunctorBase);
			P(StatusId);
		}

		{
			CLS(ExtenderFunctor);
			INHERIT(StatsFunctorBase);
		}
	}
}
