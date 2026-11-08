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
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraUpdatingEventParams evt{
        .Entity = view->Entity,
        .Camera = view->Camera,
        .DeltaTime = time->DeltaTime
    };
    extension.ThrowEvent<bg3se::CameraUpdatingEvent>(evt);
}

void CameraEventHooks::OnCameraUpdated(void* self, void* systemContext, ecl::CameraUpdateTime const* time, ecl::CameraUpdateComponentView* view)
{
    ContextGuardAnyThread context(ContextType::Client);
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraUpdatedEventParams evt{
        .Entity = view->Entity,
        .Camera = view->Camera,
        .DeltaTime = time->DeltaTime
    };
    extension.ThrowEvent<bg3se::CameraUpdatedEvent>(evt);
}

void CameraEventHooks::OnCameraPitchUpdating(void* self, void* entity, ecl::GameCameraBehavior* camera, ecl::CameraUpdateTime const* time)
{
    ContextGuardAnyThread context(ContextType::Client);
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraPitchUpdatingEventParams evt{
        .Camera = camera,
        .DeltaTime = time->DeltaTime
    };
    extension.ThrowEvent<bg3se::CameraPitchUpdatingEvent>(evt);
}

void CameraEventHooks::OnCameraPitchUpdated(void* self, void* entity, ecl::GameCameraBehavior* camera, ecl::CameraUpdateTime const* time)
{
    ContextGuardAnyThread context(ContextType::Client);
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraPitchUpdatedEventParams evt{
        .Camera = camera,
        .DeltaTime = time->DeltaTime
    };
    extension.ThrowEvent<bg3se::CameraPitchUpdatedEvent>(evt);
}

void CameraEventHooks::OnCameraZoomUpdated(void* self, void* entity, ecl::CameraZoomUpdateComponentView* view, ecl::CameraUpdateTime const* time)
{
    ContextGuardAnyThread context(ContextType::Client);
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraZoomUpdatedEventParams evt{
        .Camera = view->Camera,
        .DeltaTime = time->DeltaTime
    };
    extension.ThrowEvent<bg3se::CameraZoomUpdatedEvent>(evt);
}

float CameraEventHooks::OnCameraPitchCalculated(ecl::GameCameraBehavior__CalculatePitchProc* next, ecl::GameCameraBehavior* camera, bool ignoreAvoidance, bool selectMode)
{
    auto pitch = next(camera, ignoreAvoidance, selectMode);
    ContextGuardAnyThread context(ContextType::Client);
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraPitchCalculatedEventParams evt{
        .Camera = camera,
        .IgnoreAvoidance = ignoreAvoidance,
        .SelectMode = selectMode,
        .OriginalPitch = pitch,
        .Pitch = pitch
    };
    auto result = extension.ThrowEvent<bg3se::CameraPitchCalculatedEvent>(evt);
    // Check for a finite number; otherwise return the original native pitch.
    if (result == EventResult::Successful && (std::bit_cast<uint32_t>(evt.Pitch) & 0x7f800000u) != 0x7f800000u) {
        return evt.Pitch;
    }

    return pitch;
}

uint16_t* CameraEventHooks::OnCameraInput(ecl::CameraSystem__HandleInputProc* next, void* self, uint16_t* result, ecl::CameraInputComponentView* view, bg3se::input::InputEvent* input)
{
    {
        ContextGuardAnyThread context(ContextType::Client);
        auto& extension = gExtender->GetClient().GetExtensionState();
        CameraInputEventParams evt{
            .Camera = view->Camera,
            .Input = input
        };
        if (extension.ThrowEvent<bg3se::CameraInputEvent>(evt) == EventResult::ActionPrevented) {
            *result = evt.Result;
            return result;
        }
    }

    auto ret = next(self, result, view, input);

    {
        ContextGuardAnyThread context(ContextType::Client);
        auto& extension = gExtender->GetClient().GetExtensionState();
        CameraInputHandledEventParams evt{
            .Camera = view->Camera,
            .Input = input,
            .Result = *ret
        };
        extension.ThrowEvent<bg3se::CameraInputHandledEvent>(evt);
    }

    return ret;
}

void CameraEventHooks::OnCameraZoomResetting(ecl::CameraDefaultZoomComponentView* view)
{
    ContextGuardAnyThread context(ContextType::Client);
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraZoomResettingEventParams evt{
        .Camera = view->Camera
    };
    extension.ThrowEvent<bg3se::CameraZoomResettingEvent>(evt);
}

void CameraEventHooks::OnCameraZoomReset(ecl::CameraDefaultZoomComponentView* view)
{
    ContextGuardAnyThread context(ContextType::Client);
    auto& extension = gExtender->GetClient().GetExtensionState();
    CameraZoomResetEventParams evt{
        .Camera = view->Camera
    };
    extension.ThrowEvent<bg3se::CameraZoomResetEvent>(evt);
}

END_NS()
