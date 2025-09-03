#pragma once

#include <GameDefinitions/Base/Base.h>
#include "Enumerations.h"
#include "EntitySystem.h"
#include "Module.h"
#include <glm/gtc/quaternion.hpp>

BEGIN_NS(esv)

struct SavegameVisitor : public ObjectVisitor
{
    HashMap<uint64_t, uint64_t> field_48;
    __int64 field_88;
    ModuleSettings Settings;
    STDString ModuleHash;
    ObjectVisitor* LSFVisitor;
    bool OwnsLSFVisitor;
    __int64 field_E0;
    __int64 field_E8;
};

struct OsirisVariableHelper : public ProtectedGameObject<OsirisVariableHelper> {};

using OsirisVariableHelper__SavegameVisitProc = bool (OsirisVariableHelper*, SavegameVisitor*);

END_NS()

namespace bg3se
{
    using FindFirstFileWProc = WINBASEAPI HANDLE WINAPI(_In_ LPCWSTR lpFileName, _Out_ LPWIN32_FIND_DATAW lpFindFileData);
    using FindNextFileWProc = WINBASEAPI BOOL WINAPI(_In_ HANDLE hFindFile, _Out_ LPWIN32_FIND_DATAW lpFindFileData);
    using FindCloseProc = WINBASEAPI BOOL WINAPI(_Inout_ HANDLE hFindFile);

    struct CoreGlobals
    {
        glm::vec2 MousePos;
        GameTime RealTime;
        GameTime Time;
        GameTime field_38;
        void* GlobalWorkerPool;
        uint8_t InputType;
    };


    struct GlobalSwitches
    {
        struct SoundSetting
        {
            float ConfigValue[3];
            FixedString ConfigKey;
            HashSet<FixedString> RTPCs;
            char field_40;
            TranslatedString field_44;
            void* UpdateProc;
        };

        struct GlobalCameraSwitches
        {
            float field_0[101];
        };

        struct ControlSwitch
        {
            float field_0[37];
        };

        struct SomeSetting
        {
            __int64 field_0;
            __int64 field_8;
        };

        void* VMT;
        uint8_t field_8;
        uint8_t field_9;
        uint8_t field_A;
        uint8_t field_B;
        uint8_t field_C;
        uint8_t field_D;
        uint8_t field_E;
        uint8_t field_F;
        float field_10;
        uint8_t field_14;
        uint8_t field_15;
        float field_18;
        int field_1C;
        float field_20;
        float field_24;
        float field_28;
        float field_2C;
        float field_30;
        int UIScaling;
        uint8_t ControllerMode;
        uint8_t field_39;
        float NodeWaitTimeMultiplier;
        float AnimationBlueprintSpeedScale_M;
        float field_44;
        float field_48;
        int field_4C;
        int field_50;
        float field_54;
        float field_58;
        float field_5C;
        float field_60;
        int field_64;
        int field_68;
        uint8_t field_6C;
        uint8_t field_6D;
        uint8_t field_6E;
        uint8_t field_6F;
        uint8_t field_70;
        uint8_t field_71;
        float field_74;
        float field_78;
        float field_7C;
        float field_80;
        float field_84;
        float field_88;
        uint8_t field_8C;
        uint8_t field_8D;
        uint8_t field_8E;
        uint8_t field_8F;
        int MouseSensitivity;
        int MouseScrollSensitivity;
        int ControllerSensitivityLeft;
        int ControllerSensitivityRight;
        int ControllerStickDeadZone;
        int ControllerStickPressDeadZone;
        int ControllerTriggerDeadZone;
        bool field_AC;
        bool ServerMonitor;
        bool SaveStory;
        bool StoryLog;
        bool Story;
        bool StoryEvents;
        bool DisableStoryPatching;
        bool ForceStoryPatching;
        bool RandomStartOrigin;
        bool Stats;
        int Modded;
        uint8_t field_BC;
        STDString Language;
        bool ShowLocalizationMarkers;
        int16_t ForcePort;
        bool EnablePortmapping;
        uint32_t GameVisibilityOnline;
        uint32_t GameVisibilityLAN;
        uint32_t GameVisibilityDirect;

        bool CrossplayEnabled;
        bool CrossplayInUse;
        bool field_EE;
        bool field_EF;
        bool field_F0;
        STDString ScreenshotDir;
        bool AutoListenEnabled;
        bool DisableLocalMessagePassing;
        bool EnableSteamP2P;
        uint8_t field_113;
        uint8_t field_114;
        uint8_t field_115;

        uint16_t field_F4;
        uint16_t field_F6;
        uint8_t field_F8;
        uint8_t field_F9;
        uint8_t field_FA;
        uint8_t field_FB;
        uint8_t field_FC;
        bool DoUnlearnCheck;
        uint8_t field_FE;
        uint8_t field_FF;

        uint8_t field_100;
        uint8_t field_101;
        uint8_t field_102;
        uint8_t field_103;
        uint8_t field_104;
        bool CameraShakeEnabled;
        uint8_t field_105;
        uint8_t field_106;
        uint8_t field_107;

        bool TwitchSomeFlag_108;
        bool TwitchEnableOverlay;
        uint8_t field_108;
        bool TwitchUseLarianAuth;
        STDString EBSUrl;
        STDString TwitchExtSecret;
        STDString TwitchExtSecret2_M;
        STDString field_158;
        float LongRestScriptWaitingTime;
        STDString LongRestDefaultTimeline;
        uint8_t field_190;
        uint8_t field_191;
        bool useSaveSystemECBChecker;
        bool saveSystemECBCheckerEnableLogging;
        bool saveSystemECBCheckerEnableDetailedLogging;
        bool saveSystemECBCheckerLogSuccessfulAttempts;
        bool saveSystemECBCheckerAllowSaveOnFailure;
        int saveSystemECBCheckNumberOfFramesToWait;
        uint8_t field_19C;
        float field_1A0;
        float field_1A4;

        bool UseLevelCache;
        bool LoadAllEffectPools;
        bool ScriptLog;
        bool ShroudEnabled;
        bool field_1AC;
        bool GodMode;
        bool Fading;
        bool CheckRequirements;

        bool UseEndTurnFallback;
        bool ShowCharacterCreation;
        bool ShowOriginIntroInCC;
        bool UseSavingThrows;
        bool AlwaysShowSplitter;
        bool field_1B5;
        uint8_t ControllerLayout;
        bool NorthFacingMinimap;
        
        bool HoldToEndTurn;
        uint8_t ExtraTooltipDescriptions;
        uint8_t field_1BA;
        uint8_t field_1BB;
        uint8_t field_1BC;
        uint8_t field_1BD;
        uint8_t field_1BE;
        uint8_t field_1BF;
        
        SoundSetting SoundSettings[12];
        int SoundVolumeDynamicRange;
        int SoundOcclusionQuality;
        bool MuteSoundWhenNotFocused;
        bool CentreDialogue;
        bool ForceMono;
        int SoundPartyLosingThreshold;
        int SoundPartyWinningThreshold;

        float GameCameraRotation;
        float GameCameraSpeedMultiplier;

        bool GameCameraRotationLocked;
        bool GameCameraEnableCloseUpDialog;
        bool GameCameraEnableDynamicCombatCamera;
        uint8_t field_65F;
        bool GameCameraEnableAttackCamera;
        bool GameCameraEnableAttackCameraOtherPlayers;
        bool GameCameraEnableFlyFollow;

        uint8_t field_664;
        uint8_t field_665;
        bool GameCameraAutoRotateEnabled;
        float GameCameraAutoRotateSpeed;
        uint8_t field_66C;
        uint8_t field_66D;
        uint8_t field_66E;
        float field_670;
        float field_674;
        bool field_698;
        bool field_699;
        bool field_69A;
        bool field_69B;
        bool field_69C;

        int NrOfQuickSaves;
        int NrOfAutoSaves;
        int MaxNrOfQuickSaves;
        int MaxNrOfAutoSaves;
        bool CanAutoSave;
        bool CanCrossSave;
        bool CanSaveDuringTimeline;
        bool CanLoadDuringTimeline;
        bool FirstTimeWithCrossSave;
        bool HasSeenEAMsg;
        bool ForceSplitscreen;
        int MaxAmountDialogsInLog;

        float FadeSpeed;
        float ControllerCharacterWalkThreshold;
        float ControllerCharacterRunThreshold;
        float field_6A4;
        float field_6AC;
        float field_6B0;
        int ControllerRumbleStrength;
        uint64_t field_6C0;
        
        uint8_t field_6C8;
        uint8_t field_6C9;
        uint8_t field_6CA;
        uint8_t field_6CB;
        int field_6CC;
        
        uint8_t field_6D0;
        uint8_t field_6D1;
        uint8_t field_6D2;
        uint8_t field_6D3;
        int field_6D4;
        
        uint64_t field_6D8;
        uint64_t field_6E0;
        uint8_t field_6E8;
        uint8_t field_6E9;
        uint8_t field_6EA;
        uint8_t field_6EB;
        uint8_t field_6EC;
        uint8_t field_6ED;
        uint8_t field_6EE;
        bool PeaceMode;
        
        bool EnableVoiceLogging;
        bool EnableLongPressAndHold;
        bool MouseLock;
        bool DisableEdgePanning;
        bool EscClosesAllUI;
        bool ShowSubtitles;
        bool DisplaySpeaker;
        bool ShowTextBackground;
        
        int TextBackgroundOpacity;
        int DialogueFontSizeModifier;
        int OverheadFontSizeModifier;
        int field_704;
        
        uint8_t field_708;
        bool UseRadialContextMenu;
        bool LogSaveLoadErrors;
        uint8_t field_70B;
        float field_70C;

        uint8_t field_710;
        uint8_t field_711;
        uint8_t field_712;
        uint8_t field_713;
        uint8_t field_714;
        uint8_t field_715;
        uint8_t Difficulty;

        Array<Guid> Rulesets;
        // FIXME: First variant is void
        HashMap<Guid, std::variant<uint8_t, int32_t, float, FixedString, bool>> RulesetModifiers;
        uint8_t field_768;
        FixedString field_76C;

        uint8_t PeaceCharacterHighlightMode;
        uint8_t CombatCharacterHighlightMode;
        uint8_t TacticalCharacterHighlightMode;
        uint8_t UnitSystem;
        bool WeightedRolls;
        uint8_t field_775;
        uint8_t field_776;
        uint8_t PostCombatAssignation;

        uint8_t field_778;
        uint8_t field_779;
        uint8_t field_77A;
        uint8_t field_77B;
        uint8_t field_77C;
        uint8_t field_77D;
        uint8_t field_77E;
        int ServerFrameCap;

        GlobalCameraSwitches CameraSwitches[4];
        ControlSwitch ControlSwitches;
        SomeSetting SomeSettings[72];

        uint8_t AutoRemoveHotbarSpells;
        uint8_t field_12F1;
        uint16_t AutoFillHotbarCategories;
        int field_12F4;
        uint8_t field_12F8;
        uint8_t field_12F9;
        uint8_t field_12FA;
        uint8_t field_12FB;
        float field_12FC;
        float field_1300;

        bool TimelinesAD;
        bool TimelinesAttitudeMimicry;
        bool TimelinesEmoteMimicry;
        bool TimelinesLookAtMimicry;
        uint8_t field_1308;
        bool TimelinesAutoContinue;
        uint8_t field_130A;
        bool TimelinesShowDisabled;

        bool TimelinesQCameras;
        uint8_t field_130D;
        bool TimelinesCombatAuthored;
        uint8_t field_130F;
        bool TimelinesFade;
        uint8_t field_1311;
        uint8_t field_1312;
        uint8_t field_1313;
        float field_1314;
        float field_1318;

        uint8_t field_131C;
        uint8_t field_131D;
        uint8_t field_131E;
        bool ShowGenitals;
        bool ShowExplicitRomanceScenes;
        bool NoPrivateDialogues;
        uint8_t field_1321;
        bool EnableGameplayLightSystem;
        uint8_t field_1323;
        float field_1324;

        int field_1328;
        int field_132C;
        int field_1330;
        uint8_t field_1334;
        uint8_t field_1335;
        uint8_t field_1336;
        int field_1338;
        float field_133C;
        int field_1340;
        int field_1350;
        int field_1354;
        int field_1358;

        uint8_t field_1358x;
        float field_135C;
        float field_1360;

        float field_1364;
        float field_1368;
        float field_136C;
        float field_1370;
        float field_1374;
        float field_1378;
        float field_137C;
        float field_1380;
        uint8_t field_1384;
        uint8_t field_1385;
        uint8_t field_1386;
        uint8_t field_1387;
        uint8_t field_1388;
        uint8_t field_1389;
        uint8_t field_138A;
        uint8_t field_138B;
        uint8_t field_138C;
        uint8_t field_138D;
        float field_1390;
        float field_1394;
        float field_1398;

        float field_1398x;
        float field_139C;
        float field_13A0;

        uint8_t field_13A0_X;
        uint8_t field_13A0_Y;
        bool ShowTutorials;
        bool ShowTutorialsAnswered;
        bool ShowPings;
        bool ShowCombatFeed;
        bool ShowCombatFeedSplitScreen;
        bool ShowFirstTimeSetup;
        uint8_t field_13A3;
        uint8_t field_13A4;
        // bool WasLarianModPolicyAccepted; - Unknown?
        float PointAndClickSoundFrequency;
        STDString ActiveCustomDice;
        uint8_t field_13C8;
        int field_13CC;
        uint8_t field_13C0;
        bool AiEnableSwarm;
        uint8_t field_13D2;
        uint8_t field_13D3;
        uint8_t field_13D4;
        uint8_t field_13D5;
        bool AnalyticsLevelEnded;
        bool AnalyticsSessionEnded;
        int StartYear;
        int StartDay;
        int field_13E0;
        uint8_t UIType;
        bool SkipLarianSignUp;
        bool SkipSplashScreen;
        uint8_t field_13E7;
        bool HideCompletedQuests;
        uint8_t field_13E8;
        uint8_t field_13E9;
        int field_13FC;
        bool field_1400;
    };


    /*

    struct InputValue
    {
        int field_0;
        int field_4;
        bool field_8;
    };

    struct InputEvent
    {
        int EventId;
        int InputPlayerIndex;
        int16_t InputDeviceId;
        InputValue OldValue;
        InputValue NewValue;
        InputType Type;
        bool WasPreferred;
    };


    struct TranslatedArgumentString
    {
        void* VMT{ nullptr };
        ObjectSet<void*>* ArgumentObjectSet{ nullptr };
        bool HasArgumentSet{ false };
        void* ArgumentBuf{ nullptr };
        STDWString Unknown;
        uint32_t ArgumentBufSize{ 0 };
        TranslatedString TranslatedStr;
    };

    struct TranslatedStringRepository : public ProtectedGameObject<TranslatedStringRepository>
    {
        typedef TranslatedStringRepository* (*GetInstance)();
        typedef STDWString * (*Get)(TranslatedStringRepository* self, RuntimeStringHandle* handle, uint64_t * length, char gender1, char gender2, bool unknown);
        typedef void (*UnloadOverrides)(TranslatedStringRepository* self);

        void* VMT;
        int field_8;
        LegacyMap<FixedString, STDWString*>* TranslatedStrings[4];
        LegacyMap<FixedString, STDWString*>* TranslatedStringOverrides[4];
        LegacyMap<FixedString, TranslatedArgumentString*> TranslatedArgumentStrings;
        LegacyMap<FixedString, RuntimeStringHandle> StringHandles;
        CRITICAL_SECTION CriticalSection;
        ScratchBuffer ScratchBuf;
        bool DoDecoration;
        bool IsLoaded;
        STDWString NotFoundStr;
    };

    struct TranslatedStringKeyManager : public ProtectedGameObject<TranslatedStringKeyManager>
    {
        typedef TranslatedStringKeyManager* (*GetInstance)();
        typedef TranslatedString * (*GetTranlatedStringFromKey)(TranslatedStringKeyManager* self, TranslatedString& string, FixedString const& handle, bool unknown);

        void* VMT;
        ObjectSet<Path> Paths;
        LegacyMap<FixedString, TranslatedString> StringKeys;
        LegacyMap<FixedString, FixedString> StringSpeakers;
        bool Unknown;
    };*/

    namespace eoc
    {
        /*struct PathMover
        {
            glm::quat PathRotateOrig;
            glm::quat PathRotateTarget;
            glm::vec3 StartingPosition;
            glm::vec3 DestinationPos;
            float PathInterpolateValue;
            float PathSpeedSet;
            float PathSpeed;
            float PathAcceleration;
            uint8_t PathType;
            int PathRotationType;
            float PathRadius;
            float PathShift;
            float PathMinArcDist;
            float PathMaxArcDist;
            uint64_t PathRepeat;
            uint8_t PathRandom;
        };

        struct VoiceTextMetaData
        {
            float Length;
            bool IsRecorded;
            int Priority;
            Path Source;
            int CodecID;
        };

        struct SpeakerManager
        {
            void * VMT;
            LegacyRefMap<FixedString, LegacyRefMap<FixedString, VoiceTextMetaData>> * SpeakerMetaDataHashMap;
        };*/
    }

    namespace esv
    {

    /*struct SummonHelperResults
    {
        ComponentHandle SummonHandle;
        uint32_t Unknown{ 0 };
    };

    struct SummonHelperSummonArgs
    {
        ComponentHandle OwnerCharacterHandle;
        FixedString GameObjectTemplateFS;
        FixedString Level;
        glm::vec3 Position{ .0f };
        int32_t SummonLevel{ -1 };
#if defined(OSI_EOCAPP)
        int32_t SummoningAbilityLevel;
#endif
        float Lifetime{ 0.0f };
        bool IsTotem{ false };
        bool MapToAiGrid{ true };
    };

    typedef void(*SummonHelpers__Summon)(SummonHelperResults * Results, SummonHelperSummonArgs * Args);*/

    struct SavegameManager : public ProtectedGameObject<SavegameManager>
    {
        ecs::EntityWorld* EntityWorld;
        __int64 field_8;
        __int64 field_10;
        __int64 field_18;
        __int64 field_20;
        void* Loaders; // esv::SavegameManager::Loaders*
        __int64 field_30;
        int field_38;
        int field_3C;
        __int64 field_40;
        __int64 field_48;
        int field_50;
        int field_54;
        __int64 field_58;
        LegacyMap<FixedString, void*> field_60;
        STDWString field_78;
        ObjectSet<void*> ComponentFactories;
        BYTE field_B0;
        char field_B1;
        char field_B2;
        char field_B3;
        char field_B4;
        char field_B5;
        int field_B8;
        ObjectSet<STDString> OS;
        __int64 field_D8;
        __int64 field_E0;
        int field_E8;
        Array<void*> field_F0;
        Array<void*> field_108;
        ScratchBuffer NewAge;
        __int64 field_178;
        uint8_t LevelCacheDesc[112];
        __int64 field_1F0;
        __int64 field_1F8;
    };

    }

    enum class FileType
    {
        Unknown,
        MemBuffer,
        MemoryMapped
    };

    struct FileReader : public Noncopyable<FileReader>
    {
        using CtorProc = FileReader * (FileReader* self, Path const& path, unsigned int type, unsigned int unknown);
        using DtorProc = void (FileReader* self);

        bool IsLoaded{ false };
        void* DataPtr{ nullptr };
        void* ReadPtr{ nullptr };
        uint64_t FileSize{ 0 };
        uint64_t FileHandle{ 0 };
        uint64_t FileHandle2{ 0 };
        ScratchBuffer Buffer;
        FileType Type{ FileType::Unknown };
        void * FileObject{ nullptr };
    };

    class FileReaderPin
    {
    public:
        inline FileReaderPin(FileReader * reader)
            : reader_(reader)
        {}

        ~FileReaderPin();

        FileReaderPin(FileReaderPin const &) = delete;
        FileReaderPin & operator =(FileReaderPin const &) = delete;
        FileReaderPin & operator =(FileReaderPin &&) = delete;

        inline FileReaderPin(FileReaderPin && other) noexcept
        {
            reader_ = other.reader_;
            if (&other != this) {
                other.reader_ = nullptr;
            }
        }

        bool IsLoaded() const
        {
            return reader_ != nullptr && reader_->IsLoaded;
        }

        void * Buf() const
        {
            if (IsLoaded()) {
                return reader_->DataPtr;
            } else {
                return nullptr;
            }
        }

        std::size_t Size() const
        {
            if (IsLoaded()) {
                return reader_->FileSize;
            } else {
                return 0;
            }
        }

        STDString ToString() const;

    private:
        FileReader * reader_;
    };

    using CoreLibInitProc = void (void*);
    using AppUpdatePathsProc = void (void*);

    class TempStrings
    {
    public:
        char const * Make(STDString const & str);
        char const * Make(std::string const & str);

    private:
        std::vector<char *> pool_;
    };

    extern TempStrings gTempStrings;

    /*struct App
    {
    public:
        struct VMT
        {
            using OnInputEventProc = void (App* self, uint16_t& retval, InputEvent const& inputEvent);

            void* Destroy;
            void* GetInputListenerPriority;
            void* GetInputListenerFilter;
            void* GetInputListenerName;
            void* IsModal;
            void* InTextInput;
            OnInputEventProc* OnInputEvent;
            void* OnInputModifierChangedEvent;
            void* OnInputEventText;
            void* OnUnlinkedInput;
        };

        VMT* __vftable;
        void* InputDeviceListenerVMT;
        void* WindowEventListenerVMT;
        void* API;
    };*/
}
