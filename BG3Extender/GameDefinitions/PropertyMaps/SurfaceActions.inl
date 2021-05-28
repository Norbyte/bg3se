BEGIN_CLS(esv::SurfaceAction)
P(StoryActionID)
P_REF(Originator)
P_RO(Handle)
END_CLS()


BEGIN_CLS(esv::CreateSurfaceActionBase)
INHERIT(esv::SurfaceAction)
P(Owner)
P(Duration)
P(IsControlledByConcentration)
P(Position)
P(SurfaceType)
END_CLS()


BEGIN_CLS(esv::CreateSurfaceAction)
INHERIT(esv::CreateSurfaceActionBase)
P_RO(InitialChangesPushed)
P(Radius)
P(ExcludeRadius)
P(MaxHeight)
P(IgnoreIrreplacableSurfaces)
P(CheckExistingSurfaces)
P(LineCheckBlock)
P(Timer)
P(GrowTimer)
P(GrowStep)
P(CurrentCellCount)
P_RO(SurfaceLayer)
P_REF(SpellId)
P_RO(SurfaceConcentrationTarget)
END_CLS()


BEGIN_CLS(esv::CreatePuddleAction)
INHERIT(esv::CreateSurfaceActionBase)
P(SurfaceCells)
P(Step)
P(GrowSpeed)
P_RO(IsFinished)
P(IgnoreIrreplacableSurfaces)
P(GrowTimer)
END_CLS()


BEGIN_CLS(esv::ExtinguishFireAction)
INHERIT(esv::CreateSurfaceActionBase)
P(Position)
P(Radius)
P(Percentage)
P(GrowTimer)
P(Step)
END_CLS()


BEGIN_CLS(esv::ZoneActionParams)
P(Shape)
P(Radius)
P(ZoneParam)
P(FrontOffset)
P(MaxHeight)
P(Height)
P(Flags)
END_CLS()


BEGIN_CLS(esv::ZoneAction)
INHERIT(esv::CreateSurfaceActionBase)
P_REF(Spell)
P(TextKey)
P(Target)
P_REF(Params)
P(GrowStep)
// P_REF(Targets) - TODO - array support
P_RO(CurrentCellCount)
P(Flags)
END_CLS()


BEGIN_CLS(esv::TransformSurfaceAction)
INHERIT(esv::SurfaceAction)
P(Timer)
P(SurfaceTransformAction)
P(OriginSurface)
P(SurfaceLayer)
P(GrowCellPerSecond)
P_RO(Finished)
P(OwnerHandle)
P(Position)
P(Position)
P(SurfaceLifetime)
P(PlayerCharacterNearby)
END_CLS()


BEGIN_CLS(esv::ChangeSurfaceOnPathAction)
INHERIT(esv::CreateSurfaceActionBase)
P(FollowHandle)
P(Radius)
P(IsFinished)
P(IgnoreIrreplacableSurfaces)
P(CheckExistingSurfaces)
P(IgnoreOwnerCells)
END_CLS()


BEGIN_CLS(esv::RectangleSurfaceAction)
INHERIT(esv::CreateSurfaceActionBase)
P(Target)
P(SurfaceArea_M)
P(Width)
P(Length)
P(GrowTimer)
P(MaxHeight)
P(GrowStep)
// TODO - DamageList
P(DeathType)
P(LineCheckBlock)
// TODO - SkillProperties_M, CurrentGrowTimer_M
//P_REF(Characters) - TODO - array support
//P_REF(Items) - TODO - array support
P_RO(CurrentCellCount)
END_CLS()


BEGIN_CLS(esv::PolygonSurfaceAction)
INHERIT(esv::CreateSurfaceActionBase)
P(PolygonVertices)
// TODO - DamageList
P(CurrentGrowTimer)
P(GrowTimer)
P(SomePosition)
P(GrowStep)
P(LastSurfaceCellCount)
//P_REF(Characters) - TODO - array support
//P_REF(Items) - TODO - array support
END_CLS()
