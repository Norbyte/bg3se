BEGIN_CLS(Transform)
P(RotationQuat)
P(Translate)
P(Scale)
END_CLS()


BEGIN_CLS(TypeInformation)
P_RO(TypeName)
P_RO(Kind)
P_RO(NativeName)
P_RO(KeyType)
P_RO(ElementType)
P_RO(ParentType)
P(Members)
P(Methods)
P_RO(HasWildcardProperties)
P(EnumValues)
P(ReturnValues)
P(Params)
P_RO(VarargParams)
P_RO(VarargsReturn)
P_RO(IsBitfield)
P_RO(IsBuiltin)
P_RO(ModuleRole)
P_RO(ComponentName)
END_CLS()


BEGIN_CLS(RuntimeStringHandle)
P(Handle)
P(Version)
END_CLS()


BEGIN_CLS(TranslatedString)
P(Handle)
P(ArgumentString)
P_FUN(Get, TranslatedString::Get)
END_CLS()


BEGIN_CLS(TranslatedFSString)
P(Handle)
P_FUN(Get, TranslatedFSString::Get)
END_CLS()


BEGIN_CLS(StatsExpressionParam)
P(Params)
P(Code)
END_CLS()


BEGIN_CLS(StatsExpressionParamEx)
INHERIT(StatsExpressionParam)
END_CLS()


BEGIN_CLS(StatsExpressionWithMetadata)
INHERIT(StatsExpressionParam)
P(field_28)
P(Cause)
P(field_48)
P(field_58)
END_CLS()


BEGIN_CLS(stats::Requirement)
P(RequirementId)
P(IntParam)
P(TagParam)
P(Not)
END_CLS()


BEGIN_CLS(BaseComponent)
END_CLS()

BEGIN_CLS(BaseProxyComponent)
END_CLS()


BEGIN_CLS(stats::FunctorGroup)
P(TextKey)
// Lua read-only helper until RO properties or transparent index support is added
P_GETTER(Functors, GetFunctors)
END_CLS()


BEGIN_CLS(AiTilePos)
P(X)
P(Y)
P(SubgridId)
END_CLS()


BEGIN_CLS(SurfacePathInfluence)
P(SurfaceType)
P(IsCloud)
P(Influence)
END_CLS()


BEGIN_CLS(ecs::ECSComponentLog)
P_BITMASK(Flags)
P_GETTER(Name, GetName)
P_GETTER(Type, GetType)
END_CLS()


BEGIN_CLS(ecs::ECSEntityLog)
P(Components)
P(Entity)
P_BITMASK(Flags)
END_CLS()


BEGIN_CLS(ecs::ECSChangeLog)
P(Entities)
END_CLS()


BEGIN_CLS(gn::GenomeVariant)
P_GETTER(Type, GetTypeName)
P_GETTER(Value, LuaGetValue)
END_CLS()
