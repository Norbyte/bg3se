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
