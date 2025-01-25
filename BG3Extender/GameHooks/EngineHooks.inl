HOOK_DEFN(RPGStats__Load, RPGStats__Load, stats::RPGStats::LoadProc)
HOOK_DEFN(RPGStats__PreParseDataFolder, RPGStats__PreParseDataFolder, stats::RPGStats::ParseStructureFolderProc)

HOOK_DEFN(FileReader__ctor, ls__FileReader__ctor, FileReader::CtorProc)
HOOK_DEFN(Kernel_FindFirstFileW, Kernel_FindFirstFileW, FindFirstFileWProc)
HOOK_DEFN(Kernel_FindNextFileW, Kernel_FindNextFileW, FindNextFileWProc)
HOOK_DEFN(Kernel_FindClose, Kernel_FindClose, FindCloseProc)

HOOK_DEFN(ecs__EntityWorld__Update, ecs__EntityWorld__Update, ecs::EntityWorld::UpdateProc)
HOOK_DEFN(ecs__EntityWorld__FlushECBs, ecs__EntityWorld__FlushECBs, ecs::EntityWorld::FlushECBsProc)
HOOK_DEFN(esv__OsirisVariableHelper__SavegameVisit, esv__OsirisVariableHelper__SavegameVisit, esv::OsirisVariableHelper__SavegameVisitProc)
HOOK_DEFN(stats__DealDamageFunctor__ApplyDamage, stats__DealDamageFunctor__ApplyDamage, stats::DealDamageFunctor::ApplyDamageProc)
HOOK_DEFN(esv__StatsSystem__ThrowDamageEvent, esv__StatsSystem__ThrowDamageEvent, stats::StatsSystem_ThrowDamageEventProc)
HOOK_DEFN(eoc__AiGrid__FindPath, eoc__AiGrid__FindPath, AiGrid::FindPathProc)

HOOK_DEFN(esv__ExecuteStatsFunctor_AttackTargetContext, esv__ExecuteStatsFunctor_AttackTargetContext, stats::ExecuteFunctorProc<stats::AttackTargetContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_AttackPositionContext, esv__ExecuteStatsFunctor_AttackPositionContext, stats::ExecuteFunctorProc<stats::AttackPositionContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_MoveContext, esv__ExecuteStatsFunctor_MoveContext, stats::ExecuteFunctorProc<stats::MoveContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_TargetContext, esv__ExecuteStatsFunctor_TargetContext, stats::ExecuteFunctorProc<stats::TargetContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_NearbyAttackedContext, esv__ExecuteStatsFunctor_NearbyAttackedContext, stats::ExecuteFunctorProc<stats::NearbyAttackedContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_NearbyAttackingContext, esv__ExecuteStatsFunctor_NearbyAttackingContext, stats::ExecuteFunctorProc<stats::NearbyAttackingContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_EquipContext, esv__ExecuteStatsFunctor_EquipContext, stats::ExecuteFunctorProc<stats::EquipContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_SourceContext, esv__ExecuteStatsFunctor_SourceContext, stats::ExecuteFunctorProc<stats::SourceContextData>)
HOOK_DEFN(esv__ExecuteStatsFunctor_InterruptContext, esv__ExecuteStatsFunctor_InterruptContext, stats::ExecuteInterruptFunctorProc)
