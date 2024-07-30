#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Stats/Functors.h>

BEGIN_NS(stats)

struct DescriptionInfo
{
	TranslatedString DisplayName;
	FixedString Icon;
	TranslatedString Description;
	STDString DescriptionParams;
	TranslatedString ExtraDescription;
	STDString ExtraDescriptionParams;
	TranslatedString ShortDescription;
	STDString ShortDescriptionParams;
	TranslatedString LoreDescription;
};

struct TextKeyHitCosts
{
	FixedString TextKey;
	Array<ActionResourceCost> Costs;
};

struct VariableResourceCost
{
	Guid Resource;
	float Amount;
};

struct SpellPrototypeAnimationData
{
	std::array<FixedString, 3> Part0;
	std::array<FixedString, 3> Part6;
	std::array<FixedString, 3> Part4;
	std::array<FixedString, 3> Part1;
	std::array<FixedString, 3> Part5;
	Array<std::array<FixedString, 3>> Part2;
	std::array<FixedString, 3> Part3;
	std::array<FixedString, 3> Part7;
	std::array<FixedString, 3> Part8;
	uint8_t Flags;
};

struct SpellPrototype : public Noncopyable<SpellPrototype>
{
	using InitProc = void(SpellPrototype* self, FixedString const& spellId);

	[[bg3::readonly]] int StatsObjectIndex;
	SpellType SpellTypeId;
	FixedString SpellId;
	uint8_t SpellSchool;
	SpellFlags SpellFlags;
	uint8_t SpellActionType;
	uint8_t SpellAnimationType;
	uint8_t SpellJumpType;
	uint8_t SpellHitAnimationType;
	uint8_t SpellAnimationIntentType;
	uint8_t HitAnimationType;
	uint32_t LineOfSightFlags;
	uint32_t CinematicArenaFlags;
	Guid CinematicArenaTimelineOverride;
	uint32_t SpellCategory;
	int Level;
	int PowerLevel;
	bool HasMemoryCost;
	FixedString SpellContainerId;
	int RechargeValuesFrom;
	int RechargeValuesTo;
	DiceSizeId DiceValue;
	int8_t Cooldown;
	uint32_t WeaponTypes;
	DescriptionInfo Description;
	uint8_t AiFlags;
	uint8_t field_101;
	DamageType DamageType;
	SpellPrototype* ParentPrototype;
	Array<SpellPrototype*> ChildPrototypes;
	Array<ActionResourceCost> UseCosts;
	Array<ActionResourceCost> RitualCosts;
	Array<ActionResourceCost> DualWieldingUseCosts;
	Array<TextKeyHitCosts> HitCostGroups;
	Array<VariableResourceCost> VariableUseCosts;
	Array<VariableResourceCost> VariableDualWieldingUseCosts;
	Array<VariableResourceCost> VariableRitualCosts;
	uint32_t VerbalIntent;
	SpellPrototypeAnimationData SpellAnimation;
	SpellPrototypeAnimationData DualWieldingSpellAnimation;
	FixedString PrepareEffect;
	FixedString PrepareSound;
	FixedString PrepareLoopSound;
	FixedString CastSound;
	uint8_t CastSoundType;
	uint8_t field_299;
	uint8_t Sheathing;
	Array<FixedString> AlternativeCastTextEvents;
	int8_t SourceLimbIndex;
	Array<FixedString> ContainerSpells;
	Array<Array<FixedString>> Trajectories;
	uint32_t RequirementEvents;
	[[bg3::hidden]] MultiHashMap<uint8_t, Array<uint64_t>> MetaConditions;
	FixedString ItemWall;
	FixedString InterruptPrototype;
	FixedString CombatAIOverrideSpell;
	Array<FixedString> CombatAIOverrideSpells;
	float SteerSpeedMultipler;

	Object* GetStats() const;
};


struct [[bg3::hidden]] SpellPrototypeManager : public ProtectedGameObject<SpellPrototypeManager>
{
	void* VMT;
	MultiHashMap<FixedString, SpellPrototype*> Spells;
	MultiHashMap<FixedString, SpellPrototype*> CombatAIOverrideSpells;
	Array<FixedString> SpellNames;
	bool Initialized;

	void SyncStat(Object* object);
	bool SyncStat(Object* object, SpellPrototype* proto);
};


struct StatusPrototype : public Noncopyable<StatusPrototype>
{
	using InitProc = void(StatusPrototype* self, FixedString const& statusId, uint8_t flags);

	[[bg3::readonly]] int StatsObjectIndex{ -1 };
	StatusType StatusId;
	FixedString StatusName;
	uint64_t StatusPropertyFlags;
	uint64_t StatusGroups;
	DescriptionInfo Description;
	uint32_t StackType;
	uint8_t LEDEffect;
	uint8_t TickType;
	uint32_t ImmuneFlag;
	uint8_t Flags;
	ObjectSet<SurfaceType>* AbsorbSurfaceTypes{ nullptr };
	Array<Guid> Boosts;
	uint32_t RemoveEvents;
	Array<FixedString> SoundStart;
	Array<FixedString> SoundLoop;
	Array<FixedString> SoundStop;
	uint8_t HitAnimationType;
	uint8_t Sheathing;
	uint8_t AuraFlags;

	Object* GetStats() const;
};


struct [[bg3::hidden]] StatusPrototypeManager : public ProtectedGameObject<StatusPrototypeManager>
{
	void* VMT;
	MultiHashMap<FixedString, StatusPrototype*> Statuses;
	Array<FixedString> Unk;
	bool Initialized;

	void SyncStat(Object* object);
	bool SyncStat(Object* object, StatusPrototype* proto);
};


struct PassivePrototype : public Noncopyable<PassivePrototype>
{
	using InitProc = void(PassivePrototype* self, stats::Object* stats);

	uint32_t Properties{ 0 };
	FixedString Name;
	DescriptionInfo Description;
	[[bg3::readonly]] int EnabledConditions{ -1 };
	uint64_t EnabledContext{ 0 };
	FixedString ToggleOnEffect;
	FixedString ToggleOffEffect;
	uint64_t StatsFunctorContext{ 0 };
	[[bg3::readonly]] int ConditionsIndex{ -1 };
	stats::Functors StatsFunctors;
	stats::Functors ToggleOnFunctors;
	stats::Functors ToggleOffFunctors;
	FixedString ToggleGroup;
	uint64_t ToggleOffContext{ 0 };
	uint64_t BoostContext{ 0 };
	[[bg3::readonly]] int BoostConditionsIndex{ -1 };
	[[bg3::hidden]] Array<void*> Boosts_SV;
	int32_t PriorityOrder{ 0 };
	FixedString TooltipConditionalDamage;
};


struct [[bg3::hidden]] PassivePrototypeManager : public ProtectedGameObject<PassivePrototypeManager>
{
	void* VMT;
	RefMap<FixedString, PassivePrototype> Passives;
	bool Initialized;

	void SyncStat(stats::Object* object);
	bool SyncStat(stats::Object* object, PassivePrototype* proto);
};


struct InterruptPrototype
{
	using InitProc = void(InterruptPrototype* self, stats::Object* stats);

	FixedString Name;
	DescriptionInfo Description;
	uint16_t InterruptContext{ 0 };
	uint8_t InterruptContextScope{ 0 };
	[[bg3::readonly]] int Conditions{ -1 };
	FixedString Container;
	[[bg3::readonly]] int RollCondition{ -1 };
	[[bg3::hidden]] Array<void*> Costs;
	FixedString Stack;
	Functors PropertiesFunctors;
	Functors SuccessFunctors;
	Functors FailureFunctors;
	uint8_t InterruptDefaultValue{ 0 };
	[[bg3::readonly]] int EnableCondition{ -1 };
	uint64_t EnableContext{ 0 };
	uint8_t InterruptFlags{ 0 };
};


struct [[bg3::hidden]] InterruptPrototypeManager : public ProtectedGameObject<InterruptPrototypeManager>
{
	void* VMT;
	MultiHashMap<FixedString, InterruptPrototype> Interrupts;
	bool IsLoaded;

	void SyncStat(Object* object);
	bool SyncStat(Object* object, InterruptPrototype* proto);
};


END_NS()
