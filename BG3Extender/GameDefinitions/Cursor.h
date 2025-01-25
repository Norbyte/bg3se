#pragma once

BEGIN_NS(ecl)

struct CursorControl : ProtectedGameObject<CursorControl>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::hidden]] void* VMT2;
    bool ShowCursor;
    float VisibleTimer;
    float FadeTimer;
    FixedString CursorName;
    CursorType CurrentOverlay;
    CursorType RequestedCursor;
    CursorType RequestedOverlay;
    Array<CursorType> CursorOverrides;
};

END_NS()
