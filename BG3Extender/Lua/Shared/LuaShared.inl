namespace bg3se::lua
{

template <class TObject, class TStatus>
int GameObjectGetStatus(lua_State* L, TObject* self)
{
	auto statusId = luaL_checkstring(L, 2);
	FixedString statusIdFs(statusId);

	if (!self->StatusMachine || !statusIdFs) {
		return 0;
	}

	auto status = self->StatusMachine->GetStatus(statusIdFs);
	if (status) {
		// FIXME - use handle based proxy
		ObjectProxy::MakeRef<TStatus>(L, status, GetCurrentLifetime());
		return 1;
	}
	else {
		return 0;
	}
}

template <class TObject, class TStatus>
int GameObjectGetStatusByType(lua_State* L, TObject* self)
{
	auto statusType = checked_get<StatusType>(L, 2);

	if (!self->StatusMachine) {
		return 0;
	}

	for (auto status : self->StatusMachine->Statuses) {
		if (status->GetStatusId() == statusType) {
			// FIXME - use handle based proxy
			ObjectProxy::MakeRef<TStatus>(L, status, GetCurrentLifetime());
			return 1;
		}
	}

	return 0;
}

template <class TObject>
int GameObjectGetStatuses(lua_State* L, TObject* self)
{
	if (!self->StatusMachine) {
		return 0;
	}

	lua_newtable(L);

	int32_t index = 1;
	for (auto status : self->StatusMachine->Statuses) {
		settable(L, index++, status->StatusId);
	}

	return 1;
}

template <class TObject, class TStatus>
int GameObjectGetStatusObjects(lua_State* L, TObject* self)
{
	if (!self->StatusMachine) {
		return 0;
	}

	lua_newtable(L);

	int32_t index = 1;
	for (auto status : self->StatusMachine->Statuses) {
		push(L, index++);
		// FIXME - use handle based proxy
		ObjectProxy::MakeRef<TStatus>(L, status, GetCurrentLifetime());
		lua_settable(L, -3);
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
	auto scale = checked_get<float>(L, 2);
	// Weird things happen if scale is too large/small
	if (scale < 0.01f || scale > 100.0f) {
		OsiError("Scale must be between 0.01 and 100")
	} else {
		character->SetScale(scale);
	}

	return 0;
}

template <class TObject>
int GameObjectGetDisplayName(lua_State* L, TObject* object)
{
	return luaL_error(L, "Not implemented yet!");

	/*TranslatedString name;
	object->GetDisplayName(name);
	push(L, name.Handle.ReferenceString);
	return 1;*/
}

}
