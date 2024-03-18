#include <GameDefinitions/UI.h>
#include <NsGui/UIElementEvents.h>
#include <Lua/Libs/ClientUI/Symbols.inl>

BEGIN_BARE_NS(Noesis)

Ptr<FrameworkElement> LoadXaml(char const* path);

void* BaseObject::operator new(unsigned __int64 sz)
{
	return GameAllocRaw(sz);
}

void BaseObject::operator delete(void* ptr)
{
	GameFree(ptr);
}

BaseObject::BaseObject() {}
BaseObject::~BaseObject() {}

const TypeClass* BaseObject::GetClassType() const
{
	return gStaticSymbols.TypeClasses.BaseObject.Type;
}

String BaseObject::ToString() const
{
	return GetClassType()->GetName();
}

bool BaseObject::Equals(const BaseObject* left, const BaseObject* right)
{
	return left == right
		|| left->Equals(right);
}

bool BaseObject::Equals(const BaseObject* o) const
{
	return this == o;
}

BaseComponent::BaseComponent() {}
BaseComponent::~BaseComponent() {}

const TypeClass* BaseComponent::GetClassType() const
{
	return gStaticSymbols.TypeClasses.BaseComponent.Type;
}

const TypeClass* BaseRefCounted::GetClassType() const
{
	return gStaticSymbols.TypeClasses.BaseRefCounted.Type;
}

uint32_t SymbolManager::FindString(char const* k)
{
	auto it = GetSymbolManager()->Keys.Find(k);
	if (it != GetSymbolManager()->Keys.End()) {
		return it->value;
	}

	auto it2 = gStaticSymbols.Symbols.find(k);
	if (it2 != gStaticSymbols.Symbols.end()) {
		return it2->second;
	}

	return 0;
}

using AddVisualChildProc = void (Visual*, Visual*);

void Visual::AddVisualChild(Visual* child)
{
	auto addProc = (AddVisualChildProc*)GetStaticSymbols().Noesis__Visual__AddVisualChild;
	if (addProc) addProc(this, child);
}

void Visual::RemoveVisualChild(Visual* child)
{
	auto removeProc = (AddVisualChildProc*)GetStaticSymbols().Noesis__Visual__RemoveVisualChild;
	if (removeProc) removeProc(this, child);
}

template <class T>
Symbol StaticSymbol()
{
	return SymbolInfo<T>::Name;
}

TypeClass* ObjectHelpers::GetClassType(BaseObject* o)
{
	return const_cast<TypeClass*>(o->GetClassType());
}

Symbol ObjectHelpers::GetClassTypeName(BaseObject* o)
{
	return o->GetClassType()->GetTypeId();
}

bg3se::STDString ObjectHelpers::ToString(BaseObject* o)
{
	return o->ToString().Str();
}

uint32_t ObjectHelpers::GetNumReferences(BaseRefCounted* o)
{
	return o->GetNumReferences();
}

Symbol RoutedEventHelpers::GetName(RoutedEvent* o)
{
	return o->GetName();
}

TypeClass* RoutedEventHelpers::GetOwnerType(RoutedEvent* o)
{
	return const_cast<TypeClass*>(o->GetOwnerType());
}

PropertyOperationResult DependencyObjectHelpers::FallbackGetProperty(lua_State* L, lua::LifetimeHandle const& lifetime, DependencyObject* object, bg3se::FixedString const& prop)
{
	auto dependencyProp = TypeHelpers::GetDependencyProperty(object->GetClassType(), Symbol(prop.GetString(), Symbol::NullIfNotFound{}));
	if (dependencyProp != nullptr) {
		GetDependencyProperty(L, object, dependencyProp);
		return PropertyOperationResult::Success;
	}

	return PropertyOperationResult::NoSuchProperty;
}

PropertyOperationResult DependencyObjectHelpers::FallbackSetProperty(lua_State* L, DependencyObject* object, bg3se::FixedString const& prop, int index)
{
	auto dependencyProp = TypeHelpers::GetDependencyProperty(object->GetClassType(), Symbol(prop.GetString(), Symbol::NullIfNotFound{}));
	if (dependencyProp != nullptr) {
		SetDependencyProperty(L, object, dependencyProp, lua::AnyRef(index));
		return PropertyOperationResult::Success;
	}

	return PropertyOperationResult::NoSuchProperty;
}

UserReturn DependencyObjectHelpers::GetProperty(lua_State* L, DependencyObject* o, Symbol name)
{
	auto prop = TypeHelpers::GetDependencyProperty(o->GetClassType(), name);
	if (prop == nullptr) {
		OsiError("Dependency object " << o->GetClassType()->GetName() << " has no property named '" << name.Str() << "'");
		lua::push(L, nullptr);
		return 1;
	}

	return GetDependencyProperty(L, o, prop);
}

UserReturn DependencyObjectHelpers::GetDependencyProperty(lua_State* L, DependencyObject* o, DependencyProperty const* prop)
{
	auto valIt = o->mValues.Find(prop);
	if (valIt == o->mValues.End()) {
		lua::push(L, nullptr);
		return 1;
	}

	StoredValueHelpers::PushValue(L, prop->GetType(), valIt->value);
	return 1;
}

void DependencyObjectHelpers::SetProperty(lua_State* L, DependencyObject* o, Symbol name, lua::AnyRef value)
{
	auto prop = TypeHelpers::GetDependencyProperty(o->GetClassType(), name);
	if (prop == nullptr) {
		OsiError("Dependency object " << o->GetClassType()->GetName() << " has no property named '" << name.Str() << "'");
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

UserReturn DependencyObjectHelpers::GetAllProperties(lua_State* L, DependencyObject* o)
{
	lua_newtable(L);

	for (auto& prop : o->mValues) {
		lua::push(L, prop.key->GetName());
		StoredValueHelpers::PushValue(L, prop.key->GetType(), prop.value);
		lua_settable(L, -3);
	}

	return 1;
}

Symbol DependencyPropertyHelpers::GetName(DependencyProperty* o)
{
	return o->GetName();
}

Type* DependencyPropertyHelpers::GetType(DependencyProperty* o)
{
	return const_cast<Type *>(o->GetType());
}

TypeClass* DependencyPropertyHelpers::GetOwnerType(DependencyProperty* o)
{
	return const_cast<TypeClass*>(o->GetOwnerType());
}

bool DependencyPropertyHelpers::IsReadOnly(DependencyProperty* o)
{
	return o->IsReadOnly();
}

template <class T>
void StoredValueHelpers::PushRawValue(lua_State* L, Type const* type, void* val)
{
	lua::push(L, *reinterpret_cast<T*>(&val));
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

void StoredValueHelpers::PushValue(lua_State* L, Type const* type, void* val)
{
	auto& classes = gStaticSymbols.TypeClasses;
	auto& types = gStaticSymbols.Types;

	if (type == types.Int8.Type) {
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
	} else if (type == types.String.Type) {
		lua::push(L, *reinterpret_cast<String *>(val));

	} else if (TypeHelpers::IsDescendantOf(type, classes.BaseObject.Type)) {
		auto obj = reinterpret_cast<BaseObject*>(val);

		if (obj != nullptr && obj->GetClassType()->GetBase() == classes.BoxedValue.Type) {
			auto boxed = reinterpret_cast<BoxedValue*>(val);
			PushValue(L, boxed->GetValueType(), const_cast<void*>(boxed->GetValuePtr()));
		} else {
			lua::MakeObjectRef(L, obj);
		}

	} else if (TypeHelpers::IsDescendantOf(type->GetClassType(), classes.TypeEnum.Type)) {
		auto enumVal = *reinterpret_cast<int*>(&val);
		auto enumType = static_cast<TypeEnum const*>(type);
		PushValue(L, enumType, enumVal);
	} else {
		ERR("Don't know how to fetch properties of type '%s'", type->GetName());
		lua::push(L, nullptr);
	}
}

void StoredValueHelpers::PushValue(lua_State* L, Type const* type, StoredValue const* o)
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
			PushValue(L, type, o->value.complex->base);
		}
	} else {
		PushValue(L, type, o->value.simple);
	}
}

template <class T>
void* StoredValueHelpers::GetRawValue(lua_State* L, Type const* type, lua::AnyRef value)
{
	auto val = lua::get<T>(L, value.Index);
	return *reinterpret_cast<void**>(&val);
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

		return (void*)(*label);

	} else if (TypeHelpers::IsDescendantOf(type, classes.BaseObject.Type)) {
		return lua::get<BaseObject*>(L, value.Index);
	} else {
		ERR("Don't know how to parse type '%s'", type->GetName());
		return {};
	}
}

Visual* VisualHelpers::GetVisualParent(Visual* o)
{
	return o->mVisualParent;
}

uint32_t VisualHelpers::GetVisualChildrenCount(Visual* o)
{
	return o->GetVisualChildrenCount();
}

Visual* VisualHelpers::GetVisualChild(Visual* o, uint32_t index)
{
	if (index > 0 && index <= o->GetVisualChildrenCount()) {
		return o->GetVisualChild(index - 1);
	} else {
		return nullptr;
	}
}

RoutedEvent* UIElementDataHelpers::GetEvent(UIElementData* o, Symbol evt)
{
	auto it = o->mEvents.Find(evt);
	if (it == o->mEvents.End()) {
		return nullptr;
	} else {
		return const_cast<RoutedEvent*>(it->GetPtr());
	}
}

Array<RoutedEvent*> UIElementDataHelpers::GetAllEvents(UIElementData* o)
{
	Array<RoutedEvent*> events;
	for (auto evt : o->mEvents) {
		events.push_back(const_cast<RoutedEvent*>(evt.GetPtr()));
	}

	return events;
}

uint64_t UIElementHelpers::Subscribe(lua_State* L, UIElement* target, Symbol evt, lua::FunctionRef func)
{
	auto routedEvent = TypeHelpers::GetRoutedEvent(target->GetClassType(), evt);
	if (routedEvent == nullptr) {
		OsiError("UI element " << target->GetClassType()->GetName() << " has no event named '" << evt.Str() << "'");
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

FrameworkElement* FrameworkElementHelpers::GetLogicalParent(FrameworkElement* o)
{
	return o->mParent;
}

uint32_t FrameworkElementHelpers::GetLogicalChildrenCount(FrameworkElement* o)
{
	return o->GetLogicalChildrenCount();
}

BaseComponent* FrameworkElementHelpers::GetLogicalChild(FrameworkElement* o, uint32_t index)
{
	if (index > 0 && index <= o->GetLogicalChildrenCount()) {
		return o->GetLogicalChild(index - 1).GetPtr();
	} else {
		return nullptr;
	}
}

BaseComponent* FrameworkElementHelpers::FindNodeName(FrameworkElement* o, char const* name)
{
	return o->FindNodeName(name);
}

BaseComponent* FrameworkElementHelpers::GetResource(FrameworkElement* o, char const* key, std::optional<bool> fullElementSearch)
{
	return o->FindNodeResource(key, fullElementSearch ? *fullElementSearch : false);
}

BaseObject* FrameworkElementHelpers::GetTreeParent(FrameworkElement* o)
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

FrameworkElement* FrameworkElementHelpers::SetXamlProperty(FrameworkElement* o, char const* prop, char const* path)
{
	auto dependencyProp = TypeHelpers::GetDependencyProperty(o->GetClassType(), Symbol(prop, Symbol::NullIfNotFound{}));
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

Symbol TypePropertyHelpers::GetName(TypeProperty* o)
{
	return o->mName;
}

Type* TypePropertyHelpers::GetContentType(TypeProperty* o)
{
	return const_cast<Type*>(o->GetContentType());
}

Vector<TypeMetaData*>* TypePropertyHelpers::GetMeta(TypeProperty* o)
{
	return &o->mMetaData;
}

bool TypePropertyHelpers::IsReadOnly(TypeProperty* o)
{
	return o->IsReadOnly();
}


bg3se::STDString TypeHelpers::GetName(Type* o)
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

TypeClass* TypeHelpers::GetBase(TypeClass* o)
{
	return const_cast<TypeClass *>(o->GetBase());
}

bool TypeHelpers::IsInterface(TypeClass* o)
{
	return o->IsInterface();
}

DependencyData* TypeHelpers::GetDependencyData(TypeClass* o)
{
	gStaticSymbols.Initialize();
	return static_cast<DependencyData *>(TypeHelpers::FindMetaRecursive(o, (TypeClass const*)Reflection::GetType(StaticSymbol<DependencyData>())));
}

DependencyProperty const* TypeHelpers::GetDependencyProperty(TypeClass const* o, Symbol name)
{
	gStaticSymbols.Initialize();
	auto dependencyDataType = (TypeClass const*)Reflection::GetType(StaticSymbol<DependencyData>());
	auto uiElementDataType = (TypeClass const*)Reflection::GetType(StaticSymbol<UIElementData>());

	do {
		auto meta = o->FindMeta(dependencyDataType);
		if (meta == nullptr) meta = o->FindMeta(uiElementDataType);

		if (meta != nullptr) {
			auto deps = static_cast<DependencyData*>(meta);

			auto propIt = deps->mProperties.Find(name);
			if (propIt != deps->mProperties.End()) {
				return propIt->GetPtr();
			}
		}

		o = o->GetBase();
	} while (o != nullptr);

	return nullptr;
}

RoutedEvent const* TypeHelpers::GetRoutedEvent(TypeClass const* o, Symbol name)
{
	gStaticSymbols.Initialize();
	auto uiElementDataType = (TypeClass const*)Reflection::GetType(StaticSymbol<UIElementData>());

	do {
		auto meta = o->FindMeta(uiElementDataType);

		if (meta != nullptr) {
			auto deps = static_cast<UIElementData*>(meta);

			auto eventIt = deps->mEvents.Find(name);
			if (eventIt != deps->mEvents.End()) {
				return eventIt->GetPtr();
			}
		}

		o = o->GetBase();
	} while (o != nullptr);

	return nullptr;
}

UIElementData* TypeHelpers::GetUIElementData(TypeClass* o)
{
	gStaticSymbols.Initialize();
	return static_cast<UIElementData*>(TypeHelpers::FindMetaRecursive(o, (TypeClass const*)Reflection::GetType(StaticSymbol<UIElementData>())));
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

Array<DependencyProperty*> TypeHelpers::GetDependencyProperties(TypeClass* o)
{
	Array<DependencyProperty*> props;

	ForEachMeta(o, gStaticSymbols.TypeClasses.DependencyData.Type, [&](TypeMetaData const* meta) {
		auto deps = static_cast<DependencyData const*>(meta);
		for (auto const& dep : deps->mProperties) {
			props.push_back(const_cast<DependencyProperty*>(dep.GetPtr()));
		}
	});

	return props;
}

Array<RoutedEvent*> TypeHelpers::GetRoutedEvents(TypeClass* o)
{
	Array<RoutedEvent*> events;

	ForEachMeta(o, gStaticSymbols.TypeClasses.DependencyData.Type, [&](TypeMetaData const* meta) {
		auto uiData = static_cast<UIElementData const*>(meta);
		for (auto const& evt : uiData->mEvents) {
			events.push_back(const_cast<RoutedEvent*>(evt.GetPtr()));
		}
	});

	return events;
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

TypeClass::AncestorVector* TypeHelpers::GetInterfaces(TypeClass* o)
{
	return &o->mInterfaces;
}

TypeClass::PropertyVector* TypeHelpers::GetProperties(TypeClass* o)
{
	return &o->mProperties;
}

TypeClass::PropertyVector* TypeHelpers::GetEvents(TypeClass* o)
{
	return &o->mEvents;
}

TypeMetaData* TypeHelpers::FindMetaRecursive(TypeClass const* o, const TypeClass* metaDataType)
{
	do {
		auto meta = o->FindMeta(metaDataType);
		if (meta != nullptr) return meta;

		o = o->GetBase();
	} while (o != nullptr);

	return nullptr;
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

TypeMetaData* TypeMeta::FindMeta(const TypeClass* metaDataType) const
{
	for (auto const& meta : mMetaData) {
		auto m = meta->GetClassType();
		if (m == metaDataType) {
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
