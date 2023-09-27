#pragma once

#include <GameDefinitions/Base/Base.h>
#include "Enumerations.h"
#include "EntitySystem.h"
#include "Module.h"
#include <glm/gtc/quaternion.hpp>

BEGIN_NS(esv)

struct SavegameVisitor : public ObjectVisitor
{
	MultiHashMap<uint64_t, uint64_t> field_48;
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
	struct DescriptionInfo
	{
		TranslatedString DisplayNameTSK;
		FixedString Icon;
		TranslatedString Description;
		STDString DescriptionParams;
		TranslatedString ExtraDescription;
		STDString ExtraDescriptionParams;
		TranslatedString ShortDescription;
		STDString ShortDescriptionParams;
		TranslatedString LoreDescription;
	};

	struct GlobalSwitches
	{
		struct SoundSetting
		{
			char* ConfigValue;
			int field_8;
			FixedString ConfigKey;
			__int64 field_10;
			__int64 field_18;
			Array<void*> field_20;
			Array<void*> field_30;
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
			float field_0[33];
		};


		void* VMT;
		uint8_t field_8[0xA4];
		int MouseSensitivity;
		int MouseScrollSensitivity;
		int ControllerSensitivity;
		int ControllerStickDeadZone;
		int ControllerStickPressDeadZone;
		int ControllerTriggerDeadZone;
		char field_C4;
		char field_C5;
		char StoryLog;
		char Story;
		char StoryEvents;
		bool DisableStoryPatching;
		bool ForceStoryPatching;
		char field_CB;
		char Stats;
		char field_CD;
		char field_CE;
		char field_CF;
		int Modded;
		BYTE field_D4;
		STDString field_D8;
		BYTE field_F0;
		char field_F1;
		__int16 ForcePort;
		char EnablePortmapping;
		char field_F5;
		char field_F6;
		char field_F7;
		int GameVisibilityOnline;
		int GameVisibilityLAN;
		int GameVisibilityDirect;
		char field_104;
		char EnableSteamP2P;
		char field_106;
		char field_107;
		BYTE field_108;
		char field_109;
		char field_10A;
		char field_10B;
		BYTE field_10C;
		char field_10D;
		char field_10E;
		char field_10F;
		char field_110;
		char field_111;
		char field_112;
		char DoUnlearnCheck;
		char field_114;
		char field_115;
		char field_116;
		char field_117;
		BYTE field_118;
		char field_119;
		char field_11A;
		char CameraShakeEnabled;
		BYTE field_11C;
		char field_11D;
		char field_11E;
		char TwitchEnableOverlay;
		BYTE field_120;
		char field_121;
		char field_122;
		STDString field_128;
		STDString TwitchExtSecret;
		STDString field_158;
		STDString field_170;
		char DLCDebugEnable;
		STDString DLCDebugList;
		float LongRestScriptWaitingTime;
		STDString LongRestDefaultTimeline;
		char field_1C8;
		char field_1C9;
		char field_1CA;
		char useSaveSystemECBChecker;
		char saveSystemECBCheckerEnableLogging;
		char saveSystemECBCheckerEnableDetailedLogging;
		char saveSystemECBCheckerLogSuccessfulAttempts;
		char saveSystemECBCheckerAllowSaveOnFailure;
		int saveSystemECBCheckNumberOfFramesToWait;
		BYTE field_1D4;
		float field_1D8;
		float field_1DC;
		float field_1E0;
		float field_1E4;
		char UseLevelCache;
		char LoadAllEffectPools;
		char ScriptLog;
		char ShroudEnabled;
		char field_1EC;
		char field_1ED;
		char Fading;
		char CheckRequirements;
		char UseEndTurnFallback;
		char field_1F1;
		char ShowOriginIntroInCC;
		char field_1F3;
		char AlwaysShowSplitter;
		char field_1F5;
		char ControllerLayout;
		char NorthFacingMinimap;
		char ExtraTooltipDescriptions;
		char field_1F9;
		char field_1FA;
		char field_1FB;
		char field_1FC;
		char field_1FD;
		char field_1FE;
		char field_1FF;
		SoundSetting SoundSettings[12];
		int SoundVolumeDynamicRange;
		int SoundOcclusionQuality;
		BYTE MuteSoundWhenNotFocused;
		char CentreDialogue;
		char ForceMono;
		char field_68B;
		int SoundPartyLosingThreshold;
		int SoundPartyWinningThreshold;
		int _GameCameraRotation;
		float GameCameraSpeedMultiplier;
		BYTE GameCameraRotationLocked;
		char GameCameraEnableCloseUpDialog;
		char GameCameraEnableDynamicCombatCamera;
		char field_69F;
		char field_6A0;
		char CombatTimelines;
		char GameCameraEnableAttackCameraOtherPlayers;
		char GameCameraEnableFlyFollow;
		char GameCameraAutoRotateEnabled;
		char field_6A5;
		char field_6A6;
		char field_6A7;
		float GameCameraAutoRotateSpeed;
		char field_6AC;
		__int64 field_6B0;
		int field_6B8;
		int field_6BC;
		int NrOfQuickSaves;
		int NrOfAutoSaves;
		char CanAutoSave;
		char CanCrossSave;
		char CanSaveDuringTimeline;
		char CanLoadDuringTimeline;
		char FirstTimeWithCrossSave;
		char HasSeenEAMsg;
		char ForceSplitscreen;
		char field_6CF;
		int MaxAmountDialogsInLog;
		float field_6D4;
		char field_6D8;
		float ControllerCharacterWalkThreshold;
		float ControllerCharacterRunThreshold;
		float field_6E4;
		float field_6E8;
		float field_6EC;
		__int64 field_6F0;
		int ControllerRumbleStrength;
		int field_6FC;
		__int64 field_700;
		char field_708;
		char field_709;
		char field_70A;
		char field_70B;
		int field_70C;
		char field_710;
		char field_711;
		char field_712;
		char field_713;
		int field_714;
		char field_718;
		char field_719;
		char field_71A;
		char field_71B;
		char field_71C;
		char field_71D;
		char field_71E;
		char field_71F;
		__int64 field_720;
		char field_728;
		char field_729;
		char field_72A;
		char field_72B;
		char field_72C;
		char field_72D;
		char field_72E;
		char field_72F;
		BYTE field_730;
		char EnableVoiceLogging;
		char EnableLongPressAndHold;
		char MouseLock;
		BYTE DisableEdgePanning;
		char EscClosesAllUI;
		char ShowSubtitles;
		char DisplaySpeaker;
		char ShowTextBackground;
		char field_739;
		char field_73A;
		char field_73B;
		int TextBackgroundOpacity;
		int DialogueFontSizeModifier;
		int OverheadFontSizeModifier;
		char field_748;
		char UseRadialContextMenu;
		char LogSaveLoadErrors;
		char field_74B;
		char field_74C;
		char field_74D;
		char field_74E;
		char field_74F;
		BYTE field_750;
		char field_751;
		char field_752;
		char field_753;
		char field_754;
		char field_755;
		char field_756;
		char field_757;
		Array<void*> field_758;
		int field_768;
		FixedString field_76C;
		char PeaceCharacterHighlightMode;
		char CombatCharacterHighlightMode;
		char TacticalCharacterHighlightMode;
		char UnitSystem;
		char WeightedRolls;
		char field_775;
		char field_776;
		char PostCombatAssignation;
		char field_778;
		char field_779;
		char field_77A;
		char field_77B;
		char field_77C;
		char field_77D;
		char field_77E;
		char field_77F;
		int field_780;
		int field_784;
		int ServerFrameCap;
		std::array<GlobalCameraSwitches, 4> CameraSwitches;
		ControlSwitch field_DDC;
		std::array<glm::vec4, 72> OverlayColors;
		char AutoRemoveHotbarSpells;
		char field_12E1;
		__int16 AutoFillHotbarCategories;
		int field_12E4;
		char field_12E8;
		char field_12E9;
		char field_12EA;
		char field_12EB;
		float field_12EC;
		float field_12F0;
		BYTE TimelinesAD;
		char TimelinesAttitudeMimicry;
		char TimelinesEmoteMimicry;
		char TimelinesLookAtMimicry;
		char field_12F8;
		char TimelinesAutoContinue;
		char field_12FA;
		char TimelinesShowDisabled;
		char TimelinesQCameras;
		char field_12FD;
		char TimelinesCombatAuthored;
		char field_12FF;
		char TimelinesFade;
		char field_1301;
		char field_1302;
		char field_1303;
		float field_1304;
		float field_1308;
		BYTE field_130C;
		char field_130D;
		char ShowGenitals;
		char ShowExplicitRomanceScenes;
		char NoPrivateDialogues;
		char field_1311;
		char field_1312;
		char field_1313;
		char field_1314;
		char field_1315;
		char field_1316;
		char field_1317;
		char field_1318;
		int field_131C;
		int field_1320;
		char field_1324;
		char field_1325;
		char field_1326;
		int field_1328;
		int field_132C;
		int field_1330;
		int field_1334;
		float field_1338;
		int field_133C;
		int field_1340;
		int field_1344;
		char field_1348;
		int field_134C;
		float field_1350;
		float field_1354;
		float field_1358;
		float field_135C;
		float field_1360;
		float field_1364;
		float field_1368;
		float field_136C;
		float field_1370;
		BYTE field_1374;
		char field_1375;
		char field_1376;
		char field_1377;
		char field_1378;
		char field_1379;
		char field_137A;
		char field_137B;
		char field_137C;
		char field_137D;
		float field_1380;
		float field_1384;
		float field_1388;
		char field_138C;
		char field_138D;
		char ShowTutorials;
		char ShowTutorialsAnswered;
		BYTE ShowPings;
		char ShowCombatFeed;
		char ShowFirstTimeSetup;
		char field_1393;
		char field_1394;
		float PointAndClickSoundFrequencyFloat;
		STDString ActiveCustomDice;
		int field_13B8;
		int field_13BC;
		int field_13C0;
		char field_13C4;
		char field_13C5;
		char field_13C6;
		char field_13C7;
		char field_13C8;
		int StartYear;
		int StartDay;
		char field_13D8;
		char SkipLarianSignUp;
		char field_13DA;
		char field_13DB;
		char HideCompletedQuests;
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
		Map<FixedString, STDWString*>* TranslatedStrings[4];
		Map<FixedString, STDWString*>* TranslatedStringOverrides[4];
		Map<FixedString, TranslatedArgumentString*> TranslatedArgumentStrings;
		Map<FixedString, RuntimeStringHandle> StringHandles;
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
		Map<FixedString, TranslatedString> StringKeys;
		Map<FixedString, FixedString> StringSpeakers;
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
			RefMap<FixedString, RefMap<FixedString, VoiceTextMetaData>> * SpeakerMetaDataHashMap;
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
		Map<FixedString, void*> field_60;
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
		void * FileObject;
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
