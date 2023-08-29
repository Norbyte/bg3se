#include "stdafx.h"
#include <GameDefinitions/Base/Base.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

void LogLuaError(std::string_view msg)
{
	gExtender->LogLuaError(msg);
}

void LogOsirisError(std::string_view msg)
{
	gExtender->LogOsirisError(msg);
}

void LogOsirisWarning(std::string_view msg)
{
	gExtender->LogOsirisWarning(msg);
}

void LogOsirisMsg(std::string_view msg)
{
	gExtender->LogOsirisMsg(msg);
}

END_SE()
