#pragma once

BEGIN_NS(hit)

struct TargetInfo
{
	FixedString SpellId;
	FixedString field_4;
	uint8_t field_8;
	glm::vec3 ImpactPosition;
	glm::vec3 ImpactDirection;
	int TotalDamageDone;
	int ArmorAbsorption;
	uint32_t EffectFlags;
	DamageType MainDamageType;
	CauseType CauseType;
	HitWith HitWith;
	Array<DamagePair> Damage;
	uint8_t AttackFlags;
	Array<bool> ConditionRollResults;
	HashSet<uint8_t> ResistanceTypes;
	Guid SpellCastGuid;
	uint8_t field_A0;
	uint8_t field_A1;
	uint8_t field_A2;
	uint8_t field_A3;
};


struct TargetComponent : public BaseComponent
{
	DEFINE_COMPONENT(HitTarget, "eoc::hit::TargetComponent")

	EntityHandle field_0;
	EntityHandle field_8;
	TargetInfo Target;
};


struct AttackerComponent : public BaseComponent
{
	DEFINE_COMPONENT(HitAttacker, "eoc::hit::AttackerComponent")

	EntityHandle Attacker;
};


struct WeaponComponent : public BaseComponent
{
	DEFINE_COMPONENT(HitWeapon, "eoc::hit::WeaponComponent")

	EntityHandle Weapon;
};


struct ThrownObjectComponent : public BaseComponent
{
	DEFINE_COMPONENT(HitThrownObject, "eoc::hit::ThrownObjectComponent")

	EntityHandle ThrownObject;
};


struct ProxyComponent : public BaseComponent
{
	DEFINE_COMPONENT(HitProxy, "eoc::hit::ProxyComponent")

	EntityHandle Owner;
	FixedString field_8;
};


struct ProxyOwnerComponent : public BaseComponent
{
	DEFINE_COMPONENT(HitProxyOwner, "eoc::hit::ProxyOwnerComponent")

	Array<EntityHandle> Owners;
};


struct ReactionSource
{
	uint64_t field_0;
	TargetInfo Target;
};

struct ReactionComponent : public BaseComponent
{
	DEFINE_COMPONENT(HitReaction, "eoc::hit::ReactionComponent")

	HashMap<EntityHandle, ReactionSource> Reactions;
};

END_NS()

BEGIN_NS(esv::hit)

struct AnimationEventRequestsData
{
	uint8_t HitAnimationType;
	uint8_t field_1;
	bool field_2;
	glm::vec3 field_4;
	glm::vec3 field_10;
	glm::vec3 field_1C;
	EntityHandle field_28;
};

struct HitAnimationRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(HitAnimationRequest, "esv::hit::HitAnimationRequestOneFrameComponent")

	Array<AnimationEventRequestsData> Requests;
};

struct HitResultEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(HitResultEvent, "esv::hit::HitResultEventOneFrameComponent")

	ActionOriginator field_0;
	HitDesc field_20;
	uint8_t field_1D0;
	EntityHandle Target;
	bool Lethal;
	bool ShouldBeDowned;
	int AC;
	uint8_t field_1E8;
};

struct OnHitJoinCombatRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(HitJoinCombatRequest, "esv::hit::OnHitJoinCombatRequestOneFrameComponent")

	HashSet<EntityHandle> Entities;
};

END_NS()
