#include <stdafx.h>
#include <bit>
#include <GameDefinitions/Input.h>
#include <GameDefinitions/Components/Camera.h>
#include <Lua/Client/LuaBindingClient.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Client/ExtensionStateClient.h>

BEGIN_NS(ecl::lua)

CameraEventHooks::CameraEventHooks()
{
    auto& hooks = gExtender->GetEngineHooks();
    hooks.ecl__CameraSystem__UpdateCamera.SetPrePostHook(&CameraEventHooks::OnCameraUpdating, &CameraEventHooks::OnCameraUpdated, this);
    hooks.ecl__CameraSystem__UpdateCameraPitch.SetPrePostHook(&CameraEventHooks::OnCameraPitchUpdating, &CameraEventHooks::OnCameraPitchUpdated, this);
    hooks.ecl__CameraSystem__UpdateCameraZoom.SetPostHook(&CameraEventHooks::OnCameraZoomUpdated, this);
    hooks.ecl__GameCameraBehavior__CalculatePitch.SetWrapper(&CameraEventHooks::OnCameraPitchCalculated, this);
    hooks.ecl__CameraSystem__HandleInput.SetWrapper(&CameraEventHooks::OnCameraInput, this);
    hooks.ecl__CameraSystem__SetDefaultZoom.SetPrePostHook(&CameraEventHooks::OnCameraZoomResetting, &CameraEventHooks::OnCameraZoomReset, this);
}

CameraEventHooks::~CameraEventHooks()
{
    auto& hooks = gExtender->GetEngineHooks();
    hooks.ecl__CameraSystem__UpdateCamera.ClearHook();
    hooks.ecl__CameraSystem__UpdateCameraPitch.ClearHook();
    hooks.ecl__CameraSystem__UpdateCameraZoom.ClearHook();
    hooks.ecl__GameCameraBehavior__CalculatePitch.ClearHook();
    hooks.ecl__CameraSystem__HandleInput.ClearHook();
    hooks.ecl__CameraSystem__SetDefaultZoom.ClearHook();
}

void CameraEventHooks::OnCameraUpdating(void* self, void* systemContext, ecl::CameraUpdateTime const* time, ecl::CameraUpdateComponentView* view)
{
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraUpdatingEvent evt{
            .Entity = view->Entity,
            .Camera = view->Camera,
            .DeltaTime = time->DeltaTime
        };
        lua->ThrowEvent("CameraUpdating", evt, false, 0);
    }
}

void CameraEventHooks::OnCameraUpdated(void* self, void* systemContext, ecl::CameraUpdateTime const* time, ecl::CameraUpdateComponentView* view)
{
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraUpdatedEvent evt{
            .Entity = view->Entity,
            .Camera = view->Camera,
            .DeltaTime = time->DeltaTime
        };
        lua->ThrowEvent("CameraUpdated", evt, false, 0);
    }
}

void CameraEventHooks::OnCameraPitchUpdating(void* self, void* entity, ecl::GameCameraBehavior* camera, ecl::CameraUpdateTime const* time)
{
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraPitchUpdatingEvent evt{
            .Camera = camera,
            .DeltaTime = time->DeltaTime
        };
        lua->ThrowEvent("CameraPitchUpdating", evt, false, 0);
    }
}

void CameraEventHooks::OnCameraPitchUpdated(void* self, void* entity, ecl::GameCameraBehavior* camera, ecl::CameraUpdateTime const* time)
{
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraPitchUpdatedEvent evt{
            .Camera = camera,
            .DeltaTime = time->DeltaTime
        };
        lua->ThrowEvent("CameraPitchUpdated", evt, false, 0);
    }
}

void CameraEventHooks::OnCameraZoomUpdated(void* self, void* entity, ecl::CameraZoomUpdateComponentView* view, ecl::CameraUpdateTime const* time)
{
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraZoomUpdatedEvent evt{
            .Camera = view->Camera,
            .DeltaTime = time->DeltaTime
        };
        lua->ThrowEvent("CameraZoomUpdated", evt, false, 0);
    }
}

float CameraEventHooks::OnCameraPitchCalculated(ecl::GameCameraBehavior__CalculatePitchProc* next, ecl::GameCameraBehavior* camera, bool ignoreAvoidance, bool selectMode)
{
    auto pitch = next(camera, ignoreAvoidance, selectMode);
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraPitchCalculatedEvent evt{
            .Camera = camera,
            .IgnoreAvoidance = ignoreAvoidance,
            .SelectMode = selectMode,
            .OriginalPitch = pitch,
            .Pitch = pitch
        };
        auto result = lua->ThrowEvent("CameraPitchCalculated", evt, false, 0);
        // Check for a finite number; otherwise return the original native pitch.
        if (result == EventResult::Successful && (std::bit_cast<uint32_t>(evt.Pitch) & 0x7f800000u) != 0x7f800000u) {
            return evt.Pitch;
        }
    }

    return pitch;
}

uint16_t* CameraEventHooks::OnCameraInput(ecl::CameraSystem__HandleInputProc* next, void* self, uint16_t* result, ecl::CameraInputComponentView* view, bg3se::input::InputEvent* input)
{
    {
        ContextGuardAnyThread context(ContextType::Client);
        LuaClientPin lua(gExtender->GetClient().GetExtensionState());
        if (lua) {
            CameraInputEvent evt{
                .Camera = view->Camera,
                .Input = input
            };
            if (lua->ThrowEvent("CameraInput", evt, true, 0) == EventResult::ActionPrevented) {
                *result = evt.Result;
                return result;
            }
        }
    }

    auto ret = next(self, result, view, input);

    {
        ContextGuardAnyThread context(ContextType::Client);
        LuaClientPin lua(gExtender->GetClient().GetExtensionState());
        if (lua) {
            CameraInputHandledEvent evt{
                .Camera = view->Camera,
                .Input = input,
                .Result = *ret
            };
            lua->ThrowEvent("CameraInputHandled", evt, false, 0);
        }
    }

    return ret;
}

void CameraEventHooks::OnCameraZoomResetting(ecl::CameraDefaultZoomComponentView* view)
{
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraZoomResettingEvent evt{
            .Camera = view->Camera
        };
        lua->ThrowEvent("CameraZoomResetting", evt, false, 0);
    }
}

void CameraEventHooks::OnCameraZoomReset(ecl::CameraDefaultZoomComponentView* view)
{
    ContextGuardAnyThread context(ContextType::Client);
    LuaClientPin lua(gExtender->GetClient().GetExtensionState());
    if (lua) {
        CameraZoomResetEvent evt{
            .Camera = view->Camera
        };
        lua->ThrowEvent("CameraZoomReset", evt, false, 0);
    }
}

END_NS()
