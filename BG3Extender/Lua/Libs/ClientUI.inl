#include <GameDefinitions/UI.h>

BEGIN_BARE_NS(Noesis)

BEGIN_BARE_NS(StaticSymbols)

bool SymbolsInitialized{ false };

template <class T>
struct SymbolInfo
{
	static Symbol Name;
};

#define FOR_NOESIS_TYPE(T) Symbol SymbolInfo<T>::Name;
FOR_EACH_NOESIS_TYPE()
#undef FOR_NOESIS_TYPE

Symbol MakeNoesisSymbol(std::unordered_map<bg3se::STDString, Symbol> & keys, char const* name)
{
	bg3se::STDString n(name);
	if (n.starts_with("Noesis::")) {
		n = n.substr(8);
	}

	auto it = keys.find(n);
	if (it == keys.end()) {
		return Symbol{};
	} else {
		return it->second;
	}
}

std::unordered_map<bg3se::STDString, Symbol> gSymbolStrings;

struct TypeInfo
{
	Symbol Name;
	Type const* Type;

	void Setup(char const* name)
	{
		Name = MakeNoesisSymbol(gSymbolStrings, name);
		Type = Reflection::GetType(Name);
	}
};

struct BuiltinTypes
{
	TypeInfo Int8;
	TypeInfo Int16;
	TypeInfo Int32;
	TypeInfo Int64;
	TypeInfo UInt8;
	TypeInfo UInt16;
	TypeInfo UInt32;
	TypeInfo UInt64;
	TypeInfo Single;
	TypeInfo Double;
	TypeInfo Bool;
	TypeInfo String;
	TypeInfo BaseObject;
	TypeInfo TypeClass;
	TypeInfo BoxedValue;

	void Setup()
	{
		Int8.Setup("Int8");
		Int16.Setup("Int16");
		Int32.Setup("Int32");
		Int64.Setup("Int64");
		UInt8.Setup("UInt8");
		UInt16.Setup("UInt16");
		UInt32.Setup("UInt32");
		UInt64.Setup("UInt64");
		Single.Setup("Single");
		Double.Setup("Double");
		Bool.Setup("Bool");
		String.Setup("String");
		BaseObject.Setup("BaseObject");
		TypeClass.Setup("TypeClass");
		BoxedValue.Setup("BoxedValue");
	}
};

BuiltinTypes gBuiltinTypes;

void InitializeSymbolInfo()
{
	if (SymbolsInitialized) return;

	auto& strings = Noesis::GetSymbolManager()->Strings;
	for (uint32_t i = 0; i < strings.Size(); i++) {
		gSymbolStrings.insert(std::make_pair(strings[i], i));
	}

	#define FOR_NOESIS_TYPE(T) SymbolInfo<T>::Name = MakeNoesisSymbol(gSymbolStrings, #T);
	FOR_EACH_NOESIS_TYPE()
	#undef FOR_NOESIS_TYPE

	gBuiltinTypes.Setup();

	SymbolsInitialized = true;
}

END_BARE_NS()

uint32_t SymbolManager::FindString(char const* k)
{
	auto it = GetSymbolManager()->Keys.Find(k);
	if (it != GetSymbolManager()->Keys.End()) {
		return it->value;
	}

	auto it2 = StaticSymbols::gSymbolStrings.find(k);
	if (it2 != StaticSymbols::gSymbolStrings.end()) {
		return it2->second;
	}

	return 0;
}

template <class T>
Symbol StaticSymbol()
{
	return StaticSymbols::SymbolInfo<T>::Name;
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

PropertyOperationResult DependencyObjectHelpers::FallbackGetProperty(lua_State* L, lua::LifetimeHandle const& lifetime, void* object, bg3se::FixedString const& prop)
{
	return PropertyOperationResult::NoSuchProperty;
}

PropertyOperationResult DependencyObjectHelpers::FallbackSetProperty(lua_State* L, void* object, bg3se::FixedString const& prop, int index)
{
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

	auto valIt = o->mValues.Find(prop);
	if (valIt == o->mValues.End()) {
		lua::push(L, nullptr);
		return 1;
	}

	StoredValueHelpers::PushValue(L, prop->GetType(), valIt->value);
	return 1;
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

void StoredValueHelpers::PushValue(lua_State* L, Type const* type, void* val)
{
	auto& types = StaticSymbols::gBuiltinTypes;

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

	} else if (TypeHelpers::IsDescendantOf(type, static_cast<TypeClass const *>(types.BaseObject.Type))) {
		auto obj = reinterpret_cast<BaseObject*>(val);

		if (obj != nullptr && obj->GetClassType()->GetBase() == StaticSymbols::gBuiltinTypes.BoxedValue.Type) {
			auto boxed = reinterpret_cast<BoxedValue*>(val);
			PushValue(L, boxed->GetValueType(), const_cast<void*>(boxed->GetValuePtr()));
		} else {
			lua::MakeObjectRef(L, obj);
		}
	} else {
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
		lua::push(L, nullptr);
		return;
	}

	PushValue(L, type, o->value.simple);
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
	if (index < o->GetVisualChildrenCount()) {
		return o->GetVisualChild(index);
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
	if (index < o->GetLogicalChildrenCount()) {
		return o->GetLogicalChild(index).GetPtr();
	} else {
		return nullptr;
	}
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
	auto& types = StaticSymbols::gBuiltinTypes;

	if (type->GetClassType() == types.TypeClass.Type) {
		auto base = static_cast<TypeClass const*>(type);
		do {
			if (base == cls) {
				return true;
			}

			base = base->GetBase();
		} while (base != nullptr);
	}

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
	StaticSymbols::InitializeSymbolInfo();
	return static_cast<DependencyData *>(TypeHelpers::FindMetaRecursive(o, (TypeClass const*)Reflection::GetType(StaticSymbol<DependencyData>())));
}

DependencyProperty const* TypeHelpers::GetDependencyProperty(TypeClass const* o, Symbol name)
{
	StaticSymbols::InitializeSymbolInfo();
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

UIElementData* TypeHelpers::GetUIElementData(TypeClass* o)
{
	StaticSymbols::InitializeSymbolInfo();
	return static_cast<UIElementData*>(TypeHelpers::FindMetaRecursive(o, (TypeClass const*)Reflection::GetType(StaticSymbol<UIElementData>())));
}

Array<DependencyProperty*> TypeHelpers::GetDependencyProperties(TypeClass* o)
{
	auto deps = GetDependencyData(o);
	Array<DependencyProperty*> props;

	if (deps != nullptr) {
		for (auto const& dep : deps->mProperties) {
			props.push_back(const_cast<DependencyProperty*>(dep.GetPtr()));
		}
	}

	return props;
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

END_BARE_NS()

BEGIN_NS(lua)

#define FOR_NOESIS_TYPE(T) if (typeName == Noesis::StaticSymbol<T>()) { \
	MakeDirectObjectRef(L, static_cast<T*>(obj), lifetime); return; \
}

void NoesisPush(lua_State* L, Noesis::BaseObject* obj, LifetimeHandle const& lifetime)
{
	Noesis::StaticSymbols::InitializeSymbolInfo();

	auto cls = obj->GetClassType();

	do {
		auto typeName = cls->GetTypeId();
		FOR_EACH_NOESIS_TYPE()
		cls = cls->GetBase();
	} while (cls != nullptr);

	MakeDirectObjectRef(L, obj, lifetime);
}

#undef FOR_NOESIS_TYPE

#define FOR_NOESIS_TYPE(T) void LuaPolymorphic<T>::MakeRef(lua_State* L, T* value, LifetimeHandle const& lifetime) { \
	NoesisPush(L, value, lifetime); \
}

FOR_EACH_NOESIS_TYPE()
#undef FOR_NOESIS_TYPE


END_NS()

/// <lua_module>UI</lua_module>
BEGIN_NS(ecl::lua::ui)

Noesis::FrameworkElement* GetRoot()
{
	return (*GetStaticSymbols().ls__gGlobalResourceManager)->UIManager->field_70.MainCanvasGrid;
}

void RegisterUILib()
{
	DECLARE_MODULE(UI, Client)
	BEGIN_MODULE()
	MODULE_FUNCTION(GetRoot)
	END_MODULE()
}

END_NS()
