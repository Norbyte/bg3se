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
	Array<uint8_t> field_38;
	uint8_t field_48;
	Array<uint8_t> field_50;
	MultiHashSet<uint8_t> field_60;
	Guid field_90;
	uint8_t field_A0;
	uint8_t field_A1;
	uint8_t field_A2;
	uint8_t field_A3;
};


struct TargetComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HitTarget;
	static constexpr auto EngineClass = "eoc::hit::TargetComponent";

	EntityHandle field_0;
	EntityHandle field_8;
	TargetInfo Target;
};


struct AttackerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HitAttacker;
	static constexpr auto EngineClass = "eoc::hit::AttackerComponent";

	EntityHandle Attacker;
};


struct WeaponComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HitWeapon;
	static constexpr auto EngineClass = "eoc::hit::WeaponComponent";

	EntityHandle Weapon;
};


struct ThrownObjectComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HitThrownObject;
	static constexpr auto EngineClass = "eoc::hit::ThrownObjectComponent";

	EntityHandle ThrownObject;
};


struct ProxyComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HitProxy;
	static constexpr auto EngineClass = "eoc::hit::ProxyComponent";

	EntityHandle Owner;
	FixedString field_8;
};


struct ProxyOwnerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HitProxyOwner;
	static constexpr auto EngineClass = "eoc::hit::ProxyOwnerComponent";

	Array<EntityHandle> Owners;
};


struct ReactionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HitReaction;
	static constexpr auto EngineClass = "eoc::hit::ReactionComponent";

	struct Reaction
	{
		uint64_t field_0;
		TargetInfo Target;
	};


	MultiHashMap<EntityHandle, Reaction> Reactions;
};


END_NS()
