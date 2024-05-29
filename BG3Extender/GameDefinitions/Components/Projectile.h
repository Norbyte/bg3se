#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Hit.h>

BEGIN_NS(eoc::projectile)

struct SourceInfoComponent : public BaseComponent
{
	DEFINE_COMPONENT(ProjectileSource, "eoc::projectile::SourceInfoComponent")

	SpellId Spell;
};

END_NS()

BEGIN_NS(esv::projectile)

struct Bezier3Trajectory
{
	float DistanceMin;
	float DistanceMax;
	float OffsetMin[2];
	float OffsetMax[2];
	float ShiftMin;
	float ShiftMax;
};

struct Bezier4Trajectory
{
	float DistanceMin;
	float DistanceMax;
	float OffsetAMin[2];
	float OffsetAMax[2];
	float OffsetBMin[2];
	float OffsetBMax[2];
	float ShiftAMin;
	float ShiftAMax;
	float ShiftBMin;
	float ShiftBMax;
};

struct ConstantVelocity
{
	float ConstantVelocity;
};

struct LinearVelocity
{
	float Acceleration;
	float InitialSpeed;
};

struct MappedVelocity
{
	FixedString Mapping;
};

struct MovementSettings
{
	std::variant<Bezier3Trajectory, Bezier4Trajectory> Trajectory;
	[[bg3::hidden]] uint32_t _Pad;
	uint8_t RotateMode;
	std::variant<ConstantVelocity, LinearVelocity, MappedVelocity> Velocity;
};

struct PathDescription : public MovementSettings
{
	glm::vec3 SourcePosition;
	glm::vec4 SourceRotation;
	glm::vec3 TargetPosition;
	glm::vec4 TargetRotation;
	float InterpolateValue;
    std::array<float, 32> ComputedTrajectoryValues;
	Array<glm::vec3> ComputedVelocityValues;
	bool Active;
};

struct ProjectileMovementSettings : public MovementSettings
{
	FixedString field_50;
	EntityHandle field_58;
	__int64 field_60;
	int field_68;
	float InterpolateValue;
};

struct OnHitActionBase : public ProtectedGameObject<OnHitActionBase>
{
	[[bg3::hidden]] void* VMT;
	EntityHandle Caster;
	bool IsFromItem;
	bool IgnoreTargetChecks;
	SpellId Spell;
};

struct PierceOnHit : public OnHitActionBase
{
	int PierceCount;
	glm::vec3 Direction;
	Array<EntityHandle> HitTargets;
};

struct ForkOnHit : public OnHitActionBase
{
	int ForkCount;
	int ForkLevels;
	Array<EntityHandle> HitTargets;
};

struct AttachmentComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerProjectileAttachment, "esv::projectile::AttachmentComponent")

	EntityHandle Attachment;
};

struct SpellComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerProjectileSpell, "esv::projectile::SpellComponent")

	EntityHandle Spell;
	bg3se::spell_cast::MultiTargetInfo2 Target;
	int field_B0;
	uint8_t field_B4;
};

struct InitializationData
{
	FixedString Projectile;
	glm::vec3 StartPosition;
	glm::vec3 TargetPosition;
	int field_1C;
	SpellId Spell;
	Guid SpellCastUuid;
	bool IsFromItem;
	bool IgnoreTargetChecks;
	EntityHandle Caster;
	EntityHandle Source;
	EntityHandle BeamSource;
	EntityHandle SourceWeapon;
	EntityHandle TargetTracking_M;
	EntityHandle TargetTracking2_M;
	bool MainDamageType;
	AbilityId SpellCastingAbility;
	bool IsTrap;
	uint8_t field_93;
	bool CanDeflect;
	bool IgnoreObjects;
	CauseType CauseType;
	int StoryActionId;
	ActionOriginator Originator;
	FixedString TextKey;
	Hit Hit;
	std::optional<glm::vec3> BeamTargetPos;
	EntityHandle TargetObject;
	float ExplodeRadius;
	bool NoMovement;
	OnHitActionBase* HitAction;
	EntityHandle ThrownObject;
	bool DamageMovingObjectOnLand;
	std::optional<ProjectileMovementSettings> Path;
	std::optional<MovementSettings> MovementSettings;
	EntityHandle MovingObject;
	SpellComponent SpellData;
};

struct InitializationComponent : public BaseProxyComponent
{
	DEFINE_COMPONENT(ServerProjectileInitialization, "esv::projectile::InitializationComponent")

	[[bg3::hidden]] void* field_0;
	InitializationData Data;
};


struct Projectile : public BaseProxyComponent
{
	DEFINE_COMPONENT(ServerProjectile, "esv::Projectile")

	[[bg3::hidden]] void* VMT;
	__int64 field_8;
	EntityHandle Entity;
	EntityHandle Caster;
	EntityHandle Source;
	EntityHandle TargetObject;
	EntityHandle HitObject;
	EntityHandle ClientHitObject;
	EntityHandle BeamSource;
	EntityHandle SourceWeapon;
	glm::vec3 SourcePos;
	glm::vec3 TargetPos;
	glm::vec3 TargetObjectPos;
	FixedString TextKey;
	Hit Hit;
	DamageType MainDamageType;
	CauseType Cause;
	float LifeTime;
	float HitInterpolation;
	float FallbackTimer;
	PathDescription Path;
	PathDescription Path2;
	float ExplodeRadius;
	float ExplodeRadius2;
	SpellId Spell;
	stats::Functors* HitFunctors;
	AbilityId SpellCastingAbility;
	Guid SpellCastUuid;
	EntityHandle MovingObject;
	glm::quat MovingObjectRotation;
	glm::vec3 MovingObjectPosition;
	glm::vec3 MovingObjectScale;
	FixedString SpawnEffect;
	bool SpawnFXOverridesImpactFX;
	EntityHandle BeamEffect;
	bool RequestDelete;
	bool Launched;
	bool IsOnHold;
	bool IsTrap;
	bool IsThrown;
	uint8_t field_51D;
	bool IsFromItem;
	bool IgnoreTargetChecks;
	bool IgnoreRoof;
	bool CanDeflect;
	bool IgnoreObjects;
	uint8_t field_523;
	bool Used;
	bool Success;
	uint8_t field_526;
	bool DamageMovingObjectOnLand;
	bool NoMovement;
	bool Reacted;
	bool ItemActivated;
	OnHitActionBase* HitAction;
	int32_t StoryActionId;
	ActionOriginator Originator;
	uint64_t Flags;
	glm::vec3 SourcePosition;
	glm::vec3 field_574;
	FixedString Level;
	ProjectileTemplate* Template;
	[[bg3::hidden]] Array<void*> field_590;
	ecs::EntityRef Owner;
	bool ShouldFall;
};


END_NS()
