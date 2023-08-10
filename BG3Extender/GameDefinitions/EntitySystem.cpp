#include "stdafx.h"

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/GuidResources.h>
#include <Extender/ScriptExtender.h>

#undef DEBUG_INDEX_MAPPINGS

#if defined(DEBUG_INDEX_MAPPINGS)
#define DEBUG_IDX(x) std::cout << x << std::endl
#else
#define DEBUG_IDX(x)
#endif

namespace bg3se
{
	void* ComponentFactoryBase::FindByHandle(ComponentHandle handle) const
	{
		// FIXME - check TypeIndex in callers!
		if (!handle /*|| handle.GetType() != TypeIndex*/) {
			return nullptr;
		}

		auto index = handle.GetIndex();
		if (index >= ComponentsByHandleIndex.Size()) {
			return nullptr;
		}

		if (Salts[index] != handle.GetSalt()) {
			return nullptr;
		}

		auto& entry = ComponentsByHandleIndex[index];
		if (entry.Component != nullptr && (std::uintptr_t)entry.Component != 0xDEC1A551F1EDDEEDul) {
			return entry.Component;
		} else {
			return nullptr;
		}
	}


	void* NetworkComponentFactoryBase::FindByGuid(FixedString const& guid) const
	{
		if (!guid) return nullptr;

		auto component = Guids.find(guid);
		if (component != Guids.end()) {
			return component.Value();
		} else {
			return nullptr;
		}
	}


	void* NetworkComponentFactoryBase::FindByNetId(NetId netId) const
	{
		if (!netId) return nullptr;

		auto index = netId.GetIndex();
		if (index >= NetIdSalts.Size()
			|| netId.GetSalt() != NetIdSalts[(uint32_t)index]) {
			return nullptr;
		}

		auto object = InUseNetIds.find((uint32_t)index);
		if (object != InUseNetIds.end()) {
			return object.Value();
		} else {
			return nullptr;
		}
	}


	void* NetworkComponentFactoryBase::FindByIndex(uint32_t index) const
	{
		if (index < 0 || index > Components.Size) {
			return nullptr;
		}

		return Components[index];
	}


	ComponentPoolBase* EntityWorldBase::GetComponentPool(HandleTypeIndex handleType, bool logError)
	{
		auto componentIdx = HandleIndexToComponentIndexMap.find((int32_t)handleType);
		if (!componentIdx == HandleIndexToComponentIndexMap.end()) {
			if (logError) {
				OsiError("No component type registered for handle type " << handleType << "!");
			}

			return nullptr;
		}

		return Components.Types[componentIdx.Value()].Pool;
	}

	ComponentPoolBase* EntityWorldBase::GetComponentPool(ComponentTypeIndex componentType, bool logError)
	{
		return Components.Types[(int32_t)componentType].Pool;
	}

	void* EntityWorldBase::GetComponent(ComponentHandle componentHandle, bool logError)
	{
		if (!componentHandle) {
			return nullptr;
		}

		auto pool = GetComponentPool(HandleTypeIndex(componentHandle.GetType()), logError);
		if (!pool) return nullptr;

		return pool->Factory.FindByHandle(componentHandle);
	}

	void* EntityWorldBase::GetComponent(ComponentHandle componentHandle, HandleTypeIndex type, bool logError)
	{
		if (!componentHandle) {
			return nullptr;
		}

		if (componentHandle.GetType() != (int32_t)type) {
			if (logError) {
				OsiError("Attempted to resolve handle of type " << componentHandle.GetType() << " in pool of type " << (int32_t)type);
			}
			return nullptr;
		}

		return GetComponent(componentHandle, logError);
	}

	void* EntityWorldBase::GetComponent(ComponentHandle componentHandle, ComponentTypeIndex type, bool logError)
	{
		if (!componentHandle) {
			return nullptr;
		}

		auto const& componentInfo = Components.Types[(int32_t)type];
		if (componentHandle.GetType() != (int32_t)componentInfo.HandleIndex) {
			if (logError) {
				OsiError("Attempted to resolve handle of type " << componentHandle.GetType() << " in pool of type " << (int32_t)componentInfo.HandleIndex);
			}
			return nullptr;
		}

		return GetComponent(componentHandle, logError);
	}

	void* EntityWorldBase::GetComponent(char const* nameGuid, HandleTypeIndex type, bool logError)
	{
		if (nameGuid == nullptr) {
			OsiError("Attempted to look up component with null name!");
			return nullptr;
		}

		auto fs = NameGuidToFixedString(nameGuid);
		if (!fs) {
			OsiError("Could not map GUID '" << nameGuid << "' to FixedString");
			return nullptr;
		}

		return GetComponent(fs, type, logError);
	}

	void* EntityWorldBase::GetComponent(char const* nameGuid, ComponentTypeIndex type, bool logError)
	{
		if (nameGuid == nullptr) {
			OsiError("Attempted to look up component with null name!");
			return nullptr;
		}

		auto fs = NameGuidToFixedString(nameGuid);
		if (!fs) {
			OsiError("Could not map GUID '" << nameGuid << "' to FixedString");
			return nullptr;
		}

		return GetComponent(fs, type, logError);
	}

	void* EntityWorldBase::GetComponent(FixedString const& guid, HandleTypeIndex type, bool logError)
	{
		if (!guid) {
			OsiError("Attempted to look up component with null GUID!");
			return nullptr;
		}

		auto pool = GetComponentPool(type, logError);
		if (!pool) return nullptr;

		return pool->Factory.FindByGuid(guid);
	}

	void* EntityWorldBase::GetComponent(FixedString const& guid, ComponentTypeIndex type, bool logError)
	{
		if (!guid) {
			OsiError("Attempted to look up component with null GUID!");
			return nullptr;
		}

		auto pool = GetComponentPool(type, logError);
		if (!pool) return nullptr;

		return pool->Factory.FindByGuid(guid);
	}

	void* EntityWorldBase::GetComponent(NetId netId, HandleTypeIndex type, bool logError)
	{
		if (!netId) {
			OsiError("Attempted to look up component with null NetId!");
			return nullptr;
		}

		auto pool = GetComponentPool(type, logError);
		if (!pool) return nullptr;

		return pool->Factory.FindByNetId(netId);
	}

	void* EntityWorldBase::GetComponent(NetId netId, ComponentTypeIndex type, bool logError)
	{
		if (!netId) {
			OsiError("Attempted to look up component with null NetId!");
			return nullptr;
		}

		auto pool = GetComponentPool(type, logError);
		if (!pool) return nullptr;

		return pool->Factory.FindByNetId(netId);
	}

	EntityWorldBase::Entity* EntityWorldBase::GetEntity(EntityHandle entityHandle, bool logError)
	{
		auto typeIndex = entityHandle.GetType();
		if (typeIndex >= std::size(EntityTypes)) {
			if (logError) {
				OsiError("Entity type index " << typeIndex << " too large!");
			}
			return nullptr;
		}

		auto& entityTypes = EntityTypes[typeIndex];
		auto index = entityHandle.GetIndex();
		if (index >= entityTypes.HandleToIndexRemaps.Size()) {
			if (logError) {
				OsiError("Entity index " << index << " too large!");
			}
			return nullptr;
		}

		auto remap = entityTypes.HandleToIndexRemaps[index];
		if (remap == -1) {
			return nullptr;
		}

		auto salt = entityHandle.GetSalt();
		if (salt != entityTypes.Handles[remap].GetSalt()) {
			if (logError) {
				OsiError("Salt mismatch on index " << index << "; " << salt << " != " << entityTypes.Handles[remap].GetSalt());
			}
			return nullptr;
		}

		return &entityTypes.Entities[remap];
	}

	ComponentHandle EntityWorldBase::Entity::GetComponentHandle(int32_t type, bool logError)
	{
		if (type >= (int32_t)ComponentIdToSlotIndexMap.Size()) {
			if (logError) {
				OsiError("Entity " << Handle << " has no component slot for component " << type);
			}
			return {};
		}

		auto slot = ComponentIdToSlotIndexMap[(int32_t)type];
		if (slot == -1) {
			if (logError) {
				OsiError("Entity " << Handle << " has no component bound of type " << type);
			}
			return {};
		}

		return ComponentHandles[slot];
	}

	ComponentHandle EntityWorldBase::GetEntityComponentHandle(EntityHandle entityHandle, ComponentTypeIndex type, bool logError)
	{
		auto entity = GetEntity(entityHandle, logError);
		if (!entity) return ComponentHandle{};

		return entity->GetComponentHandle((int32_t)type, logError);
	}

	ComponentHandle EntityWorldBase::GetEntityComponentHandle(EntityHandle entityHandle, HandleTypeIndex type, bool logError)
	{
		auto componentIndex = HandleIndexToComponentIndexMap.find((int32_t)type);
		if (componentIndex == HandleIndexToComponentIndexMap.end()) {
			if (logError) {
				OsiError("No component index mapping registered for handle type " << (int32_t)type);
			}
			return {};
		}

		return GetEntityComponentHandle(entityHandle, ComponentTypeIndex(componentIndex.Value()), logError);
	}

	void* EntityWorldBase::GetEntityComponent(EntityHandle entityHandle, ComponentTypeIndex type, bool logError)
	{
		auto componentHandle = GetEntityComponentHandle(entityHandle, type, logError);
		if (!componentHandle) return nullptr;

		return GetComponent(componentHandle, type, logError);
	}

	void* EntityWorldBase::GetEntityComponent(EntityHandle entityHandle, HandleTypeIndex type, bool logError)
	{
		auto componentHandle = GetEntityComponentHandle(entityHandle, type, logError);
		if (!componentHandle) return nullptr;

		return GetComponent(componentHandle, type, logError);
	}



	EntitySystemHelpersBase::EntitySystemHelpersBase()
		: componentIndices_{ UndefinedIndex }, handleIndices_{ UndefinedIndex }, resourceManagerIndices_{ UndefinedIndex }
	{}

	void EntitySystemHelpersBase::ComponentIndexMappings::Add(int32_t index)
	{
		if (NumIndices < Indices.size()) {
			Indices[NumIndices++] = index;
		}
	}

	STDString SimplifyComponentName(char const* name)
	{
		STDString key{ name };
		if (key.length() > 6 && strncmp(key.c_str(), "class ", 6) == 0) {
			key = key.substr(6);
		}
		else if (key.length() > 7 && strncmp(key.c_str(), "struct ", 7) == 0) {
			key = key.substr(7);
		}

		if (key.length() > 7 && strncmp(key.c_str() + key.size() - 7, ">(void)", 7) == 0) {
			key = key.substr(0, key.size() - 7);
		}

		return key;
	}

	BitSet<>* EntitySystemHelpersBase::GetReplicationFlags(EntityHandle const& entity, EntityWorldBase::ReplicationTypeIndex replicationType)
	{
		auto world = GetEntityWorld();
		if (!world || !world->Replication) return nullptr;

		auto& pools = world->Replication->ComponentPools;
		auto typeId = (int)replicationType;
		if (typeId >= (int)pools.Size()) {
			OsiError("Attempted to fetch replication list for component " << entity << ", but replication pool size is " << pools.Size() << "!");
			return nullptr;
		}

		auto& pool = pools[typeId];
		auto syncFlags = pool.Find(entity);
		if (syncFlags) {
			return *syncFlags;
		} else {
			return nullptr;
		}
	}

	BitSet<>* EntitySystemHelpersBase::GetReplicationFlags(BaseComponent const& component)
	{
		auto typeIdx = GetReplicationTypeIndex((EntityWorldBase::HandleTypeIndex)component.Handle.GetType());
		if (!typeIdx) {
			OsiError("Couldn't find replication type for component handle " << component.Handle);
			return nullptr;
		}

		return GetReplicationFlags(component.Entity, *typeIdx);
	}

	BitSet<>* EntitySystemHelpersBase::GetOrCreateReplicationFlags(EntityHandle const& entity, EntityWorldBase::ReplicationTypeIndex replicationType)
	{
		auto world = GetEntityWorld();
		if (!world || !world->Replication) return nullptr;

		auto& pools = world->Replication->ComponentPools;
		auto typeId = (int)replicationType;
		if (typeId >= (int)pools.Size()) {
			OsiError("Attempted to fetch replication list for component " << entity << ", but replication pool size is " << pools.Size() << "!");
			return nullptr;
		}

		auto& pool = pools[typeId];
		auto syncFlags = pool.Find(entity);
		if (syncFlags) {
			return *syncFlags;
		}

		return pool.Set(entity, BitSet<>());
	}

	BitSet<>* EntitySystemHelpersBase::GetOrCreateReplicationFlags(BaseComponent const& component)
	{
		auto typeIdx = GetReplicationTypeIndex((EntityWorldBase::HandleTypeIndex)component.Handle.GetType());
		if (!typeIdx) {
			OsiError("Couldn't find replication type for component handle " << component.Handle);
			return nullptr;
		}

		return GetOrCreateReplicationFlags(component.Entity, *typeIdx);
	}

	void EntitySystemHelpersBase::NotifyReplicationFlagsDirtied()
	{
		auto world = GetEntityWorld();
		if (!world || !world->Replication) return;

		world->Replication->Dirty = true;
	}

	void EntitySystemHelpersBase::UpdateComponentMappings()
	{
		if (initialized_) return;

		componentNameToIndexMappings_.clear();
		componentIndexToNameMappings_.clear();
		handleIndexToNameMappings_.clear();
		componentIndexToTypeMappings_.clear();
		handleIndexToTypeMappings_.clear();
		handleIndexToComponentMappings_.clear();
		componentIndices_.fill(UndefinedIndex);
		handleIndices_.fill(UndefinedIndex);
		resourceManagerIndices_.fill(UndefinedIndex);

		auto entityWorld = GetEntityWorld();
		if (!entityWorld) {
			return;
		}

		auto const& symbolMaps = GetStaticSymbols().SymbolIdToNameMaps;

		std::unordered_map<char const*, ComponentIndexMappings> mappings;
		for (auto const& mapping : symbolMaps) {
			auto it = mappings.find(mapping.second);
			if (it == mappings.end()) {
				ComponentIndexMappings newMapping;
				std::fill(newMapping.Indices.begin(), newMapping.Indices.end(), UndefinedIndex);
				newMapping.Add(*mapping.first);
				mappings.insert(std::make_pair(mapping.second, newMapping));
			} else {
				it->second.Add(*mapping.first);
			}
		}

		for (auto& map : mappings) {
			if (map.second.NumIndices == 1) {
				systemIndexMappings_.insert(std::make_pair(SimplifyComponentName(map.first), map.second.Indices[0]));
			}

			std::sort(map.second.Indices.begin(), map.second.Indices.end(), std::greater<int32_t>());
			DEBUG_IDX("\t" << map.first << ": ");
			for (std::size_t i = 0; i < map.second.NumIndices - 1; i++) {
				auto componentIdx = entityWorld->HandleIndexToComponentIndexMap.find(map.second.Indices[i]);
				if (componentIdx != entityWorld->HandleIndexToComponentIndexMap.end()) {
					bool found{ false };
					for (std::size_t j = i + 1; j < map.second.NumIndices; j++) {
						if (map.second.Indices[j] == componentIdx.Value()) {
							DEBUG_IDX(map.second.Indices[i] << " -> " << map.second.Indices[j]);
							IndexMappings indexMapping{ map.second.Indices[i], map.second.Indices[j] };
							auto componentName = SimplifyComponentName(map.first);
							componentNameToIndexMappings_.insert(std::make_pair(componentName, indexMapping));
							componentIndexToNameMappings_.insert(std::make_pair(indexMapping.ComponentIndex, componentName));
							handleIndexToNameMappings_.insert(std::make_pair(indexMapping.HandleIndex, componentName));

							if (map.second.NumIndices == 3) {
								int syncIdx;
								if (i != 0 && j != 0) {
									syncIdx = 0;
								} else if (i != 1 && j != 1) {
									syncIdx = 1;
								} else {
									syncIdx = 2;
								}

								handleIndexToReplicationTypeMappings_.insert(std::make_pair(indexMapping.HandleIndex, map.second.Indices[syncIdx]));
							}

							found = true;
						}
					}

					if (!found) {
						DEBUG_IDX(map.second.Indices[i] << ": No valid mapping");
					}
				} else {
					DEBUG_IDX(map.second.Indices[i] << ": Handle type not registered");
				}
			}
		}

#if defined(DEBUG_INDEX_MAPPINGS)
		DEBUG_IDX("COMPONENT MAPPINGS:");

		for (auto const& map : componentNameToIndexMappings_) {
			DEBUG_IDX("\t" << map.first << ": Handle " << map.second.HandleIndex << ", Component " << map.second.ComponentIndex);
		}

		DEBUG_IDX("-------------------------------------------------------");
#endif

		MapComponentIndices("eoc::ActionResourcesComponent", ExtComponentType::ActionResources);
		MapComponentIndices("eoc::ArmorComponent", ExtComponentType::Armor);
		MapComponentIndices("eoc::BaseHpComponent", ExtComponentType::BaseHp);
		MapComponentIndices("eoc::DataComponent", ExtComponentType::Data);
		MapComponentIndices("eoc::exp::ExperienceComponent", ExtComponentType::Experience);
		MapComponentIndices("eoc::HealthComponent", ExtComponentType::Health);
		MapComponentIndices("eoc::PassiveComponent", ExtComponentType::Passive);
		MapComponentIndices("eoc::HearingComponent", ExtComponentType::Hearing);
		MapComponentIndices("eoc::spell::BookComponent", ExtComponentType::SpellBook);
		MapComponentIndices("eoc::StatsComponent", ExtComponentType::Stats);
		MapComponentIndices("eoc::StatusImmunitiesComponent", ExtComponentType::StatusImmunities);
		MapComponentIndices("eoc::SurfacePathInfluencesComponent", ExtComponentType::SurfacePathInfluences);
		MapComponentIndices("eoc::UseComponent", ExtComponentType::Use);
		MapComponentIndices("eoc::ValueComponent", ExtComponentType::Value);
		MapComponentIndices("eoc::WeaponComponent", ExtComponentType::Weapon);
		MapComponentIndices("eoc::WieldingComponent", ExtComponentType::Wielding);
		MapComponentIndices("eoc::CustomStatsComponent", ExtComponentType::CustomStats);
		MapComponentIndices("eoc::BoostConditionComponent", ExtComponentType::BoostCondition);
		MapComponentIndices("eoc::BoostsContainerComponent", ExtComponentType::BoostsContainer);
		MapComponentIndices("eoc::ActionResourceConsumeMultiplierBoostCompnent", ExtComponentType::ActionResourceConsumeMultiplierBoost);
		MapComponentIndices("eoc::combat::ParticipantComponent", ExtComponentType::CombatParticipant);
		MapComponentIndices("eoc::GenderComponent", ExtComponentType::Gender);
		MapComponentIndices("eoc::spell::ContainerComponent", ExtComponentType::SpellContainer);
		MapComponentIndices("eoc::TagComponent", ExtComponentType::Tag);
		MapComponentIndices("eoc::spell::BookPreparesComponent", ExtComponentType::SpellBookPrepares);
		MapComponentIndices("eoc::combat::StateComponent", ExtComponentType::CombatState);
		MapComponentIndices("eoc::TurnBasedComponent", ExtComponentType::TurnBased);
		MapComponentIndices("eoc::TurnOrderComponent", ExtComponentType::TurnOrder);
		MapComponentIndices("ls::TransformComponent", ExtComponentType::Transform);
		MapComponentIndices("eoc::PassiveContainerComponent", ExtComponentType::PassiveContainer);
		MapComponentIndices("eoc::BoostInfoComponent", ExtComponentType::BoostInfo);
		MapComponentIndices("eoc::RelationComponent", ExtComponentType::Relation);
		MapComponentIndices("eoc::CanInteractComponent", ExtComponentType::CanInteract);
		MapComponentIndices("eoc::CanSpeakComponent", ExtComponentType::CanSpeak);
		MapComponentIndices("eoc::OriginComponent", ExtComponentType::Origin);
		MapComponentIndices("ls::LevelComponent", ExtComponentType::Level);


		MapComponentIndices("eoc::BackgroundComponent", ExtComponentType::Background);
		MapComponentIndices("eoc::GodComponent", ExtComponentType::God);
		MapComponentIndices("eoc::LevelUpComponent", ExtComponentType::LevelUp);
		MapComponentIndices("eoc::spell::PlayerPrepareSpellComponent", ExtComponentType::PlayerPrepareSpell);
		MapComponentIndices("eoc::spell::CCPrepareSpellComponent", ExtComponentType::CCPrepareSpell);
		MapComponentIndices("eoc::spell::CastComponent", ExtComponentType::SpellCast);
		MapComponentIndices("eoc::FloatingComponent", ExtComponentType::Floating);
		MapComponentIndices("eoc::VoiceComponent", ExtComponentType::Voice);
		MapComponentIndices("eoc::CustomIconComponent", ExtComponentType::CustomIcon);
		MapComponentIndices("eoc::CharacterCreationStatsComponent", ExtComponentType::CharacterCreationStats);
		MapComponentIndices("eoc::DisarmableComponent", ExtComponentType::Disarmable);
		MapComponentIndices("eoc::rest::ShortRestComponent", ExtComponentType::ShortRest);
		MapComponentIndices("eoc::summon::IsSummonComponent", ExtComponentType::IsSummon);
		MapComponentIndices("eoc::summon::ContainerComponent", ExtComponentType::SummonContainer);
		MapComponentIndices("eoc::StealthComponent", ExtComponentType::Stealth);
		MapComponentIndices("ls::IsGlobalComponent", ExtComponentType::IsGlobal);
		MapComponentIndices("ls::SavegameComponent", ExtComponentType::Savegame);
		MapComponentIndices("eoc::DisabledEquipmentComponent", ExtComponentType::DisabledEquipment);
		MapComponentIndices("eoc::LootingStateComponent", ExtComponentType::LootingState);
		MapComponentIndices("eoc::LootComponent", ExtComponentType::Loot);
		MapComponentIndices("eoc::lock::LockComponent", ExtComponentType::Lock);
		MapComponentIndices("eoc::summon::LifetimeComponent", ExtComponentType::SummonLifetime);
		MapComponentIndices("eoc::InvisibilityComponent", ExtComponentType::Invisibility);
		MapComponentIndices("eoc::IconComponent", ExtComponentType::Icon);
		MapComponentIndices("eoc::hotbar::ContainerComponent", ExtComponentType::HotbarContainer);
		MapComponentIndices("eoc::OriginTagComponent", ExtComponentType::OriginTag);
		MapComponentIndices("eoc::OriginPassivesComponent", ExtComponentType::OriginPassives);
		MapComponentIndices("eoc::GodTagComponent", ExtComponentType::GodTag);
		MapComponentIndices("eoc::ClassTagComponent", ExtComponentType::ClassTag);
		MapComponentIndices("eoc::BackgroundTagComponent", ExtComponentType::BackgroundTag);
		MapComponentIndices("eoc::BackgroundPassivesComponent", ExtComponentType::BackgroundPassives);
		MapComponentIndices("eoc::GlobalShortRestDisabledComponent", ExtComponentType::GlobalShortRestDisabled);
		MapComponentIndices("eoc::GlobalLongRestDisabledComponent", ExtComponentType::GlobalLongRestDisabled);
		MapComponentIndices("eoc::StoryShortRestDisabledComponent", ExtComponentType::StoryShortRestDisabled);
		MapComponentIndices("eoc::FleeCapabilityComponent", ExtComponentType::FleeCapability);
		MapComponentIndices("eoc::CanDoRestComponent", ExtComponentType::CanDoRest);
		MapComponentIndices("eoc::ItemBoostsComponent", ExtComponentType::ItemBoosts);
		MapComponentIndices("eoc::light::ActiveCharacterLightComponent", ExtComponentType::ActiveCharacterLight);
		MapComponentIndices("ls::AnimationSetComponent", ExtComponentType::AnimationSet);
		MapComponentIndices("ls::AnimationBlueprintComponent", ExtComponentType::AnimationBlueprint);
		MapComponentIndices("eoc::CanModifyHealthComponent", ExtComponentType::CanModifyHealth);
		MapComponentIndices("eoc::spell::AddedSpellsComponent", ExtComponentType::AddedSpells);
		MapComponentIndices("eoc::exp::AvailableLevelComponent", ExtComponentType::AvailableLevel);
		MapComponentIndices("eoc::CanBeLootedComponent", ExtComponentType::CanBeLooted);
		MapComponentIndices("eoc::CanDoActionsComponent", ExtComponentType::CanDoActions);
		MapComponentIndices("eoc::CanMoveComponent", ExtComponentType::CanMove);
		MapComponentIndices("eoc::CanSenseComponent", ExtComponentType::CanSense);
		MapComponentIndices("eoc::ConcentrationComponent", ExtComponentType::Concentration);
		MapComponentIndices("eoc::DarknessComponent", ExtComponentType::Darkness);
		MapComponentIndices("eoc::DualWieldingComponent", ExtComponentType::DualWielding);
		MapComponentIndices("eoc::GameObjectVisualComponent", ExtComponentType::GameObjectVisual);
		MapComponentIndices("eoc::InventorySlotComponent", ExtComponentType::InventorySlot);
		MapComponentIndices("eoc::spell::BookCooldownsComponent", ExtComponentType::SpellBookCooldowns);
		MapComponentIndices("eoc::DisplayNameComponent", ExtComponentType::DisplayName);
		MapComponentIndices("eoc::EquipableComponent", ExtComponentType::Equipable);
		MapComponentIndices("eoc::GameplayLightComponent", ExtComponentType::GameplayLight);
		MapComponentIndices("eoc::ProgressionContainerComponent", ExtComponentType::ProgressionContainer);
		MapComponentIndices("eoc::progression::MetaComponent", ExtComponentType::ProgressionMeta);
		MapComponentIndices("eoc::RaceComponent", ExtComponentType::Race);
		MapComponentIndices("eoc::sight::ReplicatedDataComponent", ExtComponentType::Sight);
		MapComponentIndices("eoc::CanTravelComponent", ExtComponentType::CanTravel);
		MapComponentIndices("eoc::CanBeInInventoryComponent", ExtComponentType::CanBeInInventory);
		MapComponentIndices("eoc::MovementComponent", ExtComponentType::Movement);
		MapComponentIndices("eoc::ObjectInteractionComponent", ExtComponentType::ObjectInteraction);
		MapComponentIndices("eoc::PathingComponent", ExtComponentType::Pathing);
		MapComponentIndices("eoc::SteeringComponent", ExtComponentType::Steering);
		MapComponentIndices("eoc::CanDeflectProjectilesComponent", ExtComponentType::CanDeflectProjectiles);
		MapComponentIndices("eoc::spell::LearnedSpellsComponent", ExtComponentType::LearnedSpells);
		MapComponentIndices("eoc::spell::AiConditionsComponent", ExtComponentType::SpellAiConditions);
		MapComponentIndices("ls::ActiveSkeletonSlotsComponent", ExtComponentType::ActiveSkeletonSlots);
		MapComponentIndices("ls::NetComponent", ExtComponentType::Net);
		MapComponentIndices("ls::PhysicsComponent", ExtComponentType::Physics);
		MapComponentIndices("eoc::ftb::ParticipantComponent", ExtComponentType::FTBParticipant);
		MapComponentIndices("eoc::unsheath::InfoComponent", ExtComponentType::UnsheathInfo);
		MapComponentIndices("eoc::approval::Ratings", ExtComponentType::ApprovalRatings);
		MapComponentIndices("eoc::character_creation::AppearanceComponent", ExtComponentType::CharacterCreationAppearance);


		MapResourceManagerIndex("ls::TagManager", ExtResourceManagerType::Tag);
		MapResourceManagerIndex("eoc::FactionContainer", ExtResourceManagerType::Faction);
		MapResourceManagerIndex("eoc::RaceManager", ExtResourceManagerType::Race);
		MapResourceManagerIndex("eoc::AbilityDistributionPresetManager", ExtResourceManagerType::AbilityDistributionPreset);
		MapResourceManagerIndex("eoc::CharacterCreationPresetManager", ExtResourceManagerType::CharacterCreationPreset);
		MapResourceManagerIndex("eoc::CharacterCreationSkinColorManager", ExtResourceManagerType::CharacterCreationSkinColor);
		MapResourceManagerIndex("eoc::CharacterCreationEyeColorManager", ExtResourceManagerType::CharacterCreationEyeColor);
		MapResourceManagerIndex("eoc::CharacterCreationHairColorManager", ExtResourceManagerType::CharacterCreationHairColor);
		MapResourceManagerIndex("eoc::CompanionPresetManager", ExtResourceManagerType::CompanionPreset);
		MapResourceManagerIndex("eoc::OriginManager", ExtResourceManagerType::Origin);
		MapResourceManagerIndex("eoc::BackgroundManager", ExtResourceManagerType::Background);
		MapResourceManagerIndex("eoc::GodManager", ExtResourceManagerType::God);
		MapResourceManagerIndex("eoc::AbilityListManager", ExtResourceManagerType::AbilityList);
		MapResourceManagerIndex("eoc::SkillListManager", ExtResourceManagerType::SkillList);
		MapResourceManagerIndex("eoc::SpellListManager", ExtResourceManagerType::SpellList);
		MapResourceManagerIndex("eoc::PassiveListManager", ExtResourceManagerType::PassiveList);
		MapResourceManagerIndex("eoc::ProgressionManager", ExtResourceManagerType::Progression);
		MapResourceManagerIndex("eoc::ProgressionDescriptionManager", ExtResourceManagerType::ProgressionDescription);
		MapResourceManagerIndex("eoc::GossipContainer", ExtResourceManagerType::Gossip);
		MapResourceManagerIndex("eoc::ActionResourceTypes", ExtResourceManagerType::ActionResource);
		MapResourceManagerIndex("eoc::ActionResourceGroupManager", ExtResourceManagerType::ActionResourceGroup);
		MapResourceManagerIndex("eoc::EquipmentTypes", ExtResourceManagerType::EquipmentType);
		MapResourceManagerIndex("eoc::VFXContainer", ExtResourceManagerType::VFX);
		MapResourceManagerIndex("eoc::DeathTypesContainer", ExtResourceManagerType::DeathType);
		MapResourceManagerIndex("eoc::CharacterCreationAppearanceMaterialManager", ExtResourceManagerType::CharacterCreationAppearanceMaterial);
		MapResourceManagerIndex("eoc::CharacterCreationAppearanceVisualManager", ExtResourceManagerType::CharacterCreationAppearanceVisual);
		MapResourceManagerIndex("eoc::CharacterCreationSharedVisualManager", ExtResourceManagerType::CharacterCreationSharedVisual);
		MapResourceManagerIndex("eoc::tutorial::EntriesManager", ExtResourceManagerType::TutorialEntries);
		MapResourceManagerIndex("eoc::FeatManager", ExtResourceManagerType::Feat);
		MapResourceManagerIndex("eoc::FeatDescriptionManager", ExtResourceManagerType::FeatDescription);
		MapResourceManagerIndex("eoc::tutorial::ModalEntriesManager", ExtResourceManagerType::TutorialModalEntries);
		MapResourceManagerIndex("eoc::AvailableClassSpellsManager", ExtResourceManagerType::AvailableClassSpells);
		MapResourceManagerIndex("eoc::ClassDescriptions", ExtResourceManagerType::ClassDescription);
		MapResourceManagerIndex("eoc::ColorDefinitions", ExtResourceManagerType::ColorDefinition);
		MapResourceManagerIndex("ls::FlagManager", ExtResourceManagerType::Flag);

#define MAP_BOOST(name) MapComponentIndices("eoc::" #name "Component", ExtComponentType::name)

		MAP_BOOST(ArmorClassBoost);
		MAP_BOOST(AbilityBoost);
		MAP_BOOST(RollBonusBoost);
		MAP_BOOST(AdvantageBoost);
		MAP_BOOST(ActionResourceValueBoost);
		MAP_BOOST(CriticalHitBoost);
		MAP_BOOST(AbilityFailedSavingThrowBoost);
		MAP_BOOST(ResistanceBoost);
		MAP_BOOST(WeaponDamageResistanceBoost);
		MAP_BOOST(ProficiencyBonusOverrideBoost);
		MAP_BOOST(JumpMaxDistanceMultiplierBoost);
		MAP_BOOST(HalveWeaponDamageBoost);
		MAP_BOOST(UnlockSpellBoost);
		MAP_BOOST(SourceAdvantageBoost);
		MAP_BOOST(ProficiencyBonusBoost);
		MAP_BOOST(ProficiencyBoost);
		MAP_BOOST(IncreaseMaxHPBoost);
		MAP_BOOST(ActionResourceBlockBoost);
		MAP_BOOST(StatusImmunityBoost);
		MAP_BOOST(UseBoosts);
		MAP_BOOST(TemporaryHPBoost);
		MAP_BOOST(WeightBoost);
		MAP_BOOST(FactionOverrideBoost);
		MAP_BOOST(ActionResourceMultiplierBoost);
		MAP_BOOST(InitiativeBoost);
		MAP_BOOST(DarkvisionRangeBoost);
		MAP_BOOST(DarkvisionRangeMinBoost);
		MAP_BOOST(DarkvisionRangeOverrideBoost);
		MAP_BOOST(AddTagBoost);
		MAP_BOOST(IgnoreDamageThresholdMinBoost);
		MAP_BOOST(SkillBoost);
		MAP_BOOST(WeaponDamageBoost);
		MAP_BOOST(NullifyAbilityBoost);
		MAP_BOOST(RerollBoost);
		MAP_BOOST(DownedStatusBoost);
		MAP_BOOST(WeaponEnchantmentBoost);
		MAP_BOOST(GuaranteedChanceRollOutcomeBoost);
		MAP_BOOST(AttributeBoost);
		MAP_BOOST(GameplayLightBoost);
		MAP_BOOST(DualWieldingBoost);
		MAP_BOOST(SavantBoost);
		MAP_BOOST(MinimumRollResultBoost);
		MAP_BOOST(CharacterWeaponDamageBoost);
		MAP_BOOST(ProjectileDeflectBoost);
		MAP_BOOST(AbilityOverrideMinimumBoost);
		MAP_BOOST(ACOverrideMinimumBoost);
		MAP_BOOST(FallDamageMultiplierBoost);

		initialized_ = true;
	}

	void EntitySystemHelpersBase::MapComponentIndices(char const* componentName, ExtComponentType type)
	{
		auto it = componentNameToIndexMappings_.find(componentName);
		if (it != componentNameToIndexMappings_.end()) {
			componentIndices_[(unsigned)type] = it->second.ComponentIndex;
			handleIndices_[(unsigned)type] = it->second.HandleIndex;
			componentIndexToTypeMappings_.insert(std::make_pair((int32_t)it->second.ComponentIndex, type));
			handleIndexToTypeMappings_.insert(std::make_pair((int32_t)it->second.HandleIndex, type));
			handleIndexToComponentMappings_.insert(std::make_pair((int32_t)it->second.HandleIndex, (int32_t)it->second.ComponentIndex));
		} else {
			OsiWarn("Could not find index for component: " << componentName);
		}
	}

	void EntitySystemHelpersBase::MapResourceManagerIndex(char const* componentName, ExtResourceManagerType type)
	{
		auto it = systemIndexMappings_.find(componentName);
		if (it != systemIndexMappings_.end()) {
			resourceManagerIndices_[(unsigned)type] = it->second;
		} else {
			OsiWarn("Could not find index for resource manager: " << componentName);
		}
	}

	void* EntitySystemHelpersBase::GetRawComponent(NetId netId, ExtComponentType type, bool logError)
	{
		auto world = GetEntityWorld();
		if (!world) {
			if (logError) {
				OsiError("EntityWorld not initialized yet!");
			}

			return nullptr;
		}

		auto componentIndex = GetComponentIndex(type);
		if (componentIndex) {
			return world->GetComponent(netId, *componentIndex, logError);
		} else {
			if (logError) {
				OsiError("No component index mapping registered for " << type);
			}

			return nullptr;
		}
	}

	void* EntitySystemHelpersBase::GetRawComponent(ComponentHandle componentHandle, ExtComponentType type, bool logError)
	{
		auto world = GetEntityWorld();
		if (!world) {
			if (logError) {
				OsiError("EntityWorld not initialized yet!");
			}

			return nullptr;
		}

		auto handleIndex = GetHandleIndex(type);
		if (handleIndex) {
			if ((int32_t)*handleIndex != (int32_t)componentHandle.GetType()) {
				if (logError) {
					OsiError("Attempted to resolve handle of type " << *handleIndex << " in a pool of type " << type);
				}

				return nullptr;
			}

			auto componentIndex = GetComponentIndex(type);
			return world->GetComponent(componentHandle, *componentIndex, logError);
		} else {
			if (logError) {
				OsiError("No component index mapping registered for " << type);
			}

			return nullptr;
		}
	}

	void* EntitySystemHelpersBase::GetRawComponent(char const* nameGuid, ExtComponentType type, bool logError)
	{
		auto world = GetEntityWorld();
		if (!world) {
			if (logError) {
				OsiError("EntityWorld not initialized yet!");
			}

			return nullptr;
		}

		auto componentIndex = GetComponentIndex(type);
		if (componentIndex) {
			return world->GetComponent(nameGuid, *componentIndex, logError);
		} else {
			if (logError) {
				OsiError("No component index mapping registered for " << type);
			}

			return nullptr;
		}
	}

	void* EntitySystemHelpersBase::GetRawComponent(FixedString const& guid, ExtComponentType type, bool logError)
	{
		auto world = GetEntityWorld();
		if (!world) {
			if (logError) {
				OsiError("EntityWorld not initialized yet!");
			}

			return nullptr;
		}

		auto componentIndex = GetComponentIndex(type);
		if (componentIndex) {
			return world->GetComponent(guid, *componentIndex, logError);
		} else {
			if (logError) {
				OsiError("No component index mapping registered for " << type);
			}

			return nullptr;
		}
	}

	void* EntitySystemHelpersBase::GetRawEntityComponent(EntityHandle entityHandle, ExtComponentType type, bool logError)
	{
		auto world = GetEntityWorld();
		if (!world) {
			if (logError) {
				OsiError("EntityWorld not initialized yet!");
			}

			return nullptr;
		}

		auto componentIndex = GetComponentIndex(type);
		if (componentIndex) {
			return world->GetEntityComponent(entityHandle, *componentIndex, logError);
		} else {
			if (logError) {
				OsiError("No component index mapping registered for " << type);
			}

			return nullptr;
		}
	}

	GuidResourceDefinitionManagerBase* EntitySystemHelpersBase::GetRawResourceManager(ExtResourceManagerType type)
	{
		auto index = resourceManagerIndices_[(int)type];
		if (index == UndefinedIndex) {
			OsiError("No resource manager index mapping registered for " << type);
			return {};
		}

		auto defns = GetStaticSymbols().ResourceDefns;
		if (!defns || !*defns) {
			OsiError("Resource definition manager not available yet!");
			return {};
		}

		auto res = (*defns)->Definitions.Find(index);
		if (!res) {
			OsiError("Resource manager missing for " << type);
			return {};
		}

		return **res;
	}

	void ServerEntitySystemHelpers::Setup()
	{
		return;
		UpdateComponentMappings();


		MapComponentIndices("esv::recruit::RecruitedByComponent", ExtComponentType::ServerRecruitedBy);
		MapComponentIndices("esv::GameTimerComponent", ExtComponentType::ServerGameTimer);
		MapComponentIndices("esv::exp::ExperienceGaveOutComponent", ExtComponentType::ServerExperienceGaveOut);
		MapComponentIndices("esv::ReplicationDependencyComponent", ExtComponentType::ServerReplicationDependency);
		MapComponentIndices("esv::summon::IsUnsummoningComponent", ExtComponentType::ServerIsUnsummoning);
		MapComponentIndices("esv::combat::FleeBlockedComponent", ExtComponentType::ServerFleeBlocked);
		MapComponentIndices("esv::ActivationGroupContainerComponent", ExtComponentType::ServerActivationGroupContainer);
		MapComponentIndices("esv::AnubisTagComponent", ExtComponentType::ServerAnubisTag);
		MapComponentIndices("esv::DialogTagComponent", ExtComponentType::ServerDialogTag);
		MapComponentIndices("esv::DisplayNameListComponent", ExtComponentType::ServerDisplayNameList);
		MapComponentIndices("esv::IconListComponent", ExtComponentType::ServerIconList);
		MapComponentIndices("esv::PlanTagComponent", ExtComponentType::ServerPlanTag);
		MapComponentIndices("esv::RaceTagComponent", ExtComponentType::ServerRaceTag);
		MapComponentIndices("esv::TemplateTagComponent", ExtComponentType::ServerTemplateTag);
		MapComponentIndices("esv::passive::ToggledPassivesComponent", ExtComponentType::ServerToggledPassives);
		MapComponentIndices("esv::BoostTagComponent", ExtComponentType::ServerBoostTag);
		MapComponentIndices("esv::TriggerStateComponent", ExtComponentType::ServerTriggerState);
		MapComponentIndices("esv::SafePositionComponent", ExtComponentType::ServerSafePosition);
		MapComponentIndices("esv::AnubisExecutorComponent", ExtComponentType::ServerAnubisExecutor);
		MapComponentIndices("esv::LeaderComponent", ExtComponentType::ServerLeader);
		MapComponentIndices("esv::BreadcrumbComponent", ExtComponentType::ServerBreadcrumb);
		MapComponentIndices("esv::death::DelayDeathCauseComponent", ExtComponentType::ServerDelayDeathCause);
		MapComponentIndices("esv::pickpocket::PickpocketComponent", ExtComponentType::ServerPickpocket);
		MapComponentIndices("esv::ReplicationDependencyOwnerComponent", ExtComponentType::ServerReplicationDependencyOwner);

		MapComponentIndices("ls::StaticPhysicsComponent", ExtComponentType::StaticPhysics);
		MapComponentIndices("ls::anubis::Component", ExtComponentType::Anubis);

		MapComponentIndices("esv::Character", ExtComponentType::ServerCharacter);
		MapComponentIndices("esv::Item", ExtComponentType::ServerItem);
		MapComponentIndices("esv::Projectile", ExtComponentType::ServerProjectile);
		MapComponentIndices("esv::OsirisTagComponent", ExtComponentType::ServerOsirisTag);
		MapComponentIndices("esv::ActiveComponent", ExtComponentType::ServerActive);
	}

	void ClientEntitySystemHelpers::Setup()
	{
		return;
		UpdateComponentMappings();

		MapComponentIndices("ls::VisualComponent", ExtComponentType::Visual);

		MapComponentIndices("ecl::Character", ExtComponentType::ClientCharacter);
		MapComponentIndices("ecl::Item", ExtComponentType::ClientItem);
		MapComponentIndices("ecl::Projectile", ExtComponentType::ClientProjectile);
	}



	EntityWorldBase* ServerEntitySystemHelpers::GetEntityWorld()
	{
		return GetStaticSymbols().GetServerEntityWorld();
	}

	EntityWorldBase* ClientEntitySystemHelpers::GetEntityWorld()
	{
		return GetStaticSymbols().GetClientEntityWorld();
	}
}
