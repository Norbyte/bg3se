BEGIN_CLS(SpellId)
P(OriginatorPrototype)
P(SourceType)
P(Class)
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
