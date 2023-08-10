#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Stats/Functors.h>

namespace bg3se
{
	struct Passive : public Noncopyable<Passive>
	{
		using InitProc = void (Passive* self, CRPGStats_Object* stats);

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

		void SyncStat(CRPGStats_Object* object);
		bool SyncStat(CRPGStats_Object* object, Passive* proto);
	};
}
