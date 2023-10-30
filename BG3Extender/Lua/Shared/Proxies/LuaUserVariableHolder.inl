#include <Lua/Shared/Proxies/LuaUserVariableHolder.h>

BEGIN_NS(lua)

int UserVariableHolderMetatable::Index(lua_State* L, CppValueMetadata& self)
{
	auto key = get<FixedString>(L, 2);

	auto& vars = State::FromLua(L)->GetVariableManager();
	auto proto = vars.GetGlobal().GetPrototype(key);
	if (!proto) {
		OsiError("Variable class '" << key << "' not registered.");
		push(L, nullptr);
		return 1;
	}
	
	if (!proto->IsAvailableFor(vars.IsServer())) {
		OsiError("Variable class '" << key << "' not available on " << (vars.IsServer() ? "server" : "client"));
		push(L, nullptr);
		return 1;
	}

	vars.Push(L, EntityHandle(self.Value), key, *proto);
	return 1;
}

int UserVariableHolderMetatable::NewIndex(lua_State* L, CppValueMetadata& self)
{
	auto key = get<FixedString>(L, 2);

	auto& vars = State::FromLua(L)->GetVariableManager();
	auto proto = vars.GetGlobal().GetPrototype(key);
	if (!proto) {
		OsiError("Variable class '" << key << "' not registered.");
		return 0;
	}

	if (!proto->IsWriteableFor(vars.IsServer())) {
		OsiError("Variable class '" << key << "' not writeable on " << (vars.IsServer() ? "server" : "client"));
		return 0;
	}

	CachedUserVariable value(L, Ref(L, 3));
	vars.Set(L, EntityHandle(self.Value), key, *proto, std::move(value));
	return 0;
}

int UserVariableHolderMetatable::Length(lua_State* L, CppValueMetadata& self)
{
	auto& vars = State::FromLua(L)->GetVariableManager();
	auto guid = vars.GetGlobal().EntityToGuid(EntityHandle(self.Value));
	auto entityVars = vars.GetGlobal().GetAll(guid);
	push(L, entityVars ? entityVars->size() : 0);
	return 1;
}

int UserVariableHolderMetatable::Next(lua_State* L, CppValueMetadata& self)
{
	auto& vars = State::FromLua(L)->GetVariableManager();
	auto guid = vars.GetGlobal().EntityToGuid(EntityHandle(self.Value));
	auto entityVars = vars.GetGlobal().GetAll(guid);
	if (!entityVars) {
		return 0;
	}

	if (lua_type(L, 2) == LUA_TNIL) {
		auto it = entityVars->begin();
		if (it) {
			push(L, it.Key());
			vars.Push(L, EntityHandle(self.Value), it.Key());
			return 2;
		}
	} else {
		auto key = get<FixedString>(L, 2);
		auto it = entityVars->find(key);

		if (it) {
			it++;
			if (it) {
				push(L, it.Key());
				vars.Push(L, EntityHandle(self.Value), it.Key());
				return 2;
			}
		}
	}

	return 0;
}

int UserVariableHolderMetatable::ToString(lua_State* L, CppValueMetadata& self)
{
	char name[100];
	sprintf_s(name, "UserVariables(%16llx)", self.Value);
	push(L, name);
	return 1;
}

char const* UserVariableHolderMetatable::GetTypeName(lua_State* L, CppValueMetadata& self)
{
	return "UserVariableHolder";
}



int ModVariableHolderMetatable::Index(lua_State* L, CppValueMetadata& self)
{
	auto key = get<FixedString>(L, 2);

	auto& vars = State::FromLua(L)->GetModVariableManager();
	auto modUuid = vars.ModIndexToGuid((uint32_t)self.Value);
	auto proto = vars.GetGlobal().GetPrototype(modUuid, key);
	if (!proto) {
		OsiError("Mod variable class '" << key << "' not registered for module '" << modUuid << "'.");
		push(L, nullptr);
		return 1;
	}
	
	if (!proto->IsAvailableFor(vars.IsServer())) {
		OsiError("Mod variable class '" << key << "' not available on " << (vars.IsServer() ? "server" : "client"));
		push(L, nullptr);
		return 1;
	}

	vars.Push(L, (uint32_t)self.Value, key, *proto);
	return 1;
}

int ModVariableHolderMetatable::NewIndex(lua_State* L, CppValueMetadata& self)
{
	auto key = get<FixedString>(L, 2);

	auto& vars = State::FromLua(L)->GetModVariableManager();
	auto modUuid = vars.ModIndexToGuid((uint32_t)self.Value);
	auto proto = vars.GetGlobal().GetPrototype(modUuid, key);
	if (!proto) {
		OsiError("Mod variable class '" << key << "' not registered for module '" << modUuid << "'.");
		return 0;
	}

	if (!proto->IsWriteableFor(vars.IsServer())) {
		OsiError("Mod variable class '" << key << "' not writeable on " << (vars.IsServer() ? "server" : "client"));
		return 0;
	}

	CachedUserVariable value(L, Ref(L, 3));
	vars.Set(L, (uint32_t)self.Value, key, *proto, std::move(value));
	return 0;
}

int ModVariableHolderMetatable::Length(lua_State* L, CppValueMetadata& self)
{
	auto& vars = State::FromLua(L)->GetModVariableManager();
	auto modUuid = vars.ModIndexToGuid((uint32_t)self.Value);
	auto modVars = vars.GetGlobal().GetAll(modUuid);
	push(L, modVars ? modVars->size() : 0);
	return 1;
}

int ModVariableHolderMetatable::Next(lua_State* L, CppValueMetadata& self)
{
	auto& vars = State::FromLua(L)->GetModVariableManager();
	auto modUuid = vars.ModIndexToGuid((uint32_t)self.Value);
	auto modVars = vars.GetGlobal().GetAll(modUuid);
	if (!modVars) {
		return 0;
	}

	if (lua_type(L, 2) == LUA_TNIL) {
		auto it = modVars->begin();
		if (it) {
			push(L, it.Key());
			vars.Push(L, (uint32_t)self.Value, it.Key());
			return 2;
		}
	} else {
		auto key = get<FixedString>(L, 2);
		auto it = modVars->find(key);

		if (it) {
			it++;
			if (it) {
				push(L, it.Key());
				vars.Push(L, (uint32_t)self.Value, it.Key());
				return 2;
			}
		}
	}

	return 0;
}

int ModVariableHolderMetatable::ToString(lua_State* L, CppValueMetadata& self)
{
	char name[100];
	sprintf_s(name, "ModVariables(%lld)", self.Value);
	push(L, name);
	return 1;
}

char const* ModVariableHolderMetatable::GetTypeName(lua_State* L, CppValueMetadata& self)
{
	return "ModVariableHolder";
}

END_NS()
