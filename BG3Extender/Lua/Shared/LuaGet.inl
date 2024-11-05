BEGIN_NS(lua)

Table* lua_get_array(lua_State* L, int idx)
{
	auto val = lua_index2addr(L, idx);
	if (!ttistable(val)) {
		luaL_error(L, "Param %d: expected a table", idx);
		return nullptr;
	}

	auto tab = hvalue(val);
	if (tab->lsizenode > 0) {
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

float lua_val_get_float(lua_State* L, TValue* val)
{
	if (ttisfloat(val)) {
		return (float)fltvalue(val);
	} else if (ttisinteger(val)) {
		return (float)ivalue(val);
	} else {
		luaL_error(L, "Expected number value, got %s", lua_typename(L, ttnov(val)));
		return 0.0f;
	}
}

glm::vec2 get_raw(lua_State* L, Table* arr, Overload<glm::vec2>)
{
	return glm::vec2{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
	};
}

glm::vec3 get_raw(lua_State* L, Table* arr, Overload<glm::vec3>)
{
	return glm::vec3{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
	};
}

glm::vec4 get_raw(lua_State* L, Table* arr, Overload<glm::vec4>)
{
	return glm::vec4{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
	};
}

glm::aligned_highp_vec4 get_raw(lua_State* L, Table* arr, Overload<glm::aligned_highp_vec4>)
{
	return glm::aligned_highp_vec4{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
	};
}

glm::quat get_raw(lua_State* L, Table* arr, Overload<glm::quat>)
{
	// quat constructor uses W,X,Y,Z
	return glm::quat{
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
	};
}

glm::mat3 get_raw(lua_State* L, Table* arr, Overload<glm::mat3>)
{
	return glm::mat3{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
	};
}

glm::mat3x4 get_raw(lua_State* L, Table* arr, Overload<glm::mat3x4>)
{
	return glm::mat3x4{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
		lua_val_get_float(L, arr->array + 9),
		lua_val_get_float(L, arr->array + 10),
		lua_val_get_float(L, arr->array + 11),
	};
}

glm::mat4x3 get_raw(lua_State* L, Table* arr, Overload<glm::mat4x3>)
{
	return glm::mat4x3{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
		lua_val_get_float(L, arr->array + 9),
		lua_val_get_float(L, arr->array + 10),
		lua_val_get_float(L, arr->array + 11),
	};
}

glm::mat4 get_raw(lua_State* L, Table* arr, Overload<glm::mat4>)
{
	return glm::mat4{
		lua_val_get_float(L, arr->array),
		lua_val_get_float(L, arr->array + 1),
		lua_val_get_float(L, arr->array + 2),
		lua_val_get_float(L, arr->array + 3),
		lua_val_get_float(L, arr->array + 4),
		lua_val_get_float(L, arr->array + 5),
		lua_val_get_float(L, arr->array + 6),
		lua_val_get_float(L, arr->array + 7),
		lua_val_get_float(L, arr->array + 8),
		lua_val_get_float(L, arr->array + 9),
		lua_val_get_float(L, arr->array + 10),
		lua_val_get_float(L, arr->array + 11),
		lua_val_get_float(L, arr->array + 12),
		lua_val_get_float(L, arr->array + 13),
		lua_val_get_float(L, arr->array + 14),
		lua_val_get_float(L, arr->array + 15),
	};
}

glm::vec2 do_get(lua_State* L, int index, Overload<glm::vec2>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 2);
	return get_raw(L, arr, Overload<glm::vec2>{});
}

glm::vec3 do_get(lua_State* L, int index, Overload<glm::vec3>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 3);
	return get_raw(L, arr, Overload<glm::vec3>{});
}

glm::vec4 do_get(lua_State* L, int index, Overload<glm::vec4>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 4);
	return get_raw(L, arr, Overload<glm::vec4>{});
}

glm::aligned_highp_vec4 do_get(lua_State* L, int index, Overload<glm::aligned_highp_vec4>)
{
	auto i = lua_absindex(L, index);
	auto arr = lua_get_array_n(L, i, 4);
	return get_raw(L, arr, Overload<glm::aligned_highp_vec4>{});
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
		val.f = lua_val_get_float(L, arg);
		val.Arity = 1;
	} else if (ttistable(arg)) {
		auto tab = hvalue(arg);
		if (tab->lsizenode > 0) {
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
		luaL_error(L, "Param %d: expected a table", index);
		val.Arity = 0;
	}

	return val;
}

stats::ConditionId do_get(lua_State* L, int index, Overload<stats::ConditionId>)
{
	luaL_error(L, "Setting ConditionId values is not supported");
	return {};
}

Ref do_get(lua_State* L, int index, Overload<Ref>)
{
	return Ref(L, index);
}

RegistryEntry do_get(lua_State* L, int index, Overload<RegistryEntry>)
{
	return RegistryEntry(L, index);
}

PersistentRef do_get(lua_State* L, int index, Overload<PersistentRef>)
{
	return PersistentRef(L, index);
}

PersistentRegistryEntry do_get(lua_State* L, int index, Overload<PersistentRegistryEntry>)
{
	return PersistentRegistryEntry(L, index);
}

END_NS()
