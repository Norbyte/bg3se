BEGIN_NS(lua)

Table* lua_get_array(lua_State* L, int idx)
{
    auto val = lua_index2addr(L, idx);
    if (!ttistable(val)) {
        luaL_error(L, "Param %d: expected a table, got %s", idx, GetDebugName(L, idx));
        return nullptr;
    }

    auto tab = hvalue(val);
    if (!tab->sizearray && tab->lastfree) {
        luaL_error(L, "Param %d must be an array-like table, not a hashtable", idx);
        return nullptr;
    }

    return tab;
}

unsigned int lua_get_array_size(Table* arr)
{
    unsigned int i;
    for (i = arr->sizearray; i > 0 && ttisnil(&arr->array[i - 1]); i--) {}
    return i;
}

Table* lua_get_array_n(lua_State* L, int idx, int size)
{
    auto arr = lua_get_array(L, idx);
    auto arrSize = lua_get_array_size(arr);
    if (arrSize != size) {
        luaL_error(L, "Param %d: expected %d-element array, got a %d-element array", idx, size, arrSize);
        return nullptr;
    }

    return arr;
}

bool lua_typecheck_array_n(lua_State* L, int idx, int size)
{
    auto val = lua_index2addr(L, idx);
    if (!ttistable(val)) return false;

    auto tab = hvalue(val);
    if (!tab->sizearray && tab->lastfree) return false;

    return lua_get_array_size(tab) == size;
}

bool lua_is_linear_array(lua_State* L, int idx)
{
    auto val = lua_index2addr(L, idx);
    if (!ttistable(val)) return false;

    auto tab = hvalue(val);
    if (tab->lastfree) return false;

    // Skip trailing nil-s
    unsigned int sz;
    for (sz = tab->sizearray; sz > 0 && ttisnil(&tab->array[sz - 1]); sz--) {}

    // Check if every real entry is non-nil
    for (unsigned i = 0; i < sz; i++) {
        if (ttisnil(&tab->array[i])) {
            return false;
        }
    }

    return true;
}

template <class T>  requires std::is_integral_v<T>
T lua_val_get(lua_State* L, TValue* val)
{
    if (ttisinteger(val)) {
        return (T)ivalue(val);
    } else {
        lua_Integer i{ 0 };
        auto isnum = luaV_tointeger(val, &i, 0);
        if (!isnum) {
            luaL_error(L, "Expected integer value, got %s", lua_typename(L, ttnov(val)));
        }
        return (T)i;
    }
}

template <class T>  requires std::is_floating_point_v<T>
T lua_val_get(lua_State* L, TValue* val)
{
    if (ttisfloat(val)) {
        return (T)fltvalue(val);
    } else if (ttisinteger(val)) {
        return (T)ivalue(val);
    } else {
        luaL_error(L, "Expected float value, got %s", lua_typename(L, ttnov(val)));
        return T{ 0 };
    }
}

template <typename T, glm::qualifier Q>
glm::vec<2, T, Q> get_raw(lua_State* L, Table* arr, Overload<glm::vec<2, T, Q>>)
{
    return glm::vec<2, T, Q>{
        lua_val_get<T>(L, arr->array),
        lua_val_get<T>(L, arr->array + 1),
    };
}

template <typename T, glm::qualifier Q>
glm::vec<3, T, Q> get_raw(lua_State* L, Table* arr, Overload<glm::vec<3, T, Q>>)
{
    return glm::vec<3, T, Q>{
        lua_val_get<T>(L, arr->array),
        lua_val_get<T>(L, arr->array + 1),
        lua_val_get<T>(L, arr->array + 2),
    };
}

template <typename T, glm::qualifier Q>
glm::vec<4, T, Q> get_raw(lua_State* L, Table* arr, Overload<glm::vec<4, T, Q>>)
{
    return glm::vec<4, T, Q>{
        lua_val_get<T>(L, arr->array),
        lua_val_get<T>(L, arr->array + 1),
        lua_val_get<T>(L, arr->array + 2),
        lua_val_get<T>(L, arr->array + 3),
    };
}

glm::quat get_raw(lua_State* L, Table* arr, Overload<glm::quat>)
{
    // quat constructor uses W,X,Y,Z
    return glm::quat{
        lua_val_get<float>(L, arr->array + 3),
        lua_val_get<float>(L, arr->array),
        lua_val_get<float>(L, arr->array + 1),
        lua_val_get<float>(L, arr->array + 2),
    };
}

glm::mat3 get_raw(lua_State* L, Table* arr, Overload<glm::mat3>)
{
    return glm::mat3{
        lua_val_get<float>(L, arr->array),
        lua_val_get<float>(L, arr->array + 1),
        lua_val_get<float>(L, arr->array + 2),
        lua_val_get<float>(L, arr->array + 3),
        lua_val_get<float>(L, arr->array + 4),
        lua_val_get<float>(L, arr->array + 5),
        lua_val_get<float>(L, arr->array + 6),
        lua_val_get<float>(L, arr->array + 7),
        lua_val_get<float>(L, arr->array + 8),
    };
}

glm::mat3x4 get_raw(lua_State* L, Table* arr, Overload<glm::mat3x4>)
{
    return glm::mat3x4{
        lua_val_get<float>(L, arr->array),
        lua_val_get<float>(L, arr->array + 1),
        lua_val_get<float>(L, arr->array + 2),
        lua_val_get<float>(L, arr->array + 3),
        lua_val_get<float>(L, arr->array + 4),
        lua_val_get<float>(L, arr->array + 5),
        lua_val_get<float>(L, arr->array + 6),
        lua_val_get<float>(L, arr->array + 7),
        lua_val_get<float>(L, arr->array + 8),
        lua_val_get<float>(L, arr->array + 9),
        lua_val_get<float>(L, arr->array + 10),
        lua_val_get<float>(L, arr->array + 11),
    };
}

glm::mat4x3 get_raw(lua_State* L, Table* arr, Overload<glm::mat4x3>)
{
    return glm::mat4x3{
        lua_val_get<float>(L, arr->array),
        lua_val_get<float>(L, arr->array + 1),
        lua_val_get<float>(L, arr->array + 2),
        lua_val_get<float>(L, arr->array + 3),
        lua_val_get<float>(L, arr->array + 4),
        lua_val_get<float>(L, arr->array + 5),
        lua_val_get<float>(L, arr->array + 6),
        lua_val_get<float>(L, arr->array + 7),
        lua_val_get<float>(L, arr->array + 8),
        lua_val_get<float>(L, arr->array + 9),
        lua_val_get<float>(L, arr->array + 10),
        lua_val_get<float>(L, arr->array + 11),
    };
}

glm::mat4 get_raw(lua_State* L, Table* arr, Overload<glm::mat4>)
{
    return glm::mat4{
        lua_val_get<float>(L, arr->array),
        lua_val_get<float>(L, arr->array + 1),
        lua_val_get<float>(L, arr->array + 2),
        lua_val_get<float>(L, arr->array + 3),
        lua_val_get<float>(L, arr->array + 4),
        lua_val_get<float>(L, arr->array + 5),
        lua_val_get<float>(L, arr->array + 6),
        lua_val_get<float>(L, arr->array + 7),
        lua_val_get<float>(L, arr->array + 8),
        lua_val_get<float>(L, arr->array + 9),
        lua_val_get<float>(L, arr->array + 10),
        lua_val_get<float>(L, arr->array + 11),
        lua_val_get<float>(L, arr->array + 12),
        lua_val_get<float>(L, arr->array + 13),
        lua_val_get<float>(L, arr->array + 14),
        lua_val_get<float>(L, arr->array + 15),
    };
}

template <glm::length_t N, typename T, glm::qualifier Q>
glm::vec<N, T, Q> do_get(lua_State* L, int index, Overload<glm::vec<N, T, Q>>)
{
    auto i = lua_absindex(L, index);
    auto arr = lua_get_array_n(L, i, N);
    return get_raw(L, arr, Overload<glm::vec<N, T, Q>>{});
}

glm::quat do_get(lua_State* L, int index, Overload<glm::quat>)
{
    auto i = lua_absindex(L, index);
    auto arr = lua_get_array_n(L, i, 4);
    return get_raw(L, arr, Overload<glm::quat>{});
}

glm::mat3 do_get(lua_State* L, int index, Overload<glm::mat3>)
{
    auto i = lua_absindex(L, index);
    auto arr = lua_get_array_n(L, i, 9);
    return get_raw(L, arr, Overload<glm::mat3>{});
}

glm::mat3x4 do_get(lua_State* L, int index, Overload<glm::mat3x4>)
{
    auto i = lua_absindex(L, index);
    auto arr = lua_get_array_n(L, i, 12);
    return get_raw(L, arr, Overload<glm::mat3x4>{});
}

glm::mat4x3 do_get(lua_State* L, int index, Overload<glm::mat4x3>)
{
    auto i = lua_absindex(L, index);
    auto arr = lua_get_array_n(L, i, 12);
    return get_raw(L, arr, Overload<glm::mat4x3>{});
}

glm::mat4 do_get(lua_State* L, int index, Overload<glm::mat4>)
{
    auto i = lua_absindex(L, index);
    auto arr = lua_get_array_n(L, i, 16);
    return get_raw(L, arr, Overload<glm::mat4>{});
}

MathParam do_get(lua_State* L, int index, Overload<MathParam>)
{
    auto i = lua_absindex(L, index);
    auto arg = lua_index2addr(L, i);
    MathParam val;
    if (ttisnumber(arg)) {
        val.f = lua_val_get<float>(L, arg);
        val.Arity = 1;
    } else if (ttistable(arg)) {
        auto tab = hvalue(arg);
        if (!tab->sizearray && tab->lastfree) {
            luaL_error(L, "Param %d must be an array-like table, not a hashtable", index);
        }

        val.Arity = lua_get_array_size(tab);
        switch (val.Arity) {
        case 3: val.vec3 = get_raw(L, tab, Overload<glm::vec3>{}); break;
        case 4: val.vec4 = get_raw(L, tab, Overload<glm::vec4>{}); break;
        case 9: val.mat3 = get_raw(L, tab, Overload<glm::mat3>{}); break;
        case 16: val.mat4 = get_raw(L, tab, Overload<glm::mat4>{}); break;
        default: luaL_error(L, "Param %d: Unsupported vector or matrix size (%d)", index, val.Arity); break;
        }
    } else {
        luaL_error(L, "Param %d: expected a table, got %s", index, GetDebugName(L, index));
        val.Arity = 0;
    }

    return val;
}

bg3se::stats::ConditionId do_get(lua_State* L, int index, Overload<bg3se::stats::ConditionId>)
{
    auto conditions = get<std::optional<StringView>>(L, index);
    if (!conditions) {
        return {};
    }

    auto id = GetStaticSymbols().GetStats()->GetOrCreateConditions(STDString(*conditions));
    return bg3se::stats::ConditionId{ id };
}

StatsExpressionRef do_get(lua_State* L, int index, Overload<StatsExpressionRef>)
{
    // If it's a string, parse through the dynamic expression manager
    auto str = get<std::optional<StringView>>(L, index);
    if (str) {
        auto manager = GetStaticSymbols().eoc__DynamicStatsExpressionManager;
        Guid hash = StatsExpressionInternal::Hash(*str);
        return (*manager)->CreateStatsExpression(hash, *str);
    }

    // If it's a refcounted stats expression, reference it directly
    auto expr = get<StatsExpressionPooled*>(L, index);
    return StatsExpressionRef(expr);
}

EntityOrVec3Variant do_get(lua_State* L, int index, Overload<EntityOrVec3Variant>)
{
    EntityOrVec3Variant v;
    if (lua_type(L, index) == LUA_TTABLE) {
        v.Position = do_get(L, index, Overload<glm::vec3>{});
        v.Type = 1;
    } else {
        v.Entity = do_get(L, index, Overload<EntityHandle>{});
        v.Type = 0;
    }

    return v;
}

Ref do_get(lua_State* L, int index, Overload<Ref>)
{
    return Ref(L, index);
}

RegistryEntry do_get(lua_State* L, int index, Overload<RegistryEntry>)
{
    return RegistryEntry(L, index);
}

PersistentRegistryEntry do_get(lua_State* L, int index, Overload<PersistentRegistryEntry>)
{
    return PersistentRegistryEntry(L, index);
}

END_NS()
