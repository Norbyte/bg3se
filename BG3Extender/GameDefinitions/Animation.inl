#pragma once

#include <GameDefinitions/Animation.h>
#include <Lua/LuaHelpers.h>

BEGIN_NS(gn)

using namespace bg3se::lua;

FixedString GenomeVariant::GetTypeName() const
{
	return Type ? Type->TypeName : FixedString{};
}


UserReturn GenomeVariant::LuaGetValue(lua_State* L) const
{
	if (Type == nullptr) {
		push(L, nullptr);
		return 1;
	}

	if (Type->TypeName == GFS.strInt 
		|| Type->TypeName == GFS.strEnum) {
		push(L, *reinterpret_cast<int32_t const*>(&Value));
	} else if (Type->TypeName == GFS.strGuid) {
		push(L, *reinterpret_cast<Guid const*>(Value));
	} else if (Type->TypeName == GFS.strFixedString 
		|| Type->TypeName == GFS.strAnimationSimpleName
		|| Type->TypeName == GFS.strAnimationSetID
		|| Type->TypeName == GFS.strAnimationComplexName) {
		push(L, *reinterpret_cast<FixedString const*>(&Value));
	} else if (Type->TypeName == GFS.strString 
		|| Type->TypeName == GFS.strPath) {
		push(L, *reinterpret_cast<STDString const*>(Value));
	} else if (Type->TypeName == GFS.strVector3) {
		push(L, *reinterpret_cast<glm::vec3 const*>(Value));
	} else if (Type->TypeName == GFS.strRotator3) {
		push(L, *reinterpret_cast<glm::quat const*>(Value));
	} else if (Type->TypeName == GFS.strFloat3x3) {
		push(L, *reinterpret_cast<glm::mat3 const*>(Value));
	} else if (Type->TypeName == GFS.strFloat4x4) {
		push(L, *reinterpret_cast<glm::mat4 const*>(Value));
	} else {
		push(L, nullptr);
	}

	return 1;
}

END_NS()
