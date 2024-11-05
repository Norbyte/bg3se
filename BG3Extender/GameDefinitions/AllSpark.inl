#pragma once

#include <GameDefinitions/AllSpark.h>

BEGIN_NS(aspk)

FixedString Component::LuaGetTypeName() const
{
	return GetTypeName();
}

END_NS()
