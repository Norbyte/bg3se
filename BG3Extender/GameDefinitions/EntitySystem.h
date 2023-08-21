#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Module.h>
#include <GameDefinitions/GlobalFixedStrings.h>

namespace bg3se
{
	struct GameTime
	{
		double Time;
		float DeltaTime;
		int32_t Ticks;
	};

	struct EntityWorld;

	struct EntityWorldHandle
	{
		EntityHandle Handle;
		EntityWorld* World{ nullptr };
	};

	struct CriticalSection : public CRITICAL_SECTION
	{
		__int64 field_28;
		__int64 field_30;
	};

	struct BaseComponent
	{
		// FIXME - not sure if this even exists anymore
		EntityHandle Entity;
	};

	struct EntityWorld : public ProtectedGameObject<EntityWorld>
	{
		// Handle type index, registered statically during game startup
		// FIXME - delete all ComponentHandle logic!
		enum class HandleTypeIndexTag {};
		using HandleTypeIndex = TypedIntegral<uint16_t, HandleTypeIndexTag>;
		// Component type index, registered statically during game startup
		enum class ComponentTypeIndexTag {};
		using ComponentTypeIndex = TypedIntegral<uint16_t, ComponentTypeIndexTag>;

		static constexpr ComponentTypeIndex UndefinedComponent{ 0xffff };
		static constexpr HandleTypeIndex UndefinedHandle{ 0xffff };

		struct UpdateInfo
		{
			__int64 field_50;
			__int64 field_58;
			__int64 field_60;
			__int64 field_68;
			__int64 field_70;
			__int64 field_78;
			__int64 field_80;
			__int64 field_88;
			__int64 field_90;
			__int64 field_98;
			__int64 field_A0;
			__int64 field_A8;
			__int64 field_B0;
			__int64 field_B8;
			__int64 field_C0;
			__int64 field_C8;
			__int64 field_D0;
			__int64 field_D8;
			__int64 field_E0;
			__int64 field_E8;
			__int64 field_F0;
			__int64 field_F8;
			__int64 field_100;
			__int64 field_108;
			__int64 field_110;
			__int64 field_118;
			__int64 field_120;
		};

		struct ComponentType
		{
			__int16 field_0;
			int field_4;
			BYTE field_8;
			char field_9;
			char field_A;
			bool QueryFlags[4];
			uint16_t field_10;
			__int16 field_12;
			void* DtorProc;
			Array<int16_t> DependentComponentIndices;
			Array<int16_t> DependencyComponentIndices;
		};

		struct QueryManager
		{
			Array<void*> Queries;
			Array<int16_t> field_10;
			Array<int16_t> field_20;
			Array<int16_t> field_30;
			Array<int16_t> field_40;
			Array<int16_t> field_50;
			Array<int16_t> field_60;
		};

		struct ComponentRegistry
		{
			BitSet<> Bitmask;
			Array<ComponentType> Types;
		};

		struct SystemType
		{
			using ID = uint32_t;

			void* System;
			int32_t SystemIndex0;
			int32_t SystemIndex1;
			__int16 field_10;
			char field_12;
			void* SomeProc1;
			__int64 field_20;
			void* SomeProc2;
			MultiHashSet<SystemType::ID> DependencySystems;
			MultiHashSet<SystemType::ID> DependentSystems;
			MultiHashSet<uint32_t> HandleMappings2;
			MultiHashSet<uint32_t> HandleMappings;
		};

		struct SystemRegistry
		{
			void* VMT;
			Array<SystemType> Systems;
			uint32_t Unknown;
			uint32_t GrowSize;
		};

		struct ComponentReplication
		{
			Array<MultiHashMap<EntityHandle, BitSet<>>> ComponentPools;
			bool Dirty;
		};

		struct EntityTypeSalts
		{
			struct Entry
			{
				int Index;
				int Salt;
			};

			Entry** Buckets;
			uint32_t NumElements;
			uint16_t NumBuckets;
			uint16_t BitsPerBucket;
			__int64 field_10;
			int field_18;
			__int64 field_20;
			__int64 field_28;
			__int64 field_30;
			__int64 field_38;
		};
		
		struct Entity
		{
			struct ComponentEntry
			{
				__int16 Index;
				__int16 ComponentTypeId;
				char field_4;
				char field_5;
				void* DtorProc;
			};
			
			struct ComponentBucket
			{
				__int16 A;
				__int16 B;
			};
			
			struct ComponentPoolEntry
			{
				void** Components;
				void* B;
			};
			
			struct EntityComponentPool
			{
				ComponentPoolEntry Pool[256];
			};


			uint64_t field_0[30];
			__int64 field_F0;
			uint16_t* SomeListBuf_2b;
			ComponentEntry* ComponentDtors;
			__int16 TypeId;
			__int16 field_10A;
			__int16 SomeListSize;
			char field_10E;
			char field_10F;
			char field_110;
			Array<EntityComponentPool*> Components;
			Array<void*> field_128;
			BitSet<> field_138;
			int field_148;
			Array<void*> field_150;
			MultiHashMap<uint16_t, uint8_t> ComponentTypeToIndex;
			MultiHashMap<uint64_t, ComponentBucket> ComponentBuckets;
			MultiHashMap<uint64_t, uint64_t> field_1E0;
			MultiHashMap<uint64_t, uint64_t> field_220;
			Array<void*> field_260;
			Array<void*> field_270;
			MultiHashMap<uint16_t, void*> Components_u16_Unk;
			char field_2C0;
			__int64 field_2C8;
			uint64_t field_2D0[4];
			Array<int16_t> field_2F0;
			Array<int16_t> field_300;
			uint64_t field_310[38];
			Array<void*> field_440;
			uint64_t field_450[38];

			void* GetComponent(EntityHandle entityHandle, ComponentTypeIndex type) const;
		};


		struct EntityStore
		{
			struct TypeSalts
			{
				struct Entry
				{
					int Salt;
					uint16_t EntityIndex;
				};

				Entry** Buckets;
				uint32_t NumElements;
				uint16_t NumBuckets;
				uint16_t BitsPerBucket;
			};

			struct SaltMap
			{
				std::array<TypeSalts, 0x40> Buckets;
				uint32_t Size;
			};

			Array<Entity*> Entities;
			MultiHashMap<uint64_t, uint16_t> TypeHashToEntityIndex;
			SaltMap Salts;
			MultiHashMap<uint64_t, uint64_t> field_458;
			BitSet<> UsedTypeIndices;
			ComponentRegistry* ComponentRegistry;
			QueryManager* Queries;

			Entity* GetEntity(EntityHandle entityHandle) const;
		};

		struct SomeStore
		{
			struct Elem
			{
				__int64 field_0;
				int field_8;
				int field_C;
			};

			__int64 FastLock;
			__int64 FastLock2;
			CRITICAL_SECTION field_10;
			Elem field_38[2];
		};

		struct ComponentOps
		{
			__int64 VMT;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
			__int64 field_20;
			__int16 TypeId;
		};

		struct ComponentPool2
		{
			Array<void*> Components;
			void* UpdateProc;
			uint64_t field_18;
			SomeStore* Store;
			int SomeHandle;
			WORD field_2C;
			int16_t ComponentTypeId;
			void* DtorProc;
		};

		struct ComponentRegistryEntry1
		{
			void* VMT;
			__int64 field_8;
			Array<void*> field_10;
			__int64 field_20;
			Array<void*> field_28;
		};

		struct EntityComponentsEntity
		{
			uint64_t ComponentTypeIdMask[30];
			uint64_t ComponentUpdateFlags1[30];
			uint64_t ComponentUpdateFlags2[30];
			uint64_t field_2D0[8];
			__int64 field_310;
			__int64 field_318;
			Array<uint64_t> ComponentHandles;
			Array<uint64_t> field_330;
			int Flags;
			int field_344;
			__int64 field_348;
		};

		struct EntityComponents
		{
			struct SparseHashMap
			{
				BitSet<> SetValues;
				Array<int16_t> NextIds;
				Array<int16_t> Keys;
				Array<ComponentPool2*> Values;
			};

			MultiHashMap<EntityHandle, EntityComponentsEntity*> Entities;
			SparseHashMap ComponentPools;
			char field_80;
			MultiHashMap<uint16_t, MultiHashMap<uint64_t, void*>> ComponentsByType;
			MultiHashMap<uint16_t, MultiHashMap<uint64_t, void*>> ComponentsByType2;
			void* field_108;
			void* field_110;
			Array<void*>* ComponentTypes;
			SomeStore* SomeStore;
			EntityWorld* EntityWorld;
			EntityTypeSalts* Salts;
			__int64 field_128;
		};


		ComponentReplication* Replication;
		ComponentRegistry ComponentRegistry_;
		SystemRegistry Systems;
		__int64 field_48;
		UpdateInfo UpdateInfos;
		__int64 field_128;
		__int64 field_130;
		__int64 field_138;
		uint64_t GameTime[3];
		void*  ECSUpdateBatch;
		int field_160;
		__int64 field_168;
		Array<void*> field_170;
		Array<ComponentRegistryEntry1*> ComponentTypes;
		bool field_190;
		bool NeedsUpdate;
		bool field_192;
		bool field_193;
		QueryManager Queries;
		std::array<EntityTypeSalts, 0x40>* EntitySalts;
		EntityStore* Entities;
		SomeStore field_218;
		__int64 field_270;
		MultiHashMap<uint64_t, uint64_t> field_278;
		Array<void*> field_2B8;
		Array<ComponentOps*> ComponentOpsList;
		ScratchString Scratch;
		void* UpdateBatches;
		CriticalSection CS;
		int field_338;
		EntityComponents* Components;
		__int64 field_348;
		__int64 field_350;
		__int64 field_358;
		__int64 field_360;
		__int64 field_368;
		__int64 field_370;
		__int64 field_378;

		void* GetComponent(EntityHandle entityHandle, ComponentTypeIndex type);
		void* GetComponent(char const* nameGuid, ComponentTypeIndex type);
		void* GetComponent(FixedString const& guid, ComponentTypeIndex type);

		Entity* GetEntity(EntityHandle entityHandle) const;
		bool IsValid(EntityHandle entityHandle) const;
	};

	struct IGameObject : public ProtectedGameObject<IGameObject>
	{
		virtual ~IGameObject() = 0;
		virtual void HandleTextKeyEvent() = 0;
		virtual uint64_t Ret5() = 0;
		virtual void SetComponentHandle(ComponentHandle Handle) = 0;
		virtual void GetComponentHandle(ComponentHandle& Handle) const = 0;
		virtual void SetGuid(FixedString const& fs) = 0;
		virtual FixedString* GetGuid() const = 0;
		virtual void SetNetId(NetId netId) = 0;
		virtual void GetNetId(NetId& netId) const = 0;
		virtual void SetCurrentTemplate(void* esvTemplate) = 0;
		virtual void* GetCurrentTemplate() const = 0;
		virtual void SetGlobal(bool isGlobal) = 0;
		virtual bool IsGlobal() const = 0;
		virtual uint32_t GetComponentType() = 0;
		virtual void* GetEntityObjectByHandle(ComponentHandle handle) = 0;
		virtual void* GetUnknownHandle() = 0;
		virtual STDString* GetName() = 0;
		virtual void SetFlags(uint64_t flag) = 0;
		virtual void ClearFlags(uint64_t flag) = 0;
		virtual bool HasFlag(uint64_t flag) const = 0;
		virtual void SetAiColliding_M(bool colliding) = 0;
		virtual void Unknown1() = 0;
		virtual void Unknown2() = 0;
		virtual void Unknown3() = 0;
		virtual void Unknown4() = 0;
		virtual void Unknown5() = 0;
		/*virtual void GetTags(ObjectSet<FixedString> & tags) = 0;
		virtual bool IsTagged(FixedString & tag) = 0;*/
		virtual Vector3 const* GetTranslate() const = 0;
		virtual glm::mat3 const* GetRotation() const = 0;
		virtual float GetScale() const = 0;
		virtual void SetTranslate(Vector3 const& translate) = 0;
		virtual void SetRotation(glm::mat3 const& rotate) = 0;
		virtual void SetScale(float scale) = 0;
		virtual float GetHeight_M() = 0;
		virtual FixedString& GetParentUUID_M() = 0;
		virtual FixedString& GetCurrentLevel() = 0;
		virtual void SetCurrentLevel(FixedString const& fs) = 0;

		void* VMT2;
	};

	/*struct IEocClientObject : public IGameObject
	{
		virtual eoc::Ai * GetAi() = 0;
		virtual void LoadAi() = 0;
		virtual void UnloadAi() = 0;
		virtual void Unknown0() = 0;
		virtual void Unknown1() = 0;
		virtual void Unknown2() = 0;
		virtual void Unknown3() = 0;
		virtual FixedString * Unknown4() = 0;
		virtual bool Unknown5() = 0;
		virtual TranslatedString* GetDisplayName(TranslatedString* name) = 0;
		virtual float Unknown6() = 0;
		virtual void SavegameVisit() = 0;
		virtual void SetLight(FixedString *) = 0;
		virtual void * GetLight() = 0;
		virtual void RemoveLight() = 0;
		virtual FixedString * GetPlayerRace(bool returnPolymorph, bool excludeUninitialized) = 0;
		virtual FixedString* GetPlayerOrigin(bool returnPolymorph, bool excludeUninitialized) = 0;
	};*/

	struct IEoCServerObject : public IGameObject
	{
		virtual void MarkVisibleToPeerId(uint32_t* peerId) = 0;
		virtual void UnmarkVisibleToPeerId(uint32_t* peerId) = 0;
		virtual void ClearAllVisiblePeerIds() = 0;
		virtual void IsVisibleToPeer(uint32_t* peerId) = 0;
		virtual void* GetBoundComponent() = 0;
		virtual void LoadAi_M() = 0;
		virtual bool UnloadAi_M() = 0;
		virtual TranslatedString* GetDisplayName(TranslatedString& name) = 0;
		virtual void SetOriginalTemplate(void* esvTemplate) = 0;
		virtual bool SavegameVisit(void* visitor) = 0;
		virtual NetId& GetEntityNetworkId(NetId& netId) const = 0;
		virtual void SetTemplate(void* esvTemplate) = 0;
		virtual void SetTemplateId(uint8_t templateType, FixedString const& templateId) = 0;
		virtual void* GetSomeTransformValue() = 0;
		virtual void SetSomeTransformValue(void* unknown) = 0;

		BaseComponent Base;
	};

	namespace esv
	{
		struct EntityManager;

		/*
		struct CharacterFactory : public NetworkObjectFactory<esv::Character, (uint32_t)ObjectType::ServerCharacter>
		{
			void* VMT2;
			void* VMT3;
			Map<FixedString, void*> FSMap_ReloadComponent;
			EntityWorld* Entities;
			uint64_t Unkn8[2];
		};

		struct ItemFactory : public NetworkObjectFactory<esv::Item, (uint32_t)ObjectType::ServerItem>
		{
			void* VMT2;
			void* VMT3;
			Map<FixedString, void*> FSMap_ReloadComponent;
			EntityWorld* Entities;
			uint64_t Unkn8[2];
		};

		struct InventoryFactory : public NetworkObjectFactory<esv::Inventory, (uint32_t)ObjectType::ServerInventory>
		{
			// TODO
		};*/

		struct GameStateMachine : public ProtectedGameObject<GameStateMachine>
		{
			uint8_t Unknown;
			void * CurrentState;
			GameState State;
			void ** TargetStates;
			uint32_t TargetStateBufSize;
			uint32_t NumTargetStates;
			uint32_t ReadStateIdx;
			uint32_t WriteStateIdx;
		};

		struct EoCServer
		{
			char field_0;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
			__int64 field_20;
			__int64 field_28;
			__int64 field_30;
			__int64 field_38;
			__int64 field_40;
			__int64 field_48;
			__int64 field_50;
			__int64 field_58;
			__int64 field_60;
			__int64 field_68;
			ScratchString field_70;
			FixedString field_90;
			FixedString field_94;
			FixedString field_98;
			FixedString field_9C;
			GameStateMachine* GameStateMachine;
			__int64 GameServer;
			__int64 field_B0;
			int Rand1;
			int Rand2;
			__int64 GlobalRandom;
			__int64 ConditionManager;
			ModManager ModManager;
			char field_280;
			EntityWorld* EntityWorld;
			__int64 field_290;
			__int64 LobbyManager;
			char field_2A0;
			__int64 field_2A8;
			__int64 field_2B0;
		};

		typedef void(*GameStateEventManager__ExecuteGameStateChangedEvent)(void* self, GameState fromState, GameState toState);
		typedef void(*GameStateThreaded__GameStateWorker__DoWork)(void* self);
		typedef void(*GameStateMachine__Update)(void* self, GameTime* time);

		/*
		EntityWorld* GetEntityWorld();*/
	}

	namespace ecl
	{
			/*inline Character* GetCharacter(char const* nameGuid, bool logError = true)
			{
				auto component = GetComponent(ComponentType::Character, nameGuid, logError);
				if (component != nullptr) {
					return (Character*)((uint8_t*)component - 8);
				}
				else {
					return nullptr;
				}
			}

			inline Character* GetCharacter(ComponentHandle handle, bool logError = true)
			{
				auto component = GetComponent(ComponentType::Character, ObjectType::ClientCharacter, handle, logError);
				if (component != nullptr) {
					return (Character*)((uint8_t*)component - 8);
				}
				else {
					return nullptr;
				}
			}

			inline Character* GetCharacter(NetId netId, bool logError = true)
			{
				auto component = GetComponent(ComponentType::Character, netId, logError);
				if (component != nullptr) {
					return (Character*)((uint8_t*)component - 8);
				}
				else {
					return nullptr;
				}
			}

			inline Item* GetItem(char const* nameGuid, bool logError = true)
			{
				auto component = GetComponent(ComponentType::Item, nameGuid, logError);
				if (component != nullptr) {
					return (Item*)((uint8_t*)component - 8);
				}
				else {
					return nullptr;
				}
			}

			inline Item* GetItem(ComponentHandle handle, bool logError = true)
			{
				auto component = GetComponent(ComponentType::Item, ObjectType::ClientItem, handle, logError);
				if (component != nullptr) {
					return (Item*)((uint8_t*)component - 8);
				}
				else {
					return nullptr;
				}
			}

			inline Item* GetItem(NetId netId, bool logError = true)
			{
				auto component = GetComponent(ComponentType::Item, netId, logError);
				if (component != nullptr) {
					return (Item*)((uint8_t*)component - 8);
				}
				else {
					return nullptr;
				}
			}

			inline eoc::CustomStatsComponent* GetCustomStatsComponentByEntityHandle(ComponentHandle entityHandle)
			{
				return (eoc::CustomStatsComponent*)GetComponentByEntityHandle(ComponentType::CustomStats, entityHandle);
			}*/


		/*

		struct InventoryFactory : public NetworkObjectFactory<ecl::Inventory, (uint32_t)ObjectType::ClientInventory>
		{
		};

		struct ActivationManager
		{
			struct ActivationGroup
			{
				ObjectSet<Item*> Items;
				ObjectSet<Character*> Characters;
			};

			void* VMT;
			__int64 field_8;
			void* VMT2;
			__int64 field_18;
			RefMap<FixedString, ActivationGroup> ChangedGroups;
			float ActivationRange1;
			float DeactivationRange1;
			float ActivationRange2;
			float DeactivationRange2;
		};*/


		struct GameStateMachine : public ProtectedGameObject<GameStateMachine>
		{
			void* CurrentState;
			GameState State;
		};
		

		struct EoCClient
		{
			using HandleErrorProc = void (void* self, TranslatedString const& message, bool exitGame, TranslatedString const& a4);

			__int64 VMT;
			__int64 VMT2;
			__int64 field_10;
			__int64 VMT3;
			__int64 field_20;
			__int64 VMT4;
			__int64 VMT5;
			__int64 field_38;
			Array<void*> field_40;
			__int64 field_50;
			Array<void*> field_58;
			__int64 field_68;
			Array<void*> field_70;
			__int64 field_80;
			GameStateMachine* GameStateMachine;
			void* GameClient;
			void* field_98;
			__int64 field_A0;
			__int64 field_A8;
			__int64 field_B0;
			FixedString FS1;
			FixedString LevelName;
			FixedString SomeGUID;
			FixedString FS_CurrentSaveGameGUID;
			char IsLoading;
			char IsLoading2;
			char field_CA;
			__int64 field_D0;
			__int64 field_D8;
			char field_E0;
			__int64 field_E8;
			__int64 field_F0;
			char field_F8;
			ScratchString field_100;
			ModManager* ModManager;
			__int64 field_128;
			TranslatedString LastError;
			int field_140;
			int field_144;
			int field_148;
			EntityWorld* EntityWorld;
			__int64 EntityManager_M;
			__int64 field_160;
			int field_168;
			int field_16C;
			// etc.
		};

		/*
		struct EoCClient : public ProtectedGameObject<EoCClient>
		{
			void* VMT;
			void* GameEventManagerVMT;
			uint64_t field_10;
			void* NetEventManagerVMT;
			uint64_t field_20;
			void* VMT2;
			void* VMT3;
			void* EoC;
			GameStateMachine** GameStateMachine;
			net::Client* GameClient;
			uint64_t field_50;
			void* LobbyLogicManager;
			void* ArenaManager;
			FixedString FS1;
			FixedString LevelName;
			FixedString SomeGUID;
			FixedString FS_CurrentSaveGameGUID;
			bool IsLoading;
			bool IsLoading2;
			PrimitiveSet<int> PrimitiveSetUnkn;
			uint16_t field_B0;
			void* Random;
			void* ItemCombinationManager;
			char field_C8;
			uint64_t ScratchStr[4];
			ScratchBuffer ScratchBuf;
			ModManager* ModManager;
			void* ChatManager;
			STDWString WStr_CurrentHost_M;
			uint64_t SomeObject[16];
			int field_1C0;
			uint64_t field_1C8[2];
			void* EntityWorldManager;
			EntityWorld* EntityWorld;
			EntityManager* EntityManager;
		};*/

		/*
		typedef void (*EoCClient__HandleError)(void* self, STDWString const* message, bool exitGame, STDWString const* a4);*/
		typedef void (*GameStateThreaded__GameStateWorker__DoWork)(void* self);
		typedef void (*GameStateEventManager__ExecuteGameStateChangedEvent)(void* self, GameState fromState, GameState toState);

		/*EntityWorld* GetEntityWorld();*/
	}
}
