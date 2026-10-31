#pragma once

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

END_NS()
