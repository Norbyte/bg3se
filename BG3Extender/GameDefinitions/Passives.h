#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Stats/Functors.h>

BEGIN_NS(stats)

struct Passive : public Noncopyable<Passive>
{
	using InitProc = void (Passive* self, stats::Object* stats);

	uint32_t Properties{ 0 };
	FixedString Name;
	DescriptionInfo DescriptionInfo;
	FixedString ToggleOnEffect;
	FixedString ToggleOffEffect;
	uint32_t StatsFunctorContext{ 0 };
	int ConditionsIndex{ -1 };
	StatsFunctorSetImpl StatsFunctors;
	StatsFunctorSetImpl ToggleOnFunctors;
	StatsFunctorSetImpl ToggleOffFunctors;
	FixedString ToggleGroup;
	uint32_t ToggleOffContext{ 0 };
	uint32_t BoostContext{ 0 };
	int BoostConditionsIndex{ -1 };
	Array<Guid> Boosts;
};


struct PassiveManager : public ProtectedGameObject<PassiveManager>
{
	void* VMT;
	RefMap<FixedString, Passive> Passives;
	bool Initialized;

	void SyncStat(stats::Object* object);
	bool SyncStat(stats::Object* object, Passive* proto);
};

END_NS()
