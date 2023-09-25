BEGIN_CLS(SpellIdBase)
P(OriginatorPrototype)
P(SourceType)
P(ProgressionSource)
END_CLS()


BEGIN_CLS(SpellId)
P(OriginatorPrototype)
P(SourceType)
P(ProgressionSource)
P(Prototype)
END_CLS()


BEGIN_CLS(SpellIdWithPrototype)
INHERIT(SpellId)
END_CLS()


BEGIN_CLS(Transform)
// TODO - P(Matrix)
// TODO - P(Rotate)
P(Translate)
P(Scale)
END_CLS()


BEGIN_CLS(GameTime)
P_RO(Time)
P_RO(DeltaTime)
P_RO(Ticks)
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
// TODO - P(Params)
P(Code)
END_CLS()


BEGIN_CLS(StatsExpressionParamEx)
INHERIT(StatsExpressionParam)
END_CLS()
