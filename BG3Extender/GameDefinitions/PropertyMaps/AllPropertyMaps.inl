// Wrapper file for including all property map files

#include <GameDefinitions/PropertyMaps/CommonTypes.inl>
#include <GameDefinitions/PropertyMaps/Functors.inl>
#include <GameDefinitions/PropertyMaps/FunctorExecParams.inl>
#include <GameDefinitions/PropertyMaps/ServerObjects.inl>
#include <GameDefinitions/PropertyMaps/SurfaceActions.inl>
#include <GameDefinitions/PropertyMaps/Statuses.inl>
#include <GameDefinitions/PropertyMaps/Events.inl>

// WIP - not exported yet!
BEGIN_CLS(GlobalSwitches)
END_CLS()

BEGIN_CLS(stats::TreasureTable)
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
