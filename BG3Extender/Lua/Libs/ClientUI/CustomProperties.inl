BEGIN_BARE_NS(Noesis)

class NsCustomDataContext : public BaseComponent, public INotifyPropertyChanged
{
public:
    NsCustomDataContext(TypeClass* cls, BaseComponent* wrappedContext)
        : class_(cls),
        wrappedContext_(wrappedContext),
        installedChangeHandler_(false)
    {
        if (wrappedContext_) {
            auto notifies = DynamicCast<INotifyPropertyChanged*, BaseComponent*>(wrappedContext_);
            if (notifies) {
                notifies->PropertyChanged().Add(MakeDelegate(this, &NsCustomDataContext::OnWrappedPropertyChanged));
                installedChangeHandler_ = true;
            }
        }
    }

    ~NsCustomDataContext()
    {
        if (installedChangeHandler_) {
            auto notifies = DynamicCast<INotifyPropertyChanged*, BaseComponent*>(wrappedContext_);
            notifies->PropertyChanged().Remove(MakeDelegate(this, &NsCustomDataContext::OnWrappedPropertyChanged));
        }
    }

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

    BaseComponent* GetWrappedContext() const
    {
        return wrappedContext_.GetPtr();
    }

    PropertyChangedEventHandler& PropertyChanged() override
    {
        return propertyChanged_;
    }

    static uint32_t WrappedContextOffset()
    {
        return (uint32_t)offsetof(NsCustomDataContext, wrappedContext_);
    }

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    TypeClass* class_;
    Ptr<BaseComponent> wrappedContext_;
    PropertyChangedEventHandler propertyChanged_;
    bool installedChangeHandler_;

    void OnWrappedPropertyChanged(BaseComponent* obj, const PropertyChangedEventArgs& evt)
    {
        propertyChanged_(obj, evt);
    }
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


class TypePropertyDataContextWrapped : public TypeProperty
{
public:
    TypePropertyDataContextWrapped(Symbol name, TypeProperty const* baseProperty)
        : TypeProperty(name, baseProperty->GetContentType()),
        baseProperty_(baseProperty)
    {}

    static BaseComponent* GetWrapped(const void* ptr)
    {
        return reinterpret_cast<NsCustomDataContext const*>(ptr)->GetWrappedContext();
    }

    void* GetContent(const void* ptr) const override
    {
        return baseProperty_->GetContent(GetWrapped(ptr));
    }

    bool IsReadOnly() const override
    {
        return baseProperty_->IsReadOnly();
    }

    Ptr<BaseComponent> GetComponent(const void* ptr) const override
    {
        return baseProperty_->GetComponent(GetWrapped(ptr));
    }

    void SetComponent(void* ptr, BaseComponent* value) const override
    {
        return baseProperty_->SetComponent(GetWrapped(ptr), value);
    }

    void GetCopy(const void* ptr, void* dest) const override
    {
        return baseProperty_->GetCopy(GetWrapped(ptr), dest);
    }

    const void* Get(const void* ptr) const override
    {
        return baseProperty_->Get(GetWrapped(ptr));
    }

    void Set(void* ptr, const void* value) const override
    {
        return baseProperty_->Set(GetWrapped(ptr), value);
    }

private:
    TypeProperty const* baseProperty_;
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
    Noesis::TypeClass const* WrappedContextType{ nullptr };
    Array<DynamicPropertyDefinition> Properties;
    uint32_t Size{ 0 };
    Symbol Name;

    bool ValidateWrappedContext(Noesis::BaseComponent*& wrappedContext)
    {
        if (WrappedContextType == nullptr) {
            if (wrappedContext != nullptr) {
                ERR("Passing wrapped context to type '%s' that doesn't support wrapped contexts", Name.Str());
                return false;
            }

            return true;
        }

        if (wrappedContext == nullptr) {
            ERR("Context '%s' requires a wrapped context", Name.Str());
            return false;
        }

        if (!TypeHelpers::IsDescendantOf(wrappedContext->GetClassType(), WrappedContextType)) {
            // Check if it is an SE context that wraps another context; if so, redo the check with the wrapped context
            if (TypeHelpers::IsDescendantOf(wrappedContext->GetClassType(), Noesis::TypeOf<Noesis::NsCustomDataContext>())) {
                auto baseCtx = static_cast<Noesis::NsCustomDataContext*>(wrappedContext)->GetWrappedContext();
                if (baseCtx && ValidateWrappedContext(baseCtx)) {
                    wrappedContext = baseCtx;
                    return true;
                }
            }

            ERR("Context '%s' requires a wrapped context of type '%s', got '%s'", Name.Str(),
                WrappedContextType->GetName(), wrappedContext->GetClassType()->GetName());
            return false;
        }

        return true;
    }

    Noesis::NsCustomDataContext* Construct(Noesis::BaseComponent* wrappedContext)
    {
        if (!ValidateWrappedContext(wrappedContext)) {
            return nullptr;
        }

        auto ctx = reinterpret_cast<Noesis::NsCustomDataContext *>(GameAllocRaw(Size));
        new (ctx) NsCustomDataContext(Type, wrappedContext);
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
        return Noesis::Symbol((STDString("se::") + name.data()).c_str());
    }

    ClassDefinitionBuilder(Symbol name)
        : name_(name)
    {
        type_ = static_cast<Noesis::TypeClassBuilder*>(new TypeClass(name, false));
        type_->AddBase(Noesis::TypeOf<Noesis::NsCustomDataContext>());

        dynamicCls_ = GameAlloc<DynamicClassType>();
        dynamicCls_->Name = name_;
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
        auto nameSym = Noesis::Symbol(name.GetString());

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

    bool SetWrappedContext(Symbol name)
    {
        auto type = Noesis::Reflection::GetType(name);
        if (!type) {
            ERR("Wrapped context type '%s' does not exist", name.Str());
            return false;
        }

        if (!TypeHelpers::IsDescendantOf(type, Noesis::gStaticSymbols.TypeClasses.BaseComponent.Type)) {
            ERR("Wrapped context type '%s' must be a component", name.Str());
            return false;
        }

        if (TypeHelpers::IsDescendantOf(type, Noesis::gStaticSymbols.TypeClasses.DependencyObject.Type)) {
            ERR("Wrapped context type '%s' cannot be a dependency object", name.Str());
            return false;
        }

        auto cls = static_cast<Noesis::TypeClass const*>(type);
        dynamicCls_->WrappedContextType = cls;

        auto baseContextProp = GameAlloc<Noesis::TypePropertyOffset<Noesis::Ptr<Noesis::BaseComponent>>>(
            Noesis::Symbol("WrappedContext"), Noesis::NsCustomDataContext::WrappedContextOffset());
        type_->AddProperty(baseContextProp);

        // Use the cached class to ensure we also get properties from all parents
        auto cacheCls = gClassCache.GetClass(cls);
        for (auto prop : cacheCls.Names) {
            if (prop->Value().Property) {
                auto propName = prop->Value().Property->GetName();
                if (type_->mProperties.FindIf([&] (TypeProperty* p) { return p->GetName() == propName; }) == type_->mProperties.End()) {
                    auto wrappedProp = GameAlloc<Noesis::TypePropertyDataContextWrapped>(propName, prop->Value().Property);
                    type_->AddProperty(wrappedProp);
                }
            }
        }

        return true;
    }

private:
    Noesis::TypeClassBuilder* type_;
    DynamicClassType* dynamicCls_;
    Symbol name_;
    uint32_t lastOffset_{ sizeof(Noesis::NsCustomDataContext) };
};

END_NS()
