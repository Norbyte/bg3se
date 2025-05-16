BEGIN_NS(lua)

enum class MetatableTag : uint8_t
{
    ObjectRef = 0,
    Array = 1,
    Map = 2,
    Set = 3,
    EnumValue = 4,
    BitfieldValue = 5,
    UserVariableHolder = 6,
    ModVariableHolder = 7,
    Entity = 8,
    ImguiObject = 9,
    OsiFunctionName = 10,
    ObjectVal = 11,
    SystemMap = 12,
    Max = SystemMap
};

struct LuaInternalState;
LuaInternalState* lua_new_internal_state();
void lua_release_internal_state(LuaInternalState* state);

// Object API for storing pointer-like data in a Lua TValue.
void lua_push_lightcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void* object, LifetimeHandle lifetime);
void lua_push_lightcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void const* object, LifetimeHandle lifetime);
CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx);
CppObjectMetadata lua_get_unchecked_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag);
CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag);
CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag, int expectedPropertyMap);
CppObjectOpaque* lua_get_opaque_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag);
bool lua_try_get_lightcppobject(lua_State* L, int idx, CppObjectMetadata& obj);
bool lua_try_get_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppObjectMetadata& obj);

LifetimeHandle lua_get_opaque_lifetime(CppObjectOpaque* meta);
uint16_t lua_get_opaque_property_map(CppObjectOpaque* meta);
void* lua_get_opaque_ptr(CppObjectOpaque* meta);

void* lua_push_newcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint32_t size);

// Value API for storing raw 64-bit data in a Lua TValue.
void lua_push_cppvalue(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint64_t object);
CppObjectMetadata lua_get_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag);
CppObjectMetadata lua_get_cppvalue(lua_State* L, int idx);
bool lua_try_get_cppvalue(lua_State* L, int idx, CppObjectMetadata& obj);
bool lua_try_get_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppObjectMetadata& obj);

CppValueOpaque* lua_get_opaque_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag);

uint32_t lua_get_opaque_property_map(CppValueOpaque* meta);
uint64_t lua_get_opaque_value(CppValueOpaque* meta);

// Object/Value combined getter API
CppObjectMetadata lua_get_lightcppany(lua_State* L, int idx);

void* LuaCppAlloc(lua_State* L, size_t size);
void LuaCppFree(lua_State* L, void* block, size_t size);
CMetatable* LuaCppGetLightMetatable(lua_State* L, unsigned long long val, unsigned long long extra);
CMetatable* LuaCppGetMetatable(lua_State* L, void* val, unsigned long long extra);
void LuaCppFinalize(lua_State* L, void* udata);
void* LuaCppCanonicalize(lua_State* L, void* val);

void LuaCacheString(lua_State* L, TString* s);
void LuaReleaseString(lua_State* L, TString* s);

END_NS()
