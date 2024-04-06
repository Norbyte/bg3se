#include <GameDefinitions/Base/Base.h>
#include <Extender/Client/IMGUI/Backends.h>
#include <CoreLib/Wrappers.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <SDL.h>

using SDLPollEventProc = int(SDL_Event* event);
using SDLStartTextInputProc = void();

BEGIN_SE()

#define SDL_HOOK(name) enum class SDL##name##HookTag {}; \
    using SDL##name##HookType = WrappableFunction<SDL##name##HookTag, decltype(SDL_##name)>; \
    SDL##name##HookType* SDL##name##HookType::gHook;

SDL_HOOK(CreateWindow)
SDL_HOOK(PollEvent)
SDL_HOOK(StartTextInput)
SDL_HOOK(StopTextInput)

END_SE()

BEGIN_NS(extui)

template <class T>
T* ResolveFunctionTrampoline(T* ref)
{
    auto p = reinterpret_cast<uint8_t*>(ref);
    if (*p == 0xE9) {
        return ResolveFunctionTrampoline(reinterpret_cast<T*>(p + 5 + *(int32_t*)(p + 1)));
    } else {
        return ref;
    }
}

class SDLBackend : public PlatformBackend
{
public:
    SDLBackend() {}
    ~SDLBackend() override
    {
        DestroyUI();
        DisableHooks();
    }

    void EnableHooks() override
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreateWindowHook_.Wrap(ResolveFunctionTrampoline(&SDL_CreateWindow));
        PollEventHook_.Wrap(ResolveFunctionTrampoline(&SDL_PollEvent));
        StartTextInputHook_.Wrap(ResolveFunctionTrampoline(&SDL_StartTextInput));
        StopTextInputHook_.Wrap(ResolveFunctionTrampoline(&SDL_StopTextInput));
        DetourTransactionCommit();

        CreateWindowHook_.SetPostHook(&SDLBackend::SDLCreateWindowHooked, this);
        PollEventHook_.SetWrapper(&SDLBackend::SDLPollEventHooked, this);
        StartTextInputHook_.SetWrapper(&SDLBackend::SDLStartTextInputHooked, this);
        StopTextInputHook_.SetWrapper(&SDLBackend::SDLStopTextInputHooked, this);
    }

    void DisableHooks() override
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreateWindowHook_.Unwrap();
        PollEventHook_.Unwrap();
        StartTextInputHook_.Unwrap();
        StopTextInputHook_.Unwrap();
        DetourTransactionCommit();
    }

    void InitializeUI() override
    {
        if (enableUI_) return;

        ImGui_ImplSDL2_InitForVulkan(window_);
        enableUI_ = true;
    }

    void DestroyUI() override
    {
        if (!enableUI_) return;

        enableUI_ = false;
        ImGui_ImplSDL2_Shutdown();
    }

    void NewFrame() override
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantTextInput) {
            if (!SDL_IsTextInputActive()) {
                SDL_StartTextInput();
            }
        }

        ImGui_ImplSDL2_NewFrame();
    }

private:
    void SDLCreateWindowHooked(const char* title,
        int x, int y, int w,
        int h, uint32_t flags,
        SDL_Window* window)
    {
        window_ = window;
    }

    int SDLPollEventHooked(SDLPollEventProc* wrapped, SDL_Event* event)
    {
        int result = wrapped(event);

        if (enableUI_ && result == 1) {
            ImGui_ImplSDL2_ProcessEvent(event);

            ImGuiIO& io = ImGui::GetIO();
            if (io.WantCaptureKeyboard
                && (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP || event->type == SDL_TEXTEDITING || event->type == SDL_TEXTINPUT)) {
                result = 0;
            }

            if (io.WantCaptureMouse
                && (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEWHEEL)) {
                result = 0;
            }
        }

        return result;
    }

    void SDLStartTextInputHooked(SDLStartTextInputProc* wrapped)
    {
        wrapped();
    }

    void SDLStopTextInputHooked(SDLStartTextInputProc* wrapped)
    {
        if (enableUI_ && ImGui::GetIO().WantTextInput) {
            if (!SDL_IsTextInputActive()) {
                SDL_StartTextInput();
            }
        } else {
            wrapped();
        }
    }

    SDL_Window* window_{ nullptr };
    bool enableUI_{ false };

    SDLCreateWindowHookType CreateWindowHook_;
    SDLPollEventHookType PollEventHook_;
    SDLStartTextInputHookType StartTextInputHook_;
    SDLStopTextInputHookType StopTextInputHook_;
};

END_NS()
