BEGIN_NS(lua)

enum class MetatableTag : uint8_t
{
	ObjectProxyByRef = 0,
	ArrayProxy = 1,
	MapProxy = 2,
	SetProxy = 3,
	EnumValue = 4,
	BitfieldValue = 5,
	UserVariableHolder = 6,
	ModVariableHolder = 7,
	Entity = 8,
	ImguiObject = 9,
	OsiFunctionName = 10,
	Max = OsiFunctionName
};

struct LuaInternalState;
LuaInternalState* lua_new_internal_state();
void lua_release_internal_state(LuaInternalState* state);

// Object API for storing pointer-like data in a Lua TValue.
void lua_push_cppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void* object, LifetimeHandle const& lifetime);
void lua_push_cppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void const* object, LifetimeHandle const& lifetime);
void lua_get_cppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppObjectMetadata& obj);
void lua_get_cppobject(lua_State* L, int idx, CppObjectMetadata& obj);
bool lua_try_get_cppobject(lua_State* L, int idx, CppObjectMetadata& obj);
bool lua_try_get_cppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppObjectMetadata& obj);

// Value API for storing raw 64-bit data in a Lua TValue.
void lua_push_cppvalue(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint64_t object);
void lua_get_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppValueMetadata& obj);
void lua_get_cppvalue(lua_State* L, int idx, CppValueMetadata& obj);
bool lua_try_get_cppvalue(lua_State* L, int idx, CppValueMetadata& obj);
bool lua_try_get_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppValueMetadata& obj);

void* LuaCppAlloc(lua_State* L, size_t size);
void LuaCppFree(lua_State* L, void* block, size_t size);
CMetatable* LuaCppGetLightMetatable(lua_State* L, unsigned long long val, unsigned long long extra);
CMetatable* LuaCppGetMetatable(lua_State* L, void* val, unsigned long long extra);
void* LuaCppCanonicalize(lua_State* L, void* val);

void LuaCacheString(lua_State* L, TString* s);
void LuaReleaseString(lua_State* L, TString* s);

END_NS()
