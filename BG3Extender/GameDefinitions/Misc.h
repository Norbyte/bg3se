#pragma once

#include "BaseTypes.h"
#include "Enumerations.h"
#include "EntitySystem.h"
#include "Module.h"
#include <glm/gtc/quaternion.hpp>

namespace bg3se
{
	struct GameTime
	{
		double Time;
		float DeltaTime;
		int32_t Ticks;
	};

	struct ActionOriginator
	{
		GUID ActionGuid;
		FixedString PassiveId;
	};

	struct SomeGuidId
	{
		FixedString OriginatorPrototype;
		uint8_t _Pad[4];
		uint8_t SourceType;
		UUID Class;
		FixedString Prototype;
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


	/*struct GlobalSwitches
	{
		void * VMT;
		uint32_t ShowDebugView;
		bool Cleave_M;
		bool ShowCloths;
		bool field_E;
		bool ShowDrawStats;
		bool ShowDebugLines;
		bool field_11;
		bool ShowFPS;
		bool ShowPhysics;
		bool field_14;
		bool ShowRagdollInfo;
		bool field_16;
		bool ShowRaycasting;
		bool ShowPhysXBoxes;
		bool field_19;
		bool YieldOnLostFocus;
		bool field_1B;
		bool EnableAngularCulling;
		bool field_1D;
		bool field_1E;
		bool field_1F;
		bool field_20;
		bool field_21;
		bool LoadScenery;
		bool LoadTextShaders;
		float TexelDensityMin;
		float TexelDensityIdeal;
		float TexelDensityMax;
		float TexelDensityExtreme;
		float UIScaling;
		float NodeWaitTimeMultiplier;
		bool EnableGenome;
		bool field_3D;
		bool UpdateOffstageOverlayMaterials_M;
		bool UpdateInvisibilityOverlayMaterials_M;
		bool FileLoadingLog;
		bool FileSavingLog;
		uint16_t _Pad1;
		uint32_t MouseSensitivity;
		uint32_t MouseScrollSensitivity;
		uint32_t ControllerSensitivity;
		uint32_t ControllerStickDeadZone;
		uint32_t ControllerStickPressDeadZone;
		uint32_t ControllerTriggerDeadZone;
		bool ServerMonitor;
		bool StoryLog;
		bool Story;
		bool StoryEvents;
		bool DisableStoryPatching;
		bool ForceStoryPatching;
		bool StatsArgPassed;
		bool field_63;
		bool EnableSoundErrorLogging;
		bool _Pad2[3];
		STDString ChatLanguage;
		bool ShowLocalizationMarkers;
		bool field_89;
		uint16_t ForcePort;
		bool EnablePortmapping;
		char _Pad3[3];
		uint32_t GameVisibilityOnline;
		uint32_t GameVisibilityLAN;
		uint32_t GameVisibilityDirect;
		bool DisableLocalMessagePassing;
		bool EnableSteamP2P;
		uint16_t _Pad4;
		STDString DirectConnectAddress;
		uint32_t field_C0;
		bool UpdateScene;
		bool UpdatePhysXScene;
		bool DoUnlearnCheck;
		bool VisualizeTextures;
		bool field_C8;
		bool field_C9;
		bool EnableModuleHashing;
		bool SomePhysXRagdollFlag;
		bool field_CC;
		bool field_CD;
		bool field_CE;
		bool field_CF;
		bool UseLevelCache;
		bool LoadAllEffectPools;
		bool ScriptLog;
		bool ShroudEnabled;
		bool GodMode;
		bool Fading;
		bool CheckRequirements;
		bool UseEndTurnFallback;
		bool ShowCharacterCreation;
		bool ShorOriginIntroInCC;
		bool DisableArmorSavingThrows;
		bool AlwaysShowSplitterInTrade;
		bool ResetTutorialsOnNewGame;
		bool field_DD;
		bool field_DE;
		bool field_DF;
		uint32_t ControllerLayout;
		bool RotateMinimap;
		bool ShowOverheadDamage;
		bool ShowOverheadText;
		bool ShowOverheadDialog;
		// Rest of structure not properly mapped yet
	};*/

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
		ObjectHandle SummonHandle;
		uint32_t Unknown{ 0 };
	};

	struct SummonHelperSummonArgs
	{
		ObjectHandle OwnerCharacterHandle;
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
