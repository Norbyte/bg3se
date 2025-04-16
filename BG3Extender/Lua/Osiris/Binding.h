#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>
#include <Osiris/Shared/OsirisHelpers.h>
#include <Osiris/Shared/CustomFunctions.h>
#include <Lua/Osiris/ValueHelpers.h>
#include <Lua/Osiris/NameCache.h>
#include <Lua/Osiris/CallbackManager.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

class OsirisBinding;
class ServerState;

class OsirisBinding : Noncopyable<OsirisBinding>
{
public:
    OsirisBinding(ExtensionState& state);
    ~OsirisBinding();

    inline uint32_t GenerationId() const
    {
        return generationId_;
    }

    inline IdentityAdapterMap & GetIdentityAdapterMap()
    {
        return identityAdapters_;
    }

    inline OsirisNameResolver& GetNameResolver()
    {
        return resolver_;
    }

    inline OsiArgumentPool<OsiArgumentDesc> & GetArgumentDescPool()
    {
        return argDescPool_;
    }

    inline OsiArgumentPool<TypedValue> & GetTypedValuePool()
    {
        return tvPool_;
    }

    inline OsiArgumentPool<ListNode<TypedValue *>> & GetTypedValueNodePool()
    {
        return tvNodePool_;
    }

    inline OsirisCallbackManager& GetOsirisCallbacks()
    {
        return osirisCallbacks_;
    }

    void StoryLoaded();
    void StorySetMerging(bool isMerging);

private:
    OsiArgumentPool<OsiArgumentDesc> argDescPool_;
    OsiArgumentPool<TypedValue> tvPool_;
    OsiArgumentPool<ListNode<TypedValue *>> tvNodePool_;
    IdentityAdapterMap identityAdapters_;
    OsirisNameResolver resolver_;
    // ID of current story instance.
    // Used to invalidate function/node pointers in Lua userdata objects
    uint32_t generationId_{ 0 };
    OsirisCallbackManager osirisCallbacks_;
};

END_NS()
