#pragma once

#include <CoreLib/Base/Base.h>

#include <NsCore/BaseComponent.h>
#include <NsCore/TypeClass.h>
#include <NsGui/DependencyData.h>
#include <NsGui/DependencyObjectValueData.h>
#include <NsGui/RoutedEvent.h>
#include <NsGui/UIElement.h>
#include <NsGui/UIElementData.h>
#include <NsGui/FrameworkElement.h>
#include <NsGui/Panel.h>

BEGIN_BARE_NS(Noesis)

using namespace bg3se;

using LoadXamlProc = Ptr<BaseComponent>* (Ptr<BaseComponent>& ret, char const* path);

struct SymbolManagerInternals
{
	Vector<char const*> Strings;
	HashMap<char const*, uint32_t> Keys;
};

struct ReflectionInternals
{
	Vector<Type*> Types;
	HashMap<uint32_t, Type*> Names;
};

inline void* Alloc(SizeT size)
{
	return GameAllocRaw(size);
}

inline void* Realloc(void* ptr, SizeT size)
{
	if (ptr != nullptr) {
		auto newMem = Alloc(size);
		memcpy(newMem, ptr, size);
		Dealloc(ptr);
		return newMem;
	} else {
		return Alloc(size);
	}
}

inline void Dealloc(void* ptr)
{
	return GameFree(ptr);
}

inline char const* Type::GetName() const
{
	return mName.Str();
}

inline Symbol Type::GetTypeId() const
{
	return mName;
}

inline ReflectionInternals* GetReflection()
{
	return (ReflectionInternals* )GetStaticSymbols().Noesis__gReflection;
}

inline SymbolManagerInternals* GetSymbolManager()
{
	return (SymbolManagerInternals * )((uintptr_t)GetStaticSymbols().Noesis__SymbolManager__Buf1 - 8);
}

inline char const* SymbolManager::GetString(uint32_t index)
{
	return GetSymbolManager()->Strings[index];
}

inline const Type* Reflection::GetType(Symbol name)
{
	auto it = GetReflection()->Names.Find((uint32_t)name);
	if (it != GetReflection()->Names.End()) {
		return it->value;
	} else {
		return nullptr;
	}
}

inline TypeClass const* TypeClass::GetBase() const
{
	return mBase;
}

inline bool TypeClass::IsInterface() const
{
	return mIsInterface;
}

struct ObjectHelpers
{
	static PropertyOperationResult FallbackGetProperty(lua_State* L, lua::LifetimeHandle const& lifetime, BaseObject* object, bg3se::FixedString const& prop);
	static PropertyOperationResult FallbackSetProperty(lua_State* L, BaseObject* object, bg3se::FixedString const& prop, int index);
	static UserReturn GetNamedProperty(lua_State* L, BaseObject* o, Symbol name);
	static UserReturn GetProperty(lua_State* L, BaseObject* o, TypeProperty const* prop);
	static void SetNamedProperty(lua_State* L, BaseObject* o, Symbol name, lua::AnyRef value);
	static void SetProperty(lua_State* L, BaseObject* o, TypeProperty const* prop, lua::AnyRef value);
	static UserReturn GetAllProperties(lua_State* L, BaseObject* o);

	static TypeClass* GetClassType(BaseObject* o);
	static Symbol GetClassTypeName(BaseObject* o);
	static STDString ToString(BaseObject* o);
	static uint32_t GetNumReferences(BaseRefCounted* o);
};

struct RoutedEventHelpers
{
	static Symbol GetName(RoutedEvent* o);
	static TypeClass* GetOwnerType(RoutedEvent* o);
};

struct DependencyObjectHelpers
{
	static UserReturn GetProperty(lua_State* L, DependencyObject* o, Symbol name);
	static UserReturn GetDependencyProperty(lua_State* L, DependencyObject* o, DependencyProperty const* prop);
	static void SetProperty(lua_State* L, DependencyObject* o, Symbol name, lua::AnyRef value);
	static void SetDependencyProperty(lua_State* L, DependencyObject* o, DependencyProperty const* prop, lua::AnyRef value);
};

struct DependencyPropertyHelpers
{
	static Symbol GetName(DependencyProperty* o);
	static Type* GetType(DependencyProperty* o);
	static TypeClass* GetOwnerType(DependencyProperty* o);
	static bool IsReadOnly(DependencyProperty* o);
};

struct StoredValueHolder
{
	inline StoredValueHolder() {}
	inline StoredValueHolder(void* val) : Value(val), IsIntegral(true) {}
	inline StoredValueHolder(BaseObject* val) : Value(val), IsIntegral(false) {}
	inline StoredValueHolder(String* val) : Value(val), IsIntegral(false) {}


	void* Value{ nullptr };
	bool IsIntegral{ true };
};

struct StoredValueHelpers
{
	static std::optional<int64_t> TryParseIntegralValue(lua_State* L, Type const* type, void* val);
	static void PushValue(lua_State* L, Type const* type, StoredValue const* o, Type const* objectType = nullptr, Symbol* propertyName = nullptr);
	static void PushValue(lua_State* L, Type const* type, void* val, Type const* objectType = nullptr, Symbol* propertyName = nullptr);
	static void PushProperty(lua_State* L, BaseObject* obj, TypeClass const* objType, TypeProperty const* prop);
	static void PushValue(lua_State* L, TypeEnum const* type, uint64_t val);

	template <class T>
	static void PushRawValue(lua_State* L, Type const* type, void* val);
	template <class T>
	static void PushPtrValue(lua_State* L, Type const* type, void* val);

	template <class T>
	static void* GetRawValue(lua_State* L, Type const* type, lua::AnyRef value);

	static std::optional<StoredValueHolder> GetValue(lua_State* L, Type const* type, lua::AnyRef value);
};

struct VisualHelpers
{
	static Visual* GetVisualParent(Visual* o);
	static uint32_t GetVisualChildrenCount(Visual* o);
	static Visual* GetVisualChild(Visual* o, uint32_t index);
};

struct UIElementDataHelpers
{
	static RoutedEvent* GetEvent(UIElementData* o, Symbol evt);
	static Array<RoutedEvent*> GetAllEvents(UIElementData* o);
};

struct UIElementHelpers
{
	static uint64_t Subscribe(lua_State* L, UIElement* o, Symbol evt, lua::FunctionRef func);
	static bool Unsubscribe(lua_State* L, UIElement* o, uint64_t index);
};

struct FrameworkElementHelpers
{
	static FrameworkElement* GetLogicalParent(FrameworkElement* o);
	static uint32_t GetLogicalChildrenCount(FrameworkElement* o);
	static BaseComponent* GetLogicalChild(FrameworkElement* o, uint32_t index);
	static BaseComponent* FindNodeName(FrameworkElement* o, char const* name);
	static BaseComponent* GetResource(FrameworkElement* o, char const* key, std::optional<bool> fullElementSearch);
	static BaseObject* GetTreeParent(FrameworkElement* o);
	static FrameworkElement* AttachXamlChild(FrameworkElement* o, char const* path);
	static FrameworkElement* SetXamlProperty(FrameworkElement* o, char const* prop, char const* path);
};

struct TypePropertyHelpers
{
	static Symbol GetName(TypeProperty* o);
	static Type* GetContentType(TypeProperty* o);
	static Vector<TypeMetaData*>* GetMeta(TypeProperty* o);
	static bool IsReadOnly(TypeProperty* o);
};

struct TypeHelpers
{
	static STDString GetName(Type* o);
	static bool IsDescendantOf(Type const* type, TypeClass const* base);
	static bool IsDescendantOf(TypeClass const* type, TypeClass const* base);
	static TypeClass* GetBase(TypeClass* o);
	static bool IsInterface(TypeClass* o);
	static TypeClass::AncestorVector* GetInterfaces(TypeClass* o);
	static TypeClass::PropertyVector* GetProperties(TypeClass* o);
	static TypeClass::PropertyVector* GetEvents(TypeClass* o);
	static TypeProperty const* GetProperty(TypeClass const* o, Symbol name);
	static TypeMetaData* FindMetaRecursive(TypeClass const* o, const TypeClass* metaDataType);
	static TypeMetaData* FindMetaOrDescendant(TypeClass const* o, const TypeClass* metaDataType);
	static DependencyData* GetDependencyData(TypeClass* o);
	static DependencyProperty const* GetDependencyProperty(TypeClass const* o, Symbol name);
	static RoutedEvent const* GetRoutedEvent(TypeClass const* o, Symbol name);
	static UIElementData* GetUIElementData(TypeClass* o);
	static Array<DependencyProperty*> GetDependencyProperties(TypeClass* o);
	static Array<RoutedEvent*> GetRoutedEvents(TypeClass* o);

	static std::optional<uint64_t> StringToEnum(TypeEnum const* e, char const* value);
	static std::optional<uint64_t> StringToEnum(TypeEnum const* e, Symbol value);

	template <class Fun>
	static void ForEachMeta(TypeClass const* cls, const TypeClass* metaDataType, Fun fun);
};

END_BARE_NS()

BEGIN_NS(lua)

#define FOR_EACH_NOESIS_TYPE() \
	FOR_NOESIS_TYPE(Noesis::BaseObject) \
	FOR_NOESIS_TYPE(Noesis::BaseRefCounted) \
	FOR_NOESIS_TYPE(Noesis::BaseComponent) \
	FOR_NOESIS_TYPE(Noesis::Type) \
	FOR_NOESIS_TYPE(Noesis::TypeMeta) \
	FOR_NOESIS_TYPE(Noesis::TypeMetaData) \
	FOR_NOESIS_TYPE(Noesis::TypeClass) \
	FOR_NOESIS_TYPE(Noesis::DispatcherObject) \
	FOR_NOESIS_TYPE(Noesis::DependencyObject) \
	FOR_NOESIS_TYPE(Noesis::DependencyData) \
	FOR_NOESIS_TYPE(Noesis::Visual) \
	FOR_NOESIS_TYPE(Noesis::UIElement) \
	FOR_NOESIS_TYPE(Noesis::UIElementData) \
	FOR_NOESIS_TYPE(Noesis::FrameworkElement) \
	FOR_NOESIS_TYPE(Noesis::Panel)

#define FOR_NOESIS_TYPE(c) LUA_POLYMORPHIC(c)
FOR_EACH_NOESIS_TYPE()
#undef FOR_NOESIS_TYPE

LUA_POLYMORPHIC(Noesis::RoutedEventArgs)

END_NS()


BEGIN_NS(ui)

struct UICanvas;

struct UIStateMachine : public ProtectedGameObject<UIStateMachine>
{
	void* VMT;
	__int64 field_8;
	ecs::ComponentCallbackList field_10;
	ecs::ComponentCallbackList field_28;
	ecs::ComponentCallbackList field_40;
	ecs::ComponentCallbackList field_58;
	ecs::ComponentCallbackList field_70;
	int field_88;
	bool CanProcessEvents;
	bool IsProcessingEvent;
	char field_8E;
	char field_8F;
	void* SomeEventArg;
	__int64 field_98;
	__int64 field_A0;
	MultiHashMap<uint64_t, uint64_t> field_A8;
	MultiHashMap<uint64_t, uint64_t> field_E8;
	MultiHashMap<uint64_t, uint64_t> field_128;
	MultiHashSet<uint64_t> field_168;
	MultiHashSet<uint64_t> field_198;
	MultiHashSet<uint64_t> field_1C8;
	__int64 field_1F8;
	__int64 field_200;
	__int64 field_208;
	__int64 field_210;
	__int64 field_218;
	MultiHashMap<uint64_t, uint64_t> field_220;
	MultiHashSet<uint64_t> field_260;
	MultiHashSet<uint64_t> field_290;
	Array<uint64_t> field_2C0;
	__int64 field_2D0;
	MultiHashMap<uint64_t, uint64_t> field_2D8;
	MultiHashMap<uint64_t, uint64_t> field_318;
	MultiHashSet<uint64_t> field_358;
	__int64 field_388;
	__int64 field_390;
	__int64 field_398;
	__int64 field_3A0;
	__int64 field_3A8;
	__int64 field_3B0;
	__int64 field_3B8;
	__int64 field_3C0;
	Array<uint64_t> field_3C8;
	__int64 field_3D8;
	CRITICAL_SECTION field_3E0;

	struct EventArgs
	{
		int field_0{ 0 };
		int field_4{ 3 };
		FixedString StateEvent;
		FixedString SubState;
		bool RemoveState{ false };
		int16_t PlayerId{ 0 };
		uint64_t field_18{ 0 };
	};

	struct EventResult
	{
		__int64 field_0;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		__int64 field_20;
		int field_28;
		int field_2C;
		uint16_t field_30;
		int field_34;
		BYTE field_38;
		char field_39;
	};

	struct ECSData
	{
		ecs::EntityWorld* EntityWorld{ nullptr };
		ecs::EntityWorld* EntityWorld2{ nullptr };
		ecs::EntityStore* EntityTypes{ nullptr };
		ecs::QueryManager* QuerySystem{ nullptr };
		CRITICAL_SECTION* CriticalSection{ nullptr };
	};

	struct EntityContext
	{
		ECSData const* ECS{ nullptr };
		int WorldView{ 0 };
		uint8_t field_C{ 0 };
	};

};


struct UIManager : public ProtectedGameObject<UIManager>
{
	struct Sub70
	{
		__int64 field_0;
		__int64 field_8;
		__int64 field_10;
		__int64 field_18;
		__int64 field_20;
		__int64 field_28;
		__int64 field_30;
		__int64 field_38;
		__int64 field_40;
		__int64 field_48;
		__int64 field_50;
		__int64 ElementCacheManager;
		UICanvas* MainCanvasGrid;
		Noesis::DispatcherObject* MainCanvasView;
		__int64 field_70;
		__int64 field_78;
		int field_80;
		int field_84;
		int field_88;
		int field_8C;
		float field_90;
		int field_94;
		double field_98;
		__int64 field_A0;
		char field_A8;
	};

	struct Sub120
	{
		__int64 field_0;
		ecs::ComponentCallbackList field_8;
		ecs::ComponentCallbackList field_20;
	};

	struct Sub168
	{
		Array<void*> field_0;
		int field_10;
		int field_14;
	};

	struct Sub198
	{
		ecs::ComponentCallbackList field_0;
		MultiHashSet<uint64_t> field_18;
		Array<void*> field_48;
	};

	struct Sub1F0
	{
		ecs::ComponentCallbackList field_0;
		ecs::ComponentCallbackList field_18;
		ecs::ComponentCallbackList field_30;
		ecs::ComponentCallbackList field_48;
		ecs::ComponentCallbackList field_60;
		__int64 field_78;
		__int64 field_80;
		Array<void*> field_88;
		Array<void*> field_98;
		__int64 field_A8;
		__int64 field_B0;
		char field_B8;
		char field_B9;
		char field_BA;
		char field_BB;
		char field_BC;
		char field_BD;
		char field_BE;
		char field_BF;
		__int64 field_C0;
		__int64 field_C8;
		CRITICAL_SECTION CriticalSection;
		__int64 field_F8;
		__int64 field_100;
		__int64 field_108;
		__int64 field_110;
		__int64 field_118;
		__int64 field_120;
		__int64 field_128;
		__int64 field_130;
		void* field_138;
		void* field_140;
		void* field_148;
		void* field_150;
		Array<void*> field_158;
	};

	struct Sub358
	{
		__int64 field_0;
		__int64 field_8;
		UIStateMachine* StateMachine;
	};

	__int64 VMT;
	__int64 VMT3;
	__int64 VMT2;
	__int64 field_18;
	__int64 VMT4;
	__int64 field_28;
	__int64 VMT5;
	__int64 field_38;
	__int64 field_40;
	__int64 field_48;
	__int64 field_50;
	__int64 field_58;
	int field_60;
	int field_64;
	int field_68;
	uint8_t field_6C;
	char field_6D;
	char field_6E;
	char field_6F;
	Sub70 field_70;
	Sub120 field_120;
	__int64 field_158;
	__int64 field_160;
	Sub168 field_168;
	Array<void*> field_180;
	__int64 field_190;
	Sub198 field_198;
	Sub1F0 field_1F0;
	Sub358 field_358;
};

struct UICanvas : public Noesis::Panel
{
	__int64 field_2A0;
	__int64 field_2A8;
	__int64 field_2B0;
	__int64 field_2B8;
	__int64 field_2C0;
	__int64 field_2C8;
	__int64 field_2D0;
	UIManager::Sub70* UIManager70;
	__int64 ElementCacheManager;
	Noesis::DependencyObject* field_2E8;
	float field_2F0;
	float field_2F4;
};

END_NS()
