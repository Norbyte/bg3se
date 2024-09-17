BEGIN_NS(lua)

void DeferredLuaDelegateQueue::Flush()
{
    Array<GenericDeferredLuaDelegateCall*> cur;
    std::swap(cur, queue_);
    for (auto const& call : cur) {
        call->Call();
        delete call;
    }
}

END_NS()
