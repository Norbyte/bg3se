#pragma once

namespace bg3se
{
    class GlobalFixedStrings
    {
    public:
        void Initialize();

        FixedString strNull;

#define FS(val) FixedString str##val
#define FS_NAME(name, val) FixedString str##name
#include "GlobalFixedStrings.inl"
#undef FS
#undef FS_NAME
    };

    extern GlobalFixedStrings GFS;
}