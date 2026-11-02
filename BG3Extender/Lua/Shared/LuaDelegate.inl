BEGIN_NS(lua)

void DeferredLuaDelegateQueue::Flush()
{
    if (queue_.empty()) return;

    OPTICK_D_EVENT(*description_);
    Array<GenericDeferredLuaDelegateCall*> cur;
    std::swap(cur, queue_);
    for (auto const& call : cur) {
        call->Call(state_.GetState());
        GameDelete(call);
    }
}

END_NS()
