BEGIN_BARE_NS(Noesis)

ClassCache gClassCache;

void ClassDefinition::PopulateFromParent(TypeClass const* cls)
{
    auto& defn = gClassCache.GetClass(cls);
    Names = defn.Names;
    Properties = defn.Properties;
    DependencyProperties = defn.DependencyProperties;
    Events = defn.Events;
}

void ClassDefinition::Populate(TypeClass const* cls)
{
    gStaticSymbols.Initialize();
    auto base = cls->GetBase();
    if (base) {
        PopulateFromParent(base);
    }

    for (auto evt : cls->mEvents) {
        Properties.push_back(evt);
        Names.get_or_add(bg3se::FixedString(evt->GetName().Str()))->Property = evt;
    }

    for (auto prop : cls->mProperties) {
        Properties.push_back(prop);
        Names.get_or_add(bg3se::FixedString(prop->GetName().Str()))->Property = prop;
    }

    TypeHelpers::ForEachMeta(cls, gStaticSymbols.TypeClasses.DependencyData.Type, [&] (TypeMetaData const* meta) {
        auto deps = static_cast<DependencyData const*>(meta);
        for (auto const& dep : deps->mProperties) {
            DependencyProperties.push_back(dep.GetPtr());
            Names.get_or_add(bg3se::FixedString(dep->GetName().Str()))->DepProperty = dep.GetPtr();
        }
    });

    TypeHelpers::ForEachMeta(cls, gStaticSymbols.TypeClasses.UIElementData.Type, [&] (TypeMetaData const* meta) {
        auto uiData = static_cast<UIElementData const*>(meta);
        for (auto const& evt : uiData->mEvents) {
            Events.push_back(evt.GetPtr());
            Names.get_or_add(bg3se::FixedString(evt->GetName().Str()))->Event = evt.GetPtr();
        }
    });
}

END_BARE_NS()
