#pragma once

BEGIN_NS(ecl)

struct GameCameraBehavior;

END_NS()

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

struct GameStateChangedEventParams : public lua::EventBase
{
    ecl::GameState FromState;
    ecl::GameState ToState;
};

struct KeyInputEventParams : public lua::EventBase
{
    SDLKeyEvent Event;
    SDLScanCode Key;
    SDLKeyModifier Modifiers;
    bool Pressed;
    bool Repeat;
};

struct MouseButtonEventParams : public lua::EventBase
{
    int Button;
    bool Pressed;
    int Clicks;
    int X;
    int Y;
};

struct MouseWheelEventParams : public lua::EventBase
{
    float ScrollX;
    float ScrollY;
    int X;
    int Y;
};

struct ControllerAxisEventParams : public lua::EventBase
{
    int32_t DeviceId;
    SDLControllerAxis Axis;
    float Value;
};

struct ControllerButtonEventParams : public lua::EventBase
{
    int32_t DeviceId;
    SDLKeyEvent Event;
    SDLControllerButton Button;
    bool Pressed;
};

struct ViewportResizedEventParams : public lua::EventBase
{
    int Width;
    int Height;
};

struct CameraUpdatingEventParams : public lua::EventBase
{
    [[bg3::readonly]] EntityHandle Entity;
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraUpdatedEventParams : public lua::EventBase
{
    [[bg3::readonly]] EntityHandle Entity;
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraPitchUpdatedEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraZoomUpdatedEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraPitchUpdatingEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraPitchCalculatedEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] bool IgnoreAvoidance;
    [[bg3::readonly]] bool SelectMode;
    [[bg3::readonly]] float OriginalPitch;
    float Pitch;
};

struct CameraInputEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] bg3se::input::InputEvent* Input;
    uint16_t Result{ 0 };
};

struct CameraInputHandledEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] bg3se::input::InputEvent* Input;
    [[bg3::readonly]] uint16_t Result;
};

struct CameraZoomResettingEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
};

struct CameraZoomResetEventParams : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
};

END_NS()
