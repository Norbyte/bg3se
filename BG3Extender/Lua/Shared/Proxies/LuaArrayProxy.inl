#include <Lua/Shared/Proxies/LuaArrayProxy.h>

BEGIN_NS(lua)


ArrayProxyImplBase::ArrayProxyImplBase()
{
	Register();
}

void ArrayProxyImplBase::Register()
{
	assert(registryIndex_ == -1);
	registryIndex_ = gExtender->GetPropertyMapManager().RegisterArrayProxy(this);
}

int ArrayProxyImplBase::GetRegistryIndex() const
{
	assert(registryIndex_ >= 0);
	return registryIndex_;
}

void* ArrayProxyMetatable::GetRaw(lua_State* L, int index, int propertyMapIndex)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, index, MetatableTag::Array, meta);

	if (meta.PropertyMapTag != propertyMapIndex) {
		auto curTy = gExtender->GetPropertyMapManager().GetArrayProxy(meta.PropertyMapTag);
		auto expectedTy = gExtender->GetPropertyMapManager().GetArrayProxy(propertyMapIndex);
		luaL_error(L, "Argument %d: expected %s, got %s", index, 
			expectedTy->GetContainerType().TypeName.GetString(),
			curTy->GetContainerType().TypeName.GetString());
		return nullptr;
	}

	if (!meta.Lifetime.IsAlive(L)) {
		auto curTy = gExtender->GetPropertyMapManager().GetArrayProxy(meta.PropertyMapTag);
		luaL_error(L, "Argument %d: got %s whose lifetime has expired", index,
			curTy->GetContainerType().TypeName.GetString());
		return nullptr;
	}

	return meta.Ptr;
}

ArrayProxyImplBase* ArrayProxyMetatable::GetImpl(int propertyMapIndex)
{
	return gExtender->GetPropertyMapManager().GetArrayProxy(propertyMapIndex);
}

int ArrayProxyMetatable::Index(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(self.PropertyMapTag);
	auto index = get<int>(L, 2);
	if (!impl->GetElement(L, self, index)) {
		push(L, nullptr);
	}

	return 1;
}

int ArrayProxyMetatable::NewIndex(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(self.PropertyMapTag);
	auto index = get<int>(L, 2);
	impl->SetElement(L, self, index, 3);
	return 0;
}

int ArrayProxyMetatable::Length(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(self.PropertyMapTag);
	push(L, impl->Length(self));
	return 1;
}

int ArrayProxyMetatable::Next(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(self.PropertyMapTag);
	if (lua_type(L, 2) == LUA_TNIL) {
		return impl->Next(L, self, 0);
	} else {
		auto key = get<int>(L, 2);
		return impl->Next(L, self, key);
	}
}

int ArrayProxyMetatable::ToString(lua_State* L, CppObjectMetadata& self)
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

bool ArrayProxyMetatable::IsEqual(lua_State*, CppObjectMetadata& self, CppObjectMetadata& other)
{
	return self.Ptr == other.Ptr && self.PropertyMapTag == other.PropertyMapTag;
}

char const* ArrayProxyMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(self.PropertyMapTag);
	return impl->GetContainerType().TypeName.GetString();
}

END_NS()

