BEGIN_NS(lua)


GlobalRefManager::GlobalRefManager(lua_State* L)
    : L_(L)
{
    ref_.reserve(0x1000);
}

GlobalRefManager::~GlobalRefManager()
{
#if !defined(NDEBUG) || defined(SE_RELEASE_ASSERTS)
    unsigned unreleased{ 0 };
    for (size_t i = 0; i < ref_.size(); i++) {
        if (ref_[i].RefCount > 0) {
            unreleased++;
        }
    }

    if (unreleased) {
        ERR("Leaked %d references from Lua global ref pool!", unreleased);
    }
#endif
}

int32_t GlobalRefManager::AddGlobalIndex(lua_State* L, int global)
{
    int32_t index;
    if (freeList_.try_pop(index)) {
        assert(ref_[index].RefCount == 0);
        ref_[index].Index = global;
        ref_[index].RefCount = 1;
    } else {
        auto it = ref_.push_back(GlobalEntry(global));
        index = (uint32_t)(it - ref_.begin());
    }

    return index;
}

void GlobalRefManager::IncRef(int32_t i)
{
    se_assert(i >= 0 && i < ref_.size());
    se_assert(ref_[i].RefCount > 0);
    ++ref_[i].RefCount;
}

void GlobalRefManager::DecRef(int32_t i)
{
    se_assert(i >= 0 && i < ref_.size());
    se_assert(ref_[i].RefCount > 0);

    if (--ref_[i].RefCount == 0) {
        EnterVMCheck(L_);
        luaL_unref(L_, LUA_REGISTRYINDEX, ref_[i].Index);
        freeList_.push(i);
    }
}

void GlobalRefManager::Push(lua_State* L, int32_t i)
{
    EnterVMCheck(L);
    se_assert(i >= 0 && i < ref_.size());
    se_assert(ref_[i].RefCount > 0);

    lua_rawgeti(L, LUA_REGISTRYINDEX, ref_[i].Index);
}

int32_t GlobalRefManager::GetGlobalIndex(int32_t i)
{
    se_assert(i >= 0 && i < ref_.size());
    se_assert(ref_[i].RefCount > 0);
    return ref_[i].Index;
}

RegistryEntry::RegistryEntry()
    : manager_(nullptr), global_(-1)
{}

RegistryEntry::RegistryEntry(lua_State* L, int index)
    : manager_(L ? &State::FromLua(L)->GetGlobals() : nullptr)
{
    EnterVMCheck(L);
    lua_pushvalue(L, index);
    auto ref = luaL_ref(L, LUA_REGISTRYINDEX);
    global_ = manager_->AddGlobalIndex(L, ref);
}

RegistryEntry::RegistryEntry(lua_State* L, Ref const& local)
    : manager_(L ? &State::FromLua(L)->GetGlobals() : nullptr)
{
    if ((bool)local) {
        local.Push(L);
        EnterVMCheck(L);
        auto ref = luaL_ref(L, LUA_REGISTRYINDEX);
        global_ = manager_->AddGlobalIndex(L, ref);
    } else {
        global_ = -1;
    }
}

RegistryEntry::~RegistryEntry()
{
    if (global_ != -1) {
        manager_->DecRef(global_);
    }
}

RegistryEntry::RegistryEntry(RegistryEntry const& other)
    : manager_(other.manager_), global_(other.global_)
{
    if (global_ != -1) {
        manager_->IncRef(global_);
    }
}

RegistryEntry::RegistryEntry(RegistryEntry&& other) noexcept
    : manager_(other.manager_), global_(other.global_)
{
    other.global_ = -1;
}

RegistryEntry & RegistryEntry::operator = (RegistryEntry const& other)
{
    if (global_ != -1) {
        manager_->DecRef(global_);
    }

    manager_ = other.manager_;
    global_ = other.global_;

    if (global_ != -1) {
        manager_->IncRef(global_);
    }

    return *this;
}

RegistryEntry & RegistryEntry::operator = (RegistryEntry&& other) noexcept
{
    if (global_ != -1) {
        manager_->DecRef(global_);
    }

    manager_ = other.manager_;
    global_ = other.global_;
    other.global_ = -1;
    return *this;
}

void RegistryEntry::Push(lua_State* L) const
{
    se_assert(global_ != -1);
    manager_->Push(L, global_);
}

void RegistryEntry::Reset()
{
    if (global_ != -1) {
        manager_->DecRef(global_);
        global_ = -1;
    }
}


PersistentRegistryEntry::PersistentRegistryEntry()
    : manager_(nullptr), generationId_(0), global_(-1)
{}

PersistentRegistryEntry::PersistentRegistryEntry(lua_State * L, int index)
    : manager_(&State::FromLua(L)->GetGlobals()),
    generationId_(get_generation_id(L))
{
    EnterVMCheck(L);
    lua_pushvalue(L, index);
    auto ref = luaL_ref(L, LUA_REGISTRYINDEX);
    global_ = manager_->AddGlobalIndex(L, ref);
}

PersistentRegistryEntry::PersistentRegistryEntry(lua_State* L, Ref const& local)
    : manager_(&State::FromLua(L)->GetGlobals()),
    generationId_(get_generation_id(L))
{
    if ((bool)local) {
        local.Push(L);
        EnterVMCheck(L);
        auto ref = luaL_ref(L, LUA_REGISTRYINDEX);
        global_ = manager_->AddGlobalIndex(L, ref);
    } else {
        global_ = -1;
    }
}

PersistentRegistryEntry::~PersistentRegistryEntry()
{
    Release();
}

PersistentRegistryEntry::PersistentRegistryEntry(PersistentRegistryEntry const& other)
    : manager_(other.manager_), generationId_(other.generationId_), global_(other.global_)
{
    if (global_ != -1) {
        manager_->IncRef(global_);
    }
}

PersistentRegistryEntry::PersistentRegistryEntry(PersistentRegistryEntry&& other) noexcept
    : manager_(other.manager_), generationId_(other.generationId_), global_(other.global_)
{
    other.global_ = -1;
}

PersistentRegistryEntry& PersistentRegistryEntry::operator = (PersistentRegistryEntry const& other)
{
    Release();

    manager_ = other.manager_;
    generationId_ = other.generationId_;
    global_ = other.global_;

    if (global_ != -1) {
        manager_->IncRef(global_);
    }

    return *this;
}

PersistentRegistryEntry& PersistentRegistryEntry::operator = (PersistentRegistryEntry&& other)
{
    Release();

    manager_ = other.manager_;
    generationId_ = other.generationId_;
    global_ = other.global_;
    other.global_ = -1;
    return *this;
}

void PersistentRegistryEntry::Release()
{
    if (global_ != -1) {
        auto state = gExtender->GetCurrentExtensionState()->GetLua();
        if (state && state->GetGenerationId() == generationId_) {
            manager_->DecRef(global_);
        }

        global_ = -1;
    }
}

void PersistentRegistryEntry::Release(lua_State* L)
{
    if (global_ != -1) {
        if (State::FromLua(L)->GetGenerationId() == generationId_) {
            manager_->DecRef(global_);
        }

        global_ = -1;
    }
}

bool PersistentRegistryEntry::IsValid(lua_State* L) const
{
    return L != nullptr 
        && global_ != -1
        && State::FromLua(L)->GetGenerationId() == generationId_;
}

bool PersistentRegistryEntry::TryPush(lua_State* L) const
{
    if (IsValid(L)) {
        manager_->Push(L, global_);
        return true;
    } else {
        return false;
    }
}

Ref PersistentRegistryEntry::ToRef(lua_State* L) const
{
    if (IsValid(L)) {
        return Ref(L, RefType::Registry, manager_->GetGlobalIndex(global_));
    } else {
        return Ref();
    }
}

void PersistentRegistryEntry::Bind(lua_State* L, Ref const& ref)
{
    Release(L);

    manager_ = &State::FromLua(L)->GetGlobals();
    generationId_ = get_generation_id(L);
    if (ref) {
        EnterVMCheck(L);
        ref.Push(L);
        auto ref = luaL_ref(L, LUA_REGISTRYINDEX);
        global_ = manager_->AddGlobalIndex(L, ref);
    } else {
        global_ = -1;
    }
}



void RawComponentRef::Push(lua_State* L) const
{
    if (ptr_ != nullptr) {
        auto state = State::FromLua(L);
        auto ecs = state->GetEntitySystemHelpers();
        auto ptr = reinterpret_cast<void*>((uintptr_t)ptr_ & 0x0000ffffffffffffull);
        ecs::ComponentTypeIndex type{ (uint16_t)((uintptr_t)ptr_ >> 48) };
        auto meta = ecs->GetComponentMeta(type);
        if (meta && meta->Properties) {
            PushComponent(L, ptr, *meta->Properties, state->GetCurrentLifetime());
        } else {
            push(L, nullptr);
        }
    } else {
        push(L, nullptr);
    }
}

END_NS()