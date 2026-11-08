#include <GameDefinitions/Components/Camera.h>

BEGIN_NS(ecl)

CameraGlobalSwitches* GameCameraBehavior::GetCameraDefinition() const
{
    auto getter = GetStaticSymbols().ecl__GameCameraBehavior__GetCameraDefinition;
    return getter ? getter(this) : nullptr;
}

END_NS()
