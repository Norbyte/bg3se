#include "stdafx.h"
#include "GlobalFixedStrings.h"
#include "ExtensionHelpers.h"

namespace bg3se
{
	void GlobalFixedStrings::Initialize()
	{
#define FS(val) str##val = FixedString(#val)
#define FS_NAME(name, val) str##name = FixedString(val)
#include "GlobalFixedStrings.inl"
#undef FS
#undef FS_NAME
	}

	GlobalFixedStrings GFS;
}
