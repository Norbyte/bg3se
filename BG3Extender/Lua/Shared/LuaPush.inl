BEGIN_NS(lua)

template <glm::length_t N, typename T, glm::qualifier Q>
void set_raw(Table* tab, glm::vec<N, T, Q> const& v)
{
    for (unsigned i = 0; i < N; i++) {
        if constexpr (std::is_floating_point_v<T>) {
            setfltvalue(tab->array + i, v[i]);
        } else {
            setivalue(tab->array + i, v[i]);
        }
    }
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


template <glm::length_t N, typename T, glm::qualifier Q>
void push(lua_State* L, glm::vec<N, T, Q> const& v)
{
    lua_createtable(L, N, 0);
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

void push(lua_State* L, RegistryEntry const& v)
{
    if (v) {
        v.Push(L);
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

void push(lua_State* L, Noesis::GridLengthHelper const& v)
{
    Serialize(L, &v);
}

template <glm::length_t N, typename T, glm::qualifier Q>
void assign(lua_State* L, int idx, glm::vec<N, T, Q> const& v)
{
    auto tab = lua_get_array_n(L, idx, N);
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
