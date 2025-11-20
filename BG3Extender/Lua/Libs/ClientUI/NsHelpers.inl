BEGIN_BARE_NS(Noesis)

Ptr<FrameworkElement> LoadXaml(char const* path);

template <class T>
Symbol StaticSymbol()
{
    return SymbolInfo<T>::Name;
}

TypeClass* ObjectHelpers::GetClassType(BaseObject const* o)
{
    return const_cast<TypeClass*>(o->GetClassType());
}

Symbol ObjectHelpers::GetClassTypeName(BaseObject const* o)
{
    return o->GetClassType()->GetTypeId();
}

bg3se::STDString ObjectHelpers::ToString(BaseObject const* o)
{
    return o->ToString().Str();
}

uint32_t ObjectHelpers::GetNumReferences(BaseRefCounted const* o)
{
    return o->GetNumReferences();
}

PropertyOperationResult ObjectHelpers::FallbackGetProperty(lua_State* L, lua::LifetimeHandle lifetime, BaseObject const* object, bg3se::FixedString const& name)
{
    auto const& cls = gClassCache.GetClass(object->GetClassType());
    auto prop = cls.Names.try_get(name);
    
    if (prop) {
        if (prop->Property) {
            GetProperty(L, object, prop->Property);
            return PropertyOperationResult::Success;
        }
        
        if (prop->DepProperty) {
            DependencyObjectHelpers::GetDependencyProperty(L, static_cast<DependencyObject const*>(object), prop->DepProperty);
            return PropertyOperationResult::Success;
        }
    }

    return PropertyOperationResult::NoSuchProperty;
}

PropertyOperationResult ObjectHelpers::FallbackSetProperty(lua_State* L, BaseObject* object, bg3se::FixedString const& name, int index)
{
    auto const& cls = gClassCache.GetClass(object->GetClassType());
    auto prop = cls.Names.try_get(name);

    if (prop) {
        if (prop->Property) {
            SetProperty(L, object, prop->Property, lua::AnyRef(index));
            return PropertyOperationResult::Success;
        }

        if (prop->DepProperty) {
            DependencyObjectHelpers::SetDependencyProperty(L, static_cast<DependencyObject*>(object), prop->DepProperty, lua::AnyRef(index));
            return PropertyOperationResult::Success;
        }
    }

    return PropertyOperationResult::NoSuchProperty;
}

UserReturn ObjectHelpers::GetNamedProperty(lua_State* L, BaseObject const* o, bg3se::FixedString const& name)
{
    auto result = FallbackGetProperty(L, lua::GetCurrentLifetime(L), o, name);
    if (result != PropertyOperationResult::Success) {
        OsiError("Object " << o->GetClassType()->GetName() << " has no property named '" << name << "'");
        lua::push(L, nullptr);
    }

    return 1;
}

UserReturn ObjectHelpers::GetProperty(lua_State* L, BaseObject const* o, TypeProperty const* prop)
{
    StoredValueHelpers::PushProperty(L, o, o->GetClassType(), prop);
    return 1;
}

void ObjectHelpers::SetNamedProperty(lua_State* L, BaseObject* o, bg3se::FixedString const& name, lua::AnyRef value)
{
    auto result = FallbackSetProperty(L, o, name, value.Index);
    if (result != PropertyOperationResult::Success) {
        OsiError("Object " << o->GetClassType()->GetName() << " has no property named '" << name << "'");
    }
}

void ObjectHelpers::SetProperty(lua_State* L, BaseObject* o, TypeProperty const* prop, lua::AnyRef value)
{
    if (prop->IsReadOnly()) {
        luaL_error(L, "Property %s of %s is read-only", prop->GetName().Str(), o->GetClassType()->GetName());
    }

    auto val = StoredValueHelpers::GetValue(L, prop->GetContentType(), value);
    if (!val) {
        return;
    }

    auto pValue = val->Value;
    if (val->IsIntegral) pValue = &val->Value;

    prop->Set(o, pValue);
}

void ObjectHelpers::DoGetDependencyProperties(lua_State* L, BaseObject const* o)
{
    auto type = o->GetClassType();
    if (TypeHelpers::IsDescendantOf(type, gStaticSymbols.TypeClasses.DependencyObject.Type)) {
        auto dep = static_cast<DependencyObject const*>(o);
        for (auto& prop : dep->mValues) {
            auto name = prop.key->GetName();
            lua::push(L, name);
            StoredValueHelpers::PushValue(L, prop.key->GetType(), prop.value, type, &name);
            lua_settable(L, -3);
        }
    }
}

void ObjectHelpers::DoGetDirectProperties(lua_State* L, BaseObject const* o)
{
    auto nsType = o->GetClassType();
    auto const& cls = gClassCache.GetClass(nsType);
    for (auto prop : cls.Names) {
        if (prop.Value().Property) {
            lua::push(L, prop.Key());
            StoredValueHelpers::PushProperty(L, o, nsType, prop.Value().Property);
            lua_settable(L, -3);
        }
    }
}

UserReturn ObjectHelpers::GetDependencyProperties(lua_State* L, BaseObject const* o)
{
    lua_newtable(L);
    DoGetDependencyProperties(L, o);
    return 1;
}

UserReturn ObjectHelpers::GetDirectProperties(lua_State* L, BaseObject const* o)
{
    lua_newtable(L);
    DoGetDirectProperties(L, o);
    return 1;
}

UserReturn ObjectHelpers::GetAllProperties(lua_State* L, BaseObject const* o)
{
    lua_newtable(L);
    DoGetDependencyProperties(L, o);
    DoGetDirectProperties(L, o);
    return 1;
}


Symbol RoutedEventHelpers::GetName(RoutedEvent const* o)
{
    return o->GetName();
}

TypeClass* RoutedEventHelpers::GetOwnerType(RoutedEvent const* o)
{
    return const_cast<TypeClass*>(o->GetOwnerType());
}


UserReturn DependencyObjectHelpers::GetProperty(lua_State* L, DependencyObject const* o, bg3se::FixedString const& name)
{
    auto prop = TypeHelpers::GetDependencyProperty(o->GetClassType(), name);
    if (prop == nullptr) {
        OsiError("Dependency object " << o->GetClassType()->GetName() << " has no property named '" << name << "'");
        lua::push(L, nullptr);
        return 1;
    }

    return GetDependencyProperty(L, o, prop);
}

UserReturn DependencyObjectHelpers::GetDependencyProperty(lua_State* L, DependencyObject const* o, DependencyProperty const* prop)
{
    auto valIt = o->mValues.Find(prop);
    if (valIt == o->mValues.End()) {
        lua::push(L, nullptr);
        return 1;
    }

    auto name = prop->GetName();
    StoredValueHelpers::PushValue(L, prop->GetType(), valIt->value, o->GetClassType(), &name);
    return 1;
}

void DependencyObjectHelpers::SetProperty(lua_State* L, DependencyObject* o, bg3se::FixedString const& name, lua::AnyRef value)
{
    auto prop = TypeHelpers::GetDependencyProperty(o->GetClassType(), name);
    if (prop == nullptr) {
        OsiError("Dependency object " << o->GetClassType()->GetName() << " has no property named '" << name << "'");
        return;
    }

    SetDependencyProperty(L, o, prop, value);
}

void DependencyObjectHelpers::SetDependencyProperty(lua_State* L, DependencyObject* o, DependencyProperty const* prop, lua::AnyRef value)
{
    auto val = StoredValueHelpers::GetValue(L, prop->GetType(), value);
    if (!val) {
        return;
    }

    auto pValue = val->Value;
    if (val->IsIntegral) pValue = &val->Value;

    prop->GetValueManager()->SetValue(o, prop, pValue, 0, nullptr, nullptr, Value::Destination_BaseValue);
}

Symbol DependencyPropertyHelpers::GetName(DependencyProperty const* o)
{
    return o->GetName();
}

Type* DependencyPropertyHelpers::GetType(DependencyProperty const* o)
{
    return const_cast<Type *>(o->GetType());
}

TypeClass* DependencyPropertyHelpers::GetOwnerType(DependencyProperty const* o)
{
    return const_cast<TypeClass*>(o->GetOwnerType());
}

bool DependencyPropertyHelpers::IsReadOnly(DependencyProperty const* o)
{
    return o->IsReadOnly();
}

template <class T>
void StoredValueHelpers::PushRawValue(lua_State* L, Type const* type, void* val)
{
    lua::push(L, *reinterpret_cast<T*>(&val));
}

template <class T>
void StoredValueHelpers::PushPtrValue(lua_State* L, Type const* type, void* val)
{
    lua::push(L, *reinterpret_cast<T*>(val));
}

void StoredValueHelpers::PushValue(lua_State* L, TypeEnum const* type, uint64_t val)
{
    for (auto const& enumEle : type->mValues) {
        if (enumEle.second == val) {
            lua::push(L, enumEle.first);
            return;
        }
    }

    lua::push(L, val);
}

std::optional<int64_t> StoredValueHelpers::TryParseIntegralValue(lua_State* L, Type const* type, void* val)
{
    auto& types = gStaticSymbols.Types;

    if (type == types.Int8.Type) {
        return *reinterpret_cast<int8_t*>(&val);
    } else if (type == types.Int16.Type) {
        return *reinterpret_cast<int16_t*>(&val);
    } else if (type == types.Int32.Type) {
        return *reinterpret_cast<int32_t*>(&val);
    } else if (type == types.Int64.Type) {
        return *reinterpret_cast<int64_t*>(&val);
    } else if (type == types.UInt8.Type) {
        return *reinterpret_cast<uint8_t*>(&val);
    } else if (type == types.UInt16.Type) {
        return *reinterpret_cast<uint16_t*>(&val);
    } else if (type == types.UInt32.Type) {
        return *reinterpret_cast<uint32_t*>(&val);
    } else if (type == types.UInt64.Type) {
        return (int64_t)*reinterpret_cast<uint64_t*>(&val);
    } else {
        return {};
    }
}

void StoredValueHelpers::PushValue(lua_State* L, Type const* type, void* val, Type const* objectType, Symbol* propertyName)
{
    auto& classes = gStaticSymbols.TypeClasses;
    auto& types = gStaticSymbols.Types;
    auto typeOfType = type->GetClassType();

    if (typeOfType == types.TypeConst.Type) {
        PushValue(L, static_cast<TypeConst const*>(type)->GetContentType(), val, objectType, propertyName);

    } else if (type == types.Int8.Type) {
        PushRawValue<int8_t>(L, type, val);
    } else if (type == types.Int16.Type) {
        PushRawValue<int16_t>(L, type, val);
    } else if (type == types.Int32.Type) {
        PushRawValue<int32_t>(L, type, val);
    } else if (type == types.Int64.Type) {
        PushRawValue<int64_t>(L, type, val);
    } else if (type == types.UInt8.Type) {
        PushRawValue<uint8_t>(L, type, val);
    } else if (type == types.UInt16.Type) {
        PushRawValue<uint16_t>(L, type, val);
    } else if (type == types.UInt32.Type) {
        PushRawValue<uint32_t>(L, type, val);
    } else if (type == types.UInt64.Type) {
        PushRawValue<uint64_t>(L, type, val);
    } else if (type == types.Single.Type) {
        PushRawValue<float>(L, type, val);
    } else if (type == types.Double.Type) {
        PushRawValue<double>(L, type, val);
    } else if (type == types.Bool.Type) {
        PushRawValue<bool>(L, type, val);
    } else if (type == types.Symbol.Type) {
        PushPtrValue<Symbol>(L, type, val);
    } else if (type == types.Color.Type) {
        PushPtrValue<Color>(L, type, val);
    } else if (type == types.Vector2.Type) {
        PushRawValue<Vector2>(L, type, val);
    } else if (type == types.Vector3.Type) {
        PushPtrValue<Vector3>(L, type, val);
    } else if (type == types.Point.Type) {
        PushRawValue<Point>(L, type, val);
    } else if (type == types.Rect.Type) {
        PushPtrValue<Rect>(L, type, val);
    } else if (type == types.Int32Rect.Type) {
        PushPtrValue<Recti>(L, type, val);
    } else if (type == types.Thickness.Type) {
        PushPtrValue<Thickness>(L, type, val);
    } else if (type == types.String.Type) {
        PushPtrValue<String>(L, type, val);
    } else if (type == types.Uri.Type) {
        lua::push(L, reinterpret_cast<Uri*>(val)->mUri.Str());

    } else if (TypeHelpers::IsDescendantOf(type, classes.BaseObject.Type)) {
        auto obj = reinterpret_cast<BaseObject*>(val);

        if (objectType != nullptr
            && obj != nullptr
            && obj->GetClassType()->GetBase() == classes.BaseCommand.Type) {

            if (objectType->GetTypeId() == gStaticSymbols.DeferredNames.DCSavegames) {
                lua::push(L, nullptr);
                return;
            }

            if (objectType->GetTypeId() == gStaticSymbols.DeferredNames.DCModBrowser
                && (*propertyName == gStaticSymbols.DeferredNames.EndorseModCommand
                    || *propertyName == gStaticSymbols.DeferredNames.ReportModCommand
                    || *propertyName == gStaticSymbols.DeferredNames.SubscribeModCommand
                    || *propertyName == gStaticSymbols.DeferredNames.UninstallModCommand
                    || *propertyName == gStaticSymbols.DeferredNames.UnsubscribeModCommand)) {
                lua::push(L, nullptr);
                return;
            }
        }

        if (obj == nullptr) {
            lua::push(L, nullptr);
        } else if (obj->GetClassType()->GetBase() == classes.BoxedValue.Type) {
            auto boxed = reinterpret_cast<BoxedValue*>(val);
            PushValue(L, boxed->GetValueType(), const_cast<void*>(boxed->GetValuePtr()), objectType, propertyName);
        } else if (TypeHelpers::IsDescendantOf(type, classes.BaseCollection.Type)) {
            auto coll = static_cast<BaseCollection*>(val);
            lua::ArrayProxyMetatable::Make(L, coll, lua::GetCurrentLifetime(L));
        } else {
            lua::MakeObjectRef(L, obj);
        }

    } else if (TypeHelpers::IsDescendantOf(typeOfType, classes.TypeEnum.Type)) {
        // This can also be an int64, but we have no way to determine for now
        auto enumVal = *reinterpret_cast<int*>(&val);
        auto enumType = static_cast<TypeEnum const*>(type);
        PushValue(L, enumType, enumVal);
    } else {
        if (objectType) {
            auto cn = type->GetClassType()->GetName();
            ERR("Don't know how to fetch property %s:%s of type '%s'", objectType->GetName(),
                propertyName->Str(), type->GetName());
        } else {
            ERR("Don't know how to fetch properties of type '%s'", type->GetName());
        }
        lua::push(L, nullptr);
    }
}

template <class T>
void PushTyped(lua_State* L, TypeProperty const* prop, BaseObject const* obj)
{
    T value;
    prop->GetCopy(obj, &value);
    lua::push(L, value);
}

template <class T>
void PushTypedRef(lua_State* L, TypeProperty const* prop, BaseObject const* obj)
{
    auto value = reinterpret_cast<T*>(const_cast<void*>(prop->Get(obj)));
    lua::push(L, *value);
}

StoredValueHolder::~StoredValueHolder()
{
    if (IsOwned && Value != nullptr) {
        GameFree(Value);
    }
}

void StoredValueHelpers::PushProperty(lua_State* L, BaseObject const* obj, TypeClass const* objType, TypeProperty const* prop)
{
    auto& types = gStaticSymbols.Types;
    auto& classes = gStaticSymbols.TypeClasses;
    auto type = prop->GetContentType();
    auto typeOfType = type->GetClassType();
    auto typeName = type->GetName();
    auto propertyName = prop->GetName();

    if (type == types.CStringPtr.Type) {
        PushTyped<char const*>(L, prop, obj);

    } else if (type == types.CTypePtr.Type) {
        Type* value;
        prop->GetCopy(obj, &value);
        lua::MakeObjectRef(L, value);

    } else if (typeOfType == types.TypePtr.Type) {
        auto propValue = reinterpret_cast<Ptr<BaseRefCounted>*>(const_cast<void*>(prop->Get(obj)));
        PushValue(L, static_cast<TypePtr const*>(type)->GetStaticContentType(), propValue->GetPtr(), objType, &propertyName);

    } else if (typeOfType == types.TypePointer.Type) {
        BaseObject* propValue{ nullptr };
        prop->GetCopy(obj, &propValue);
        PushValue(L, static_cast<TypePointer const*>(type)->GetStaticContentType(), propValue, objType, &propertyName);

    } else if (TypeHelpers::IsDescendantOf(type->GetClassType(), classes.TypeEnum.Type)) {
        auto cn = type->GetClassType()->GetName();
        int64_t enumLabel{ 0 };
        prop->GetCopy(obj, &enumLabel);

        auto enumType = static_cast<TypeEnum const*>(type);
        PushValue(L, enumType, enumLabel);

    } else {
        if (type == types.Int8.Type) {
            PushTyped<int8_t>(L, prop, obj);
        } else if (type == types.Int16.Type) {
            PushTyped<int16_t>(L, prop, obj);
        } else if (type == types.Int32.Type) {
            PushTyped<int32_t>(L, prop, obj);
        } else if (type == types.Int64.Type) {
            PushTyped<int64_t>(L, prop, obj);
        } else if (type == types.UInt8.Type) {
            PushTyped<uint8_t>(L, prop, obj);
        } else if (type == types.UInt16.Type) {
            PushTyped<uint16_t>(L, prop, obj);
        } else if (type == types.UInt32.Type) {
            PushTyped<uint32_t>(L, prop, obj);
        } else if (type == types.UInt64.Type) {
            PushTyped<uint64_t>(L, prop, obj);
        } else if (type == types.Single.Type) {
            PushTyped<float>(L, prop, obj);
        } else if (type == types.Double.Type) {
            PushTyped<double>(L, prop, obj);
        } else if (type == types.Bool.Type) {
            PushTyped<bool>(L, prop, obj);
        } else if (type == types.Symbol.Type) {
            PushTyped<Symbol>(L, prop, obj);
        } else if (type == types.Color.Type) {
            PushTypedRef<Color>(L, prop, obj);
        } else if (type == types.Vector2.Type) {
            PushTypedRef<Vector2>(L, prop, obj);
        } else if (type == types.Vector3.Type) {
            PushTypedRef<Vector3>(L, prop, obj);
        } else if (type == types.Point.Type) {
            PushTypedRef<Point>(L, prop, obj);
        } else if (type == types.Rect.Type) {
            PushTypedRef<Rect>(L, prop, obj);
        } else if (type == types.Int32Rect.Type) {
            PushTypedRef<Recti>(L, prop, obj);
        } else if (type == types.Thickness.Type) {
            PushTypedRef<Thickness>(L, prop, obj);
        } else if (type == types.String.Type) {
            auto value = reinterpret_cast<String const*>(prop->Get(obj));
            lua::push(L, *value);
            
        } else if (type == types.Uri.Type) {
            auto value = reinterpret_cast<Uri const*>(prop->Get(obj));
            lua::push(L, value->mUri.Str());
            
        } else if (type == types.LocaString.Type) {
            auto str = reinterpret_cast<TranslatedString const*>(prop->Get(obj));
            lua::push(L, str->Handle.Handle);

        } else {
            auto cn = type->GetClassType()->GetName();
            ERR("Don't know how to fetch property %s:%s of type '%s'", obj->GetClassType()->GetName(), 
                prop->GetName().Str(), type->GetName());
            lua::push(L, nullptr);
        }
    }
}

void StoredValueHelpers::PushValue(lua_State* L, Type const* type, StoredValue const* o, Type const* objectType, Symbol* propertyName)
{
    if (!o->flags.isInitialized) {
        lua::push(L, nullptr);
        return;
    }

    // TODO - complex values not supported yet
    if (o->flags.isComplex) {
        if (o->flags.isExpression) {
            lua::push(L, nullptr);
        } else {
            PushValue(L, type, o->value.complex->base, objectType, propertyName);
        }
    } else {
        PushValue(L, type, o->value.simple, objectType, propertyName);
    }
}

template <class T>
StoredValueHolder StoredValueHelpers::GetRawValue(lua_State* L, Type const* type, lua::AnyRef value)
{
    auto val = lua::get<T>(L, value.Index);
    if constexpr (sizeof(T) <= sizeof(void*)) {
        return StoredValueHolder (*reinterpret_cast<void**>(&val), false);
    } else {
        auto buf = GameAlloc<T>(val);
        return StoredValueHolder(buf, true);
    }
}

std::optional<StoredValueHolder> StoredValueHelpers::GetValue(lua_State* L, Type const* type, lua::AnyRef value)
{
    auto& classes = gStaticSymbols.TypeClasses;
    auto& types = gStaticSymbols.Types;

    if (type == types.Int8.Type) {
        return GetRawValue<int8_t>(L, type, value);
    } else if (type == types.Int16.Type) {
        return GetRawValue<int16_t>(L, type, value);
    } else if (type == types.Int32.Type) {
        return GetRawValue<int32_t>(L, type, value);
    } else if (type == types.Int64.Type) {
        return GetRawValue<int64_t>(L, type, value);
    } else if (type == types.UInt8.Type) {
        return GetRawValue<uint8_t>(L, type, value);
    } else if (type == types.UInt16.Type) {
        return GetRawValue<uint16_t>(L, type, value);
    } else if (type == types.UInt32.Type) {
        return GetRawValue<uint32_t>(L, type, value);
    } else if (type == types.UInt64.Type) {
        return GetRawValue<uint64_t>(L, type, value);
    } else if (type == types.Single.Type) {
        return GetRawValue<float>(L, type, value);
    } else if (type == types.Double.Type) {
        return GetRawValue<double>(L, type, value);
    } else if (type == types.Bool.Type) {
        return GetRawValue<bool>(L, type, value);
    } else if (type == types.Symbol.Type) {
        return GetRawValue<Symbol>(L, type, value);
    } else if (type == types.Color.Type) {
        return GetRawValue<glm::vec4>(L, type, value);
    } else if (type == types.Vector2.Type) {
        return GetRawValue<glm::vec2>(L, type, value);
    } else if (type == types.Vector3.Type) {
        return GetRawValue<glm::vec3>(L, type, value);
    } else if (type == types.Point.Type) {
        return GetRawValue<glm::vec2>(L, type, value);
    } else if (type == types.Rect.Type) {
        return GetRawValue<glm::vec4>(L, type, value);
    } else if (type == types.Int32Rect.Type) {
        return GetRawValue<glm::ivec4>(L, type, value);
    } else if (type == types.Thickness.Type) {
        return GetRawValue<glm::vec4>(L, type, value);
    } else if (type == types.String.Type) {
        return GameAlloc<String>(lua::get<char const*>(L, value.Index));

    } else if (TypeHelpers::IsDescendantOf(type->GetClassType(), classes.TypeEnum.Type)) {
        auto enumVal = lua::get<char const*>(L, value.Index);
        auto enumType = static_cast<TypeEnum const*>(type);
        
        auto label = TypeHelpers::StringToEnum(enumType, enumVal);
        if (!label) {
            OsiError("Invalid enum value '" << enumVal << "' for enumeration '" << type->GetName() << "'");
            return {};
        }

        return StoredValueHolder((void*)(*label), false);

    } else if (TypeHelpers::IsDescendantOf(type, classes.BaseObject.Type)) {
        return lua::get<BaseObject*>(L, value.Index);

    } else {
        auto typeOfType = type->GetClassType();

        if (typeOfType == types.TypePtr.Type) {
            auto contentType = static_cast<TypePtr const*>(type)->GetStaticContentType();
            // Ptr only supports refcounted types, i.e. at least BaseComponent
            auto obj = lua::get<BaseComponent*>(L, value.Index);
            if (TypeHelpers::IsDescendantOf(obj->GetClassType(), static_cast<TypeClass const*>(contentType))) {
                // Simulate 'returning' a Ptr<BaseComponent>
                obj->AddReference();
                return obj;
            } else {
                ERR("Expected object of type '%s', got '%s'", type->GetName(), obj->GetClassType()->GetName());
                return {};
            }

        } else if (typeOfType == types.TypePointer.Type) {
            auto contentType = static_cast<TypePointer const*>(type)->GetStaticContentType();
            if (contentType->GetClassType() == classes.TypeClass.Type) {
                auto obj = lua::get<BaseObject*>(L, value.Index);
                if (TypeHelpers::IsDescendantOf(obj->GetClassType(), static_cast<TypeClass const*>(contentType))) {
                    return obj;
                } else {
                    ERR("Expected object of type '%s', got '%s'", type->GetName(), obj->GetClassType()->GetName());
                    return {};
                }
            } else {
                ERR("Pointer to non-object '%s' not supported", type->GetName());
                return {};
            }
        }
    }

    ERR("Don't know how to parse type '%s'", type->GetName());
    return {};
}

bool CommandHelpers::CanExecute(lua_State* L, BaseCommand const* o, std::optional<BaseComponent*> arg)
{
    return o->CanExecute(arg ? * arg : nullptr);
}

void CommandHelpers::Execute(lua_State* L, BaseCommand const* o, std::optional<BaseComponent*> arg)
{
    o->Execute(arg ? *arg : nullptr);
}

Visual* VisualHelpers::GetVisualParent(Visual const* o)
{
    return o->mVisualParent;
}

uint32_t VisualHelpers::GetVisualChildrenCount(lua_State* L, Visual const* o)
{
    auto cls = o->GetClassType();
    if (!TypeHelpers::IsDescendantOf(cls, gStaticSymbols.TypeClasses.Visual.Type)) {
        luaL_error(L, "Trying to get visual child count of type %s?", cls->GetName());
        return 0;
    }

    return o->GetVisualChildrenCount();
}

Visual* VisualHelpers::GetVisualChild(Visual const* o, uint32_t index)
{
    if (index > 0 && index <= o->GetVisualChildrenCount()) {
        return o->GetVisualChild(index - 1);
    } else {
        return nullptr;
    }
}

RoutedEvent* UIElementDataHelpers::GetEvent(UIElementData const* o, Symbol evt)
{
    auto it = o->mEvents.Find(evt);
    if (it == o->mEvents.End()) {
        return nullptr;
    } else {
        return const_cast<RoutedEvent*>(it->GetPtr());
    }
}

Array<RoutedEvent*> UIElementDataHelpers::GetAllEvents(UIElementData const* o)
{
    Array<RoutedEvent*> events;
    for (auto evt : o->mEvents) {
        events.push_back(const_cast<RoutedEvent*>(evt.GetPtr()));
    }

    return events;
}

uint64_t UIElementHelpers::Subscribe(lua_State* L, UIElement* target, bg3se::FixedString const& evt, lua::FunctionRef func)
{
    auto routedEvent = TypeHelpers::GetRoutedEvent(target->GetClassType(), evt);
    if (routedEvent == nullptr) {
        OsiError("UI element " << target->GetClassType()->GetName() << " has no event named '" << evt << "'");
        return 0;
    }

    auto state = static_cast<ecl::lua::ClientState*>(lua::State::FromLua(L));
    return state->GetUIEvents().Subscribe(target, routedEvent, evt, lua::RegistryEntry(L, func.Index));
}

bool UIElementHelpers::Unsubscribe(lua_State* L, UIElement* o, uint64_t index)
{
    auto state = static_cast<ecl::lua::ClientState*>(lua::State::FromLua(L));
    return state->GetUIEvents().Unsubscribe((ecl::lua::UIEventHooks::SubscriptionIndex)index);
}

FrameworkElement* FrameworkElementHelpers::GetLogicalParent(FrameworkElement const* o)
{
    return o->mParent;
}

uint32_t FrameworkElementHelpers::GetLogicalChildrenCount(FrameworkElement const* o)
{
    return o->GetLogicalChildrenCount();
}

BaseComponent* FrameworkElementHelpers::GetLogicalChild(FrameworkElement const* o, uint32_t index)
{
    if (index > 0 && index <= o->GetLogicalChildrenCount()) {
        return o->GetLogicalChild(index - 1).GetPtr();
    } else {
        return nullptr;
    }
}

BaseComponent* FrameworkElementHelpers::FindNodeName(FrameworkElement const* o, char const* name)
{
    return o->FindNodeName(name);
}

BaseComponent* FrameworkElementHelpers::GetResource(FrameworkElement const* o, char const* key, std::optional<bool> fullElementSearch)
{
    return o->FindNodeResource(key, fullElementSearch ? *fullElementSearch : false);
}

BaseObject* FrameworkElementHelpers::GetTreeParent(FrameworkElement const* o)
{
    auto parent = o->GetNodeParent();
    if (parent) {
        return parent->GetBaseObject();
    } else {
        return nullptr;
    }
}

FrameworkElement* FrameworkElementHelpers::AttachXamlChild(FrameworkElement* o, char const* path)
{
    auto element = LoadXaml(path);
    if (!element) return nullptr;

    o->AddVisualChild(element.GetPtr());
    // TODO: do we need AddLogicalChild?

    return element.GetPtr();
}

FrameworkElement* FrameworkElementHelpers::SetXamlProperty(FrameworkElement* o, bg3se::FixedString const& prop, char const* path)
{
    auto dependencyProp = TypeHelpers::GetDependencyProperty(o->GetClassType(), prop);
    if (dependencyProp == nullptr) {
        OsiError("FrameworkElement " << o->GetClassType()->GetName() << " has no property named '" << prop << "'");
        return nullptr;
    }

    if (!TypeHelpers::IsDescendantOf(dependencyProp->GetType(), gStaticSymbols.TypeClasses.BaseObject.Type)) {
        OsiError("XAML attach error: Cannot bind objects to property '" << prop << "'");
        return nullptr;
    }

    auto element = LoadXaml(path);
    if (!element) return nullptr;

    dependencyProp->GetValueManager()->SetValue(o, dependencyProp, (void*)element.GetPtr(), 0, nullptr, nullptr, Value::Destination_BaseValue);

    return element.GetPtr();
}

Symbol TypePropertyHelpers::GetName(TypeProperty const* o)
{
    return o->mName;
}

Type* TypePropertyHelpers::GetContentType(TypeProperty const* o)
{
    return const_cast<Type*>(o->GetContentType());
}

Vector<TypeMetaData*> const* TypePropertyHelpers::GetMeta(TypeProperty const* o)
{
    return &o->mMetaData;
}

bool TypePropertyHelpers::IsReadOnly(TypeProperty const* o)
{
    return o->IsReadOnly();
}


bg3se::STDString TypeHelpers::GetName(Type const* o)
{
    return o->GetName();
}

bool TypeHelpers::IsDescendantOf(Type const* type, TypeClass const* cls)
{
    auto& types = gStaticSymbols.TypeClasses;

    if (type->GetClassType() == types.TypeClass.Type) {
        auto base = static_cast<TypeClass const*>(type);
        return IsDescendantOf(base, cls);
    }

    return false;
}

bool TypeHelpers::IsDescendantOf(TypeClass const* type, TypeClass const* base)
{
    do {
        if (type == base) {
            return true;
        }

        type = type->GetBase();
    } while (type != nullptr);

    return false;
}

TypeClass* TypeHelpers::GetBase(TypeClass const* o)
{
    return const_cast<TypeClass *>(o->GetBase());
}

bool TypeHelpers::IsInterface(TypeClass const* o)
{
    return o->IsInterface();
}

template <class Fun>
void TypeHelpers::ForEachMeta(TypeClass const* cls, const TypeClass* metaDataType, Fun fun)
{
    do {
        auto meta = FindMetaOrDescendant(cls, metaDataType);
        if (meta != nullptr) {
            fun(meta);
        }

        cls = cls->GetBase();
    } while (cls != nullptr);
}

std::optional<uint64_t> TypeHelpers::StringToEnum(TypeEnum const* e, char const* value)
{
    auto sym = SymbolManager::FindString(value);
    if (!sym) {
        return {};
    }

    return StringToEnum(e, Symbol(sym));
}

std::optional<uint64_t> TypeHelpers::StringToEnum(TypeEnum const* e, Symbol value)
{
    for (auto const& label : e->mValues) {
        if (label.first == value) {
            return label.second;
        }
    }

    return {};
}

TypeClass::AncestorVector const* TypeHelpers::GetInterfaces(TypeClass const* o)
{
    return &o->mInterfaces;
}

Array<TypeProperty const*> const* TypeHelpers::GetProperties(TypeClass const* o)
{
    auto const& cls = gClassCache.GetClass(o);
    return &cls.Properties;
}

Array<DependencyProperty const*> const* TypeHelpers::GetDependencyProperties(TypeClass const* o)
{
    auto const& cls = gClassCache.GetClass(o);
    return &cls.DependencyProperties;
}

Array<RoutedEvent const*> const* TypeHelpers::GetRoutedEvents(TypeClass const* o)
{
    auto const& cls = gClassCache.GetClass(o);
    return &cls.Events;
}

TypeProperty const* TypeHelpers::GetProperty(TypeClass const* o, bg3se::FixedString const& name)
{
    auto const& cls = gClassCache.GetClass(o);
    auto prop = cls.Names.try_get(name);
    return prop ? prop->Property : nullptr;
}

DependencyProperty const* TypeHelpers::GetDependencyProperty(TypeClass const* o, bg3se::FixedString const& name)
{
    auto const& cls = gClassCache.GetClass(o);
    auto prop = cls.Names.try_get(name);
    return prop ? prop->DepProperty : nullptr;
}

RoutedEvent const* TypeHelpers::GetRoutedEvent(TypeClass const* o, bg3se::FixedString const& name)
{
    auto const& cls = gClassCache.GetClass(o);
    auto prop = cls.Names.try_get(name);
    return prop ? prop->Event : nullptr;
}

TypeMetaData* TypeHelpers::FindMetaOrDescendant(TypeClass const* o, const TypeClass* metaDataType)
{
    for (auto const& meta : o->mMetaData) {
        auto m = meta->GetClassType();
        if (IsDescendantOf(m, metaDataType)) {
            return meta;
        }
    }

    return nullptr;
}

Ptr<FrameworkElement> LoadXaml(char const* path)
{
    auto loadProc = (LoadXamlProc*)GetStaticSymbols().Noesis__GUI__LoadXaml;
    if (!loadProc) {
        ERR("XAML loader binding not found");
        return nullptr;
    }

    Ptr<BaseComponent> widget;
    loadProc(widget, path);

    if (!widget) {
        ERR("Unable to instantiate UI from XAML: %s", path);
        return nullptr;
    }

    if (!TypeHelpers::IsDescendantOf(widget->GetClassType(), gStaticSymbols.TypeClasses.FrameworkElement.Type)) {
        ERR("Instantiated UI is not a FrameworkElement, but a %s", widget->GetClassType()->GetName());
        return nullptr;
    }

    return Ptr(static_cast<FrameworkElement*>(widget.GetPtr()));
}

END_BARE_NS()
