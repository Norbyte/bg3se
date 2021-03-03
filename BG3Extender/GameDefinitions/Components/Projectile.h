#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Hit.h>

namespace bg3se
{
	struct StatsFunctorSet;
}


namespace bg3se::esv
{
	struct ProjectileComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerProjectile;

		void* TEMP_PAD;
		FixedString field_20;
		NetId MyNetID;
		ObjectHandle Caster;
		ObjectHandle Source;
		ObjectHandle TargetObject;
		ObjectHandle HitObject;
		ObjectHandle SourceWeapon;
		float SourcePos[3];
		float TargetPos[3];
		float PathRadius;
		FixedString TextKey;
		Hit Hit;
		char DamageType;
		char CauseType;
		float field_22C;
		float HitInterpolation;
		__int64 field_238;
		PathMover PathMover1;
		PathMover PathMover2;
		float ExplodeRadius;
		float field_484;
		SomeGuidId GuidId;
		StatsFunctorSet* PropertyListFunctor;
		__int64 MovingObject;
		float MovingObjectTransform[16];
		FixedString SpawnEffect;
		bool SpawnFXOverridesImpactFX;
		ObjectHandle EffectHandle;
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
		float PrevTranslate[3];
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
