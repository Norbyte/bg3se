#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Stats/Functors.h>

namespace bg3se
{
	struct Passive : public Noncopyable<Passive>
	{
		using InitProc = void (Passive* self, CRPGStats_Object* stats);

		uint32_t Properties;
		FixedString Name;
		DescriptionInfo DescriptionInfo;
		FixedString ToggleOnEffect;
		FixedString ToggleOffEffect;
		uint32_t StatsFunctorContext;
		int ConditionsIndex;
		StatsFunctorSetImpl StatsFunctors;
		StatsFunctorSetImpl ToggleOnFunctors;
		StatsFunctorSetImpl ToggleOffFunctors;
		FixedString ToggleGroup;
		uint32_t ToggleOffContext;
		uint32_t BoostContext;
		int BoostConditionsIndex;
		Array<UUID> Boosts;
	};


	struct PassiveManager : public ProtectedGameObject<PassiveManager>
	{
		void* VMT;
		RefMap<FixedString, Passive> Passives;
		bool Initialized;

		void SyncStat(CRPGStats_Object* object);
		bool SyncStat(CRPGStats_Object* object, Passive* proto);
	};
}
