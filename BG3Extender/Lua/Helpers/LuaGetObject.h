#pragma once

BEGIN_NS(lua)

template <class T>
typename std::enable_if_t<!IsByVal<T>, T> do_get(lua_State* L, int index, Overload<T>)
{
	T val;
	Unserialize(L, index, &val);
	return val;
}

END_NS()
