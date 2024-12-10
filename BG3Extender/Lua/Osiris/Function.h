#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

class OsirisBinding;

class OsiFunction
{
public:
    inline bool IsBound() const
    {
        return function_ != nullptr;
    }

    inline bool IsDB() const
    {
        return IsBound() && function_->Type == FunctionType::Database;
    }

    inline OsiFunctionDef const* GetOsiFunction() const
    {
        return function_;
    }

    bool Bind(OsiFunctionDef const * func, OsirisBinding & binding);
    void Unbind();

    int LuaCall(lua_State * L) const;
    int LuaGet(lua_State * L) const;
    int LuaDelete(lua_State * L) const;
    int LuaDeferredNotification(lua_State * L) const;

private:
    OsiFunctionDef const * function_{ nullptr };
    AdapterRef adapter_;

    void OsiCall(lua_State * L) const;
    void OsiDeferredNotification(lua_State * L) const;
    void OsiInsert(lua_State * L, bool deleteTuple) const;
    int OsiQuery(lua_State * L) const;
    int OsiUserQuery(lua_State * L) const;

    bool MatchTuple(lua_State * L, int firstIndex, TupleVec const & tuple) const;
    void ConstructTuple(lua_State * L, TupleVec const & tuple) const;
};

END_NS()
