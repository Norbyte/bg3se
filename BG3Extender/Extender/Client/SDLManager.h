#pragma once

#include <GameDefinitions/Base/Base.h>
#include <CoreLib/Wrappers.h>
#include <atomic>
#include <mutex>
#include <string>
#include <SDL.h>

BEGIN_SE()

using SDLPollEventProc = int(SDL_Event* event);
using SDLStartTextInputProc = void();

#define SDL_HOOK(name) enum class SDL##name##HookTag {}; \
    using SDL##name##HookType = WrappableFunction<SDL##name##HookTag, decltype(SDL_##name)>; \
    SDL##name##HookType* SDL##name##HookType::gHook;

SDL_HOOK(CreateWindow)
SDL_HOOK(DestroyWindow)
SDL_HOOK(Quit)
SDL_HOOK(QuitSubSystem)
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
    void InitializeRotationCursor();
    bool SetRotationCursorEnabled(bool enabled);
    void ResetRotationCursor();

private:
    void SDLCreateWindowHooked(const char* title,
        int x, int y, int w,
        int h, uint32_t flags,
        SDL_Window* window);
    void SDLDestroyWindowHooked(SDL_Window* window);
    void SDLQuitHooked();
    void SDLQuitSubSystemHooked(decltype(SDL_QuitSubSystem)* wrapped, uint32_t flags);
    int SDLPollEventHooked(SDLPollEventProc* wrapped, SDL_Event* event);
    int SDLPollEventInternal(SDL_Event* event, int result);
    int SDLPollEventSEH(SDL_Event* event, int result);
    void SDLIsTextInputActiveHooked(SDL_bool active);
    void SDLStartTextInputHooked(SDLStartTextInputProc* wrapped);
    void SDLStopTextInputHooked(SDLStartTextInputProc* wrapped);

    static void CameraRotationStartHooked(void* self, int command);
    static void CameraRotationStopHooked(void* self, int command);
    static glm::vec2* GetRotationCursorPositionHooked(void* self, char* arg, int* value);
    void OnCameraRotationStart();
    void OnCameraRotationStop();
    void UpdateRotationCursor();
    void OnCursorFocusLost(uint32_t windowId);
    void UpdateVirtualCursorPosition(glm::vec2& position);
    void ReleaseRotationCursor();
    void RestoreCursorHints();
    void RefreshCursor();

    SDL_Window* window_{ nullptr };
    bool enableUI_{ false };
    std::recursive_mutex mutex_;
    std::recursive_mutex injectedEventMutex_;
    Array<SDL_Event> injectedEvents_;

    enum class VirtualCursorState { Normal, Pending, Override, Restore };

    bool rotationCursorAvailable_{ false };
    std::atomic<bool> rotationCursorWork_{ false };
    std::atomic<bool> virtualCursorWork_{ false };
    std::mutex cursorMutex_;
    bool rotationCursorEnabled_{ false };
    bool cameraRotating_{ false };
    bool cursorCaptured_{ false };
    bool releaseCursor_{ false };
    bool cursorCaptureFailed_{ false };
    SDL_Window* cursorWindow_{ nullptr };
    int savedCursorX_{ 0 };
    int savedCursorY_{ 0 };
    int cursorWindowWidth_{ 0 };
    glm::vec2 virtualCursorPosition_{ 0.0f };
    VirtualCursorState virtualCursorState_{ VirtualCursorState::Normal };
    bool hadRelativeModeCenter_{ false };
    bool hadRelativeCursorVisible_{ false };
    std::string previousRelativeModeCenter_;
    std::string previousRelativeCursorVisible_;

    SDLCreateWindowHookType CreateWindowHook_;
    SDLDestroyWindowHookType DestroyWindowHook_;
    SDLQuitHookType QuitHook_;
    SDLQuitSubSystemHookType QuitSubSystemHook_;
    SDLPollEventHookType PollEventHook_;
    SDLIsTextInputActiveHookType IsTextInputActiveHook_;
    SDLStartTextInputHookType StartTextInputHook_;
    SDLStopTextInputHookType StopTextInputHook_;
};

END_NS()
