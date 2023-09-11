#pragma once

#include <GameDefinitions/Base/Base.h>

namespace bg3se
{
    struct GameObjectTemplate : public HasObjectProxy
    {
        void* VMT;
        OverrideableProperty<uint32_t> Flags;
        void* Tags;
        FixedString Id;
        STDString Name;
        FixedString TemplateName;
        FixedString ParentTemplateId;
        FixedString field_50;
        bool IsGlobal;
        bool IsDeleted;
        bool field_56;
        bool IsForcedType;
        FixedString LevelName;
        uint8_t _Pad[4];
        OverrideableProperty<uint32_t> GroupID;
        __int64 field_68;
        Transform Transform;
        void* Unknown;
        OverrideableProperty<FixedString> VisualTemplate;
        OverrideableProperty<FixedString> PhysicsTemplate;
        OverrideableProperty<FixedString> PhysicsOpenTemplate;
        OverrideableProperty<bool> CastShadow;
        OverrideableProperty<bool> ReceiveDecal;
        OverrideableProperty<bool> AllowReceiveDecalWhenAnimated;
        OverrideableProperty<bool> IsReflecting;
        OverrideableProperty<bool> IsShadowProxy;
        OverrideableProperty<uint8_t> RenderChannel;
        OverrideableProperty<glm::vec3> CameraOffset;
        bool HasParentModRelation;
        bool Unknown2;
        uint8_t _Pad3[6];
        OverrideableProperty<bool> HasGameplayValue;
        STDString FileName;
    };


    struct EoCGameObjectTemplate : public GameObjectTemplate
    {
        OverrideableProperty<ObjectSet<void*>> AIBounds;
        TranslatedString DisplayName;
        uint8_t _Pad10[1];
        OverrideableProperty<float> Opacity;
        uint8_t _Pad11[1];
        OverrideableProperty<bool> Fadeable;
        OverrideableProperty<bool> FadeIn;
        OverrideableProperty<bool> SeeThrough;
        OverrideableProperty<bool> CollideWithCamera;
        OverrideableProperty<bool> HierarchyOnlyFade;
        OverrideableProperty<FixedString> FadeGroup;
        OverrideableProperty<ObjectSet<FixedString>> FadeChildren;
    };


    struct EoCGameObjectTemplate2 : public EoCGameObjectTemplate
    {
        OverrideableProperty<bool> CoverAmount;
        OverrideableProperty<bool> CanClimbOn;
        OverrideableProperty<bool> CanShootThrough;
        OverrideableProperty<bool> WalkThrough;
        OverrideableProperty<bool> WalkOn;
        OverrideableProperty<bool> Wadable;
        OverrideableProperty<bool> CanClickThrough;
        OverrideableProperty<bool> IsPointerBlocker;
        OverrideableProperty<bool> IsBlocker;
        OverrideableProperty<bool> IsDecorative;
        OverrideableProperty<bool> GenerateRunningDeepWater;
        OverrideableProperty<FixedString> LoopSound;
        OverrideableProperty<FixedString> SoundInitEvent;
        OverrideableProperty<int16_t> SoundAttenuation;
        OverrideableProperty<Guid> HLOD;
        OverrideableProperty<uint8_t> ShootThroughType;
        OverrideableProperty<bool> CanShineThrough;
    }; 


    struct CombatComponentTemplate
    {
        void* VMT;
        OverrideableProperty<FixedString> Archetype;
        OverrideableProperty<Guid> Alignment;
        OverrideableProperty<bool> CanFight;
        OverrideableProperty<bool> CanJoinCombat;
        OverrideableProperty<FixedString> CombatGroupID;
        OverrideableProperty<STDString> CombatName;
        OverrideableProperty<bool> IsBoss;
        OverrideableProperty<bool> StayInAiHints;
        OverrideableProperty<Guid> AiHint;
        OverrideableProperty<bool> IsInspector;
        OverrideableProperty<bool> Unknown;
        OverrideableProperty<float> StartCombatRange;
        bool Unknown2;
    };


    struct CharacterTemplate : public EoCGameObjectTemplate
    {
        CombatComponentTemplate CombatComponent;
        OverrideableProperty<ObjectSet<void*>> ScriptConfigGlobalParameters;
        OverrideableProperty<FixedString> Icon;
        OverrideableProperty<FixedString> Stats;
        OverrideableProperty<FixedString> SpellSet;
        OverrideableProperty<FixedString> Equipment;
        OverrideableProperty<ObjectSet<Guid>> EquipmentTypes;
        OverrideableProperty<ObjectSet<FixedString>> Treasures;
        OverrideableProperty<ObjectSet<FixedString>> TradeTreasures;
        OverrideableProperty<FixedString> LightID;
        OverrideableProperty<FixedString> ActiveCharacterLightID;
        OverrideableProperty<FixedString> BloodType;
        OverrideableProperty<FixedString> DefaultDialog;
        VirtualMultiHashSet<Guid>* SpeakerGroupList;
        OverrideableProperty<STDString> GeneratePortrait;
        OverrideableProperty<float> LadderAttachOffset;
        OverrideableProperty<float> LadderLoopSpeed;
        OverrideableProperty<bool> CanShootThrough;
        OverrideableProperty<uint8_t> ShootThroughType;
        OverrideableProperty<bool> WalkThrough;
        OverrideableProperty<bool> CanClimbLadders;
        OverrideableProperty<bool> IsPlayer;
        OverrideableProperty<bool> SpotSneakers;
        OverrideableProperty<bool> CanOpenDoors;
        OverrideableProperty<bool> AvoidTraps;
        OverrideableProperty<bool> InfluenceTreasureLevel;
        OverrideableProperty<bool> HardcoreOnly;
        OverrideableProperty<bool> NotHardcore;
        OverrideableProperty<bool> JumpUpLadders;
        OverrideableProperty<Guid> EquipmentRace;
        OverrideableProperty<ObjectSet<void*>> OnDeathActions;
        OverrideableProperty<float> DeathRaycastMinLength;
        OverrideableProperty<float> DeathRaycastMaxLength;
        OverrideableProperty<float> DeathRaycastHeight;
        OverrideableProperty<float> DeathRaycastVerticalLength;
        int field_3D8;
        OverrideableProperty<FixedString> ExplodedResourceID;
        OverrideableProperty<FixedString> ExplosionFX;
        OverrideableProperty<FixedString> AnubisConfigName;
        OverrideableProperty<ObjectSet<void*>> SkillList;
        OverrideableProperty<ObjectSet<void*>> ItemList;
        OverrideableProperty<ObjectSet<void*>> StatusList;
        OverrideableProperty<FixedString> TrophyID;
        OverrideableProperty<FixedString> SoundInitEvent;
        OverrideableProperty<FixedString> SoundMovementStartEvent;
        OverrideableProperty<FixedString> SoundMovementStopEvent;
        OverrideableProperty<int8_t> SoundObjectIndex;
        OverrideableProperty<int16_t> SoundAttenuation;
        OverrideableProperty<FixedString> FoleyShortResourceID;
        OverrideableProperty<FixedString> FoleyMediumResourceID;
        OverrideableProperty<FixedString> FoleyLongResourceID;
        OverrideableProperty<FixedString> VocalAlertResourceID;
        OverrideableProperty<FixedString> VocalAngryResourceID;
        OverrideableProperty<FixedString> VocalAnticipationResourceID;
        OverrideableProperty<FixedString> VocalAttackResourceID;
        OverrideableProperty<FixedString> VocalAwakeResourceID;
        OverrideableProperty<FixedString> VocalBoredResourceID;
        OverrideableProperty<FixedString> VocalBuffResourceID;
        OverrideableProperty<FixedString> VocalDeathResourceID;
        OverrideableProperty<FixedString> VocalDodgeResourceID;
        OverrideableProperty<FixedString> VocalEffortsResourceID;
        OverrideableProperty<FixedString> VocalExhaustedResourceID;
        OverrideableProperty<FixedString> VocalFallResourceID;
        OverrideableProperty<FixedString> VocalIdle1ResourceID;
        OverrideableProperty<FixedString> VocalIdle2ResourceID;
        OverrideableProperty<FixedString> VocalIdle3ResourceID;
        OverrideableProperty<FixedString> VocalInitiativeResourceID;
        OverrideableProperty<FixedString> VocalLaughterResourceID;
        OverrideableProperty<FixedString> VocalPainResourceID;
        OverrideableProperty<FixedString> VocalRebornResourceID;
        OverrideableProperty<FixedString> VocalRecoverResourceID;
        OverrideableProperty<FixedString> VocalRelaxedResourceID;
        OverrideableProperty<FixedString> VocalShoutResourceID;
        OverrideableProperty<FixedString> VocalSnoreResourceID;
        OverrideableProperty<FixedString> VocalSpawnResourceID;
        OverrideableProperty<FixedString> VocalVictoryResourceID;
        OverrideableProperty<FixedString> VocalWeakResourceID;
        OverrideableProperty<FixedString> CharacterVisualResourceID;
        OverrideableProperty<uint8_t> CoverAmount;
        OverrideableProperty<int32_t> LevelOverride;
        OverrideableProperty<bool> ForceUnsheathSkills;
        OverrideableProperty<bool> CanBeTeleported;
        OverrideableProperty<FixedString> ActivationGroupId;
        OverrideableProperty<RefMap<FixedString, FixedString>> PickingPhysicsTemplates;
        OverrideableProperty<FixedString> SoftBodyCollisionTemplate;
        OverrideableProperty<FixedString> RagdollTemplate;
        OverrideableProperty<ObjectSet<void*>> FootStepInfos;
        OverrideableProperty<uint8_t> DefaultState;
        OverrideableProperty<bool> IsLootable;
        OverrideableProperty<bool> IsEquipmentLootable;
        OverrideableProperty<uint8_t> LightChannel;
        OverrideableProperty<uint8_t> AliveInventoryType;
        OverrideableProperty<uint8_t> InventoryType;
        OverrideableProperty<Guid> Race;
        OverrideableProperty<TranslatedString> Title;
        OverrideableProperty<FixedString> AnimationSetResourceID;
        OverrideableProperty<bool> HasPlayerApprovalRating;
        OverrideableProperty<bool> DisableEquipping;
        OverrideableProperty<float> WorldClimbingSpeed;
        OverrideableProperty<bool> UseStandAtDestination;
        OverrideableProperty<bool> IsMovementEnabled;
        OverrideableProperty<float> MovementAcceleration;
        OverrideableProperty<float> MovementSpeedStroll;
        OverrideableProperty<float> MovementSpeedWalk;
        OverrideableProperty<float> MovementSpeedRun;
        OverrideableProperty<float> MovementSpeedSprint;
        OverrideableProperty<float> MovementSpeedDash;
        OverrideableProperty<float> MaxDashDistance;
        OverrideableProperty<float> MovementStepUpHeight;
        OverrideableProperty<FixedString> MovementTiltToRemap;
        OverrideableProperty<float> TurningNodeAngle;
        OverrideableProperty<float> TurningNodeOffset;
        OverrideableProperty<bool> IsSteeringEnabled;
        OverrideableProperty<FixedString> SteeringSpeedCurveWithoutTransitions;
        OverrideableProperty<float> SteeringSpeedFallback;
        OverrideableProperty<bool> IsWorldClimbingEnabled;
        OverrideableProperty<FixedString> WorldClimbingBlendspace_DownA;
        OverrideableProperty<FixedString> WorldClimbingBlendspace_DownB;
        OverrideableProperty<float> WorldClimbingBlendspace_DownBHeight;
        OverrideableProperty<FixedString> WorldClimbingBlendspace_UpA;
        OverrideableProperty<FixedString> WorldClimbingBlendspace_UpB;
        OverrideableProperty<float> WorldClimbingBlendspace_UpBHeight;
        OverrideableProperty<float> WorldClimbingHeight;
        OverrideableProperty<float> ProbeLookAtOffset;
        OverrideableProperty<float> ProbeSpineAOffset;
        OverrideableProperty<float> ProbeSpineBOffset;
        OverrideableProperty<float> ProbeTiltToOffset;
        OverrideableProperty<float> VFXScale;
        OverrideableProperty<FixedString> LadderBlendspace_Attach_Up;
        OverrideableProperty<FixedString> LadderBlendspace_Detach_Down;
        OverrideableProperty<FixedString> LadderBlendspace_Detach_Up;
    };


    struct ItemTemplate : public EoCGameObjectTemplate2
    {
        CombatComponentTemplate CombatComponent;
        OverrideableProperty<FixedString> Icon;
        OverrideableProperty<bool> CanBePickedUp;
        OverrideableProperty<bool> CanBeMoved;
        OverrideableProperty<bool> Destroyed;
        OverrideableProperty<bool> IsInteractionDisabled;
        OverrideableProperty<bool> StoryItem;
        OverrideableProperty<bool> FreezeGravity;
        OverrideableProperty<bool> IsPlatformOwner;
        OverrideableProperty<bool> IsKey;
        OverrideableProperty<bool> IsTrap;
        OverrideableProperty<bool> IsSurfaceBlocker;
        OverrideableProperty<bool> IsSurfaceCloudBlocker;
        OverrideableProperty<bool> TreasureOnDestroy;
        OverrideableProperty<bool> HardcoreOnly;
        OverrideableProperty<bool> NotHardcore;
        OverrideableProperty<bool> UsePartyLevelForTreasureLevel;
        OverrideableProperty<bool> Unimportant;
        OverrideableProperty<bool> Hostile;
        OverrideableProperty<bool> UseOnDistance;
        OverrideableProperty<bool> UseRemotely;
        OverrideableProperty<bool> PhysicsFollowAnimation;
        OverrideableProperty<TranslatedString> UnknownDisplayName;
        OverrideableProperty<uint32_t> Tooltip;
        OverrideableProperty<FixedString> Stats;
        OverrideableProperty<ObjectSet<FixedString>> InventoryList;
        OverrideableProperty<ObjectSet<void*>> OnUsePeaceActions;
        OverrideableProperty<ObjectSet<void*>> OnDestroyActions;
        OverrideableProperty<TranslatedString> OnUseDescription;
        OverrideableProperty<ObjectSet<void*>> Scripts;
        OverrideableProperty<VirtualMultiHashMap<FixedString, void*>> ScriptOverrides;
        OverrideableProperty<ObjectSet<void*>> ScriptConfigGlobalParameters;
        OverrideableProperty<FixedString> AnubisConfigName;
        OverrideableProperty<ObjectSet<void*>> ItemList;
        OverrideableProperty<ObjectSet<void*>> StatusList;
        OverrideableProperty<FixedString> DefaultState;
        OverrideableProperty<FixedString> Owner;
        OverrideableProperty<FixedString> Key;
        OverrideableProperty<FixedString> BloodType;
        OverrideableProperty<int32_t> LockDC;
        OverrideableProperty<int32_t> DisarmDC;
        OverrideableProperty<int32_t> Amount;
        OverrideableProperty<int32_t> MaxStackAmount;
        OverrideableProperty<int32_t> TreasureLevel;
        OverrideableProperty<void*> SomeTemplateInfo;
        OverrideableProperty<FixedString> DropSound;
        OverrideableProperty<FixedString> PickupSound;
        OverrideableProperty<FixedString> UseSound;
        OverrideableProperty<FixedString> EquipSound;
        OverrideableProperty<FixedString> UnequipSound;
        OverrideableProperty<FixedString> InventoryMoveSound;
        OverrideableProperty<FixedString> ImpactSound;
        OverrideableProperty<FixedString> PhysicsCollisionSound;
        OverrideableProperty<uint8_t> SoundObjectIndex;
        OverrideableProperty<uint8_t> BloodSurfaceType;
        OverrideableProperty<uint8_t> InventoryType;
        OverrideableProperty<TranslatedString> Description;
        OverrideableProperty<TranslatedString> UnknownDescription;
        VirtualMultiHashSet<Guid>* SpeakerGroupList;
        VirtualMultiHashSet<Guid>* InteractionFilterList;
        OverrideableProperty<uint8_t> InteractionFilterType;
        OverrideableProperty<FixedString> ActivationGroupId;
        OverrideableProperty<int32_t> Race;
        OverrideableProperty<int32_t> LevelOverride;
        OverrideableProperty<bool> IsSourceContainer;
        OverrideableProperty<FixedString> MeshProxy;
        OverrideableProperty<FixedString> ShortHair;
        OverrideableProperty<FixedString> LongHair;
        OverrideableProperty<bool> IsPublicDomain;
        OverrideableProperty<bool> IgnoreGenerics;
        OverrideableProperty<bool> AllowSummonTeleport;
        OverrideableProperty<bool> IsPortalProhibitedToPlayers;
        OverrideableProperty<uint8_t> LightChannel;
        OverrideableProperty<Guid> EquipmentTypeID;
        bool SomeVersionFlag;
        OverrideableProperty<ObjectSet<FixedString>> AttachedRoomTriggerList;
    };


    struct ProjectileTemplate : public EoCGameObjectTemplate
    {
        OverrideableProperty<float> LifeTime;
        OverrideableProperty<float> Speed;
        OverrideableProperty<float> Acceleration;
        OverrideableProperty<FixedString> CastBone;
        OverrideableProperty<FixedString> ImpactFX;
        OverrideableProperty<STDString> GroundImpactFX;
        OverrideableProperty<FixedString> TrailFX;
        OverrideableProperty<bool> DestroyTrailFXOnImpact;
        OverrideableProperty<FixedString> BeamFX;
        OverrideableProperty<FixedString> PreviewPathMaterial;
        OverrideableProperty<FixedString> PreviewPathImpactFX;
        OverrideableProperty<float> PreviewPathRadius;
        OverrideableProperty<bool> RotateImpact;
        OverrideableProperty<bool> IgnoreRoof;
        OverrideableProperty<bool> DetachBeam;
        OverrideableProperty<bool> NeedsImpactSFX;
        OverrideableProperty<uint8_t> ProjectilePath;
        OverrideableProperty<float> PathShift;
        OverrideableProperty<float> PathRadius;
        OverrideableProperty<float> MinPathRadius;
        OverrideableProperty<float> MaxPathRadius;
        OverrideableProperty<float> PathMinArcDist;
        OverrideableProperty<float> PathMaxArcDist;
        OverrideableProperty<int32_t> PathRepeat;
    };

    struct SurfaceTemplate : public GameObjectTemplate
    {
        struct VisualData
        {
            FixedString Visual;
            glm::vec2 Height;
            glm::ivec2 Rotation;
            glm::vec2 Scale;
            float GridSize;
            int SpawnCell;
            int RandomPlacement;
            int SurfaceNeeded;
            int SurfaceRadiusMax;
        };

        struct StatusData
        {
            FixedString StatusId;
            uint8_t ApplyTypes;
            float Chance;
            float Duration;
            bool Remove;
            bool ApplyToCharacters;
            bool ApplyToItems;
            bool KeepAlive;
            bool VanishOnApply;
            bool Force;
            bool AffectedByRoll;
            bool OnlyOncePerTurn;
        };

        SurfaceType SurfaceType;
        FixedString SurfaceName;
        uint64_t field_158[4];
        uint32_t field_188;
        OverrideableProperty<TranslatedString> DisplayName;
        OverrideableProperty<TranslatedString> Description;
        OverrideableProperty<FixedString> DecalMaterial;
        OverrideableProperty<uint8_t> MaterialType;
        OverrideableProperty<uint8_t> SurfaceCategory;
        OverrideableProperty<bool> CanEnterCombat;
        OverrideableProperty<bool> AlwaysUseDefaultLifeTime;
        OverrideableProperty<float> DefaultLifeTime;
        OverrideableProperty<float> SurfaceGrowTimer;
        OverrideableProperty<float> FadeInSpeed;
        OverrideableProperty<float> FadeOutSpeed;
        OverrideableProperty<float> FallDamageMultiplier;
        OverrideableProperty<int32_t> Seed;
        OverrideableProperty<float> NormalBlendingFactor;
        Array<VisualData> InstanceVisual;
        Array<VisualData> IntroFX;
        Array<VisualData> FX;
        Array<StatusData> Statuses;
        OverrideableProperty<FixedString> Summon;
        OverrideableProperty<bool> RemoveDestroyedItems;
        OverrideableProperty<bool> CanSeeThrough;
        OverrideableProperty<bool> CanShootThrough;
        OverrideableProperty<STDString> RollConditions;
        OverrideableProperty<uint8_t> ObscuredStateOverride;
        OverrideableProperty<float> OnEnterDistanceOverride;
        OverrideableProperty<float> OnMoveDistanceOverride;
        OverrideableProperty<glm::vec3> AiPathColor;
        OverrideableProperty<FixedString> AiPathIconFX;
    };
}
