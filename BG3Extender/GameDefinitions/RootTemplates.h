#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>

namespace bg3se
{
    template <class T>
    struct OverrideableProperty
    {
        using Type = T;

        T Value;
        bool IsOverridden;
    };

    struct GameObjectTemplate
    {
        void* VMT;
        uint64_t Flag;
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
        uint64_t field_68;
        Transform Transform;
        uint8_t _Pad2[4];
        bool NonUniformScale;
        void* Unknown;
        OverrideableProperty<FixedString> VisualTemplate;
        OverrideableProperty<FixedString> PhysicsTemplate;
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
        bool HasGameplayValue;
        STDString FileName;
    };

    struct EoCGameObjectTemplate : public GameObjectTemplate
    {
        uint64_t Unknown;
        OverrideableProperty<ObjectSet<void*>> AIBounds;
        TranslatedString DisplayName;
        uint8_t _Pad10[1];
        OverrideableProperty<float> Opacity;
        uint8_t _Pad11[1];
        OverrideableProperty<bool> Fadeable;
        OverrideableProperty<bool> FadeIn;
        OverrideableProperty<bool> SeeThrough;
        OverrideableProperty<bool> HierarchyOnlyFade;
        OverrideableProperty<FixedString> FadeGroup;
        ObjectSet<FixedString> FadeChildren;
        OverrideableProperty<uint32_t> GameMasterSpawnSubSection;
        uint64_t Unknown4[4];
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
        OverrideableProperty<uint64_t> SoundAttenuation;
        OverrideableProperty<GUID> HLOD;
    };

    struct CombatComponentTemplate
    {
        void* VMT;
        OverrideableProperty<FixedString> Archetype;
        OverrideableProperty<GUID> Alignment;
        OverrideableProperty<bool> CanFight;
        OverrideableProperty<bool> CanJoinCombat;
        OverrideableProperty<FixedString> CombatGroupID;
        OverrideableProperty<STDString> CombatName;
        OverrideableProperty<bool> IsBoss;
        OverrideableProperty<bool> StayInAiHints;
        OverrideableProperty<GUID> AiHint;
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
        OverrideableProperty<ObjectSet<FixedString>> Treasures;
        OverrideableProperty<ObjectSet<FixedString>> TradeTreasures;
        OverrideableProperty<FixedString> LightID;
        OverrideableProperty<FixedString> ActiveCharacterLightID;
        OverrideableProperty<FixedString> BloodType;
        OverrideableProperty<FixedString> DefaultDialog;
        void* SpeakerGroupList;
        OverrideableProperty<STDString> GeneratePortrait;
        OverrideableProperty<float> LadderAttachSpeed;
        OverrideableProperty<float> LadderLoopSpeed;
        OverrideableProperty<float> LadderDetachSpeed;
        OverrideableProperty<bool> CanShootThrough;
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
        OverrideableProperty<bool> IsHuge;
        OverrideableProperty<GUID> EquipmentRace;
        OverrideableProperty<ObjectSet<void*>> OnDeathActions;
        OverrideableProperty<float> DeathRaycastMinLength;
        OverrideableProperty<float> DeathRaycastMaxLength;
        OverrideableProperty<float> DeathRaycastHeight;
        OverrideableProperty<float> DeathRaycastVerticalLength;
        int field_3D8;
        OverrideableProperty<FixedString> ExplodedResourceID;
        OverrideableProperty<FixedString> ExplosionFX;
        OverrideableProperty<ObjectSet<void*>> Scripts;
        OverrideableProperty<FixedString> AnubisConfigName;
        OverrideableProperty<ObjectSet<void*>> SkillList;
        OverrideableProperty<ObjectSet<void*>> ItemList;
        OverrideableProperty<FixedString> TrophyID;
        OverrideableProperty<FixedString> SoundInitEvent;
        OverrideableProperty<uint8_t> SoundObjectIndex;
        OverrideableProperty<uint16_t> SoundAttenuation;
        OverrideableProperty<FixedString> FoleyShortResourceID;
        OverrideableProperty<FixedString> FoleyMediumResourceID;
        OverrideableProperty<FixedString> FoleyLongResourceID;
        OverrideableProperty<FixedString> VocalAlertResourceID;
        OverrideableProperty<FixedString> VocalAngryResourceID;
        OverrideableProperty<FixedString> VocalAnticipationResourceID;
        OverrideableProperty<FixedString> VocalAttackResourceID;
        OverrideableProperty<FixedString> VocalAwakeResourceID;
        OverrideableProperty<FixedString> VocalBoredResourceID;
        OverrideableProperty<FixedString> VocalDeathResourceID;
        OverrideableProperty<FixedString> VocalDodgeResourceID;
        OverrideableProperty<FixedString> VocalEffortsResourceID;
        OverrideableProperty<FixedString> VocalExhaustedResourceID;
        OverrideableProperty<FixedString> VocalFallResourceID;
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
        OverrideableProperty<FixedString> GhostTemplate;
        OverrideableProperty<bool> IsLootable;
        OverrideableProperty<bool> IsEquipmentLootable;
        OverrideableProperty<uint8_t> LightChannel;
        OverrideableProperty<uint8_t> AliveInventoryType;
        OverrideableProperty<uint8_t> InventoryType;
        OverrideableProperty<GUID> Race;
        OverrideableProperty<TranslatedString> Title;
        OverrideableProperty<FixedString> AnimationSetResourceID;
        OverrideableProperty<bool> HasPlayerApprovalRating;
        OverrideableProperty<bool> CanLongRest;
        OverrideableProperty<bool> DisableEquipping;
        OverrideableProperty<float> WorldClimbingSpeed;
        OverrideableProperty<bool> IsMovementEnabled;
        OverrideableProperty<float> MovementAcceleration;
        OverrideableProperty<float> MovementSpeedStroll;
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
    };


    struct ItemTemplate : public EoCGameObjectTemplate2
    {
        __int64 field_238;
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
        OverrideableProperty<bool> IsHuge;
        OverrideableProperty<bool> HardcoreOnly;
        OverrideableProperty<bool> NotHardcore;
        OverrideableProperty<bool> UsePartyLevelForTreasureLevel;
        OverrideableProperty<bool> Unimportant;
        OverrideableProperty<bool> Hostile;
        OverrideableProperty<bool> UseOnDistance;
        OverrideableProperty<bool> UseRemotely;
        OverrideableProperty<bool> PhysicsFollowAnimation;
        OverrideableProperty<FixedString> UnknownDisplayName;
        __int64 field_310;
        OverrideableProperty<uint32_t> Tooltip;
        OverrideableProperty<FixedString> Stats;
        OverrideableProperty<ObjectSet<FixedString>> InventoryList;
        OverrideableProperty<ObjectSet<void*>> OnUsePeaceActions;
        OverrideableProperty<ObjectSet<void*>> OnDestroyActions;
        OverrideableProperty<TranslatedString> OnUseDescription;
        OverrideableProperty<ObjectSet<void*>> Scripts;
        OverrideableProperty<ObjectSet<void*>> ScriptConfigGlobalParameters;
        OverrideableProperty<FixedString> AnubisConfigName;
        OverrideableProperty<ObjectSet<void*>> ItemList;
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
        __int64 field_460; // maybe DropSound?
        OverrideableProperty<FixedString> PickupSound;
        OverrideableProperty<FixedString> UseSound;
        OverrideableProperty<FixedString> EquipSound;
        OverrideableProperty<FixedString> UnequipSound;
        OverrideableProperty<FixedString> InventoryMoveSound;
        OverrideableProperty<FixedString> ImpactSound;
        OverrideableProperty<uint8_t> SoundObjectIndex;
        char field_49A;
        char field_49B;
        OverrideableProperty<uint8_t> InventoryType;
        OverrideableProperty<TranslatedString> Description;
        OverrideableProperty<TranslatedString> UnknownDescription;
        OverrideableProperty<FixedString> Speaker;
        OverrideableProperty<FixedString> AltSpeaker;
        ObjectSet<STDWString> SpeakerGroupList;
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
        OverrideableProperty<GUID> EquipmentTypeID;
        bool SomeVersionFlag;
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
            float Height[2];
            int Rotation[2];
            float Scale[2];
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

        uint64_t field_158[7]; // SurfaceTypeId, SurfaceType?
        OverrideableProperty<TranslatedString> DisplayName;
        OverrideableProperty<TranslatedString> Description;
        OverrideableProperty<FixedString> DecalMaterial;
        OverrideableProperty<uint8_t> MaterialType;
        OverrideableProperty<uint8_t> SurfaceCategory;
        OverrideableProperty<bool> CanEnterCombat;
        OverrideableProperty<bool> AlwaysUseDefaultLifeTime;
        OverrideableProperty<float> DefaultLifeTime;
        OverrideableProperty<float> SurfaceGrowTimer;
        float field_1D0;
        OverrideableProperty<float> FadeInSpeed;
        OverrideableProperty<float> FadeOutSpeed;
        OverrideableProperty<float> FallDamageMultiplier;
        OverrideableProperty<int32_t> Seed;
        OverrideableProperty<int32_t> NormalBlendingFactor;
        OverrideableProperty<ObjectSet<VisualData>> InstanceVisual;
        OverrideableProperty<ObjectSet<VisualData>> IntroFX;
        OverrideableProperty<ObjectSet<VisualData>> FX;
        OverrideableProperty<ObjectSet<StatusData>> Statuses;
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
