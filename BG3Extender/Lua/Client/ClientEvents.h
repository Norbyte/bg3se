#pragma once

BEGIN_NS(ecl)

struct GameCameraBehavior;

END_NS()

BEGIN_NS(ecl::lua)

using namespace bg3se::lua;

struct GameStateChangedEvent : public lua::EventBase
{
    ecl::GameState FromState;
    ecl::GameState ToState;
};

struct KeyInputEvent : public lua::EventBase
{
    SDLKeyEvent Event;
    SDLScanCode Key;
    SDLKeyModifier Modifiers;
    bool Pressed;
    bool Repeat;
};

struct MouseButtonEvent : public lua::EventBase
{
    int Button;
    bool Pressed;
    int Clicks;
    int X;
    int Y;
};

struct MouseWheelEvent : public lua::EventBase
{
    float ScrollX;
    float ScrollY;
    int X;
    int Y;
};

struct ControllerAxisEvent : public lua::EventBase
{
    int32_t DeviceId;
    SDLControllerAxis Axis;
    float Value;
};

struct ControllerButtonEvent : public lua::EventBase
{
    int32_t DeviceId;
    SDLKeyEvent Event;
    SDLControllerButton Button;
    bool Pressed;
};

struct ViewportResizedEvent : public lua::EventBase
{
    int Width;
    int Height;
};

struct CameraUpdatingEvent : public lua::EventBase
{
    [[bg3::readonly]] EntityHandle Entity;
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraUpdatedEvent : public lua::EventBase
{
    [[bg3::readonly]] EntityHandle Entity;
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraPitchUpdatedEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraZoomUpdatedEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraPitchUpdatingEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] float DeltaTime;
};

struct CameraPitchCalculatedEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] bool IgnoreAvoidance;
    [[bg3::readonly]] bool SelectMode;
    [[bg3::readonly]] float OriginalPitch;
    float Pitch;
};

struct CameraInputEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] bg3se::input::InputEvent* Input;
    uint16_t Result{ 0 };
};

struct CameraInputHandledEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
    [[bg3::readonly]] bg3se::input::InputEvent* Input;
    [[bg3::readonly]] uint16_t Result;
};

struct CameraZoomResettingEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
};

struct CameraZoomResetEvent : public lua::EventBase
{
    [[bg3::readonly]] ecl::GameCameraBehavior* Camera;
};

END_NS()
