#pragma once

#include <GameDefinitions/Base/Base.h>
#include "Enumerations.h"
#include "EntitySystem.h"
#include "Module.h"
#include <glm/gtc/quaternion.hpp>

namespace bg3se
{
	struct ActionOriginator : public HasObjectProxy
	{
		Guid ActionGuid;
		FixedString PassiveId;
	};

	struct DescriptionInfo
	{
		TranslatedString DisplayNameTS;
		uint64_t field_10;
		STDString DisplayName;
		STDString DescriptionParams;
		STDString ExtraDescription;
		STDString ExtraDescriptionParams;
	};


	struct LuaExpressionBase
	{
		Array<STDString> ExpressionParams;
		STDString Code;
	};

	struct LuaExpression : public LuaExpressionBase
	{
		int FastLock;
	};

	struct LuaExpressionManager : public ProtectedGameObject<LuaExpressionManager>
	{
		void* VMT;
		RefMap<Guid, LuaExpression*> Expressions;
		char field_18;
	};

	struct GlobalSwitches
	{
		struct SoundSetting
		{
			uint64_t ConfigValue;
			int field_8;
			FixedString ConfigKey;
			char* field_10;
			char* field_18;
			char field_20;
			char field_21;
			char field_22;
			TranslatedString field_24;
			void* SomeProc;
		};


		void* VMT;
		uint8_t field_8[0x70];
		int MouseSensitivity;
		int MouseScrollSensitivity;
		int ControllerSensitivity;
		int ControllerStickDeadZone;
		int ControllerStickPressDeadZone;
		int ControllerTriggerDeadZone;
		bool field_90;
		bool field_91;
		bool StoryLog;
		bool Story;
		bool StoryEvents;
		bool field_95;
		uint64_t field_98;
		STDString Language;
		bool field_C0;
		int16_t ForcePort;
		bool EnablePortmapping;
		int GameVisibilityOnline;
		int GameVisibilityLAN;
		int GameVisibilityDirect;
		bool field_D4;
		bool EnableSteamP2P;
		STDString DirectConnectAddress;
		bool field_F8;
		bool field_F9;
		bool field_FA;
		bool field_FB;
		bool field_FC;
		bool field_FD;
		bool field_FE;
		bool field_FF;
		bool field_100;
		bool DoUnlearnCheck;
		bool field_102;
		bool field_103;
		bool field_104;
		bool EnableHashing;
		bool field_106;
		bool field_107;
		bool field_108;
		bool CameraShakeEnabled;
		BYTE TwitchEnableOverlay;
		bool EnableTwitchIntegration;
		STDString TwitchExtSecret;
		STDString TwitchAccessToken;
		STDString TwitchChannelID;
		float LongRestScriptWaitingTime;
		STDString LongRestDefaultTimeline;
		bool UseLevelCache;
		bool LoadAllEffectPools;
		bool ScriptLog;
		bool ShroudEnabled;
		bool IsGodMode_M;
		bool SomeGameOverFlag;
		bool Fading;
		bool CheckRequirements;
		bool UseEndTurnFallback;
		bool field_1A1;
		bool ShowOriginIntroInCC;
		bool field_1A3;
		bool AlwaysShowSplitterInTrade;
		bool field_1A5;
		bool ControllerLayout;
		bool RotateMinimap;
		__int64 field_1A8;
		SoundSetting SoundSettings[12];
		int SoundQuality;
		int SoundConfiguration;
		bool MuteSoundWhenNotFocused;
		char field_4B9;
		char field_4BA;
		char field_4BB;
		int SoundPartyLosingThreshold;
		int SoundPartyWinningThreshold;
		float GameCameraRotation;
		float GameCameraSpeedMultiplier;
		bool GameCameraRotationLocked;
		bool GameCameraEnableCloseUpDialog;
		bool GameCameraEnableDynamicCombatCamera;
		char field_4CF;
		char field_4D0;
		char field_4D1;
		bool GameCameraEnableAttackCamera;
		bool GameCameraEnableAttackCameraOtherPlayers;
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
		bool field_4EB;
		bool MaxAmountDialogsInLog;
		float field_4F0;
		bool field_4F4;
		float ControllerCharacterWalkThreshold;
		float ControllerCharacterRunThreshold;
		// Rest of structure unmapped
	};

	struct TranslatedStringRepository : public ProtectedGameObject<TranslatedStringRepository>
	{
		struct TextEntry
		{
			RuntimeStringHandle* Handle;
			StringView* Text;
			__int64 field_10;
			__int64 field_18;
		};

		struct TextPool
		{
			Array<STDString*> Strings;
			MultiHashMap<RuntimeStringHandle, StringView> Texts;
		};

		int field_0;
		TextPool* TranslatedStrings[9];
		TextPool* TranslatedArgumentStrings;
		TextPool* StringHandles;
		Array<void*> field_60;
		MultiHashMap<FixedString, void*> field_70;
		MultiHashMap<FixedString, void*> field_B0;
		DWORD FastLock;
		DWORD CurrentThreadId;
		int field_F8;
		char field_FC;
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

	typedef void(*SummonHelpers__Summon)(SummonHelperResults * Results, SummonHelperSummonArgs * Args);
	typedef void(*GameStateEventManager__ExecuteGameStateChangedEvent)(void * self, GameState fromState, GameState toState);
	typedef void(*GameStateThreaded__GameStateWorker__DoWork)(void * self);*/

	struct SavegameManager : public ProtectedGameObject<SavegameManager>
	{
		EntityWorld* EntityWorld;
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


	struct FileReader : public Noncopyable<FileReader>
	{
		using CtorProc = FileReader * (FileReader* self, Path const& path, unsigned int type, unsigned int unknown);
		using DtorProc = void (FileReader* self);

		FileReader(std::string_view path);
		~FileReader();

		bool IsLoaded;
		void * ScratchBufPtr;
		void * MemBuffer;
		uint64_t FileSize;
		int FileHandle;
		int FileHandle2;
		uint64_t ScratchBuffer;
		uint64_t G;
		int H;
		uint64_t I;
		int FileType;
		void * FileObject;
		uint64_t _Fill[16];
	};

	class FileReaderPin
	{
	public:
		FileReaderPin(std::string_view path);
		FileReaderPin(std::string_view path, PathRootType root, bool canonicalize = true);

		~FileReaderPin();

		FileReaderPin(FileReaderPin const &) = delete;
		FileReaderPin & operator =(FileReaderPin const &) = delete;
		FileReaderPin & operator =(FileReaderPin &&) = delete;

		inline FileReaderPin(FileReaderPin && other) noexcept
		{
			reader_.swap(other.reader_);
			if (&other != this) {
				other.reader_.reset();
			}
		}

		bool IsLoaded() const
		{
			return reader_ && reader_->IsLoaded;
		}

		void * Buf() const
		{
			if (IsLoaded()) {
				return reader_->ScratchBufPtr;
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
		std::unique_ptr<FileReader> reader_;
	};

	/*
	typedef StringView * (* ls__Path__GetPrefixForRoot)(StringView * path, unsigned int rootType);*/

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
