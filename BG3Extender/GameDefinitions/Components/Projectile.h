#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Hit.h>

namespace bg3se
{
	struct StatsFunctorSet;

	struct PathMover
	{
		float field_0[32];
		float field_80;
		int field_84;
		int field_88;
		int field_8C;
		float PathOrig2[3];
		float PathTarget2[3];
		char PathRandom2;
		uint32_t _Pad;
		float PathRotateOrig[4];
		float PathRotateTarget[4];
		float PathOrig[3];
		float PathTarget[3];
		float PathInterpolateValue;
		float PathSpeedSet;
		float PathSpeed;
		float PathAcceleration;
		char PathType;
		int PathRotationType;
		float PathRadius;
		float PathShift;
		float PathMinArcDist;
		float PathMaxArcDist;
		__int64 PathRepeat;
		char PathRandom;
		char field_119;
	};

}


namespace bg3se::esv
{
	struct Projectile : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerProjectile;

		void* TEMP_PAD;
		FixedString field_20;
		NetId NetID;
		ComponentHandle Caster;
		ComponentHandle Source;
		ComponentHandle TargetObject;
		ComponentHandle HitObject;
		ComponentHandle SourceWeapon;
		glm::vec3 SourcePosition;
		glm::vec3 TargetPosition;
		float PathRadius;
		FixedString TextKey;
		Hit Hit;
		DamageType DamageType;
		uint8_t CauseType;
		float field_22C;
		float HitInterpolation;
		__int64 field_238;
		PathMover PathMover1;
		PathMover PathMover2;
		float ExplodeRadius;
		float field_484;
		SpellId SpellId;
		StatsFunctorSet* PropertyListFunctor;
		__int64 MovingObject;
		glm::mat4 MovingObjectTransform;
		FixedString SpawnEffect;
		bool SpawnFXOverridesImpactFX;
		ComponentHandle EffectHandle;
		bool RequestDelete;
		bool Launched;
		bool IsOnHold;
		bool IsTrap;
		bool IsThrown;
		bool field_515;
		bool IsFromItem;
		bool IgnoreTargetChecks;
		bool field_518;
		bool CanDeflect;
		bool IgnoreObjects;
		bool SpellFlag0x04;
		bool Used;
		bool Success;
		bool field_51E;
		bool DamageMovingObjectOnLand;
		bool field_520;
		void* OnHitAction;
		FixedString CleanseStatuses;
		float CleanseChance;
		int StoryActionID;
		ActionOriginator Originator;
		uint64_t Flags;
		glm::vec3 PreviousTranslate;
		__int64 field_570;
		FixedString LevelName;
		void* ProjectileTemplate;
		__int64 field_588;
		int field_590;
		int field_594;
		int field_598;
		int field_59C;
		EntityWorldHandle MyEntityHandle;
		bool ShouldFall;
	};
}
