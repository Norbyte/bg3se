/// <lua_module>Log</lua_module>
BEGIN_NS(lua::log)

void ArgsToStream(lua_State * L, std::stringstream & ss)
{
	int nargs = lua_gettop(L);  /* number of arguments */

	lua_getglobal(L, "tostring");
	for (int i = 1; i <= nargs; i++) {
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		const char * str = lua_tostring(L, -1);  /* get result */
		if (str == nullptr)
			luaL_error(L, "'tostring' must return a string to 'print'");
		if (i > 1) ss << "\t";
		ss << str;
		lua_pop(L, 1);  /* pop result */
	}
	lua_pop(L, 1); // pop tostring
}


void Print(lua_State* L)
{
	std::stringstream ss;
	ArgsToStream(L, ss);
	gExtender->LogOsirisMsg(ss.str());
}

void PrintWarning(lua_State* L)
{
	std::stringstream ss;
	ArgsToStream(L, ss);
	gExtender->LogOsirisWarning(ss.str());
}

void PrintError(lua_State* L)
{
	std::stringstream ss;
	ArgsToStream(L, ss);
	gExtender->LogLuaError(ss.str());
}

void RegisterLogLib()
{
	DECLARE_MODULE(Log, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(Print)
	MODULE_FUNCTION(PrintError)
	MODULE_FUNCTION(PrintWarning)
	END_MODULE()
}

END_NS()
