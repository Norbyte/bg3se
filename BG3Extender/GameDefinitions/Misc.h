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
	using FindFirstFileWProc = WINBASEAPI HANDLE WINAPI(_In_ LPCWSTR lpFileName, _Out_ LPWIN32_FIND_DATAW lpFindFileData);
	using FindNextFileWProc = WINBASEAPI BOOL WINAPI(_In_ HANDLE hFindFile, _Out_ LPWIN32_FIND_DATAW lpFindFileData);
	using FindCloseProc = WINBASEAPI BOOL WINAPI(_Inout_ HANDLE hFindFile);

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


		uint64_t VMT;
		int field_8;
		uint8_t field_C;
		uint8_t field_D;
		uint8_t field_E;
		uint8_t field_F;
		uint64_t field_10;
		uint64_t field_18;
		uint8_t field_20;
		uint8_t field_21;
		uint8_t field_22;
		uint8_t field_23;
		float field_24;
		uint8_t field_28;
		uint8_t field_29;
		uint8_t field_2A;
		uint8_t field_2B;
		uint8_t field_2C;
		uint8_t field_2D;
		uint8_t field_2E;
		float field_30;
		int UIScaling;
		uint8_t ControllerMode;
		float NodeWaitTimeMultiplier;
		float field_5C;
		float field_60;
		float field_64;
		int field_68;
		int field_6C;
		float field_70;
		int field_74;
		float field_78;
		int field_7C;
		int field_80;
		int field_84;
		uint8_t field_88;
		uint8_t field_89;
		uint8_t field_8A;
		uint8_t field_8B;
		uint8_t field_8C;
		uint8_t field_8D;
		uint8_t field_8E;
		uint8_t field_8F;
		int field_90;
		int field_94;
		int field_98;
		int field_9C;
		int field_A0;
		int field_A4;
		BYTE field_A8;
		uint8_t field_A9;
		BYTE field_AA;
		uint8_t field_AB;
		int MouseSensitivity;
		int MouseScrollSensitivity;
		int ControllerSensitivity;
		int ControllerStickDeadZone;
		int ControllerStickPressDeadZone;
		int ControllerTriggerDeadZone;
		bool ServerMonitor_M;
		uint8_t field_C5;
		uint8_t field_C6;
		bool StoryLog;
		bool Story;
		bool StoryEvents;
		bool DisableStoryPatching;
		bool ForceStoryPatching;
		uint8_t field_CB;
		bool Stats;
		uint8_t field_CD;
		uint8_t field_CE;
		int Modded;
		BYTE field_D4;
		STDString Language;
		bool ShowLocalizationMarkers;
		__int16 ForcePort;
		bool EnablePortmapping;
		int GameVisibilityOnline;
		int GameVisibilityLAN;
		int GameVisibilityDirect;
		bool DisableLocalMessagePassing;
		bool EnableSteamP2P;
		uint8_t field_106;
		uint8_t field_107;
		BYTE field_108;
		uint8_t field_109;
		uint8_t field_10A;
		uint8_t field_10B;
		BYTE field_10C;
		uint8_t field_10D;
		uint8_t field_10E;
		uint8_t field_10F;
		uint8_t field_110;
		uint8_t field_111;
		uint8_t field_112;
		uint8_t field_FB;
		uint8_t field_114;
		bool DoUnlearnCheck;
		uint8_t field_116;
		uint8_t field_117;
		BYTE field_118;
		uint8_t field_119;
		uint8_t field_11A;
		bool CameraShakeEnabled;
		BYTE field_11C;
		uint8_t field_11D;
		uint8_t field_11E;
		bool TwitchEnableOverlay;
		BYTE field_120;
		uint8_t field_121;
		uint8_t field_122;
		STDString field_128;
		STDString TwitchExtSecret;
		STDString field_158;
		STDString field_170;
		float LongRestScriptWaitingTime;
		STDString LongRestDefaultTimeline;
		uint8_t field_1C8;
		uint8_t field_1C9;
		uint8_t field_1CA;
		bool useSaveSystemECBChecker;
		bool saveSystemECBCheckerEnableLogging;
		bool saveSystemECBCheckerEnableDetailedLogging;
		bool saveSystemECBCheckerLogSuccessfulAttempts;
		bool saveSystemECBCheckerAllowSaveOnFailure;
		int saveSystemECBCheckNumberOfFramesToWait;
		BYTE field_1D4;
		float field_1D8;
		float field_1DC;
		bool UseLevelCache;
		bool LoadAllEffectPools;
		bool ScriptLog;
		bool ShroudEnabled;
		uint8_t field_1EC;
		uint8_t field_1ED;
		bool Fading;
		bool CheckRequirements;
		bool UseEndTurnFallback;
		bool ShowCharacterCreation;
		bool ShowOriginIntroInCC;
		bool DisableArmorSavingThrows;
		bool AlwaysShowSplitter;
		bool ResetTutorialsOnNewGame;
		bool ControllerLayout;
		bool NorthFacingMinimap;
		bool HoldToEndTurn;
		bool ExtraTooltipDescriptions;
		uint8_t field_1F9;
		uint8_t field_1FA;
		uint8_t field_1FB;
		uint8_t field_1FC;
		uint8_t field_1FD;
		SoundSetting SoundSettings[12];
		int SoundVolumeDynamicRange;
		int SoundOcclusionQuality;
		bool MuteSoundWhenNotFocused;
		bool CentreDialogue;
		bool ForceMono;
		int SoundPartyLosingThreshold;
		int SoundPartyWinningThreshold;
		int GameCameraRotation;
		float GameCameraSpeedMultiplier;
		bool GameCameraRotationLocked;
		bool GameCameraEnableCloseUpDialog;
		bool GameCameraEnableDynamicCombatCamera;
		uint8_t field_69F;
		uint8_t field_6A0;
		bool CombatTimelines;
		bool GameCameraEnableAttackCameraOtherPlayers;
		bool GameCameraEnableFlyFollow;
		bool GameCameraAutoRotateEnabled;
		uint8_t field_6A5;
		uint8_t field_6A6;
		uint8_t field_6A7;
		float GameCameraAutoRotateSpeed;
		uint8_t field_6AC;
		uint64_t field_6B0;
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
		uint8_t field_6CF;
		int MaxAmountDialogsInLog;
		float field_6D4;
		uint8_t field_6D8;
		float ControllerCharacterWalkThreshold;
		float ControllerCharacterRunThreshold;
		float field_6E4;
		float field_6E8;
		float field_6EC;
		uint64_t field_6F0;
		int ControllerRumbleStrength;
		int field_6FC;
		uint64_t field_700;
		uint8_t field_708;
		uint8_t field_709;
		uint8_t field_70A;
		uint8_t field_70B;
		int field_70C;
		uint8_t field_710;
		uint8_t field_711;
		uint8_t field_712;
		uint8_t field_713;
		int field_714;
		uint8_t field_718;
		uint8_t field_719;
		uint8_t field_71A;
		uint8_t field_71B;
		uint8_t field_71C;
		uint8_t field_71D;
		uint8_t field_71E;
		uint8_t field_71F;
		uint64_t field_720;
		uint8_t field_728;
		uint8_t field_729;
		uint8_t field_72A;
		uint8_t field_72B;
		uint8_t field_72C;
		uint8_t field_72D;
		uint8_t field_72E;
		uint8_t field_72F;
		BYTE field_730;
		bool EnableVoiceLogging;
		bool EnableLongPressAndHold;
		bool MouseLock;
		bool DisableEdgePanning;
		bool EscClosesAllUI;
		bool ShowSubtitles;
		bool DisplaySpeaker;
		bool ShowTextBackground;
		uint8_t field_739;
		uint8_t field_73A;
		uint8_t field_73B;
		int TextBackgroundOpacity;
		int DialogueFontSizeModifier;
		int OverheadFontSizeModifier;
		uint8_t field_748;
		bool UseRadialContextMenu;
		bool LogSaveLoadErrors;
		bool EnableAiThinking_M;
		bool EnableBlending_M;
		uint8_t field_74D;
		uint8_t field_74E;
		uint8_t field_74F;
		BYTE field_750;
		uint8_t field_751;
		uint8_t field_752;
		uint8_t field_753;
		uint8_t field_754;
		uint8_t field_755;
		uint8_t field_756;
		uint8_t field_757;
		Array<void*> field_758;
		int field_768;
		FixedString field_76C;
		bool PeaceCharacterHighlightMode;
		bool CombatCharacterHighlightMode;
		bool TacticalCharacterHighlightMode;
		bool UnitSystem;
		bool WeightedRolls;
		uint8_t field_775;
		uint8_t field_776;
		bool PostCombatAssignation;
		uint8_t field_778;
		uint8_t field_779;
		uint8_t field_77A;
		uint8_t field_77B;
		uint8_t field_77C;
		uint8_t field_77D;
		uint8_t field_77E;
		uint8_t field_77F;
		int field_780;
		int field_784;
		int ServerFrameCap;
		GlobalCameraSwitches CameraSwitches[4];
		ControlSwitch ControlSwitches;
		uint64_t field_E60[72*2];
		uint64_t P1;
		uint64_t P2;
		uint8_t AutoRemoveHotbarSpells;
		uint8_t field_12E1;
		__int16 AutoFillHotbarCategories;
		int field_12E4;
		uint8_t field_12E8;
		uint8_t field_12E9;
		uint8_t field_12EA;
		uint8_t field_12EB;
		float field_12EC;
		float field_12F0;
		bool TimelinesAD;
		bool TimelinesAttitudeMimicry;
		bool TimelinesEmoteMimicry;
		bool TimelinesLookAtMimicry;
		uint8_t field_12F8;
		bool TimelinesAutoContinue;
		uint8_t field_12FA;
		bool TimelinesShowDisabled;
		bool TimelinesQCameras;
		uint8_t field_12FD;
		bool TimelinesCombatAuthored;
		uint8_t field_12FF;
		bool TimelinesFade;
		uint8_t field_1301;
		uint8_t field_1302;
		uint8_t field_1303;
		float field_1304;
		float field_1308;
		BYTE field_130C;
		uint8_t field_130D;
		bool ShowGenitals;
		bool ShowExplicitRomanceScenes;
		bool NoPrivateDialogues;
		uint8_t field_1311;
		uint8_t field_1312;
		uint8_t field_1313;
		uint8_t field_1314;
		uint8_t field_1315;
		uint8_t field_1316;
		uint8_t field_1317;
		uint8_t field_1318;
		int field_131C;
		int field_1320;
		uint8_t field_1324;
		uint8_t field_1325;
		uint8_t field_1326;
		int field_1328;
		int field_132C;
		int field_1330;
		int field_1334;
		float field_1338;
		int field_133C;
		int field_1340;
		int field_1344;
		uint8_t field_1348;
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
		uint8_t field_1375;
		uint8_t field_1376;
		uint8_t field_1377;
		uint8_t field_1378;
		uint8_t field_1379;
		uint8_t field_137A;
		uint8_t field_137B;
		uint8_t field_137C;
		uint8_t field_137D;
		float field_1380;
		float field_1384;
		float field_1388;
		uint8_t field_138C;
		uint8_t field_138D;
		uint8_t ShowTutorials;
		bool ShowTutorialsAnswered;
		bool ShowPings;
		bool ShowCombatFeed;
		bool ShowFirstTimeSetup;
		uint8_t field_1393;
		uint8_t field_1394;
		float PointAndClickSoundFrequencyFloat;
		STDString ActiveCustomDice;
		int field_13B8;
		int field_13BC;
		int field_13C0;
		uint8_t field_13C4;
		uint8_t field_13C5;
		uint8_t field_13C6;
		uint8_t field_13C7;
		uint8_t field_13C8;
		int StartYear;
		int StartDay;
		int _Pad4;
		uint8_t field_13D8;
		uint8_t SkipLarianSignUp;
		uint8_t field_13DA;
		uint8_t field_13DB;
		bool HideCompletedQuests;
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
