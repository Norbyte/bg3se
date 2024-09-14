// Shared container for all property lists

#include <stdafx.h>

#include <Lua/Shared/Proxies/PropertyMapDependencies.h>
#include <Lua/Shared/Proxies/PolymorphicPush.inl>

BEGIN_NS(lua)

#if defined(_DEBUG)
#define CHECK(expr) if (!(expr)) { TryDebugBreak(); return false; }
#else
#define CHECK(expr) if (!(expr)) return false;
#endif

bool Validate(EntityHandle const& handle, ecs::EntityWorld& world)
{
	if (!handle) return true;

	CHECK(handle.GetType() < std::size(world.HandleGenerator->ThreadStates));

	// There is no observed shrink logic in per-thread paged pools, so index cannot be greater than pool size
	auto& state = world.HandleGenerator->ThreadStates[handle.GetType()];
	CHECK(handle.GetIndex() < state.Salts.Size);

	// Check that handle are within rational bounds
	CHECK(handle.GetSalt() < 0x1000);

	// Allow salt mismatches if the handle has an old salt.
	// It should never have a salt value that is newer than the current one in ECS
	auto const& salt = state.Salts[handle.GetIndex()];
	// FIXME This is weird - sometimes the index can change after an entity gets deleted?
	// CHECK(salt.Index == handle.GetIndex());
	// CHECK(salt.Salt <= handle.GetSalt());

	return true;
}

bool Validate(EntityHandle const* handle, Overload<EntityHandle>)
{
	auto lua = GetCurrentExtensionState()->GetLua();
	if (lua) {
		auto world = lua->GetEntitySystemHelpers()->GetEntityWorld();
		if (world) {
			return Validate(*handle, *world);
		}
	}

#if defined(_DEBUG)
	WARN("Validating entity handles whout an EntityWorld?");
#endif

	return true;
}

void InheritProperties(GenericPropertyMap const& base, GenericPropertyMap& child)
{
	// Check to make sure that the property map we're inheriting from is already initialized
	assert(base.Initialized);
	assert(base.InheritanceUpdated);
	assert(child.Initialized);
	assert(!child.IsInitializing);
	assert(!child.InheritanceUpdated);
	assert(child.Parent == &base);

	child.IsInitializing = true;

	for (auto prop : base.Properties) {
		auto const& p = prop.Value();
		child.AddRawProperty(prop.Key().GetString(), p.Get, p.Set, p.Serialize, p.Offset, p.Flag, 
			p.PendingNotifications, p.NewName, p.Iterable);
	}

	for (auto const& prop : base.Validators) {
		child.AddRawValidator(prop.Name.GetString(), prop.Validate, prop.Offset, prop.Flag);
	}

	for (auto const& parent : base.Parents) {
		child.Parents.push_back(parent);
	}

	for (auto const& parent : base.ParentRegistryIndices) {
		child.ParentRegistryIndices.push_back(parent);
	}

	child.Parents.push_back(base.Name);
	child.ParentRegistryIndices.push_back(base.RegistryIndex);

	if (child.FallbackGetter == nullptr) {
		child.FallbackGetter = base.FallbackGetter;
	}

	if (child.FallbackSetter == nullptr) {
		child.FallbackSetter = base.FallbackSetter;
	}

	if (child.FallbackNext == nullptr) {
		child.FallbackNext = base.FallbackNext;
	}

	child.IsInitializing = false;
	child.InheritanceUpdated = true;
}

void AddBitfieldProperty(GenericPropertyMap& pm, BitfieldTypeId typeId, std::size_t offset,
	RawPropertyAccessors::Getter* getter,
	RawPropertyAccessors::Setter* setter)
{
	auto store = BitfieldRegistry::Get().BitfieldsById[typeId];
	for (auto const& label : store->Values) {
		pm.AddRawProperty(label.Key.GetString(),
			getter,
			setter,
			nullptr,
			nullptr,
			offset,
			(uint64_t)label.Value,
			PropertyNotification::None
		);
	}
}

inline void MarkAsInherited(GenericPropertyMap const& base, GenericPropertyMap& child)
{
	assert(!child.InheritanceUpdated);
	assert(child.IsInitializing);
	assert(child.Parent == nullptr);

	child.Parent = &base;
}

enum class PropertyMapEntryType
{
	Class,
	Inheritance,
	Fallback,
	Property,
	Bitfield,
	End
};

struct PropertyMapClassEntry
{
	char const* Name;
	char const* TypeName;
	char const* ComponentName;
	StructTypeId Id;
	ExtComponentType ComponentType;
	size_t Size;
	GenericPropertyMap::TConstructor* Construct{ nullptr };
	GenericPropertyMap::TDestructor* Destroy{ nullptr };
	GenericPropertyMap::TAssigner* Assign{ nullptr };
	GenericPropertyMap::TSerializer* Serialize{ nullptr };
	GenericPropertyMap::TUnserializer* Unserialize{ nullptr };
};

struct PropertyMapInheritanceEntry
{
	StructTypeId ParentId;
};

struct PropertyMapFallbackEntry
{
	// FIXME - no typing as cast is not constexpr
	//GenericPropertyMap::TFallbackGetter* Getter{ nullptr };
	//GenericPropertyMap::TFallbackSetter* Setter{ nullptr };
	void* Getter{ nullptr };
	void* Setter{ nullptr };
	void* Next{ nullptr };
};

struct PropertyMapPropertyEntry
{
	char const* Name;
	RawPropertyAccessors::Getter* Getter{ nullptr };
	RawPropertyAccessors::Setter* Setter{ nullptr };
	GenericPropertyMap::RawPropertyValidators::Validator* Validate{ nullptr };
	RawPropertyAccessors::Serializer* Serialize{ nullptr };
	std::size_t Offset{ 0 };
	uint64_t Flag{ 0 };
	PropertyNotification Notification{ PropertyNotification::None };
	char const* NewName{ nullptr };
	bool Iterable{ true };
};

struct PropertyMapBitfieldEntry
{
	char const* Name;
	BitfieldTypeId TypeId;
	RawPropertyAccessors::Getter* Getter{ nullptr };
	RawPropertyAccessors::Setter* Setter{ nullptr };
	std::size_t Offset{ 0 };
};

struct PropertyMapRegistrationEntry
{
	PropertyMapEntryType Type;
	union {
		PropertyMapClassEntry Cls;
		PropertyMapInheritanceEntry Inherit;
		PropertyMapFallbackEntry Fallback;
		PropertyMapPropertyEntry Property;
		PropertyMapBitfieldEntry Bitfield;
	};
};


template <class T, class TBase>
inline constexpr StructTypeId CheckedGetParentStructId()
{
	static_assert(std::is_base_of_v<TBase, T>, "Can only copy properties from base class");
	// FIXME - this check is not constexpr :(
	// static_assert(static_cast<T*>(reinterpret_cast<TBase*>(nullptr)) == reinterpret_cast<T*>(nullptr), "Base and child class should start at same base ptr");

	return StructID<TBase>::ID;
}


template <class T>
inline constexpr StructTypeId CheckedGetDeclStructId()
{
	static_assert(!std::is_pointer_v<T>, "PropertyMap type should not be a pointer type!");
	static_assert(!IsByVal<T>, "PropertyMap type should not be a by-val type!");
	static_assert(!IsOptional<T>::Value, "PropertyMap type should not be an optional<T> type!");
	static_assert(!IsArrayLike<T>::Value && !IsSetLike<T>::Value && !IsMapLike<T>::Value && !IsVariantLike<T>::Value, "PropertyMap type should not be a container type!");

	return StructID<T>::ID;
}


template <class T>
inline constexpr ExtComponentType GetComponentType()
{
	if constexpr (std::is_base_of_v<bg3se::BaseComponent, T> && !std::is_same_v<T, bg3se::BaseComponent> && !std::is_same_v<T, bg3se::BaseProxyComponent>) {
		return T::ComponentType;
	} else {
		return (ExtComponentType)0xffffffff;
	}
}


template <class T>
inline constexpr char const* GetComponentName()
{
	if constexpr (std::is_base_of_v<bg3se::BaseComponent, T> && !std::is_same_v<T, bg3se::BaseComponent> && !std::is_same_v<T, bg3se::BaseProxyComponent>) {
		return T::ComponentName;
	} else {
		return nullptr;
	}
}

template <class T>
inline constexpr GenericPropertyMap::TConstructor* GetConstructor()
{
	if constexpr (std::is_default_constructible_v<T> && !std::is_base_of_v<Noesis::BaseObject, T> && !std::is_base_of_v<Noesis::Interface, T>) {
		return &(DefaultConstruct<T>);
	} else {
		return nullptr;
	}
}

template <class T>
inline constexpr GenericPropertyMap::TDestructor* GetDestructor()
{
	if constexpr (std::is_default_constructible_v<T> && !std::is_base_of_v<Noesis::BaseObject, T> && !std::is_base_of_v<Noesis::Interface, T>) {
		return &(DefaultDestroy<T>);
	} else {
		return nullptr;
	}
}

template <class T>
inline constexpr GenericPropertyMap::TAssigner* GetAssigner()
{
	if constexpr (std::is_default_constructible_v<T> && !std::is_base_of_v<Noesis::BaseObject, T> && !std::is_base_of_v<Noesis::Interface, T>
		&& std::is_assignable_v<T, T> && !std::is_base_of_v<Noncopyable<T>, T>) {
		return &(DefaultAssign<T>);
	} else {
		return nullptr;
	}
}


void ProcessPropertyMapDefinitions(PropertyMapRegistrationEntry const* defn)
{
	GenericPropertyMap* pm{ nullptr };
	TypeInformation* typeInfo{ nullptr };

	while (defn->Type != PropertyMapEntryType::End) {
		auto const& entry = *defn;
		switch (entry.Type) {
		case PropertyMapEntryType::Class:
		{
			pm = new GenericPropertyMap();

			/*auto& ty = TypeInformationRepository::GetInstance().RegisterType(FixedString(entry.Cls.TypeName));
			ty.Kind = LuaTypeId::Object;
			ty.PropertyMap = pm;
			if (entry.Cls.ComponentName != nullptr) {			
				ty.ComponentName = FixedString(entry.Cls.ComponentName);
			}*/

			pm->Init();
			pm->Name = FixedString(entry.Cls.Name);
			if (entry.Cls.ComponentType != (ExtComponentType)0xffffffff) {
				pm->ComponentType = entry.Cls.ComponentType;
			}
			pm->Construct = entry.Cls.Construct;
			pm->Destroy = entry.Cls.Destroy;
			pm->Assign = entry.Cls.Assign;
			pm->Serialize = entry.Cls.Serialize;
			pm->Unserialize = entry.Cls.Unserialize;
			//pm->TypeInfo = typeInfo;
			gStructRegistry.Register(pm, entry.Cls.Id);
			break;
		}

		case PropertyMapEntryType::Inheritance:
		{
			auto parent = gStructRegistry.Get(entry.Inherit.ParentId);
			MarkAsInherited(*parent, *pm);
			break;
		}

		case PropertyMapEntryType::Fallback:
		{
			pm->FallbackGetter = (GenericPropertyMap::TFallbackGetter*)entry.Fallback.Getter;
			pm->FallbackSetter = (GenericPropertyMap::TFallbackSetter*)entry.Fallback.Setter;
			pm->FallbackNext = (GenericPropertyMap::TFallbackNext*)entry.Fallback.Next;
			break;
		}

		case PropertyMapEntryType::Property:
		{
			auto const& p = entry.Property;
			pm->AddRawProperty(p.Name, p.Getter, p.Setter,
				p.Validate, p.Serialize, p.Offset, p.Flag,
				p.Notification, p.NewName, p.Iterable
			);
			break;
		}

		case PropertyMapEntryType::Bitfield:
		{
			auto const& bf = entry.Bitfield;
			AddBitfieldProperty(*pm, bf.TypeId,
				bf.Offset, bf.Getter, bf.Setter
			);
			break;
		}

		default:
			assert(false);
		}

		defn++;
	}

	pm->Finish();
}


bool IsResolved(PropertyMapRegistrationEntry const* defn)
{
	while (defn->Type != PropertyMapEntryType::End) {
		if (defn->Type == PropertyMapEntryType::Inheritance) {
			if (defn->Inherit.ParentId >= (int)gStructRegistry.StructsById.size()
				|| gStructRegistry.StructsById[defn->Inherit.ParentId] == nullptr) {
				return false;
			}
		}

		defn++;
	}

	return true;
}


void ProcessClassRegistrations(std::span<PropertyMapRegistrationEntry const* const> const& classDefns)
{
	Array<PropertyMapRegistrationEntry const*> pending;
	for (auto defn : classDefns) pending.push_back(defn);

	while (!pending.empty()) {
		Array<PropertyMapRegistrationEntry const*> nextGen;

		for (auto cls : pending) {
			if (!IsResolved(cls)) {
				nextGen.push_back(cls);
			} else {
				ProcessPropertyMapDefinitions(cls);
			}
		}

		pending = nextGen;
	}
}


void UpdateInheritance()
{
	auto pendingUpdates = gStructRegistry.StructsById;
	Array<GenericPropertyMap*> nextBatchUpdates;

	do {
		bool progressed{ false };
		for (auto pm : pendingUpdates) {
			if (pm == nullptr) continue;

			assert(!pm->InheritanceUpdated);
			if (pm->Parent == nullptr) {
				pm->InheritanceUpdated = true;
				progressed = true;
			} else if (pm->Parent->InheritanceUpdated) {
				InheritProperties(*pm->Parent, *pm);
				progressed = true;
			} else {
				nextBatchUpdates.push_back(pm);
			}
		}

		pendingUpdates = nextBatchUpdates;
		nextBatchUpdates.clear();
		assert(progressed && "Recursion in property map inheritance tree?");
	} while (!pendingUpdates.empty());
}


template <class T>
struct PropertyMapRegistrations;

	
#define GENERATING_PROPMAP

#define BEGIN_CLS_TN(cls, typeName, id) template <> struct PropertyMapRegistrations<cls> { \
	using ObjectType = cls; \
	static constexpr PropertyMapRegistrationEntry Definitions[] = { \
		{ .Type = PropertyMapEntryType::Class, .Cls = { \
			.Name = #cls, \
			.TypeName = #typeName, \
			.ComponentName = GetComponentName<cls>(), \
			.Id = CheckedGetDeclStructId<cls>(), \
			.ComponentType = GetComponentType<cls>(), \
			.Construct = GetConstructor<cls>(), \
			.Destroy = GetDestructor<cls>(), \
			.Assign = GetAssigner<cls>(), \
			.Serialize = &(DefaultSerialize<cls>), \
			.Unserialize = &(DefaultUnserialize<cls>) \
		} },

#define BEGIN_CLS(cls, id) BEGIN_CLS_TN(cls, cls, id)

#define END_CLS() \
			{ .Type = PropertyMapEntryType::End }, \
		}; \
	};

#define INHERIT(base) \
		{ .Type = PropertyMapEntryType::Inheritance, .Inherit = { \
			.ParentId = CheckedGetParentStructId<ObjectType, base>() \
		} },

#define PN(name, prop) \
		{ .Type = PropertyMapEntryType::Property, .Property = { \
			.Name = #name, \
			.Getter = &(GenericGetOffsetProperty<decltype(ObjectType::prop)>), \
			.Setter = &(GenericSetOffsetProperty<decltype(ObjectType::prop)>), \
			.Validate = &(GenericValidateOffsetProperty<decltype(ObjectType::prop)>), \
			.Serialize = &(GenericSerializeOffsetProperty<decltype(ObjectType::prop)>), \
			.Offset = offsetof(ObjectType, prop), \
			.Flag = 0, \
			.Notification = PropertyNotification::None \
		} },

#define PN_RO(name, prop) \
		{ .Type = PropertyMapEntryType::Property, .Property = { \
			.Name = #name, \
			.Getter = &(GenericGetOffsetProperty<decltype(ObjectType::prop)>), \
			.Setter = &GenericSetReadOnlyProperty, \
			.Validate = &(GenericValidateOffsetProperty<decltype(ObjectType::prop)>), \
			.Serialize = &(GenericSerializeOffsetProperty<decltype(ObjectType::prop)>), \
			.Offset = offsetof(ObjectType, prop), \
			.Flag = 0, \
			.Notification = PropertyNotification::None \
		} },

#define P(prop) PN(prop, prop)
		
#define P_NOTIFY(prop, notify) \
		{ .Type = PropertyMapEntryType::Property, .Property = { \
			.Name = #prop, \
			.Getter = &(GenericGetOffsetProperty<decltype(ObjectType::prop)>), \
			.Setter = &(GenericSetOffsetProperty<decltype(ObjectType::prop)>), \
			.Validate = &(GenericValidateOffsetProperty<decltype(ObjectType::prop)>), \
			.Serialize = &(GenericSerializeOffsetProperty<decltype(ObjectType::prop)>), \
			.Offset = offsetof(ObjectType, prop), \
			.Flag = 0, \
			.Notification = PropertyNotification::notify \
		} },
		
#define P_RENAMED(prop, oldName) \
		P(prop) \
		{ .Type = PropertyMapEntryType::Property, .Property = { \
			.Name = #oldName, \
			.Getter = &(GenericGetOffsetProperty<decltype(ObjectType::prop)>), \
			.Setter = &(GenericSetOffsetProperty<decltype(ObjectType::prop)>), \
			.Validate = &(GenericValidateOffsetProperty<decltype(ObjectType::prop)>), \
			.Serialize = &(GenericSerializeOffsetProperty<decltype(ObjectType::prop)>), \
			.Offset = offsetof(ObjectType, prop), \
			.Flag = 0, \
			.Notification = PropertyNotification::Renamed, \
			.NewName = #prop, \
			.Iterable = false \
		} },

#define P_RO(prop) PN_RO(prop, prop)

#define P_BITMASK_GETTER_SETTER(prop, getter, setter) \
		{ .Type = PropertyMapEntryType::Bitfield, .Bitfield = { \
			.Name = #prop, \
			.TypeId = BitfieldID<decltype(ObjectType::prop)>::ID, \
			.Getter = &getter, \
			.Setter = &setter, \
			.Offset = offsetof(ObjectType, prop), \
		} },

#define P_BITMASK(prop) P_BITMASK_GETTER_SETTER(prop, (GenericGetOffsetBitmaskFlag<std::underlying_type_t<decltype(ObjectType::prop)>>), (GenericSetOffsetBitmaskFlag<std::underlying_type_t<decltype(ObjectType::prop)>>))

#define P_FREE_GETTER(name, fun) \
		{ .Type = PropertyMapEntryType::Property, .Property = { \
			.Name = #name, \
			.Getter = [](lua_State* L, LifetimeHandle const& lifetime, void const* obj, RawPropertyAccessors const& prop) { \
				CallGetter(L, reinterpret_cast<ObjectType const*>(obj), &fun); \
				return PropertyOperationResult::Success; \
			}, \
			.Setter = &GenericSetNonWriteableProperty, \
		} },

#define P_GETTER(name, fun) P_FREE_GETTER(name, ObjectType::fun)

#define P_GETTER_SETTER(name, getter, setter) \
		{ .Type = PropertyMapEntryType::Property, .Property = { \
			.Name = #name, \
			.Getter = [](lua_State* L, LifetimeHandle const& lifetime, void const* obj, RawPropertyAccessors const& prop) { \
				CallGetter(L, reinterpret_cast<ObjectType const*>(obj), &ObjectType::getter); \
				return PropertyOperationResult::Success; \
			}, \
			.Setter = [](lua_State* L, void* obj, int index, RawPropertyAccessors const& prop) { \
				CallSetter(L, reinterpret_cast<ObjectType*>(obj), index, &ObjectType::setter); \
				return PropertyOperationResult::Success; \
			}, \
		} },

// FIXME - avoid generating a separate push function for each closure
#define P_FUN(name, fun) \
		{ .Type = PropertyMapEntryType::Property, .Property = { \
			.Name = #name, \
			.Getter = [](lua_State* L, LifetimeHandle const& lifetime, void const* obj, RawPropertyAccessors const& prop) { \
				lua_pushcfunction(L, [](lua_State* L) -> int { \
					return CallMethod(L, &fun); \
				}); \
				return PropertyOperationResult::Success; \
			}, \
			.Setter = &GenericSetNonWriteableProperty, \
			.Serialize = &GenericNullSerializeProperty, \
		} },

#define P_FALLBACK(getter, setter, next) \
		{ .Type = PropertyMapEntryType::Fallback, .Fallback = { \
			.Getter = getter, \
			.Setter = setter, \
			.Next = next \
		} },

#include <GameDefinitions/Generated/PropertyMaps.inl>

#undef GENERATING_PROPMAP
#undef BEGIN_CLS
#undef BEGIN_CLS_TN
#undef END_CLS
#undef INHERIT
#undef P
#undef P_NOTIFY
#undef P_RENAMED
#undef P_RO
#undef P_BITMASK
#undef P_BITMASK_GETTER_SETTER
#undef PN
#undef PN_RO
#undef P_GETTER
#undef P_FREE_GETTER
#undef P_GETTER_SETTER
#undef P_FUN
#undef P_FALLBACK


static constexpr PropertyMapRegistrationEntry const* AllClassDefns[] = {


#define DECLARE_CLS(id, ...) PropertyMapRegistrations<__VA_ARGS__>::Definitions,
#define DECLARE_CLS_FWD(id, cls) PropertyMapRegistrations<cls>::Definitions,
#define DECLARE_CLS_NS_FWD(id, ns, cls) PropertyMapRegistrations<ns::cls>::Definitions,
#define DECLARE_CLS_BARE_NS_FWD(id, ns, cls) PropertyMapRegistrations<::ns::cls>::Definitions,
#define DECLARE_STRUCT_BARE_NS_FWD(id, ns, cls) PropertyMapRegistrations<::ns::cls>::Definitions,
#include <GameDefinitions/Generated/PropertyMapNames.inl>
#undef DECLARE_CLS
#undef DECLARE_CLS_FWD
#undef DECLARE_CLS_NS_FWD
#undef DECLARE_CLS_BARE_NS_FWD
#undef DECLARE_STRUCT_BARE_NS_FWD

};


void InitObjectProxyPropertyMaps()
{
	static bool initialized{ false };
	if (initialized) return;

	ProcessClassRegistrations(std::span(AllClassDefns));
	UpdateInheritance();

	initialized = true;
}

END_NS()
