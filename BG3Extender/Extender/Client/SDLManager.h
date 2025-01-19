#pragma once

#include <GameDefinitions/Base/Base.h>
#include <CoreLib/Wrappers.h>
#include <mutex>
#include <SDL.h>

BEGIN_SE()

using SDLPollEventProc = int(SDL_Event* event);
using SDLStartTextInputProc = void();

#define SDL_HOOK(name) enum class SDL##name##HookTag {}; \
    using SDL##name##HookType = WrappableFunction<SDL##name##HookTag, decltype(SDL_##name)>; \
    SDL##name##HookType* SDL##name##HookType::gHook;

SDL_HOOK(CreateWindow)
SDL_HOOK(PollEvent)
SDL_HOOK(IsTextInputActive)
SDL_HOOK(StartTextInput)
SDL_HOOK(StopTextInput)


class SDLManager
{
public:
    SDLManager();
    ~SDLManager();

    inline std::recursive_mutex& GetSDLMutex()
    {
        return mutex_;
    }

    void EnableHooks();
    void DisableHooks();
    void InitializeUI();
    void DestroyUI();
    void NewFrame();
    void InjectEvent(SDL_Event const& evt);

private:
    void SDLCreateWindowHooked(const char* title,
        int x, int y, int w,
        int h, uint32_t flags,
        SDL_Window* window);
    int SDLPollEventHooked(SDLPollEventProc* wrapped, SDL_Event* event);
    int SDLPollEventInternal(SDL_Event* event, int result);
    int SDLPollEventSEH(SDL_Event* event, int result);
    void SDLIsTextInputActiveHooked(SDL_bool active);
    void SDLStartTextInputHooked(SDLStartTextInputProc* wrapped);
    void SDLStopTextInputHooked(SDLStartTextInputProc* wrapped);

    SDL_Window* window_{ nullptr };
    bool enableUI_{ false };
    std::recursive_mutex mutex_;
    std::recursive_mutex injectedEventMutex_;
    Array<SDL_Event> injectedEvents_;

    SDLCreateWindowHookType CreateWindowHook_;
    SDLPollEventHookType PollEventHook_;
    SDLIsTextInputActiveHookType IsTextInputActiveHook_;
    SDLStartTextInputHookType StartTextInputHook_;
    SDLStopTextInputHookType StopTextInputHook_;
};

END_NS()
