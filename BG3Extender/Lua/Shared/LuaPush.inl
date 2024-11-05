BEGIN_NS(lua)

void set_raw(Table* tab, glm::vec2 const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
}

void set_raw(Table* tab, glm::vec3 const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
	setfltvalue(tab->array + 2, v.z);
}

void set_raw(Table* tab, glm::vec4 const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
	setfltvalue(tab->array + 2, v.z);
	setfltvalue(tab->array + 3, v.w);
}

void set_raw(Table* tab, glm::aligned_highp_vec4 const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
	setfltvalue(tab->array + 2, v.z);
	setfltvalue(tab->array + 3, v.w);
}

void set_raw(Table* tab, glm::quat const& v)
{
	setfltvalue(tab->array + 0, v.x);
	setfltvalue(tab->array + 1, v.y);
	setfltvalue(tab->array + 2, v.z);
	setfltvalue(tab->array + 3, v.w);
}

void set_raw(Table* tab, glm::mat3 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[1].x);
	setfltvalue(tab->array + 4, m[1].y);
	setfltvalue(tab->array + 5, m[1].z);
	setfltvalue(tab->array + 6, m[2].x);
	setfltvalue(tab->array + 7, m[2].y);
	setfltvalue(tab->array + 8, m[2].z);
}

void set_raw(Table* tab, glm::mat3x4 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[0].w);
	setfltvalue(tab->array + 4, m[1].x);
	setfltvalue(tab->array + 5, m[1].y);
	setfltvalue(tab->array + 6, m[1].z);
	setfltvalue(tab->array + 7, m[1].w);
	setfltvalue(tab->array + 8, m[2].x);
	setfltvalue(tab->array + 9, m[2].y);
	setfltvalue(tab->array + 10, m[2].z);
	setfltvalue(tab->array + 11, m[2].w);
}

void set_raw(Table* tab, glm::mat4x3 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[1].x);
	setfltvalue(tab->array + 4, m[1].y);
	setfltvalue(tab->array + 5, m[1].z);
	setfltvalue(tab->array + 6, m[2].x);
	setfltvalue(tab->array + 7, m[2].y);
	setfltvalue(tab->array + 8, m[2].z);
	setfltvalue(tab->array + 9, m[3].x);
	setfltvalue(tab->array + 10, m[3].y);
	setfltvalue(tab->array + 11, m[3].z);
}

void set_raw(Table* tab, glm::mat4 const& m)
{
	setfltvalue(tab->array + 0, m[0].x);
	setfltvalue(tab->array + 1, m[0].y);
	setfltvalue(tab->array + 2, m[0].z);
	setfltvalue(tab->array + 3, m[0].w);
	setfltvalue(tab->array + 4, m[1].x);
	setfltvalue(tab->array + 5, m[1].y);
	setfltvalue(tab->array + 6, m[1].z);
	setfltvalue(tab->array + 7, m[1].w);
	setfltvalue(tab->array + 8, m[2].x);
	setfltvalue(tab->array + 9, m[2].y);
	setfltvalue(tab->array + 10, m[2].z);
	setfltvalue(tab->array + 11, m[2].w);
	setfltvalue(tab->array + 12, m[3].x);
	setfltvalue(tab->array + 13, m[3].y);
	setfltvalue(tab->array + 14, m[3].z);
	setfltvalue(tab->array + 15, m[3].w);
}

void push(lua_State* L, glm::vec2 const& v)
{
	lua_createtable(L, 2, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::vec3 const& v)
{
	lua_createtable(L, 3, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::vec4 const& v)
{
	lua_createtable(L, 4, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::aligned_highp_vec4 const& v)
{
	lua_createtable(L, 4, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::quat const& v)
{
	lua_createtable(L, 4, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, v);
}

void push(lua_State* L, glm::mat3 const& m)
{
	lua_createtable(L, 9, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

void push(lua_State* L, glm::mat3x4 const& m)
{
	lua_createtable(L, 12, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

void push(lua_State* L, glm::mat4x3 const& m)
{
	lua_createtable(L, 12, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

void push(lua_State* L, glm::mat4 const& m)
{
	lua_createtable(L, 16, 0);
	auto tab = lua_get_top_table_unsafe(L);
	set_raw(tab, m);
}

void push(lua_State* L, Ref const& v)
{
	v.Push(L);
}

void push(lua_State* L, PersistentRef const& v)
{
	v.Push(L);
}

void push(lua_State* L, RegistryEntry const& v)
{
	if (v) {
		v.Push();
	} else {
		lua_pushnil(L);
	}
}

void push(lua_State* L, PersistentRegistryEntry const& v)
{
	if (!v.TryPush(L)) {
		lua_pushnil(L);
	}
}

void push(lua_State* L, RawComponentRef const& v)
{
	v.Push(L);
}

void assign(lua_State* L, int idx, glm::vec2 const& v)
{
	auto tab = lua_get_array_n(L, idx, 2);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::vec3 const& v)
{
	auto tab = lua_get_array_n(L, idx, 3);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::vec4 const& v)
{
	auto tab = lua_get_array_n(L, idx, 4);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::aligned_highp_vec4 const& v)
{
	auto tab = lua_get_array_n(L, idx, 4);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::quat const& v)
{
	auto tab = lua_get_array_n(L, idx, 4);
	set_raw(tab, v);
}

void assign(lua_State* L, int idx, glm::mat3 const& m)
{
	auto tab = lua_get_array_n(L, idx, 9);
	set_raw(tab, m);
}

void assign(lua_State* L, int idx, glm::mat3x4 const& m)
{
	auto tab = lua_get_array_n(L, idx, 12);
	set_raw(tab, m);
}

void assign(lua_State* L, int idx, glm::mat4 const& m)
{
	auto tab = lua_get_array_n(L, idx, 16);
	set_raw(tab, m);
}

END_NS()
