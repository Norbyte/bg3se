#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Stats/Functors.h>

namespace bg3se
{
	struct Passive : public ProtectedGameObject<Passive>
	{
		int Properties;
		FixedString Name;
		DescriptionInfo DescriptionInfo;
		FixedString ToggleOnEffect;
		FixedString ToggleOffEffect;
		int StatsFunctorContext;
		int ConditionsIndex;
		StatsFunctorSetImpl StatsFunctors;
		StatsFunctorSetImpl ToggleOnFunctors;
		StatsFunctorSetImpl ToggleOffFunctors;
		int BoostContext;
		int BoostConditionsIndex;
		Array<void*> Boosts;
		__int64 field_1A8;
		__int64 field_1B0;
	};


	struct PassiveManager : public ProtectedGameObject<PassiveManager>
	{
		void* VMT;
		RefMap<FixedString, Passive> Passives;
		__int64 field_18;
	};
}
