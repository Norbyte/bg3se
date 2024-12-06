#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

class OsirisBinding;
class ServerState;

void LuaToOsi(lua_State * L, int i, TypedValue & tv, ValueType osiType, bool allowNil = false);
TypedValue * LuaToOsi(lua_State * L, int i, ValueType osiType, bool allowNil = false);
void LuaToOsi(lua_State * L, int i, OsiArgumentValue & arg, ValueType osiType, bool allowNil = false, bool reuseStrings = false);
void OsiToLua(lua_State * L, OsiArgumentValue const & arg);
void OsiToLua(lua_State * L, TypedValue const & tv);
OsiFunctionDef const* LookupOsiFunction(STDString const& name, uint32_t arity);

inline void OsiReleaseArgument(OsiArgumentDesc & arg)
{
    arg.NextParam = nullptr;
}

inline void OsiReleaseArgument(TypedValue & arg) {}
inline void OsiReleaseArgument(ListNode<TypedValue *> & arg) {}
inline void OsiReleaseArgument(ListNode<TupleLL::Item> & arg) {}

template <class T>
class OsiArgumentPool
{
public:
    OsiArgumentPool()
    {
        argumentPool_.resize(1024);
        usedArguments_ = 0;
    }

    T * AllocateArguments(uint32_t num, uint32_t & tail)
    {
        if (usedArguments_ + num > argumentPool_.size()) {
            throw std::runtime_error("Ran out of argument descriptors");
        }

        tail = usedArguments_;
        auto ptr = argumentPool_.data() + usedArguments_;
        for (uint32_t i = 0; i < num; i++) {
            new (ptr + i) T();
        }

        usedArguments_ += num;
        return ptr;
    }

    void ReleaseArguments(uint32_t tail, uint32_t num)
    {
        if (tail + num != usedArguments_) {
            throw std::runtime_error("Attempted to release arguments out of order");
        }

        for (uint32_t i = 0; i < num; i++) {
            OsiReleaseArgument(argumentPool_[tail + i]);
        }

        usedArguments_ -= num;
    }

private:
    std::vector<T> argumentPool_;
    uint32_t usedArguments_;
};

template <class T>
class OsiArgumentListPin
{
public:
    inline OsiArgumentListPin(OsiArgumentPool<T> & pool, uint32_t numArgs)
        : pool_(pool), numArgs_(numArgs)
    {
        args_ = pool.AllocateArguments(numArgs_, tail_);
    }

    inline ~OsiArgumentListPin()
    {
        pool_.ReleaseArguments(tail_, numArgs_);
    }

    inline T * Args() const
    {
        return args_;
    }

private:
    OsiArgumentPool<T> & pool_;
    uint32_t numArgs_;
    uint32_t tail_;
    T * args_;
};

END_NS()
