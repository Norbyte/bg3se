#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Hit.h>

BEGIN_NS(path)

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

struct Settings
{
	std::variant<Bezier3Trajectory, Bezier4Trajectory> Trajectory;
	[[bg3::hidden]] uint32_t _Pad;
	uint8_t RotateMode;
	std::variant<ConstantVelocity, LinearVelocity, MappedVelocity> Velocity;
};

struct PathMover : public Settings
{
	glm::vec3 SourcePosition;
	glm::quat SourceRotation;
	glm::vec3 TargetPosition;
	glm::quat TargetRotation;
	float InterpolateValue;
	std::array<float, 32> ComputedTrajectoryValues;
	Array<glm::vec3> ComputedVelocityValues;
	bool Initialized;
};

END_NS()

BEGIN_NS(projectile)

struct ProjectileResult : public path::Settings
{
	[[bg3::legacy(field_50)]] FixedString TemplateId;
	EntityHandle field_58;
	float field_60;
	float field_64;
	float field_68;
	float InterpolateValue;
};

struct SourceInfoComponent : public BaseComponent
{
	DEFINE_COMPONENT(ProjectileSource, "eoc::projectile::SourceInfoComponent")

	SpellId Spell;
	EntityHandle Entity;
};

END_NS()

BEGIN_NS(esv::projectile)

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
	bg3se::spell_cast::IntermediateTarget Target;
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
	Guid HistoryUuid;
	bool MainDamageType;
	AbilityId SpellCastingAbility;
	bool IsTrap;
	[[bg3::legacy(field_93)]] bool MovesProjectileFromCaster;
	bool CanDeflect;
	bool IgnoreObjects;
	CauseType CauseType;
	int StoryActionId;
	ActionOriginator Originator;
	FixedString TextKey;
	HitDesc Hit;
	std::optional<glm::vec3> BeamTargetPos;
	EntityHandle TargetObject;
	float ExplodeRadius;
	bool NoMovement;
	OnHitActionBase* HitAction;
	EntityHandle ThrownObject;
	bool DamageMovingObjectOnLand;
	std::optional<bg3se::projectile::ProjectileResult> Path;
	std::optional<path::Settings> MovementSettings;
	EntityHandle MovingObject;
	SpellComponent SpellData;
};

struct InitializationComponent : public BaseProxyComponent
{
	DEFINE_COMPONENT(ServerProjectileInitialization, "esv::projectile::InitializationComponent")

	[[bg3::hidden]] void* field_0;
	InitializationData Data;
	[[bg3::hidden]] void* _PAD;
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
	HitDesc Hit;
	DamageType MainDamageType;
	CauseType Cause;
	float LifeTime;
	float HitInterpolation;
	float FallbackTimer;
	path::PathMover Path;
	path::PathMover Path2;
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
	[[bg3::hidden]] void* _PAD;
};

struct ImpactEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ProjectileImpactEvent, "esv::projectile::ImpactEventOneFrameComponent")

	bg3se::spell_cast::IntermediateTarget Target;
	EntityHandle field_A8;
};

struct RequestTargetTrackingOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ProjectileRequestTargetTracking, "esv::projectile::RequestTargetTrackingOneFrameComponent")

	Guid field_0;
	HashSet<EntityHandle> Targets;
};

struct UpdateTargetTrackingOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ProjectileUpdateTargetTracking, "esv::projectile::UpdateTargetTrackingOneFrameComponent")

	Guid field_0;
	HashSet<EntityHandle> Targets;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::projectile, SplitThrowableObjectRequestOneFrameComponent, ProjectileSplitThrowableObjectRequest)

END_NS()

BEGIN_NS(spell_cast)

struct ProjectileTargetData
{
	glm::vec3 field_0;
	IntermediateTarget Target;
	FixedString TextKey;
	int field_BC;
	HitDesc Hit;
	path::Settings PathSettings;
	char PathSettingsOpt;
	projectile::ProjectileResult Result;
	char ResultOpt;
	EntityHandle ThrownObject;
};

END_NS()

BEGIN_NS(esv::spell_cast)

struct ProjectileResultsExtraData
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	Array<bg3se::projectile::ProjectileResult> Results;
};


struct ProjectileCacheComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerProjectileCache, "esv::spell_cast::ProjectileCacheComponent")

	std::optional<bg3se::spell_cast::ProjectileTargetData> Target;
	HashMap<int, float> field_350;
	[[bg3::hidden]] HashMap<int, void*> field_390; // ObjectQueue<ProjectileTargetData>
	Array<ProjectileResultsExtraData> ExtraData;
};

END_NS()
