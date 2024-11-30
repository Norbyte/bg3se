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

#if defined(__APPLE__)
bool IsMacOS() {
    return true;
}

bool IsAppleSilicon() {
#if defined(__arm64__) || defined(__aarch64__)
    return true;
#else
    return false;
#endif
}
#endif

END_SE()
