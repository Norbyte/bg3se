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

BEGIN_NS(ecs)

void* EntityClass::GetComponent(EntityHandle entityHandle, ComponentTypeIndex type) const
{
	auto componentBucket = InstanceToPageMap.Find(entityHandle.Handle);
	if (componentBucket) {
		auto compIndex = ComponentTypeToIndex.Find((uint16_t)type.Value());
		if (compIndex) {
			auto& pool = ComponentPools[(*componentBucket)->BucketIndex]->Pool[**compIndex];
			return pool.Components[(*componentBucket)->EntryIndex];
		}
	}

	return nullptr;
}
	
void* EntityWorld::GetComponent(EntityHandle entityHandle, ComponentTypeIndex type)
{
	auto entityClass = GetEntityClass(entityHandle);
	if (entityClass != nullptr) {
		auto component = entityClass->GetComponent(entityHandle, type);
		if (component != nullptr) {
			return component;
		}
	}

	auto compPool = Components->ComponentsByType.Find((uint16_t)type);
	if (compPool) {
		auto transientRef = (*compPool)->Find(entityHandle.Handle);
		if (transientRef) {
			return **transientRef;
		}
	}

	auto compPool2 = Components->ComponentsByType2.Find((uint16_t)type);
	if (compPool2) {
		auto transientRef = (*compPool2)->Find(entityHandle.Handle);
		if (transientRef) {
			return **transientRef;
		}
	}

	return nullptr;
}
	
void* EntityWorld::GetComponent(char const* nameGuid, ComponentTypeIndex type)
{
	auto fs = NameGuidToFixedString(nameGuid);
	if (!fs) {
		OsiError("Could not map GUID '" << nameGuid << "' to FixedString");
		return nullptr;
	}

	return GetComponent(fs, type);
}

void* EntityWorld::GetComponent(FixedString const& guid, ComponentTypeIndex type)
{
	ERR("FIXME!");
	return nullptr;
}

bool EntityWorld::IsValid(EntityHandle entityHandle) const
{
	if (entityHandle.GetType() < 0x40) {
		auto salts = (*EntitySalts)[entityHandle.GetType()];
		if (entityHandle.GetIndex() < salts.NumElements) {
			auto salt = salts.Buckets[entityHandle.GetIndex() >> salts.BitsPerBucket][entityHandle.GetIndex() & ((1 << salts.BitsPerBucket) - 1)];
			return salt.Salt == entityHandle.GetSalt() && salt.Index == entityHandle.GetIndex();
		}
	}

	return false;
}

EntityClass* EntityStore::GetEntityClass(EntityHandle entityHandle) const
{
	auto& componentSalts = Salts.Buckets[entityHandle.GetType()];
	if (entityHandle.GetIndex() < componentSalts.NumElements) {
		auto salt = componentSalts.Buckets[entityHandle.GetIndex() >> componentSalts.BitsPerBucket][entityHandle.GetIndex() & ((1 << componentSalts.BitsPerBucket) - 1)];
		if (salt.Salt == entityHandle.GetSalt()) {
			return EntityClasses[salt.EntityClassIndex];
		}
	}

	return nullptr;
}

EntityClass* EntityWorld::GetEntityClass(EntityHandle entityHandle) const
{
	if (!IsValid(entityHandle)) {
		return nullptr;
	}

	return EntityTypes->GetEntityClass(entityHandle);
}

EntitySystemHelpersBase::EntitySystemHelpersBase()
	: componentIndices_{ UndefinedComponent }, handleIndices_{ UndefinedHandle }, resourceManagerIndices_{ UndefinedIndex }
{}

void EntitySystemHelpersBase::ComponentIndexMappings::Add(int32_t index, IndexSymbolType type)
{
	switch (type) {
	case IndexSymbolType::Replication:
		assert(ReplicationIndex == -1);
		ReplicationIndex = index;
		break;
			
	case IndexSymbolType::Handle:
		assert(HandleIndex == -1);
		HandleIndex = index;
		break;
			
	case IndexSymbolType::Component:
		assert(ComponentIndex == -1);
		ComponentIndex = index;
		break;
			
	case IndexSymbolType::EventComponent:
		assert(EventComponentIndex == -1);
		EventComponentIndex = index;
		break;

	default:
		assert(NumIndices < Indices.size());
		if (NumIndices < Indices.size()) {
			Indices[NumIndices++] = index;
		}
		break;
	}
}

STDString SimplifyComponentName(StringView name)
{
	STDString key{ name };
	if (key.length() > 52 && strncmp(key.c_str(), "class ls::_StringView<char> __cdecl ls::GetTypeName<", 52) == 0) {
		key = key.substr(52);
	}

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

BitSet<>* EntitySystemHelpersBase::GetReplicationFlags(EntityHandle const& entity, ComponentTypeIndex type)
{
	auto world = GetEntityWorld();
	if (!world || !world->Replication) return nullptr;

	return nullptr;
	/*auto& pools = world->Replication->ComponentPools;
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
	}*/
}

BitSet<>* EntitySystemHelpersBase::GetOrCreateReplicationFlags(EntityHandle const& entity, ComponentTypeIndex replicationType)
{
	auto world = GetEntityWorld();
	if (!world || !world->Replication) return nullptr;

	return nullptr;
	/*auto& pools = world->Replication->ComponentPools;
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

	return pool.Set(entity, BitSet<>());*/
}

void EntitySystemHelpersBase::NotifyReplicationFlagsDirtied()
{
	auto world = GetEntityWorld();
	if (!world || !world->Replication) return;

	world->Replication->Dirty = true;
}

void EntitySystemHelpersBase::BindSystemName(StringView name, int32_t systemId)
{
	auto it = systemIndexMappings_.insert(std::make_pair(name, systemId));
	if (systemIndices_.size() <= systemId) {
		systemIndices_.resize(systemId + 1);
	}

	systemIndices_[systemId] = &it.first->first;
}

void EntitySystemHelpersBase::BindQueryName(StringView name, int32_t queryId)
{
	auto it = queryMappings_.insert(std::make_pair(name, queryId));
	if (queryIndices_.size() <= queryId) {
		queryIndices_.resize(queryId + 1);
	}

	queryIndices_[queryId] = &it.first->first;
}

bool EntitySystemHelpersBase::TryUpdateSystemMapping(StringView name, ComponentIndexMappings& mapping)
{
	if (mapping.NumIndices == 1
		&& mapping.ReplicationIndex == -1
		&& mapping.HandleIndex == -1
		&& mapping.ComponentIndex == -1
		&& mapping.EventComponentIndex == -1) {
		if (name.starts_with("ecs::query::spec::Spec<")) {
			BindQueryName(name, mapping.Indices[0]);
		} else {
			BindSystemName(name, mapping.Indices[0]);
		}
		return true;
	}

	return false;
}

void EntitySystemHelpersBase::TryUpdateComponentMapping(StringView name, ComponentIndexMappings& mapping)
{
	std::sort(mapping.Indices.begin(), mapping.Indices.begin() + mapping.NumIndices, std::less<int32_t>());
	DEBUG_IDX("\t" << name << ": ");

	auto totalIndices = mapping.NumIndices
		+ ((mapping.ReplicationIndex != -1) ? 1 : 0)
		+ ((mapping.HandleIndex != -1) ? 1 : 0)
		+ ((mapping.ComponentIndex != -1) ? 1 : 0)
		+ ((mapping.EventComponentIndex != -1) ? 1 : 0);

	if (totalIndices == 1) {
		assert(mapping.ReplicationIndex == -1);
		assert(mapping.HandleIndex == -1);

		if (mapping.ComponentIndex == -1) {
			mapping.ComponentIndex = mapping.Indices[0];
		}
	} else if (totalIndices == 2) {
		// Only HandleIndex and ComponentIndex, no replication
		assert(mapping.ReplicationIndex == -1);
		assert(mapping.HandleIndex == -1);

		if (mapping.EventComponentIndex != -1) {
			DEBUG_IDX("Event component, ignored.");
			return;
		}

		unsigned nextIndex{ 0 };
		if (mapping.ComponentIndex == -1) {
			// Maybe this is a system?
			if (name.find("Component") == StringView::npos) {
				BindSystemName(name, mapping.Indices[0]);
			} else {
				mapping.ComponentIndex = mapping.Indices[nextIndex++];
			}
		}

		mapping.HandleIndex = mapping.Indices[nextIndex++];
	} else if (totalIndices == 3) {
		unsigned nextIndex{ 0 };

		if (mapping.ReplicationIndex == -1) {
			mapping.ReplicationIndex = mapping.Indices[nextIndex++];
		}

		if (mapping.ComponentIndex == -1) {
			mapping.ComponentIndex = mapping.Indices[nextIndex++];
		}

		if (mapping.HandleIndex == -1) {
			mapping.HandleIndex = mapping.Indices[nextIndex++];
		}
	} else {
		WARN("Component with strange configuration: %s", name.data());
		return;
	}

	DEBUG_IDX("Repl " << mapping.ReplicationIndex << ", Handle " << mapping.HandleIndex << ", Comp " << mapping.ComponentIndex);
	IndexMappings indexMapping{ (uint16_t)mapping.HandleIndex, (uint16_t)mapping.ComponentIndex };
	auto it = componentNameToIndexMappings_.insert(std::make_pair(name, indexMapping));
	componentIndexToNameMappings_.insert(std::make_pair(indexMapping.ComponentIndex, &it.first->first));
	handleIndexToNameMappings_.insert(std::make_pair(indexMapping.HandleIndex, &it.first->first));
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

	auto const& symbolMaps = GetStaticSymbols().IndexSymbolToNameMaps;

	std::unordered_map<char const*, ComponentIndexMappings> mappings;
	for (auto const& mapping : symbolMaps) {
		auto it = mappings.find(mapping.second.name);
		if (it == mappings.end()) {
			ComponentIndexMappings newMapping;
			std::fill(newMapping.Indices.begin(), newMapping.Indices.end(), UndefinedIndex);
			newMapping.Add(*mapping.first, mapping.second.type);
			mappings.insert(std::make_pair(mapping.second.name, newMapping));
		} else {
			it->second.Add(*mapping.first, mapping.second.type);
		}
	}

	std::vector<std::pair<STDString, ComponentIndexMappings>> pendingMappings;
	for (auto& map : mappings) {
		auto componentName = SimplifyComponentName(map.first);
		if (!TryUpdateSystemMapping(componentName, map.second)) {
			pendingMappings.push_back({ std::move(componentName), map.second});
		}
	}
	
	for (auto& map : pendingMappings) {
		TryUpdateComponentMapping(map.first, map.second);
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
	MapResourceManagerIndex("eoc::CharacterCreationAppearanceMaterialManager", ExtResourceManagerType::CharacterCreationAppearanceMaterial);
	MapResourceManagerIndex("eoc::CharacterCreationAppearanceVisualManager", ExtResourceManagerType::CharacterCreationAppearanceVisual);
	MapResourceManagerIndex("eoc::CharacterCreationSharedVisualManager", ExtResourceManagerType::CharacterCreationSharedVisual);
	MapResourceManagerIndex("eoc::tutorial::EntriesManager", ExtResourceManagerType::TutorialEntries);
	MapResourceManagerIndex("eoc::FeatManager", ExtResourceManagerType::Feat);
	MapResourceManagerIndex("eoc::FeatDescriptionManager", ExtResourceManagerType::FeatDescription);
	MapResourceManagerIndex("eoc::tutorial::ModalEntriesManager", ExtResourceManagerType::TutorialModalEntries);
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
		componentIndexToTypeMappings_.insert(std::make_pair(it->second.ComponentIndex, type));
		handleIndexToTypeMappings_.insert(std::make_pair(it->second.HandleIndex, type));
		handleIndexToComponentMappings_.insert(std::make_pair(it->second.HandleIndex, it->second.ComponentIndex));
	} else {
		// FIXME - disabled until we map the new component list
		// OsiWarn("Could not find index for component: " << componentName);
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

void* EntitySystemHelpersBase::GetRawComponent(char const* nameGuid, ExtComponentType type)
{
	auto world = GetEntityWorld();
	if (!world) {
		return nullptr;
	}

	auto componentIndex = GetComponentIndex(type);
	if (componentIndex) {
		return world->GetComponent(nameGuid, *componentIndex);
	} else {
		return nullptr;
	}
}

void* EntitySystemHelpersBase::GetRawComponent(FixedString const& guid, ExtComponentType type)
{
	auto world = GetEntityWorld();
	if (!world) {
		return nullptr;
	}

	auto componentIndex = GetComponentIndex(type);
	if (componentIndex) {
		return world->GetComponent(guid, *componentIndex);
	} else {
		return nullptr;
	}
}

void* EntitySystemHelpersBase::GetRawEntityComponent(EntityHandle entityHandle, ExtComponentType type)
{
	auto world = GetEntityWorld();
	if (!world) {
		return nullptr;
	}

	auto componentIndex = GetComponentIndex(type);
	if (componentIndex) {
		return world->GetComponent(entityHandle, *componentIndex);
	} else {
		return nullptr;
	}
}

resource::GuidResourceBankBase* EntitySystemHelpersBase::GetRawResourceManager(ExtResourceManagerType type)
{
	auto index = resourceManagerIndices_[(int)type];
	if (index == UndefinedIndex) {
		OsiError("No resource manager index mapping registered for " << type);
		return {};
	}

	auto defns = GetStaticSymbols().eoc__gGuidResourceManager;
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
	UpdateComponentMappings();

	MapComponentIndices("ls::VisualComponent", ExtComponentType::Visual);

	MapComponentIndices("ecl::Character", ExtComponentType::ClientCharacter);
	MapComponentIndices("ecl::Item", ExtComponentType::ClientItem);
	MapComponentIndices("ecl::Projectile", ExtComponentType::ClientProjectile);
}



EntityWorld* ServerEntitySystemHelpers::GetEntityWorld()
{
	return GetStaticSymbols().GetServerEntityWorld();
}

EntityWorld* ClientEntitySystemHelpers::GetEntityWorld()
{
	return GetStaticSymbols().GetClientEntityWorld();
}

END_NS()
