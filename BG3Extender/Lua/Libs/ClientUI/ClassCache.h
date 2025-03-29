BEGIN_BARE_NS(Noesis)

struct NameInfo
{
    TypeProperty const* Property{ nullptr };
    DependencyProperty const* DepProperty{ nullptr };
    RoutedEvent const* Event{ nullptr };
};

struct ClassDefinition
{
    bg3se::HashMap<bg3se::FixedString, NameInfo> Names;
    bg3se::Array<TypeProperty const*> Properties;
    bg3se::Array<DependencyProperty const*> DependencyProperties;
    bg3se::Array<RoutedEvent const*> Events;

    void Populate(TypeClass const* cls);
    void PopulateFromParent(TypeClass const* cls);
};

struct ClassCache
{
    ClassDefinition const& GetClass(TypeClass const* cls)
    {
        auto entry = Classes.try_get(cls);
        if (entry) {
            return *entry->get();
        }

        auto cached = Classes.set(cls, std::make_unique<ClassDefinition>())->get();
        cached->Populate(cls);

        return *cached;
    }

    bg3se::HashMap<TypeClass const*, std::unique_ptr<ClassDefinition>> Classes;
};

extern ClassCache gClassCache;

END_BARE_NS()
