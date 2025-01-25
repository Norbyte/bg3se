#pragma once

#include <GameDefinitions/DragDrop.h>

BEGIN_NS(ecl)

GameUIDragController* PlayerDragData::LuaGetGameUIDrag() const
{
    return static_cast<GameUIDragController*>(Controllers[0]);
}

WorldDragController* PlayerDragData::LuaGetWorldDrag() const
{
    return static_cast<WorldDragController*>(Controllers[1]);
}

CharacterDragController* PlayerDragData::LuaGetCharacterDrag() const
{
    return static_cast<CharacterDragController*>(Controllers[2]);
}

END_NS()
