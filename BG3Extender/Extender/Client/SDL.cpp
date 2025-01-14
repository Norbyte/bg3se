#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Client/SDLManager.h>
#include <CoreLib/Wrappers.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <detours.h>

BEGIN_SE()

SDLManager::SDLManager()
{}

SDLManager::~SDLManager()
{
    DestroyUI();
    DisableHooks();
}

void SDLManager::EnableHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    CreateWindowHook_.Wrap(ResolveFunctionTrampoline(&SDL_CreateWindow));
    PollEventHook_.Wrap(ResolveFunctionTrampoline(&SDL_PollEvent));
    IsTextInputActiveHook_.Wrap(ResolveFunctionTrampoline(&SDL_IsTextInputActive));
    StartTextInputHook_.Wrap(ResolveFunctionTrampoline(&SDL_StartTextInput));
    StopTextInputHook_.Wrap(ResolveFunctionTrampoline(&SDL_StopTextInput));
    DetourTransactionCommit();

    CreateWindowHook_.SetPostHook(&SDLManager::SDLCreateWindowHooked, this);
    PollEventHook_.SetWrapper(&SDLManager::SDLPollEventHooked, this);
    IsTextInputActiveHook_.SetPostHook(&SDLManager::SDLIsTextInputActiveHooked, this);
    StartTextInputHook_.SetWrapper(&SDLManager::SDLStartTextInputHooked, this);
    StopTextInputHook_.SetWrapper(&SDLManager::SDLStopTextInputHooked, this);
}

void SDLManager::DisableHooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    CreateWindowHook_.Unwrap();
    PollEventHook_.Unwrap();
    StartTextInputHook_.Unwrap();
    StopTextInputHook_.Unwrap();
    DetourTransactionCommit();
}

void SDLManager::InitializeUI()
{
    if (enableUI_) return;

    ImGui_ImplSDL2_InitForVulkan(window_);
    enableUI_ = true;
}

void SDLManager::DestroyUI()
{
    if (!enableUI_) return;

    enableUI_ = false;
    ImGui_ImplSDL2_Shutdown();
}

void SDLManager::NewFrame()
{
    ImGui_ImplSDL2_NewFrame();
}

void SDLManager::InjectEvent(SDL_Event const& evt)
{
    std::lock_guard _(injectedEventMutex_);
    injectedEvents_.push_back(evt);
}

void SDLManager::SDLCreateWindowHooked(const char* title,
    int x, int y, int w,
    int h, uint32_t flags,
    SDL_Window* window)
{
    window_ = window;
}

int SDLManager::SDLPollEventHooked(SDLPollEventProc* wrapped, SDL_Event* event)
{
    if (!injectedEvents_.empty()) {
        std::lock_guard _(injectedEventMutex_);
        *event = *injectedEvents_.begin();
        injectedEvents_.ordered_remove_at(0);
        return 1;
    }

    int result = wrapped(event);
    result = SDLPollEventSEH(event, result);

    return result;
}

int SDLManager::SDLPollEventSEH(SDL_Event* event, int result)
{
    BEGIN_GUARDED()
    return SDLPollEventInternal(event, result);
    END_GUARDED()
    return 0;
}

int SDLManager::SDLPollEventInternal(SDL_Event* event, int result)
{
    gExtender->OnSDLEvent(event);

    if (enableUI_ && result == 1) {
        {
            std::lock_guard _(mutex_);
            ImGui_ImplSDL2_ProcessEvent(event);
        }

        ImGuiIO& io = ImGui::GetIO();
        if (io.WantTextInput
            && (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP || event->type == SDL_TEXTEDITING || event->type == SDL_TEXTINPUT)) {
            result = 0;
        }

        if (io.WantCaptureMouse
            && (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEWHEEL)) {
            result = 0;
        }
    }

    if (result == 1 && gExtender->GetClient().HasExtensionState()) {
        ecl::ExtensionState::Get().OnInputEvent(event, result);
    }

    return result;
}

void SDLManager::SDLIsTextInputActiveHooked(SDL_bool active)
{
    if (enableUI_ && ImGui::GetIO().WantTextInput && !active) {
        SDL_StartTextInput();
    }
}

void SDLManager::SDLStartTextInputHooked(SDLStartTextInputProc* wrapped)
{
    wrapped();
}

void SDLManager::SDLStopTextInputHooked(SDLStartTextInputProc* wrapped)
{
    if (enableUI_ && ImGui::GetIO().WantTextInput) {
        if (!SDL_IsTextInputActive()) {
            SDL_StartTextInput();
        }
    } else {
        wrapped();
    }
}

END_SE()
