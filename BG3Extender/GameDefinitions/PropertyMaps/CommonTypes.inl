BEGIN_CLS(Transform)
// TODO - P(Matrix)
// TODO - P(Rotate)
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


BEGIN_CLS(stats::Object::FunctorGroup)
P(TextKey)
// Lua read-only helper until RO properties or transparent index support is added
P_GETTER(Functors, GetFunctors)
END_CLS()