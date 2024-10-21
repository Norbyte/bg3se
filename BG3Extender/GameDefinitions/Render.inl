#pragma once

#include <GameDefinitions/RootTemplates.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

void MoveableObject::LuaSetWorldTranslate(glm::vec3 const& translate)
{
	SetWorldTranslate(translate);
}

void MoveableObject::LuaSetWorldRotate(glm::quat const& rotate)
{
	SetWorldRotate(rotate);
}

void MoveableObject::LuaSetWorldScale(glm::vec3 const& scale)
{
	SetWorldScale(scale);
}

END_SE()
