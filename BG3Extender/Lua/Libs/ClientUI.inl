#include <GameDefinitions/UI.h>
#include <NsGui/UIElementEvents.h>

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

struct TypeClassInfo
{
	Symbol Name;
	TypeClass const* Type;

	void Setup(char const* name)
	{
		Name = MakeNoesisSymbol(gSymbolStrings, name);
		auto type = Reflection::GetType(Name);
		assert(type->GetClassType()->GetTypeId() == SymbolInfo<TypeClass>::Name);
		Type = static_cast<TypeClass const*>(type);
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
	}
};

struct BuiltinTypeClasses
{
	TypeClassInfo BaseObject;
	TypeClassInfo BaseRefCounted;
	TypeClassInfo BaseComponent;
	TypeClassInfo TypeClass;
	TypeClassInfo TypeEnum;
	TypeClassInfo BoxedValue;
	TypeClassInfo UIElementData;
	TypeClassInfo DependencyData;

	void Setup()
	{
		BaseObject.Setup("BaseObject");
		BaseRefCounted.Setup("BaseRefCounted");
		BaseComponent.Setup("BaseComponent");
		TypeClass.Setup("TypeClass");
		TypeEnum.Setup("TypeEnum");
		BoxedValue.Setup("BoxedValue");
		UIElementData.Setup("UIElementData");
		DependencyData.Setup("DependencyData");
	}
};

struct BuiltinNames
{
	Symbol GotFocus;
	Symbol GotKeyboardFocus;
	Symbol GotMouseCapture;
	Symbol KeyDown;
	Symbol KeyUp;
	Symbol LostFocus;
	Symbol LostKeyboardFocus;
	Symbol LostMouseCapture;
	Symbol MouseDown;
	Symbol MouseEnter;
	Symbol MouseLeave;
	Symbol MouseLeftButtonDown;
	Symbol MouseLeftButtonUp;
	Symbol MouseMove;
	Symbol MouseRightButtonDown;
	Symbol MouseRightButtonUp;
	Symbol MouseUp;
	Symbol MouseWheel;
	Symbol TouchDown;
	Symbol TouchMove;
	Symbol TouchUp;
	Symbol TouchEnter;
	Symbol TouchLeave;
	Symbol GotTouchCapture;
	Symbol LostTouchCapture;
	Symbol PreviewTouchDown;
	Symbol PreviewTouchMove;
	Symbol PreviewTouchUp;
	Symbol ManipulationStarting;
	Symbol ManipulationStarted;
	Symbol ManipulationDelta;
	Symbol ManipulationInertiaStarting;
	Symbol ManipulationCompleted;
	Symbol Tapped;
	Symbol DoubleTapped;
	Symbol Holding;
	Symbol RightTapped;
	Symbol PreviewGotKeyboardFocus;
	Symbol PreviewKeyDown;
	Symbol PreviewKeyUp;
	Symbol PreviewLostKeyboardFocus;
	Symbol PreviewMouseDown;
	Symbol PreviewMouseLeftButtonDown;
	Symbol PreviewMouseLeftButtonUp;
	Symbol PreviewMouseMove;
	Symbol PreviewMouseRightButtonDown;
	Symbol PreviewMouseRightButtonUp;
	Symbol PreviewMouseUp;
	Symbol PreviewMouseWheel;
	Symbol PreviewTextInput;
	Symbol QueryCursor;
	Symbol TextInput;
	Symbol PreviewQueryContinueDrag;
	Symbol QueryContinueDrag;
	Symbol PreviewGiveFeedback;
	Symbol GiveFeedback;
	Symbol PreviewDragEnter;
	Symbol DragEnter;
	Symbol PreviewDragOver;
	Symbol DragOver;
	Symbol PreviewDragLeave;
	Symbol DragLeave;
	Symbol PreviewDrop;
	Symbol Drop;

	void Setup()
	{
		GotFocus = Symbol("GotFocus", Symbol::NullIfNotFound{});
		GotKeyboardFocus = Symbol("GotKeyboardFocus", Symbol::NullIfNotFound{});
		GotMouseCapture = Symbol("GotMouseCapture", Symbol::NullIfNotFound{});
		KeyDown = Symbol("KeyDown", Symbol::NullIfNotFound{});
		KeyUp = Symbol("KeyUp", Symbol::NullIfNotFound{});
		LostFocus = Symbol("LostFocus", Symbol::NullIfNotFound{});
		LostKeyboardFocus = Symbol("LostKeyboardFocus", Symbol::NullIfNotFound{});
		LostMouseCapture = Symbol("LostMouseCapture", Symbol::NullIfNotFound{});
		MouseDown = Symbol("MouseDown", Symbol::NullIfNotFound{});
		MouseEnter = Symbol("MouseEnter", Symbol::NullIfNotFound{});
		MouseLeave = Symbol("MouseLeave", Symbol::NullIfNotFound{});
		MouseLeftButtonDown = Symbol("MouseLeftButtonDown", Symbol::NullIfNotFound{});
		MouseLeftButtonUp = Symbol("MouseLeftButtonUp", Symbol::NullIfNotFound{});
		MouseMove = Symbol("MouseMove", Symbol::NullIfNotFound{});
		MouseRightButtonDown = Symbol("MouseRightButtonDown", Symbol::NullIfNotFound{});
		MouseRightButtonUp = Symbol("MouseRightButtonUp", Symbol::NullIfNotFound{});
		MouseUp = Symbol("MouseUp", Symbol::NullIfNotFound{});
		MouseWheel = Symbol("MouseWheel", Symbol::NullIfNotFound{});
		TouchDown = Symbol("TouchDown", Symbol::NullIfNotFound{});
		TouchMove = Symbol("TouchMove", Symbol::NullIfNotFound{});
		TouchUp = Symbol("TouchUp", Symbol::NullIfNotFound{});
		TouchEnter = Symbol("TouchEnter", Symbol::NullIfNotFound{});
		TouchLeave = Symbol("TouchLeave", Symbol::NullIfNotFound{});
		GotTouchCapture = Symbol("GotTouchCapture", Symbol::NullIfNotFound{});
		LostTouchCapture = Symbol("LostTouchCapture", Symbol::NullIfNotFound{});
		PreviewTouchDown = Symbol("PreviewTouchDown", Symbol::NullIfNotFound{});
		PreviewTouchMove = Symbol("PreviewTouchMove", Symbol::NullIfNotFound{});
		PreviewTouchUp = Symbol("PreviewTouchUp", Symbol::NullIfNotFound{});
		ManipulationStarting = Symbol("ManipulationStarting", Symbol::NullIfNotFound{});
		ManipulationStarted = Symbol("ManipulationStarted", Symbol::NullIfNotFound{});
		ManipulationDelta = Symbol("ManipulationDelta", Symbol::NullIfNotFound{});
		ManipulationInertiaStarting = Symbol("ManipulationInertiaStarting", Symbol::NullIfNotFound{});
		ManipulationCompleted = Symbol("ManipulationCompleted", Symbol::NullIfNotFound{});
		Tapped = Symbol("Tapped", Symbol::NullIfNotFound{});
		DoubleTapped = Symbol("DoubleTapped", Symbol::NullIfNotFound{});
		Holding = Symbol("Holding", Symbol::NullIfNotFound{});
		RightTapped = Symbol("RightTapped", Symbol::NullIfNotFound{});
		PreviewGotKeyboardFocus = Symbol("PreviewGotKeyboardFocus", Symbol::NullIfNotFound{});
		PreviewKeyDown = Symbol("PreviewKeyDown", Symbol::NullIfNotFound{});
		PreviewKeyUp = Symbol("PreviewKeyUp", Symbol::NullIfNotFound{});
		PreviewLostKeyboardFocus = Symbol("PreviewLostKeyboardFocus", Symbol::NullIfNotFound{});
		PreviewMouseDown = Symbol("PreviewMouseDown", Symbol::NullIfNotFound{});
		PreviewMouseLeftButtonDown = Symbol("PreviewMouseLeftButtonDown", Symbol::NullIfNotFound{});
		PreviewMouseLeftButtonUp = Symbol("PreviewMouseLeftButtonUp", Symbol::NullIfNotFound{});
		PreviewMouseMove = Symbol("PreviewMouseMove", Symbol::NullIfNotFound{});
		PreviewMouseRightButtonDown = Symbol("PreviewMouseRightButtonDown", Symbol::NullIfNotFound{});
		PreviewMouseRightButtonUp = Symbol("PreviewMouseRightButtonUp", Symbol::NullIfNotFound{});
		PreviewMouseUp = Symbol("PreviewMouseUp", Symbol::NullIfNotFound{});
		PreviewMouseWheel = Symbol("PreviewMouseWheel", Symbol::NullIfNotFound{});
		PreviewTextInput = Symbol("PreviewTextInput", Symbol::NullIfNotFound{});
		QueryCursor = Symbol("QueryCursor", Symbol::NullIfNotFound{});
		TextInput = Symbol("TextInput", Symbol::NullIfNotFound{});
		PreviewQueryContinueDrag = Symbol("PreviewQueryContinueDrag", Symbol::NullIfNotFound{});
		QueryContinueDrag = Symbol("QueryContinueDrag", Symbol::NullIfNotFound{});
		PreviewGiveFeedback = Symbol("PreviewGiveFeedback", Symbol::NullIfNotFound{});
		GiveFeedback = Symbol("GiveFeedback", Symbol::NullIfNotFound{});
		PreviewDragEnter = Symbol("PreviewDragEnter", Symbol::NullIfNotFound{});
		DragEnter = Symbol("DragEnter", Symbol::NullIfNotFound{});
		PreviewDragOver = Symbol("PreviewDragOver", Symbol::NullIfNotFound{});
		DragOver = Symbol("DragOver", Symbol::NullIfNotFound{});
		PreviewDragLeave = Symbol("PreviewDragLeave", Symbol::NullIfNotFound{});
		DragLeave = Symbol("DragLeave", Symbol::NullIfNotFound{});
		PreviewDrop = Symbol("PreviewDrop", Symbol::NullIfNotFound{});
		Drop = Symbol("Drop", Symbol::NullIfNotFound{});
	}
};

BuiltinTypes gBuiltinTypes;
BuiltinTypeClasses gBuiltinTypeClasses;
BuiltinNames gBuiltinNames;

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
	gBuiltinTypeClasses.Setup();
	gBuiltinNames.Setup();

	SymbolsInitialized = true;
}

END_BARE_NS()

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
	return StaticSymbols::gBuiltinTypeClasses.BaseObject.Type;
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
	return StaticSymbols::gBuiltinTypeClasses.BaseComponent.Type;
}

const TypeClass* BaseRefCounted::GetClassType() const
{
	return StaticSymbols::gBuiltinTypeClasses.BaseRefCounted.Type;
}

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
	auto& types = StaticSymbols::gBuiltinTypes;

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
	auto& classes = StaticSymbols::gBuiltinTypeClasses;
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
	auto& classes = StaticSymbols::gBuiltinTypeClasses;
	auto& types = StaticSymbols::gBuiltinTypes;

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

void DummyEventHandler(BaseComponent*, const EventArgs&)
{
	OsiWarnS("Test event handler fired!");
}

void DummyRoutedEventHandler(BaseComponent*, const RoutedEventArgs&)
{
	OsiWarnS("Test event handler fired!");
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
	auto& types = StaticSymbols::gBuiltinTypeClasses;

	if (type->GetClassType() == types.TypeClass.Type) {
		auto base = static_cast<TypeClass const*>(type);
		return IsDescendantOf(base, cls);
	}

	return false;
}

bool TypeHelpers::IsDescendantOf(TypeClass const* type, TypeClass const* cls)
{
	do {
		if (type == cls) {
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

RoutedEvent const* TypeHelpers::GetRoutedEvent(TypeClass const* o, Symbol name)
{
	StaticSymbols::InitializeSymbolInfo();
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
	StaticSymbols::InitializeSymbolInfo();
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

	ForEachMeta(o, StaticSymbols::gBuiltinTypeClasses.DependencyData.Type, [&](TypeMetaData const* meta) {
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

	ForEachMeta(o, StaticSymbols::gBuiltinTypeClasses.DependencyData.Type, [&](TypeMetaData const* meta) {
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

void LuaPolymorphic<Noesis::RoutedEventArgs>::MakeRef(lua_State* L, Noesis::RoutedEventArgs* value, LifetimeHandle const& lifetime)
{
	auto evtName = value->routedEvent->GetName();
	auto const& names = Noesis::StaticSymbols::gBuiltinNames;

	if (evtName == names.GotFocus) MakeDirectObjectRef(L, static_cast<Noesis::RoutedEventArgs*>(value), lifetime);
	else if (evtName == names.GotKeyboardFocus) MakeDirectObjectRef(L, static_cast<Noesis::KeyboardFocusChangedEventArgs*>(value), lifetime);
	else if (evtName == names.GotMouseCapture) MakeDirectObjectRef(L, static_cast<Noesis::MouseEventArgs*>(value), lifetime);
	else if (evtName == names.KeyDown) MakeDirectObjectRef(L, static_cast<Noesis::KeyEventArgs*>(value), lifetime);
	else if (evtName == names.KeyUp) MakeDirectObjectRef(L, static_cast<Noesis::KeyEventArgs*>(value), lifetime);
	else if (evtName == names.LostFocus) MakeDirectObjectRef(L, static_cast<Noesis::RoutedEventArgs*>(value), lifetime);
	else if (evtName == names.LostKeyboardFocus) MakeDirectObjectRef(L, static_cast<Noesis::KeyboardFocusChangedEventArgs*>(value), lifetime);
	else if (evtName == names.LostMouseCapture) MakeDirectObjectRef(L, static_cast<Noesis::MouseEventArgs*>(value), lifetime);
	else if (evtName == names.MouseDown) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.MouseEnter) MakeDirectObjectRef(L, static_cast<Noesis::MouseEventArgs*>(value), lifetime);
	else if (evtName == names.MouseLeave) MakeDirectObjectRef(L, static_cast<Noesis::MouseEventArgs*>(value), lifetime);
	else if (evtName == names.MouseLeftButtonDown) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.MouseLeftButtonUp) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.MouseMove) MakeDirectObjectRef(L, static_cast<Noesis::MouseEventArgs*>(value), lifetime);
	else if (evtName == names.MouseRightButtonDown) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.MouseRightButtonUp) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.MouseUp) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.MouseWheel) MakeDirectObjectRef(L, static_cast<Noesis::MouseWheelEventArgs*>(value), lifetime);
	else if (evtName == names.TouchDown) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.TouchMove) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.TouchUp) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.TouchEnter) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.TouchLeave) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.GotTouchCapture) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.LostTouchCapture) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewTouchDown) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewTouchMove) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewTouchUp) MakeDirectObjectRef(L, static_cast<Noesis::TouchEventArgs*>(value), lifetime);
	else if (evtName == names.ManipulationStarting) MakeDirectObjectRef(L, static_cast<Noesis::ManipulationStartingEventArgs*>(value), lifetime);
	else if (evtName == names.ManipulationStarted) MakeDirectObjectRef(L, static_cast<Noesis::ManipulationStartedEventArgs*>(value), lifetime);
	else if (evtName == names.ManipulationDelta) MakeDirectObjectRef(L, static_cast<Noesis::ManipulationDeltaEventArgs*>(value), lifetime);
	else if (evtName == names.ManipulationInertiaStarting) MakeDirectObjectRef(L, static_cast<Noesis::ManipulationInertiaStartingEventArgs*>(value), lifetime);
	else if (evtName == names.ManipulationCompleted) MakeDirectObjectRef(L, static_cast<Noesis::ManipulationCompletedEventArgs*>(value), lifetime);
	else if (evtName == names.Tapped) MakeDirectObjectRef(L, static_cast<Noesis::TappedEventArgs*>(value), lifetime);
	else if (evtName == names.DoubleTapped) MakeDirectObjectRef(L, static_cast<Noesis::DoubleTappedEventArgs*>(value), lifetime);
	else if (evtName == names.Holding) MakeDirectObjectRef(L, static_cast<Noesis::HoldingEventArgs*>(value), lifetime);
	else if (evtName == names.RightTapped) MakeDirectObjectRef(L, static_cast<Noesis::RightTappedEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewGotKeyboardFocus) MakeDirectObjectRef(L, static_cast<Noesis::KeyboardFocusChangedEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewKeyDown) MakeDirectObjectRef(L, static_cast<Noesis::KeyEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewKeyUp) MakeDirectObjectRef(L, static_cast<Noesis::KeyEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewLostKeyboardFocus) MakeDirectObjectRef(L, static_cast<Noesis::KeyboardFocusChangedEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseDown) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseLeftButtonDown) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseLeftButtonUp) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseMove) MakeDirectObjectRef(L, static_cast<Noesis::MouseEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseRightButtonDown) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseRightButtonUp) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseUp) MakeDirectObjectRef(L, static_cast<Noesis::MouseButtonEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewMouseWheel) MakeDirectObjectRef(L, static_cast<Noesis::MouseWheelEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewTextInput) MakeDirectObjectRef(L, static_cast<Noesis::TextCompositionEventArgs*>(value), lifetime);
	else if (evtName == names.QueryCursor) MakeDirectObjectRef(L, static_cast<Noesis::QueryCursorEventArgs*>(value), lifetime);
	else if (evtName == names.TextInput) MakeDirectObjectRef(L, static_cast<Noesis::TextCompositionEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewQueryContinueDrag) MakeDirectObjectRef(L, static_cast<Noesis::QueryContinueDragEventArgs*>(value), lifetime);
	else if (evtName == names.QueryContinueDrag) MakeDirectObjectRef(L, static_cast<Noesis::QueryContinueDragEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewGiveFeedback) MakeDirectObjectRef(L, static_cast<Noesis::GiveFeedbackEventArgs*>(value), lifetime);
	else if (evtName == names.GiveFeedback) MakeDirectObjectRef(L, static_cast<Noesis::GiveFeedbackEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewDragEnter) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else if (evtName == names.DragEnter) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewDragOver) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else if (evtName == names.DragOver) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewDragLeave) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else if (evtName == names.DragLeave) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else if (evtName == names.PreviewDrop) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else if (evtName == names.Drop) MakeDirectObjectRef(L, static_cast<Noesis::DragEventArgs*>(value), lifetime);
	else MakeDirectObjectRef(L, value, lifetime);
}

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
