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

BEGIN_CLS(stats::RPGStats)
END_CLS()

#include <GameDefinitions/PropertyMaps/Generated.inl>
