#pragma once

#include <CoreLib/Base/Base.h>

#define NS_STATIC_LIBRARY

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
	static PropertyOperationResult FallbackGetProperty(lua_State* L, lua::LifetimeHandle const& lifetime, void* object, bg3se::FixedString const& prop);
	static PropertyOperationResult FallbackSetProperty(lua_State* L, void* object, bg3se::FixedString const& prop, int index);

	static UserReturn GetProperty(lua_State* L, DependencyObject* o, Symbol name);
	static UserReturn GetAllProperties(lua_State* L, DependencyObject* o);
};

struct DependencyPropertyHelpers
{
	static Symbol GetName(DependencyProperty* o);
	static Type* GetType(DependencyProperty* o);
	static TypeClass* GetOwnerType(DependencyProperty* o);
	static bool IsReadOnly(DependencyProperty* o);
};

struct StoredValueHelpers
{
	static void PushValue(lua_State* L, Type const* type, StoredValue const* o);
	static void PushValue(lua_State* L, Type const* type, void* val);
	template <class T>
	static void PushRawValue(lua_State* L, Type const* type, void* val);
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

struct FrameworkElementHelpers
{
	static FrameworkElement* GetLogicalParent(FrameworkElement* o);
	static uint32_t GetLogicalChildrenCount(FrameworkElement* o);
	static BaseComponent* GetLogicalChild(FrameworkElement* o, uint32_t index);
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
	static bool IsDescendantOf(Type const* type, TypeClass const* cls);
	static TypeClass* GetBase(TypeClass* o);
	static bool IsInterface(TypeClass* o);
	static TypeClass::AncestorVector* GetInterfaces(TypeClass* o);
	static TypeClass::PropertyVector* GetProperties(TypeClass* o);
	static TypeClass::PropertyVector* GetEvents(TypeClass* o);
	static TypeMetaData* FindMetaRecursive(TypeClass const* o, const TypeClass* metaDataType);
	static DependencyData* GetDependencyData(TypeClass* o);
	static DependencyProperty const* GetDependencyProperty(TypeClass const* o, Symbol name);
	static UIElementData* GetUIElementData(TypeClass* o);
	static Array<DependencyProperty*> GetDependencyProperties(TypeClass* o);
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

END_NS()


BEGIN_NS(ui)

struct UICanvas;

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
