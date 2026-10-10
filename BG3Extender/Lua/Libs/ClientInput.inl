#include <Extender/ScriptExtender.h>
#include <Extender/Client/KeyboardMovement.h>

/// <lua_module>Input</lua_module>
BEGIN_NS(ecl::lua::input)

void InjectKeyEvent(Uint32 type, Uint8 state, SDLScanCode key, std::optional<SDLKeyModifier> modifiers)
{
    SDL_Event evt;
    memset(&evt, 0, sizeof(evt));

    evt.type = type;
    evt.key.timestamp = SDL_GetTicks();
    evt.key.windowID = 0;
    evt.key.state = state;
    evt.key.repeat = 0;
    evt.key.keysym.scancode = (SDL_Scancode)key;
    evt.key.keysym.sym = SDL_SCANCODE_TO_KEYCODE((SDL_Scancode)key);
    evt.key.keysym.mod = modifiers ? (Uint16)*modifiers : 0;

    gExtender->GetClient().GetSDL().InjectEvent(evt);
}

void InjectKeyPress(SDLScanCode key, std::optional<SDLKeyModifier> modifiers)
{
    InjectKeyEvent(SDL_KEYDOWN, SDL_PRESSED, key, modifiers);
    InjectKeyEvent(SDL_KEYUP, SDL_RELEASED, key, modifiers);
}

void InjectKeyDown(SDLScanCode key, std::optional<SDLKeyModifier> modifiers)
{
    InjectKeyEvent(SDL_KEYDOWN, SDL_PRESSED, key, modifiers);
}

void InjectKeyUp(SDLScanCode key, std::optional<SDLKeyModifier> modifiers)
{
    InjectKeyEvent(SDL_KEYUP, SDL_RELEASED, key, modifiers);
}

bg3se::input::InputManager* GetInputManager()
{
    return *GetStaticSymbols().ls__gInputManager;
}


/// Enables or disables the global keyboard movement override.
/// Returns false if the native gate could not be installed.
/// CharacterMove key bindings are required; other native movement rules remain.
bool SetKeyboardMovementOverride(bool enabled)
{
    return ecl::SetKeyboardMovementOverride(enabled);
}

/// Returns whether the global keyboard movement override is enabled.
bool GetKeyboardMovementOverride()
{
    return ecl::GetKeyboardMovementOverride();
}

void RegisterInputLib()
{
    DECLARE_MODULE(Input, Client)
    BEGIN_MODULE()
    MODULE_FUNCTION(InjectKeyPress)
    MODULE_FUNCTION(InjectKeyDown)
    MODULE_FUNCTION(InjectKeyUp)
    MODULE_FUNCTION(GetInputManager)
    MODULE_FUNCTION(SetKeyboardMovementOverride)
    MODULE_FUNCTION(GetKeyboardMovementOverride)
    END_MODULE()
}

END_NS()
