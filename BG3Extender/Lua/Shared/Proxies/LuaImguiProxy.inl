#include <Lua/Shared/Proxies/LuaImguiProxy.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(lua)

GenericPropertyMap& ImguiObjectProxyMetatable::GetPropertyMap(CppValueMetadata const& meta)
{
	assert(meta.MetatableTag == MetaTag);
	return GetRenderable(meta)->GetRTTI();
}

extui::Renderable* ImguiObjectProxyMetatable::GetRenderable(CppValueMetadata const& meta)
{
	assert(meta.MetatableTag == MetaTag);
	return ecl::ExtensionState::Get().GetClientLua()->IMGUI().GetRenderable(meta.Value);
}

extui::Renderable* ImguiObjectProxyMetatable::TryGetGeneric(lua_State* L, int index, extui::IMGUIObjectType type)
{
	CppValueMetadata meta;
	if (lua_try_get_cppvalue(L, index, MetaTag, meta)) {
		auto obj = GetRenderable(meta);
		if (obj != nullptr && obj->GetType() == type) {
			return obj;
		}
	}

	return nullptr;
}

extui::Renderable* ImguiObjectProxyMetatable::GetGeneric(lua_State* L, int index, extui::IMGUIObjectType type)
{
	CppValueMetadata meta;
	if (!lua_try_get_cppvalue(L, index, MetaTag, meta)) {
		luaL_error(L, "Argument %d: Expected IMGUI object, got '%s'", index,
			lua_typename(L, lua_type(L, index)));
		return nullptr;
	}

	auto obj = GetRenderable(meta);
	if (obj == nullptr) {
		luaL_error(L, "Argument %d: IMGUI object no longer exists", index);
		return nullptr;
	}

	if (obj->GetType() == type) {
		luaL_error(L, "Argument %d: Expected IMGUI object of type '%d', got '%d'", index,
			obj->GetType(), type);
		return nullptr;
	}

	return obj;
}

extui::Renderable* ImguiObjectProxyMetatable::GetGeneric(lua_State* L, int index)
{
	CppValueMetadata meta;
	if (!lua_try_get_cppvalue(L, index, MetaTag, meta)) {
		luaL_error(L, "Argument %d: Expected IMGUI object, got '%s'", index,
			lua_typename(L, lua_type(L, index)));
		return nullptr;
	}

	auto obj = GetRenderable(meta);
	if (obj == nullptr) {
		luaL_error(L, "Argument %d: IMGUI object no longer exists", index);
		return nullptr;
	}

	return obj;
}

int ImguiObjectProxyMetatable::Index(lua_State* L, CppValueMetadata& self)
{
	auto obj = GetRenderable(self);
	if (obj == nullptr) {
		luaL_error(L, "IMGUI object no longer exists");
		return 0;
	}

	auto& pm = obj->GetRTTI();
	auto prop = get<FixedString>(L, 2);
	auto result = pm.GetRawProperty(L, State::FromLua(L)->GetGlobalLifetime(), obj, prop);

	switch (result) {
	case PropertyOperationResult::Success:
		break;

	case PropertyOperationResult::NoSuchProperty:
		luaL_error(L, "Property does not exist: %s::%s - property does not exist", GetTypeName(L, self), prop.GetString());
		push(L, nullptr);
		break;

	case PropertyOperationResult::Unknown:
	default:
		luaL_error(L, "Cannot get property %s::%s - unknown error", GetTypeName(L, self), prop.GetString());
		push(L, nullptr);
		break;
	}

	return 1;
}

int ImguiObjectProxyMetatable::NewIndex(lua_State* L, CppValueMetadata& self)
{
	auto obj = GetRenderable(self);
	if (obj == nullptr) {
		luaL_error(L, "IMGUI object no longer exists");
		return 0;
	}

	auto& pm = obj->GetRTTI();
	auto prop = get<FixedString>(L, 2);
	auto result = pm.SetRawProperty(L, obj, prop, 3);

	switch (result) {
	case PropertyOperationResult::Success:
		break;

	case PropertyOperationResult::NoSuchProperty:
		luaL_error(L, "Cannot set property %s::%s - property does not exist", GetTypeName(L, self), prop.GetString());
		break;

	case PropertyOperationResult::ReadOnly:
		luaL_error(L, "Cannot set property %s::%s - property is read-only", GetTypeName(L, self), prop.GetString());
		break;

	case PropertyOperationResult::UnsupportedType:
		luaL_error(L, "Cannot set property %s::%s - cannot write properties of this type", GetTypeName(L, self), prop.GetString());
		break;

	case PropertyOperationResult::Unknown:
	default:
		luaL_error(L, "Cannot set property %s::%s - unknown error", GetTypeName(L, self), prop.GetString());
		break;
	}

	return 0;
}

int ImguiObjectProxyMetatable::ToString(lua_State* L, CppValueMetadata& self)
{
	char entityName[200];
	auto obj = GetRenderable(self);
	if (obj) {
		_snprintf_s(entityName, std::size(entityName) - 1, "%s (%llx)", obj->GetTypeName(), self.Value);
	} else {
		_snprintf_s(entityName, std::size(entityName) - 1, "IMGUI (%llx, DEAD REFERENCE)", self.Value);
	}

	push(L, entityName);
	return 1;
}

bool ImguiObjectProxyMetatable::IsEqual(lua_State* L, CppValueMetadata& self, int otherIndex)
{
	CppValueMetadata other;
	if (!lua_try_get_cppvalue(L, otherIndex, self.MetatableTag, other)) {
		return false;
	}

	return other.Value == self.Value;
}

int ImguiObjectProxyMetatable::Next(lua_State* L, CppValueMetadata& self)
{
	auto obj = GetRenderable(self);
	if (obj == nullptr) {
		luaL_error(L, "IMGUI object no longer exists");
		return 0;
	}

	auto& pm = obj->GetRTTI();
	if (lua_type(L, 2) == LUA_TNIL) {
		return CppObjectProxyHelpers::Next(L, pm, obj, State::FromLua(L)->GetGlobalLifetime(), FixedString{});
	} else {
		auto key = get<FixedString>(L, 2);
		return CppObjectProxyHelpers::Next(L, pm, obj, State::FromLua(L)->GetGlobalLifetime(), key);
	}
}

char const* ImguiObjectProxyMetatable::GetTypeName(lua_State* L, CppValueMetadata& self)
{
	auto obj = GetRenderable(self);
	if (obj == nullptr) {
		return "Dead IMGUI object";
	} else {
		return obj->GetTypeName();
	}
}

ImguiHandle do_get(lua_State* L, int index, Overload<ImguiHandle>)
{
	return ImguiHandle(ImguiObjectProxyMetatable::GetGeneric(L, index)->Handle);
}

extui::Renderable* do_get(lua_State* L, int index, Overload<extui::Renderable*>)
{
	return ImguiObjectProxyMetatable::GetGeneric(L, index);
}

ImguiHandle::ImguiHandle(extui::Renderable* o)
	: Handle(o ? o->Handle : extui::InvalidHandle)
{}

END_NS()
