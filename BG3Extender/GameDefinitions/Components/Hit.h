#pragma once

BEGIN_NS(hit)

struct TargetInfo
{
	FixedString field_0;
	FixedString field_4;
	uint8_t field_8;
	glm::vec3 field_C;
	glm::vec3 field_18;
	int field_24;
	int field_28;
	int field_2C;
	uint8_t field_30;
	uint8_t field_31;
	uint8_t field_32;
	Array<DamagePair> field_38;
	uint8_t field_48;
	Array<bool> field_50;
	MultiHashSet<uint8_t> ResistanceTypes;
	Guid field_90;
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

	MultiHashMap<EntityHandle, ReactionSource> Reactions;
};


END_NS()
