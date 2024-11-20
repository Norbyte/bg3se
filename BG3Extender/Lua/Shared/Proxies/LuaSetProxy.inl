#include <Lua/Shared/Proxies/LuaSetProxy.h>

BEGIN_NS(lua)

SetProxyImplBase::SetProxyImplBase()
{
	Register();
}

void SetProxyImplBase::Register()
{
	assert(registryIndex_ == -1);
	registryIndex_ = gExtender->GetPropertyMapManager().RegisterSetProxy(this);
}

int SetProxyImplBase::GetRegistryIndex() const
{
	assert(registryIndex_ >= 0);
	return registryIndex_;
}

void* SetProxyMetatable::GetRaw(lua_State* L, int index, int propertyMapIndex)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, index, MetatableTag::Set, meta);

	if (meta.PropertyMapTag != propertyMapIndex) {
		auto curTy = gExtender->GetPropertyMapManager().GetSetProxy(meta.PropertyMapTag);
		auto expectedTy = gExtender->GetPropertyMapManager().GetSetProxy(propertyMapIndex);
		luaL_error(L, "Argument %d: expected %s, got %s", index, 
			expectedTy->GetContainerType().TypeName.GetString(),
			curTy->GetContainerType().TypeName.GetString());
		return nullptr;
	}

	if (!meta.Lifetime.IsAlive(L)) {
		auto curTy = gExtender->GetPropertyMapManager().GetSetProxy(meta.PropertyMapTag);
		luaL_error(L, "Argument %d: got %s whose lifetime has expired", index,
			curTy->GetContainerType().TypeName.GetString());
		return nullptr;
	}

	return meta.Ptr;
}

SetProxyImplBase* SetProxyMetatable::GetImpl(int propertyMapIndex)
{
	return gExtender->GetPropertyMapManager().GetSetProxy(propertyMapIndex);
}

int SetProxyMetatable::Index(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetSetProxy(self.PropertyMapTag);
	push(L, impl->HasElement(L, self, 2));
	return 1;
}

int SetProxyMetatable::NewIndex(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetSetProxy(self.PropertyMapTag);
	auto val = get<bool>(L, 3);
	if (val) {
		impl->AddElement(L, self, 2);
	} else {
		impl->RemoveElement(L, self, 2);
	}

	return 0;
}

int SetProxyMetatable::Length(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetSetProxy(self.PropertyMapTag);
	push(L, impl->Length(self));
	return 1;
}

int SetProxyMetatable::Next(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetSetProxy(self.PropertyMapTag);
	if (lua_type(L, 2) == LUA_TNIL) {
		return impl->Next(L, self, 0);
	} else {
		auto key = get<int>(L, 2);
		return impl->Next(L, self, key);
	}
}

int SetProxyMetatable::ToString(lua_State* L, CppObjectMetadata& self)
{
	StackCheck _(L, 1);
	char entityName[200];
	if (self.Lifetime.IsAlive(L)) {
		_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p)", GetTypeName(L, self), self.Ptr);
	} else {
		_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p, DEAD REFERENCE)", GetTypeName(L, self), self.Ptr);
	}

	push(L, entityName);
	return 1;
}

bool SetProxyMetatable::IsEqual(lua_State*, CppObjectMetadata& self, CppObjectMetadata& other)
{
	return self.Ptr == other.Ptr && self.PropertyMapTag == other.PropertyMapTag;
}

char const* SetProxyMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetSetProxy(self.PropertyMapTag);
	return impl->GetContainerType().TypeName.GetString();
}

END_NS()
