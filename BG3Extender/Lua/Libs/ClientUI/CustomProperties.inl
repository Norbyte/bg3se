BEGIN_BARE_NS(Noesis)

class NsCustomDataContext : public BaseComponent, public INotifyPropertyChanged
{
public:
    NsCustomDataContext(TypeClass* cls)
        : class_(cls)
    {}

    ~NsCustomDataContext()
    {}

    static void* operator new(std::size_t count, void* ptr)
    {
        return ptr;
    }

    static void operator delete(void* ptr, void* place) noexcept
    {}

    static const TypeClass* StaticGetClassType(TypeTag<NsCustomDataContext>*)
    {
        static const TypeClass* type{ nullptr };

        if (NS_UNLIKELY(type == 0)) {
            type = static_cast<const TypeClass*>(Reflection::RegisterType("NsCustomDataContext",
                TypeClassCreator::Create<NsCustomDataContext>,
                TypeClassCreator::Fill<NsCustomDataContext, BaseComponent>));
        }

        return type;
    }

    NS_COLD_FUNC static void StaticFillClassType(TypeClassCreator& helper)
    {
        helper.Impl<NsCustomDataContext, INotifyPropertyChanged>();
        // ...
    }

    const TypeClass* GetClassType() const
    {
        return class_;
    }

    PropertyChangedEventHandler& PropertyChanged() override
    {
        return propertyChanged_;
    }

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    Ptr<BaseComponent> basedOn_;
    TypeClass* class_;
    PropertyChangedEventHandler propertyChanged_;
};

template <class T>
class TypePropertyOffsetNotifies : public TypePropertyOffset<T>
{
public:
    TypePropertyOffsetNotifies(Symbol name, uint32_t offset)
        : TypePropertyOffset<T>(name, offset)
    {}

    void SetComponent(void* ptr, BaseComponent* value) const override
    {
        if (this->GetComponent(ptr).GetPtr() != value) {
            TypePropertyOffset<T>::SetComponent(ptr, value);
            auto ctx = reinterpret_cast<NsCustomDataContext*>(ptr);
            ctx->PropertyChanged().Invoke(ctx, PropertyChangedEventArgs(this->GetName()));
        }
    }

    void Set(void* ptr, const void* value) const override
    {
        if (*reinterpret_cast<T const*>(this->Get(ptr)) != *reinterpret_cast<T const*>(value)) {
            TypePropertyOffset<T>::Set(ptr, value);
            auto ctx = reinterpret_cast<NsCustomDataContext*>(ptr);
            ctx->PropertyChanged().Invoke(ctx, PropertyChangedEventArgs(this->GetName()));
        }
    }
};

END_BARE_NS()


BEGIN_NS(ecl::lua::ui)

Noesis::TypeClass* gTempClass{ nullptr };

Noesis::Type* CreateTypeProc(Noesis::Symbol name)
{
    return gTempClass;
}

class DynamicPropertyType
{
public:
    virtual uint32_t GetSize() = 0;
    virtual uint32_t GetAlignment() = 0;
    virtual void ConstructProperty(void* object, uint32_t offset) = 0;
    virtual void DestroyProperty(void* object, uint32_t offset) = 0;
    virtual Noesis::TypeProperty* CreateTypeProperty(Symbol name, uint32_t offset, bool notifies) = 0;
};

template <class T>
class DynamicPropertyTypeImpl : public DynamicPropertyType
{
public:
    uint32_t GetSize()
    {
        return sizeof(T);
    }

    uint32_t GetAlignment()
    {
        return alignof(T);
    }

    void ConstructProperty(void* object, uint32_t offset) override
    {
        auto v = reinterpret_cast<T*>((uintptr_t)object + offset);
        new (v) T();
    }

    void DestroyProperty(void* object, uint32_t offset) override
    {
        auto v = reinterpret_cast<T*>((uintptr_t)object + offset);
        v->~T();
    }
};

template <class T>
class DynamicPropertyTypeImplRW : public DynamicPropertyTypeImpl<T>
{
public:
    Noesis::TypeProperty* CreateTypeProperty(Symbol name, uint32_t offset, bool notifies) override
    {
        if (notifies) {
            return GameAlloc<Noesis::TypePropertyOffsetNotifies<T>>(name, offset);
        } else {
            return GameAlloc<Noesis::TypePropertyOffset<T>>(name, offset);
        }
    }
};

template <class TE>
class DynamicPropertyTypeImplRORef : public DynamicPropertyTypeImpl<Noesis::Ptr<TE>>
{
public:
    using T = Noesis::Ptr<TE>;

    void ConstructProperty(void* object, uint32_t offset) override
    {
        auto v = reinterpret_cast<T*>((uintptr_t)object + offset);
        new (v) T(new TE());
    }

    Noesis::TypeProperty* CreateTypeProperty(Symbol name, uint32_t offset, bool notifies) override
    {
        // Property is read-only, no property change notification will be sent
        return GameAlloc<Noesis::TypePropertyOffset<const T>>(name, offset);
    }
};

struct DynamicPropertyDefinition
{
    DynamicPropertyType* Type;
    uint32_t Offset;
};

struct DynamicClassType
{
    Noesis::TypeClassBuilder* Type{ nullptr };
    Array<DynamicPropertyDefinition> Properties;
    uint32_t Size{ 0 };

    Noesis::NsCustomDataContext* Construct()
    {
        auto ctx = reinterpret_cast<Noesis::NsCustomDataContext *>(GameAllocRaw(Size));
        new (ctx) NsCustomDataContext(Type);
        for (auto const& prop : Properties) {
            prop.Type->ConstructProperty(ctx, prop.Offset);
        }

        return ctx;
    }

    void Destroy(Noesis::NsCustomDataContext* o)
    {
        for (auto const& prop : Properties) {
            prop.Type->DestroyProperty(o, prop.Offset);
        }

        o->~NsCustomDataContext();
    }
};

HashMap<FixedString, DynamicClassType*> gDynamicClasses;
HashMap<FixedString, DynamicPropertyType*> gDynamicPropertyTypes;

void InitializeDynamicPropertyTypes()
{
    if (!gDynamicPropertyTypes.empty()) return;

    gDynamicPropertyTypes.set(FixedString("Bool"), GameAlloc<DynamicPropertyTypeImplRW<bool>>());
    gDynamicPropertyTypes.set(FixedString("Int8"), GameAlloc<DynamicPropertyTypeImplRW<int8_t>>());
    gDynamicPropertyTypes.set(FixedString("Int16"), GameAlloc<DynamicPropertyTypeImplRW<int16_t>>());
    gDynamicPropertyTypes.set(FixedString("Int32"), GameAlloc<DynamicPropertyTypeImplRW<int32_t>>());
    gDynamicPropertyTypes.set(FixedString("Int64"), GameAlloc<DynamicPropertyTypeImplRW<int64_t>>());
    gDynamicPropertyTypes.set(FixedString("UInt8"), GameAlloc<DynamicPropertyTypeImplRW<uint8_t>>());
    gDynamicPropertyTypes.set(FixedString("UInt16"), GameAlloc<DynamicPropertyTypeImplRW<uint16_t>>());
    gDynamicPropertyTypes.set(FixedString("UInt32"), GameAlloc<DynamicPropertyTypeImplRW<uint32_t>>());
    gDynamicPropertyTypes.set(FixedString("UInt64"), GameAlloc<DynamicPropertyTypeImplRW<uint64_t>>());
    gDynamicPropertyTypes.set(FixedString("Single"), GameAlloc<DynamicPropertyTypeImplRW<float>>());
    gDynamicPropertyTypes.set(FixedString("Double"), GameAlloc<DynamicPropertyTypeImplRW<double>>());
    gDynamicPropertyTypes.set(FixedString("Color"), GameAlloc<DynamicPropertyTypeImplRW<Noesis::Color>>());
    gDynamicPropertyTypes.set(FixedString("Vector2"), GameAlloc<DynamicPropertyTypeImplRW<Noesis::Vector2>>());
    gDynamicPropertyTypes.set(FixedString("Vector3"), GameAlloc<DynamicPropertyTypeImplRW<Noesis::Vector3>>());
    gDynamicPropertyTypes.set(FixedString("Point"), GameAlloc<DynamicPropertyTypeImplRW<Noesis::Point>>());
    gDynamicPropertyTypes.set(FixedString("Rect"), GameAlloc<DynamicPropertyTypeImplRW<Noesis::Rect>>());
    gDynamicPropertyTypes.set(FixedString("String"), GameAlloc<DynamicPropertyTypeImplRW<Noesis::String>>());
    gDynamicPropertyTypes.set(FixedString("Object"), GameAlloc<DynamicPropertyTypeImplRW<Noesis::Ptr<Noesis::BaseComponent>>>());
    gDynamicPropertyTypes.set(FixedString("Collection"), GameAlloc<DynamicPropertyTypeImplRORef<Noesis::ObservableCollection<Noesis::BaseComponent>>>());
    gDynamicPropertyTypes.set(FixedString("Command"), GameAlloc<DynamicPropertyTypeImplRORef<Noesis::LuaDelegateCommand>>());
}

class ClassDefinitionBuilder
{
public:
    static Symbol MakeFullName(StringView name)
    {
        return Noesis::MakeDynamicSymbol(STDString("se::") + name.data());
    }

    ClassDefinitionBuilder(Symbol name)
        : name_(name)
    {
        type_ = static_cast<Noesis::TypeClassBuilder*>(new TypeClass(name, false));
        type_->AddBase(Noesis::TypeOf<Noesis::NsCustomDataContext>());

        dynamicCls_ = GameAlloc<DynamicClassType>();
        dynamicCls_->Type = type_;

        InitializeDynamicPropertyTypes();
    }

    void Register()
    {
        dynamicCls_->Size = lastOffset_;

        auto prev = gTempClass;
        gTempClass = type_;
        gAllowTypeReregistration++;
        Noesis::Reflection::RegisterType(name_.Str(), CreateTypeProc, nullptr);
        gAllowTypeReregistration--;
        gTempClass = prev;

        gDynamicClasses.set(FixedString(name_.Str()), dynamicCls_);
    }

    bool AddProperty(FixedString const& name, FixedString const& typeName, bool notifies)
    {
        auto nameSym = Noesis::MakeDynamicSymbol(STDString(name.GetStringView()));

        auto type = gDynamicPropertyTypes.try_get(typeName);
        if (!type) {
            ERR("Property type not supported: '%s'", typeName.GetString());
            return false;
        }

        auto alignment = (*type)->GetAlignment();
        if (lastOffset_ % alignment) {
            lastOffset_ += alignment - (lastOffset_ % alignment);
        }

        dynamicCls_->Properties.push_back(DynamicPropertyDefinition{
            .Type = *type,
            .Offset = lastOffset_
        });

        auto prop = (*type)->CreateTypeProperty(nameSym, lastOffset_, notifies);
        type_->AddProperty(prop);

        lastOffset_ += (*type)->GetSize();

        return true;
    }

private:
    Noesis::TypeClassBuilder* type_;
    DynamicClassType* dynamicCls_;
    Symbol name_;
    uint32_t lastOffset_{ sizeof(Noesis::NsCustomDataContext) };
};

END_NS()
