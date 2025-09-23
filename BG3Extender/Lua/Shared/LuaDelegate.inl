BEGIN_NS(lua)

void DeferredLuaDelegateQueue::Flush(lua_State* L)
{
    if (queue_.empty()) return;

    OPTICK_D_EVENT(*description_);
    Array<GenericDeferredLuaDelegateCall*> cur;
    std::swap(cur, queue_);
    for (auto const& call : cur) {
        call->Call(L);
        GameDelete(call);
    }
}

void DeferredLuaDelegateQueue::Flush()
{
    LuaVirtualPin lua(GetCurrentExtensionState());
    if (lua) {
        Flush(lua->GetState());
    }
}

END_NS()
