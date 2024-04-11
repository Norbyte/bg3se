// Wrapper file for including all property map files

#include <GameDefinitions/PropertyMaps/CommonTypes.inl>
#include <GameDefinitions/PropertyMaps/Functors.inl>
#include <GameDefinitions/PropertyMaps/FunctorExecParams.inl>
#include <GameDefinitions/PropertyMaps/ServerObjects.inl>
#include <GameDefinitions/PropertyMaps/SurfaceActions.inl>
#include <GameDefinitions/PropertyMaps/Statuses.inl>
#include <GameDefinitions/PropertyMaps/Events.inl>

#if defined(ENABLE_UI)
#include <GameDefinitions/PropertyMaps/UI.inl>
#endif

#if defined(ENABLE_IMGUI)
#include <GameDefinitions/PropertyMaps/IMGUI.inl>
#endif

// WIP - not exported yet!
BEGIN_CLS(GlobalSwitches)
END_CLS()

BEGIN_CLS(stats::TreasureTable)
P_RO(Name)
P(MinLevel)
P(MaxLevel)
P(IgnoreLevelDiff)
P(UseTreasureGroupContainers)
P(CanMerge)
P(SubTables)
END_CLS()

BEGIN_CLS(stats::TreasureSubTable)
P(Categories)
P(CategoryFrequencies)
P(TotalFrequency)
P(DropCounts)
P(Amounts)
P(TotalCount)
P(StartLevel)
P(EndLevel)
END_CLS()

BEGIN_CLS(stats::TreasureSubTable::Category)
P(Index)
P(Frequency)
P(Frequencies)
P(Frequencies2)
P(IsTreasureTable)
P(IsTreasureTable2)
END_CLS()

BEGIN_CLS(stats::TreasureSubTable::DropCount)
P(Chance)
P(Amount)
END_CLS()

BEGIN_CLS(stats::TreasureCategory)
P(Category)
P(Items)
END_CLS()

BEGIN_CLS(stats::TreasureCategory::Item)
P(Name)
P(Priority)
P(MinAmount)
P(MaxAmount)
P(ActPart)
P(Unique)
P(MinLevel)
P(MaxLevel)
END_CLS()

BEGIN_CLS(stats::RPGEnumeration)
P(Name)
P(Values)
END_CLS()

BEGIN_CLS(stats::Modifier)
P(EnumerationIndex)
P(LevelMapIndex)
P(Name)
END_CLS()

BEGIN_CLS(stats::ModifierList)
P(Name)
P(Attributes)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::RPGEnumeration>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::ModifierList>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::Modifier>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::CNamedElementManager<stats::TreasureTable>)
P(Primitives)
P(NameHashMap)
END_CLS()

BEGIN_CLS(stats::RPGStats)
P(ModifierValueLists)
P(ModifierLists)
P(TreasureTables)
P(ExtraData)
END_CLS()

#include <GameDefinitions/PropertyMaps/Generated.inl>
