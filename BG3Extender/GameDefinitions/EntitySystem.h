#pragma once

#include <GameDefinitions/BaseTypes.h>
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

	struct EntityWorldBase;

	struct EntityWorldHandle
	{
		EntityHandle Handle;
		EntityWorldBase* World{ nullptr };
	};

	struct BaseComponent : public ProtectedGameObject<BaseComponent>
	{
		// Handle of the entity this component belongs to
		EntityHandle Entity;
		// Handle of the current component
		ComponentHandle Handle;
	};

	struct ComponentFactoryBase : public ProtectedGameObject<ComponentFactoryBase>
	{
		// Hack - default implementation provided for typing purposes
		inline virtual ComponentHandle* ReevaluateHandle(ComponentHandle& handle) {}
		inline virtual ComponentHandle* GetFreeHandle(ComponentHandle& handle) {}
		inline virtual bool IsFreeIndex(uint32_t index) { return false; }
		inline virtual bool IsReservedIndex(uint32_t index) { return false; }
		inline virtual uint64_t ReserveIndex(uint32_t index) { return 0; }
		inline virtual uint64_t UnreserveIndex(uint32_t index) { return 0; }
		inline virtual void Destroy() {}

		struct ComponentEntry
		{
			BaseComponent* Component;
			int32_t ObjectIndex;
		};

		uint32_t Mutex;
		Array<ComponentEntry> ComponentsByHandleIndex;
		VirtualArray<uint32_t> Salts;
		ObjectSet<uint32_t> FreeSlotIndices;
		ObjectSet<void*> Components;
		uint8_t Unknown3;
		uint32_t Unknown4;

		void* FindByHandle(ComponentHandle handle) const;
	};


	struct NetworkComponentFactoryBase : public ComponentFactoryBase
	{
		Map<FixedString, void*> Guids;
		Map<uint64_t, void*> InUseNetIds;
		VirtualMultiHashSet<uint64_t> FreeNetIds;
		VirtualArray<uint16_t> NetIdSalts;
		uint64_t NextFreeNetId;
		char CanCreateNetIDs;
		char field_113[5];

		void* FindByGuid(FixedString const& guid) const;
		void* FindByNetId(NetId netId) const;
		void* FindByIndex(uint32_t index) const;
	};

	struct BaseComponentProcessingSystem
	{
		void * VMT;
		void * field_8;
	};

	struct ComponentPoolBase : public ProtectedGameObject<ComponentPoolBase>
	{
		virtual void Destroy() = 0;
		virtual bool DestroyComponent(ComponentHandle const& handle) = 0;
		virtual bool ForceComponent(ComponentHandle const& handle) = 0;
		virtual BaseComponent* ForceComponentGuid(FixedString const& guid, FixedString const& guid2) = 0;
		virtual int GetSize() = 0;
		virtual ComponentHandle& GetFreeHandle(ComponentHandle& handle) = 0;
		virtual bool IsReservedHandle(ComponentHandle const& handle) = 0;
		virtual void UnreserveHandle(ComponentHandle const& handle) = 0;
		virtual void UnreserveHandle2(ComponentHandle const& handle) = 0;
		virtual bool IsValidHandle(ComponentHandle const& handle) = 0;

		NetworkComponentFactoryBase Factory;
	};

	template <class T>
	struct ComponentPool : public ComponentPoolBase
	{
		inline T* GetComponentByIndex(uint32_t index) const
		{
			return reinterpret_cast<T*>(Factory.FindByIndex(index));
		}
		
		inline T* GetComponentByHandle(ComponentHandle const& handle) const
		{
			return reinterpret_cast<T*>(Factory.FindByHandle(handle));
		}

		inline T* GetComponentByGuid(FixedString const& guid) const
		{
			return reinterpret_cast<T*>(Factory.FindByGuid(guid));
		}

		inline T* GetComponentByNetId(NetId const& netId) const
		{
			return reinterpret_cast<T*>(Factory.FindByNetId(netId));
		}
	};

	struct EntityWorldBase : public ProtectedGameObject<EntityWorldBase>
	{
		// Handle type index, registered statically during game startup
		enum class HandleTypeIndexTag {};
		using HandleTypeIndex = TypedIntegral<int32_t, HandleTypeIndexTag>;
		// Component type index, registered statically during game startup
		enum class ComponentTypeIndexTag {};
		using ComponentTypeIndex = TypedIntegral<int32_t, ComponentTypeIndexTag>;
		// Replication pool type index, registered statically during game startup
		enum class ReplicationTypeIndexTag {};
		using ReplicationTypeIndex = TypedIntegral<int32_t, ReplicationTypeIndexTag>;

		struct Entity
		{
			EntityHandle Handle;
			bool field_8;
			uint32_t field_C;
			Array<int16_t> SlotIndexToComponentIdMap;
			Array<ComponentHandle> ComponentHandles;
			Array<int16_t> ComponentIdToSlotIndexMap;
			int NumUsedSlots;
			uint64_t OccupiedSlotBitmap[12];

			ComponentHandle GetComponentHandle(int32_t type, bool logError);
		};

		struct EntityType
		{
			Array<ComponentHandle> Handles;
			Array<Entity> Entities;
			Array<int> HandleToIndexRemaps;
			uint32_t LastAssignedEntityIndex;
			uint32_t field_4C;
			uint32_t* Salts; // 0x40000 salt entries
		};

		struct ComponentType
		{
			ComponentPoolBase* Pool;
			ComponentTypeIndex ComponentIndex;
			HandleTypeIndex HandleIndex;
			bool field_10;
			ObjectSet<int32_t> DependentComponentIndices;
			ObjectSet<int32_t> DependencyComponentIndices;
		};


		struct ComponentRegistry
		{
			uint64_t Bitmask[2];
			Array<ComponentType> Types;
		};

		struct ComponentTypeInfo
		{
			void* VMT;
			ComponentRegistry* ComponentRegistry;
			EntityWorldBase* EntityWorld;
			ComponentTypeIndex ComponentIndex;
		};

		struct SystemType
		{
			void* System;
			int32_t SystemIndex0;
			int32_t SystemIndex1;
			bool Unknown;
			void* SomeCallback;
			ObjectSet<int32_t> DependentSystemIndices;
		};

		struct SystemRegistry
		{
			uint64_t Unknown;
			Array<SystemType> Systems;
		};

		struct ComponentReplication
		{
			Array<VirtualMultiHashMap<EntityHandle, BitSet<>>> ComponentPools;
			bool Dirty;
		};

		void* UnknownB1;
		ComponentReplication* Replication;
		uint32_t EnterCount;
		uint32_t CurrentThreadId;
		void* Unknown;
		EntityType EntityTypes[64];
		uint64_t Unknown2[1];
		ComponentRegistry Components;
		SystemRegistry Systems;
		uint64_t Unknown3[3];
		ObjectSet<void*> SystemsToUpdate;
		RefMap<int32_t, int32_t> HandleIndexToComponentIndexMap;
		VirtualArray<void*> ComponentTypeRegistryMap;
		uint64_t Unknown4[2];
		Array<ComponentTypeInfo*> ComponentTypeInfos;

		ComponentPoolBase* GetComponentPool(HandleTypeIndex handleType, bool logError = true);
		ComponentPoolBase* GetComponentPool(ComponentTypeIndex handleType, bool logError = true);
		void* GetComponent(ComponentHandle componentHandle, bool logError = true);
		void* GetComponent(ComponentHandle componentHandle, HandleTypeIndex type, bool logError = true);
		void* GetComponent(ComponentHandle componentHandle, ComponentTypeIndex type, bool logError = true);
		void* GetComponent(char const* nameGuid, HandleTypeIndex type, bool logError = true);
		void* GetComponent(char const* nameGuid, ComponentTypeIndex type, bool logError = true);
		void* GetComponent(FixedString const& guid, HandleTypeIndex type, bool logError = true);
		void* GetComponent(FixedString const& guid, ComponentTypeIndex type, bool logError = true);
		void* GetComponent(NetId netId, HandleTypeIndex type, bool logError = true);
		void* GetComponent(NetId netId, ComponentTypeIndex type, bool logError = true);

		Entity* GetEntity(EntityHandle entityHandle, bool logError = true);
		ComponentHandle GetEntityComponentHandle(EntityHandle entityHandle, HandleTypeIndex type, bool logError = true);
		ComponentHandle GetEntityComponentHandle(EntityHandle entityHandle, ComponentTypeIndex type, bool logError = true);
		void* GetEntityComponent(EntityHandle entityHandle, HandleTypeIndex type, bool logError = true);
		void* GetEntityComponent(EntityHandle entityHandle, ComponentTypeIndex type, bool logError = true);
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
		BaseComponent Base;
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
	};

	namespace esv
	{
		struct EntityManager;

		struct EntityWorld : public EntityWorldBase // <ComponentType>
		{
			
		};

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
			void* EoC;
			uint64_t GameTime_M[2];
			ScratchBuffer ScratchBuffer1;
			ScratchString ScratchString2;
			FixedString FS1;
			FixedString CurrentLevel;
			FixedString FS3;
			FixedString FSGUID4;
			GameStateMachine * StateMachine;
			void * GameServer;
			void * field_88;
			void * GlobalRandom;
			void * ItemCombinationManager;
			void * CombineManager;
			void * field_D0;
			ModManager * ModManagerServer;
			bool ShutDown;
			EntityWorld * EntityWorld;
			EntityManager* EntityManager;
			void* unknown_F0;
			void * LobbyManagerOrigins;
			void* SomeSharedServerClientObj;
			void* field_118;
		};

		typedef void(*GameStateEventManager__ExecuteGameStateChangedEvent)(void* self, GameState fromState, GameState toState);
		typedef void(*GameStateThreaded__GameStateWorker__DoWork)(void* self);
		typedef void(*GameStateMachine__Update)(void* self, GameTime* time);

		/*
		EntityWorld* GetEntityWorld();*/
	}

	namespace ecl
	{
		struct EntityWorld : public EntityWorldBase // <ComponentType>
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
		};


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
			using HandleErrorProc = void (void* self, STDString const* message, bool exitGame, STDString const* a4);

			__int64 field_0;
			__int64 GameEventManagerVMT;
			__int64 field_10;
			__int64 NetEventManagerVMT;
			__int64 field_20;
			__int64 VMT2;
			__int64 VMT3;
			__int64 field_38;
			__int64 field_40;
			int field_48;
			int unkn;
			int field_50;
			__int64 field_58;
			GameStateMachine * GameStateMachine;
			__int64 field_68;
			__int64 field_70;
			__int64 field_78;
			__int64 field_80;
			__int64 field_88;
			FixedString field_90;
			FixedString field_94;
			FixedString field_98;
			FixedString CurrentSaveGameGUID;
			bool IsLoading;
			bool IsLoading2;
			__int64 field_A8;
			__int64 field_B0;
			__int64 field_B8;
			bool field_C0;
			__int64 field_C8;
			__int64 field_D0;
			__int64 field_D8;
			bool field_E0;
			ScratchString ScratchStr;
			ScratchBuffer ScratchBuf;
			ModManager* ModManager;
			__int64 ChatManager;
			STDString CurrentHost_M;
			int field_190;
			EntityWorld* EntityWorld;
			__int64 field_1A0;
			__int64 field_1A8;
			RefMap<int, void*> field_1B0;
			__int64 SomeSharedServerClientObj;
			__int64 field_1C8[0x60];
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
