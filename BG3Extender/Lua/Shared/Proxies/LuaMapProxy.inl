#include <Lua/Shared/Proxies/LuaMapProxy.h>

BEGIN_NS(lua)

MapProxyImplBase::MapProxyImplBase()
{
	Register();
}

void MapProxyImplBase::Register()
{
	assert(registryIndex_ == -1);
	registryIndex_ = gExtender->GetPropertyMapManager().RegisterMapProxy(this);
}

int MapProxyImplBase::GetRegistryIndex() const
{
	assert(registryIndex_ >= 0);
	return registryIndex_;
}


int MapProxyMetatable::Index(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetMapProxy(self.PropertyMapTag);
	if (!impl->GetValue(L, self, 2)) {
		push(L, nullptr);
	}

	return 1;
}

int MapProxyMetatable::NewIndex(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetMapProxy(self.PropertyMapTag);
	impl->SetValue(L, self, 2, 3);
	return 0;
}

int MapProxyMetatable::Length(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetMapProxy(self.PropertyMapTag);
	push(L, impl->Length(self));
	return 1;
}

int MapProxyMetatable::Next(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetMapProxy(self.PropertyMapTag);
	return impl->Next(L, self, 2);
}

int MapProxyMetatable::ToString(lua_State* L, CppObjectMetadata& self)
{
	StackCheck _(L, 1);
	char entityName[200];
	if (self.Lifetime.IsAlive(L)) {
		_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p)",
			GetTypeName(L, self), self.Ptr);
	} else {
		_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p, DEAD REFERENCE)",
			GetTypeName(L, self), self.Ptr);
	}

	push(L, entityName);
	return 1;
}

bool MapProxyMetatable::IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other)
{
	return self.Ptr == other.Ptr && self.PropertyMapTag == other.PropertyMapTag;
}

char const* MapProxyMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
	auto impl = gExtender->GetPropertyMapManager().GetMapProxy(self.PropertyMapTag);
	return impl->GetContainerType().TypeName.GetString();
}

void* MapProxyMetatable::GetRaw(lua_State* L, int index, int propertyMapIndex)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, index, MetatableTag::Map, meta);

	if (meta.PropertyMapTag != propertyMapIndex) {
		auto curTy = gExtender->GetPropertyMapManager().GetMapProxy(meta.PropertyMapTag);
		auto expectedTy = gExtender->GetPropertyMapManager().GetMapProxy(propertyMapIndex);
		luaL_error(L, "Argument %d: expected %s, got %s", index, 
			expectedTy->GetContainerType().TypeName.GetString(),
			curTy->GetContainerType().TypeName.GetString());
		return nullptr;
	}

	if (!meta.Lifetime.IsAlive(L)) {
		auto curTy = gExtender->GetPropertyMapManager().GetMapProxy(meta.PropertyMapTag);
		luaL_error(L, "Argument %d: got %s whose lifetime has expired", index,
			curTy->GetContainerType().TypeName.GetString());
		return nullptr;
	}

	return meta.Ptr;
}

MapProxyImplBase* MapProxyMetatable::GetImpl(int propertyMapIndex)
{
	return gExtender->GetPropertyMapManager().GetMapProxy(propertyMapIndex);
}

END_NS()

