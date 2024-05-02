BEGIN_NS(lua)

RegistryEntry::RegistryEntry()
	: L_(nullptr), ref_(-1)
{}

RegistryEntry::RegistryEntry(lua_State * L, int index)
	: L_(L)
{
	lua_pushvalue(L, index);
	ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
}

RegistryEntry::RegistryEntry(lua_State* L, Ref const& local)
	: L_(L)
{
	if ((bool)local) {
		local.Push(L);
		ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
	} else {
		ref_ = -1;
	}
}

RegistryEntry::~RegistryEntry()
{
	if (ref_ != -1) {
		luaL_unref(L_, LUA_REGISTRYINDEX, ref_);
	}
}

RegistryEntry::RegistryEntry(RegistryEntry const& other)
	: L_(other.L_), ref_(-1)
{
	if (other.ref_ != -1) {
		other.Push();
		ref_ = luaL_ref(L_, LUA_REGISTRYINDEX);
	}
}

RegistryEntry::RegistryEntry(RegistryEntry && other)
	: L_(other.L_), ref_(other.ref_)
{
	other.ref_ = -1;
}

RegistryEntry & RegistryEntry::operator = (RegistryEntry const& other)
{
	if (ref_ != -1) {
		luaL_unref(L_, LUA_REGISTRYINDEX, ref_);
	}

	L_ = other.L_;
	ref_ = other.ref_;

	if (other.ref_ != -1) {
		other.Push();
		ref_ = luaL_ref(L_, LUA_REGISTRYINDEX);
	}

	return *this;
}

RegistryEntry & RegistryEntry::operator = (RegistryEntry && other)
{
	if (ref_ != -1) {
		luaL_unref(L_, LUA_REGISTRYINDEX, ref_);
	}

	L_ = other.L_;
	ref_ = other.ref_;
	other.ref_ = -1;
	return *this;
}

void RegistryEntry::Push() const
{
	assert(ref_ != -1);
	lua_rawgeti(L_, LUA_REGISTRYINDEX, ref_);
}

void RegistryEntry::Bind(lua_State* L, Ref const& ref)
{
	assert(ref_ == -1);

	L_ = L;
	if (ref) {
		ref.Push(L);
		ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
	} else {
		ref_ = -1;
	}
}

void RegistryEntry::Reset()
{
	if (ref_ != -1) {
		luaL_unref(L_, LUA_REGISTRYINDEX, ref_);
		ref_ = -1;
		L_ = nullptr;
	}
}


PersistentRegistryEntry::PersistentRegistryEntry()
	: L_(nullptr), generationId_(0), ref_(-1)
{}

PersistentRegistryEntry::PersistentRegistryEntry(lua_State * L, int index)
	: L_(L), generationId_(get_generation_id(L))
{
	lua_pushvalue(L, index);
	ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
}

PersistentRegistryEntry::PersistentRegistryEntry(lua_State* L, Ref const& local)
	: L_(L), generationId_(get_generation_id(L))
{
	if ((bool)local) {
		local.Push(L);
		ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
	} else {
		ref_ = -1;
	}
}

PersistentRegistryEntry::~PersistentRegistryEntry()
{
	Release();
}

PersistentRegistryEntry::PersistentRegistryEntry(PersistentRegistryEntry&& other)
	: L_(other.L_), generationId_(other.generationId_), ref_(other.ref_)
{
	other.ref_ = -1;
}

PersistentRegistryEntry& PersistentRegistryEntry::operator = (PersistentRegistryEntry&& other)
{
	Release();

	L_ = other.L_;
	generationId_ = other.generationId_;
	ref_ = other.ref_;
	other.ref_ = -1;
	return *this;
}

void PersistentRegistryEntry::Release()
{
	if (ref_ != -1) {
		auto state = gExtender->GetCurrentExtensionState()->GetLua();
		if (state && state->GetGenerationId() == generationId_) {
			luaL_unref(L_, LUA_REGISTRYINDEX, ref_);
		}

		ref_ = -1;
	}
}

bool PersistentRegistryEntry::IsValid(lua_State* L) const
{
	return L != nullptr 
		&& ref_ != -1
		&& State::FromLua(L)->GetGenerationId() == generationId_;
}

bool PersistentRegistryEntry::TryPush(lua_State* L) const
{
	if (IsValid(L)) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref_);
		return true;
	} else {
		return false;
	}
}

void PersistentRegistryEntry::Bind(lua_State* L, Ref const& ref)
{
	assert(ref_ == -1);

	L_ = L;
	generationId_ = get_generation_id(L);
	if (ref) {
		ref.Push(L);
		ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
	} else {
		ref_ = -1;
	}
}



void RawComponentRef::Push(lua_State* L) const
{
	if (ptr_ != nullptr) {
		auto state = State::FromLua(L);
		auto ecs = state->GetEntitySystemHelpers();
		auto ptr = reinterpret_cast<void*>((uintptr_t)ptr_ & 0x0000ffffffffffffull);
		ecs::ComponentTypeIndex type{ (uint16_t)((uintptr_t)ptr_ >> 48) };
		auto extType = ecs->GetComponentType(type);
		if (extType) {
			PushComponent(L, ptr, *extType, state->GetCurrentLifetime());
		} else {
			push(L, nullptr);
		}
	} else {
		push(L, nullptr);
	}
}

END_NS()