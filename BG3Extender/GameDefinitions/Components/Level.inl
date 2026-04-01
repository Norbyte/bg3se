#include <GameDefinitions/Components/Level.h>

BEGIN_SE()

void LevelInstanceAttachRequestSystem::RequestLevelSwap(EntityHandle entity, EntityHandle level)
{
    EnterCriticalSection(&LevelSwapRequestsCS);
    LevelSwapRequests.set(entity, level);
    LeaveCriticalSection(&LevelSwapRequestsCS);
}

END_SE()
