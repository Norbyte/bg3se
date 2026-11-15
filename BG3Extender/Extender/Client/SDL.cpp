#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Client/ExtensionStateClient.h>
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
    DestroyWindowHook_.Wrap(ResolveFunctionTrampoline(&SDL_DestroyWindow));
    QuitHook_.Wrap(ResolveFunctionTrampoline(&SDL_Quit));
    QuitSubSystemHook_.Wrap(ResolveFunctionTrampoline(&SDL_QuitSubSystem));
    PollEventHook_.Wrap(ResolveFunctionTrampoline(&SDL_PollEvent));
    IsTextInputActiveHook_.Wrap(ResolveFunctionTrampoline(&SDL_IsTextInputActive));
    StartTextInputHook_.Wrap(ResolveFunctionTrampoline(&SDL_StartTextInput));
    StopTextInputHook_.Wrap(ResolveFunctionTrampoline(&SDL_StopTextInput));
    DetourTransactionCommit();

    CreateWindowHook_.SetPostHook(&SDLManager::SDLCreateWindowHooked, this);
    DestroyWindowHook_.SetPreHook(&SDLManager::SDLDestroyWindowHooked, this);
    QuitHook_.SetPreHook(&SDLManager::SDLQuitHooked, this);
    QuitSubSystemHook_.SetWrapper(&SDLManager::SDLQuitSubSystemHooked, this);
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
    DestroyWindowHook_.Unwrap();
    QuitHook_.Unwrap();
    QuitSubSystemHook_.Unwrap();
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

void SDLManager::SDLDestroyWindowHooked(SDL_Window* window)
{
    std::lock_guard _(cursorMutex_);
    if (cursorWindow_ != window) return;
    ReleaseRotationCursor();
    cursorWindow_ = nullptr;
    cameraRotating_ = false;
    rotationCursorWork_.store(false, std::memory_order_release);
    virtualCursorState_ = VirtualCursorState::Normal;
    virtualCursorWork_.store(false, std::memory_order_release);
}

void SDLManager::SDLQuitHooked()
{
    // Release before SDL video teardown, not from DLL_PROCESS_DETACH.
    std::lock_guard _(cursorMutex_);
    ReleaseRotationCursor();
    cameraRotating_ = false;
    cursorWindow_ = nullptr;
    rotationCursorWork_.store(false, std::memory_order_release);
    virtualCursorState_ = VirtualCursorState::Normal;
    virtualCursorWork_.store(false, std::memory_order_release);
}

void SDLManager::SDLQuitSubSystemHooked(decltype(SDL_QuitSubSystem)* wrapped, uint32_t flags)
{
    if (flags & SDL_INIT_VIDEO) {
        std::lock_guard _(cursorMutex_);
        ReleaseRotationCursor();
    }
    wrapped(flags);
    // SDL subsystem references can be released without destroying the window.
    if ((flags & SDL_INIT_VIDEO) && !(SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO)) {
        SDLQuitHooked();
    }
}

int SDLManager::SDLPollEventHooked(SDLPollEventProc* wrapped, SDL_Event* event)
{
    UpdateRotationCursor();
    if (!injectedEvents_.empty()) {
        std::lock_guard _(injectedEventMutex_);
        *event = *injectedEvents_.begin();
        injectedEvents_.ordered_remove_at(0);
        return 1;
    }

    int result = wrapped(event);
    // Focus cleanup must see raw SDL events before ImGui or Lua can consume them.
    if (result == 1 && event && event->type == SDL_WINDOWEVENT
        && event->window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
        OnCursorFocusLost(event->window.windowID);
    }
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

namespace {
using RotationProc = void(void*, int);
RotationProc* rotationStart{ nullptr };
RotationProc* rotationStop{ nullptr };

RotationProc* GetCallTarget(uint8_t const* site)
{
    int32_t offset;
    memcpy(&offset, site + 1, sizeof(offset));
    return reinterpret_cast<RotationProc*>(const_cast<uint8_t*>(site + 5 + offset));
}

// One page holds three leaf JMP relays, within CALL rel32 range of all sites.
uint8_t* AllocateRelays(uint8_t const* const* sites)
{
    uintptr_t first = reinterpret_cast<uintptr_t>(sites[0]);
    uintptr_t last = first;
    for (unsigned i = 1; i < 5; i++) {
        auto address = reinterpret_cast<uintptr_t>(sites[i]);
        first = std::min(first, address);
        last = std::max(last, address);
    }

    SYSTEM_INFO info;
    GetSystemInfo(&info);
    auto granularity = static_cast<uintptr_t>(info.dwAllocationGranularity);
    auto lower = last > 0x7fff0000ull ? last - 0x7fff0000ull : granularity;
    auto upper = first + 0x7fff0000ull;
    auto address = (lower + granularity - 1) & ~(granularity - 1);
    while (address < upper) {
        MEMORY_BASIC_INFORMATION region;
        if (!VirtualQuery(reinterpret_cast<void*>(address), &region, sizeof(region))) break;
        auto end = reinterpret_cast<uintptr_t>(region.BaseAddress) + region.RegionSize;
        if (region.State == MEM_FREE && end - address >= info.dwPageSize) {
            if (auto relay = VirtualAlloc(reinterpret_cast<void*>(address), info.dwPageSize,
                MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) return static_cast<uint8_t*>(relay);
        }
        if (end <= address) break;
        address = (end + granularity - 1) & ~(granularity - 1);
    }
    return nullptr;
}
}

void SDLManager::CameraRotationStartHooked(void* self, int command)
{
    gExtender->GetClient().GetSDL().OnCameraRotationStart();
    rotationStart(self, command);
}

void SDLManager::CameraRotationStopHooked(void* self, int command)
{
    gExtender->GetClient().GetSDL().OnCameraRotationStop();
    rotationStop(self, command);
}

glm::vec2* SDLManager::GetRotationCursorPositionHooked(void* self, char* arg, int* value)
{
    // Preserve the dynamic call at vtable byte offset 0x108.
    using Proc = glm::vec2*(void*, char*, int*);
    auto table = *reinterpret_cast<void***>(self);
    auto position = reinterpret_cast<Proc*>(table[0x108 / sizeof(void*)])(self, arg, value);
    gExtender->GetClient().GetSDL().UpdateVirtualCursorPosition(*position);
    return position;
}

void SDLManager::InitializeRotationCursor()
{
    if (rotationCursorAvailable_) return;
    auto& sym = GetStaticSymbols();
    uint8_t const* sites[] = {
        sym.CameraRotationStartCall, sym.CameraRotationStopCall1,
        sym.CameraRotationStopCall2, sym.CameraRotationStopCall3,
        sym.CameraRotationCursorCall
    };
    for (auto site : sites) {
        if (!site) return;
    }
    for (unsigned i = 0; i < 4; i++) {
        if (sites[i][0] != 0xe8) return;
    }
    uint8_t virtualCall[] = { 0xff, 0x90, 0x08, 0x01, 0x00, 0x00 };
    if (memcmp(sites[4], virtualCall, sizeof(virtualCall)) != 0) return;
    auto start = GetCallTarget(sites[0]);
    auto stop = GetCallTarget(sites[1]);
    if (start == stop || GetCallTarget(sites[2]) != stop || GetCallTarget(sites[3]) != stop) return;

    auto relays = AllocateRelays(sites);
    if (!relays) {
        WARN("Rotation cursor unavailable: cannot allocate near relays");
        return;
    }
    void* targets[] = { reinterpret_cast<void*>(&CameraRotationStartHooked),
        reinterpret_cast<void*>(&CameraRotationStopHooked), reinterpret_cast<void*>(&GetRotationCursorPositionHooked) };
    for (unsigned i = 0; i < 3; i++) {
        uint8_t jump[] = { 0xff, 0x25, 0, 0, 0, 0 };
        memcpy(relays + i * 16, jump, sizeof(jump));
        memcpy(relays + i * 16 + sizeof(jump), &targets[i], sizeof(void*));
    }

    uint8_t replacements[5][6];
    for (unsigned i = 0; i < 5; i++) {
        auto relay = relays + (i == 0 ? 0 : i == 4 ? 32 : 16);
        auto displacement = reinterpret_cast<intptr_t>(relay) - reinterpret_cast<intptr_t>(sites[i] + 5);
        if (displacement < INT32_MIN || displacement > INT32_MAX) {
            VirtualFree(relays, 0, MEM_RELEASE);
            return;
        }
        replacements[i][0] = 0xe8;
        auto rel32 = static_cast<int32_t>(displacement);
        memcpy(replacements[i] + 1, &rel32, sizeof(rel32));
        replacements[i][5] = 0x90;
    }
    DWORD protection;
    if (!VirtualProtect(relays, 48, PAGE_EXECUTE_READ, &protection)) {
        VirtualFree(relays, 0, MEM_RELEASE);
        return;
    }
    FlushInstructionCache(GetCurrentProcess(), relays, 48);

    // Publish originals before installing any call; all mappings were checked above.
    rotationStart = start;
    rotationStop = stop;
    for (unsigned i = 0; i < 5; i++) {
        auto size = i == 4 ? 6 : 5;
        {
            WriteAnchor code(sites[i], size);
            memcpy(code.ptr(), replacements[i], size);
        }
        FlushInstructionCache(GetCurrentProcess(), sites[i], size);
    }
    rotationCursorAvailable_ = true;
}

bool SDLManager::SetRotationCursorEnabled(bool enabled)
{
    if (!rotationCursorAvailable_) return false;
    std::lock_guard _(cursorMutex_);
    if (rotationCursorEnabled_ == enabled) return true;
    rotationCursorEnabled_ = enabled;
    if (!enabled) {
        releaseCursor_ = true;
        rotationCursorWork_.store(true, std::memory_order_release);
    } else if (cameraRotating_) {
        rotationCursorWork_.store(true, std::memory_order_release);
    }
    return true;
}

void SDLManager::ResetRotationCursor()
{
    std::lock_guard _(cursorMutex_);
    rotationCursorEnabled_ = false;
    cameraRotating_ = false;
    releaseCursor_ = true;
    rotationCursorWork_.store(true, std::memory_order_release);
}

void SDLManager::OnCameraRotationStart()
{
    std::lock_guard _(cursorMutex_);
    if (!cameraRotating_) {
        // Duplicate starts must not replace the anchor with relative-mode coordinates.
        if (!cursorCaptured_) {
            SDL_GetMouseState(&savedCursorX_, &savedCursorY_);
            cursorWindow_ = SDL_GetKeyboardFocus();
        }
        cameraRotating_ = true;
        cursorCaptureFailed_ = false;
        if (rotationCursorEnabled_) rotationCursorWork_.store(true, std::memory_order_release);
    }
}

void SDLManager::OnCameraRotationStop()
{
    std::lock_guard _(cursorMutex_);
    cameraRotating_ = false;
    cursorCaptureFailed_ = false;
    if (cursorCaptured_) rotationCursorWork_.store(true, std::memory_order_release);
}

void SDLManager::RefreshCursor()
{
    SDL_Event event{};
    event.type = SDL_MOUSEMOTION;
    event.motion.windowID = SDL_GetWindowID(cursorWindow_);
    event.motion.x = savedCursorX_;
    event.motion.y = savedCursorY_;
    SDL_PushEvent(&event);
}

void SDLManager::RestoreCursorHints()
{
    if (hadRelativeModeCenter_) SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, previousRelativeModeCenter_.c_str());
    else SDL_ResetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER);
    if (hadRelativeCursorVisible_) SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE, previousRelativeCursorVisible_.c_str());
    else SDL_ResetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE);
}

void SDLManager::ReleaseRotationCursor()
{
    if (!cursorCaptured_) return;
    // Warp before leaving relative mode, avoiding a synthetic rotation delta.
    SDL_WarpMouseInWindow(cursorWindow_, savedCursorX_, savedCursorY_);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    RestoreCursorHints();
    cursorCaptured_ = false;
    virtualCursorState_ = VirtualCursorState::Restore;
    virtualCursorWork_.store(true, std::memory_order_release);
    RefreshCursor();
}

void SDLManager::UpdateRotationCursor()
{
    if (!rotationCursorWork_.load(std::memory_order_acquire)) return;
    std::lock_guard _(cursorMutex_);
    if (releaseCursor_) {
        ReleaseRotationCursor();
        releaseCursor_ = false;
        cursorCaptureFailed_ = false;
    }
    if (cursorCaptured_ && SDL_GetKeyboardFocus() != cursorWindow_) {
        ReleaseRotationCursor();
        cameraRotating_ = false;
    }
    if (!rotationCursorEnabled_ || !cameraRotating_) {
        ReleaseRotationCursor();
        rotationCursorWork_.store(false, std::memory_order_release);
        return;
    }
    if (!cursorCaptured_ && !cursorCaptureFailed_) {
        if (!cursorWindow_ || SDL_GetKeyboardFocus() != cursorWindow_
            || SDL_GetRelativeMouseMode() != SDL_FALSE) {
            cursorCaptureFailed_ = true;
            rotationCursorWork_.store(false, std::memory_order_release);
            return;
        }
        auto center = SDL_GetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER);
        hadRelativeModeCenter_ = center != nullptr;
        previousRelativeModeCenter_ = center ? center : "";
        auto visible = SDL_GetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE);
        hadRelativeCursorVisible_ = visible != nullptr;
        previousRelativeCursorVisible_ = visible ? visible : "";
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");
        SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE, "0");
        if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0) {
            RestoreCursorHints();
            cursorCaptureFailed_ = true;
            rotationCursorWork_.store(false, std::memory_order_release);
            WARN("Rotation cursor relative mode unavailable; request left inactive");
            return;
        }
        int height;
        SDL_GetWindowSize(cursorWindow_, &cursorWindowWidth_, &height);
        cursorCaptured_ = true;
        virtualCursorPosition_ = glm::vec2(savedCursorX_, savedCursorY_);
        virtualCursorState_ = VirtualCursorState::Pending;
        virtualCursorWork_.store(true, std::memory_order_release);
    }
    // Retain the native WASD six-pixel drag threshold before suppressing UI hover.
    if (cursorCaptured_ && virtualCursorState_ == VirtualCursorState::Pending) {
        auto dx = static_cast<double>(virtualCursorPosition_.x) - savedCursorX_;
        auto dy = static_cast<double>(virtualCursorPosition_.y) - savedCursorY_;
        if (dx * dx + dy * dy > 36.0) {
            virtualCursorState_ = VirtualCursorState::Override;
            RefreshCursor();
        }
    }
}

void SDLManager::OnCursorFocusLost(uint32_t windowId)
{
    std::lock_guard _(cursorMutex_);
    if (cursorWindow_ && SDL_GetWindowID(cursorWindow_) == windowId) {
        ReleaseRotationCursor();
        cameraRotating_ = false;
        rotationCursorWork_.store(false, std::memory_order_release);
    }
}


void SDLManager::UpdateVirtualCursorPosition(glm::vec2& position)
{
    if (!virtualCursorWork_.load(std::memory_order_acquire)) return;
    std::lock_guard _(cursorMutex_);
    virtualCursorPosition_ = position;
    if (virtualCursorState_ == VirtualCursorState::Override) {
        position = glm::vec2(cursorWindowWidth_ / 2, -1000000.0f);
    } else if (virtualCursorState_ == VirtualCursorState::Restore) {
        position = glm::vec2(savedCursorX_, savedCursorY_);
        virtualCursorState_ = VirtualCursorState::Normal;
        virtualCursorWork_.store(false, std::memory_order_release);
    }
}

END_SE()
