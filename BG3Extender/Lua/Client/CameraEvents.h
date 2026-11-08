#pragma once

#include <GameHooks/EngineHooksFwdDecl.h>

BEGIN_NS(ecl::lua)

class CameraEventHooks
{
public:
    CameraEventHooks();
    ~CameraEventHooks();

private:
    void OnCameraUpdating(void* self, void* systemContext, ecl::CameraUpdateTime const* time, ecl::CameraUpdateComponentView* view);
    void OnCameraUpdated(void* self, void* systemContext, ecl::CameraUpdateTime const* time, ecl::CameraUpdateComponentView* view);
    void OnCameraPitchUpdating(void* self, void* entity, ecl::GameCameraBehavior* camera, ecl::CameraUpdateTime const* time);
    void OnCameraPitchUpdated(void* self, void* entity, ecl::GameCameraBehavior* camera, ecl::CameraUpdateTime const* time);
    void OnCameraZoomUpdated(void* self, void* entity, ecl::CameraZoomUpdateComponentView* view, ecl::CameraUpdateTime const* time);
    float OnCameraPitchCalculated(ecl::GameCameraBehavior__CalculatePitchProc* next, ecl::GameCameraBehavior* camera, bool ignoreAvoidance, bool selectMode);
    uint16_t* OnCameraInput(ecl::CameraSystem__HandleInputProc* next, void* self, uint16_t* result, ecl::CameraInputComponentView* view, bg3se::input::InputEvent* input);
    void OnCameraZoomResetting(ecl::CameraDefaultZoomComponentView* view);
    void OnCameraZoomReset(ecl::CameraDefaultZoomComponentView* view);
};

END_NS()
