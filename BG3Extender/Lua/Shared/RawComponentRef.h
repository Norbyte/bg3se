#pragma once

#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(lua)

class RawComponentRef
{
public:
	inline RawComponentRef()
		: ptr_(nullptr)
	{}
	
	inline RawComponentRef(void* ptr, ecs::ComponentTypeIndex type)
		: ptr_(ptr ? (void*)((uintptr_t)ptr | ((uintptr_t)type << 48)) : nullptr)
	{}

	explicit inline operator bool() const
	{
		return ptr_ != nullptr;
	}

	void Push(lua_State* L) const;

private:
	void* ptr_;
};

END_NS()
