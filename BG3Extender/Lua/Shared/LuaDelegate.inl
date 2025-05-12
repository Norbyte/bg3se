BEGIN_NS(lua)

void DeferredLuaDelegateQueue::Flush(lua_State* L)
{
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
