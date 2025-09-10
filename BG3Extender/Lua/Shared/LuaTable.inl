
#include <lvm.h>
#include <ltable.h>

BEGIN_NS(lua)

UserReturn FindValueTable(lua_State* L, TValue* tabval, TValue* value)
{
    auto tab = hvalue(tabval);
    
    if (tab->sizearray) {
        auto sz = tab->sizearray;
        for (unsigned i = 0; i < sz; i++) {
            if (luaV_rawequalobj(tab->array + i, value)) {
                push(L, i + 1);
                return 1;
            }
        }
    } else if (tab->lastfree) {
        auto nodes = sizenode(tab);
        for (unsigned i = 0; i < nodes; i++) {
            auto node = tab->node + i;
            if (luaV_rawequalobj(gval(node), value)) {
                setobj2s(L, L->top, gkey(node));
                api_incr_top(L);
                return 1;
            }
        }
    }

    push(L, nullptr);
    return 1;
}

UserReturn FindValueArray(lua_State* L, CppObjectMetadata& meta, TValue* value)
{
    auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(meta.PropertyMapTag);

    auto sz = impl->Length(meta);
    for (unsigned i = 0; i < sz; i++) {
        if (impl->GetElement(L, meta, i + 1)) {
            auto eq = luaV_rawequalobj(index2addr(L, -1), value);
            lua_pop(L, 1);
            if (eq) {
                push(L, i + 1);
                return 1;
            }
        }
    }

    push(L, nullptr);
    return 1;
}

UserReturn FindValueMap(lua_State* L, CppObjectMetadata& meta, TValue* value)
{
    auto impl = gExtender->GetPropertyMapManager().GetMapProxy(meta.PropertyMapTag);

    if (impl->IsLinear()) {
        auto sz = impl->Length(meta);
        for (unsigned i = 0; i < sz; i++) {
            if (impl->GetValueAt(L, meta, i)) {
                auto eq = luaV_rawequalobj(index2addr(L, -1), value);
                lua_pop(L, 1);
                if (eq) {
                    push(L, i + 1);
                    return 1;
                }
            }
        }
    } else {
        push(L, nullptr);
        while (impl->Next(L, meta, -1)) {
            auto eq = luaV_rawequalobj(index2addr(L, -1), value);
            lua_pop(L, 1);
            if (eq) {
                return 1;
            }
        }
    }

    push(L, nullptr);
    return 1;
}

UserReturn FindValueSet(lua_State* L, CppObjectMetadata& meta, TValue* value)
{
    auto impl = gExtender->GetPropertyMapManager().GetSetProxy(meta.PropertyMapTag);

    auto sz = impl->Length(meta);
    for (unsigned i = 0; i < sz; i++) {
        if (impl->GetElementAt(L, meta, i + 1)) {
            auto eq = luaV_rawequalobj(index2addr(L, -1), value);
            if (eq) {
                return 1;
            }
            lua_pop(L, 1);
        }
    }

    push(L, nullptr);
    return 1;
}

UserReturn FindValueUserdata(lua_State* L, TValue* tabval, TValue* value)
{
    auto meta = lua_get_cppvalue(L, tabval);
    switch (meta.MetatableTag) {
    case MetatableTag::Array: return FindValueArray(L, meta, value);
    case MetatableTag::Map: return FindValueMap(L, meta, value);
    case MetatableTag::Set: return FindValueSet(L, meta, value);
    default:
        return luaL_error(L, "Expected an array, map or set value; got %s", GetDebugName(meta));
    }
}

UserReturn FindValue(lua_State* L, int tableIndex, int valueIndex)
{
    StackCheck _(L, 1);
    auto tabval = index2addr(L, tableIndex);
    auto value = index2addr(L, valueIndex);

    if (ttistable(tabval)) {
        return FindValueTable(L, tabval, value);
    } else if (ttislightcppobject(tabval)) {
        return FindValueUserdata(L, tabval, value);
    } else if (ttisnil(tabval)) {
        push(L, nullptr);
        return 1;
    } else {
        return luaL_error(L, "Expected a table or userdata value; got %s", GetDebugName(L, tableIndex));
    }
}

END_NS()
