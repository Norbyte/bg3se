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
P_RO(IsBuiltin)
P_RO(ModuleRole)
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


BEGIN_CLS(stats::Requirement)
P(RequirementId)
P(IntParam)
P(TagParam)
P(Not)
END_CLS()


BEGIN_CLS(stats::SpellPrototype)
END_CLS()


BEGIN_CLS(stats::SpellPrototype::UseCostGroup)
P(Resources)
P(Amount)
P(SubResourceId)
P(ResourceGroup)
END_CLS()


BEGIN_CLS(BaseComponent)
END_CLS()
