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
P_REF(Members)
P_REF(Methods)
P_RO(HasWildcardProperties)
P_REF(EnumValues)
P_REF(ReturnValues)
P_REF(Params)
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
P_REF(Handle)
P_REF(ArgumentString)
END_CLS()
