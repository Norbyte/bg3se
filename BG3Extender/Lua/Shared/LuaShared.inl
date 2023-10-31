namespace bg3se::esv
{
	struct Status;
}

namespace bg3se::lua
{

void LuaMakeStatusProxy(lua_State* L, esv::Status* status, LifetimeHandle const& lifetime);
// TODO - void LuaMakeStatusProxy(lua_State* L, ecl::Status* status, LifetimeHandle const& lifetime);

template <class TObject, class TStatus>
int GameObjectGetStatus(lua_State* L, TObject* self)
{
	StackCheck _(L, 1);
	if (!self->StatusMachine) {
		push(L, nullptr);
		return 1;
	}

	esv::Status* status{ nullptr };

	if (lua_type(L, 2) == LUA_TSTRING) {
		auto statusId = get<FixedString>(L, 2);
		status = self->StatusMachine->GetStatus(statusId);
	} else {
		auto statusHandle = get<ComponentHandle>(L, 2);
		status = self->StatusMachine->GetStatus(statusHandle);
	}

	if (status) {
		LuaMakeStatusProxy(L, status, GetCurrentLifetime(L));
	} else {
		push(L, nullptr);
	}
	return 1;
}

template <class TObject, class TStatus>
int GameObjectGetStatusByType(lua_State* L, TObject* self)
{
	StackCheck _(L, 1);
	auto statusType = get<StatusType>(L, 2);

	if (!self->StatusMachine) {
		push(L, nullptr);
		return 1;
	}

	for (auto status : self->StatusMachine->StackedStatuses) {
		if (status->GetStatusId() == statusType) {
			LuaMakeStatusProxy(L, status, GetCurrentLifetime(L));
			return 1;
		}
	}

	push(L, nullptr);
	return 1;
}

template <class TObject>
int GameObjectGetStatusIds(lua_State* L, TObject* self)
{
	lua_newtable(L);

	if (!self->StatusMachine) {
		return 1;
	}

	int32_t index = 1;
	for (auto status : self->StatusMachine->StackedStatuses) {
		settable(L, index++, status->StatusId);
	}

	return 1;
}

template <class TObject, class TStatus>
int GameObjectGetStatuses(lua_State* L, TObject* self)
{
	lua_newtable(L);

	if (!self->StatusMachine) {
		return 1;
	}

	int32_t index = 1;
	for (auto status : self->StatusMachine->StackedStatuses) {
		push(L, index++);
		LuaMakeStatusProxy(L, status, GetCurrentLifetime(L));
		lua_rawset(L, -3);
	}

	return 1;
}

template <class TObject>
int GameObjectHasTag(lua_State* L, TObject* self)
{
	return luaL_error(L, "Not implemented yet!");

	/*auto tag = luaL_checkstring(L, 2);

	FixedString tagFs(tag);

	if (!object || !tagFs) {
		push(L, false);
	} else {
		push(L, object->IsTagged(tagFs));
	}

	return 1;*/
}

template <class TObject>
int GameObjectGetTags(lua_State* L, TObject* self)
{
	return luaL_error(L, "Not implemented yet!");

	/*
	lua_newtable(L);

	int32_t index = 1;
	ObjectSet<FixedString> tags;
	character->GetTags(tags);

	for (auto const& tag : tags) {
		settable(L, index++, tag);
	}

	return 1;*/
}

template <class TObject>
int GameObjectSetScale(lua_State* L, TObject* self)
{
	auto scale = get<float>(L, 2);
	// Weird things happen if scale is too large/small
	if (scale < 0.01f || scale > 100.0f) {
		OsiError("Scale must be between 0.01 and 100")
	} else {
		self->SetScale(scale);
	}

	return 0;
}

template <class TObject>
int GameObjectGetDisplayName(lua_State* L, TObject* object)
{
	TranslatedString name;
	object->GetDisplayName(name);
	push(L, name.Handle.Handle);
	return 1;
}

}
