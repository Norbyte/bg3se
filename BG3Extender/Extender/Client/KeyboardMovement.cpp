#include <stdafx.h>
#include <Extender/Client/KeyboardMovement.h>
#include <Extender/ScriptExtender.h>
#include <atomic>
#include <limits>
#include <CoreLib/JsonLibs.h>
#include <filesystem>
#include <fstream>
#include <mutex>

BEGIN_NS(ecl)

extern "C" {
std::atomic<uint8_t> KeyboardMovementOverride{ 0 };
uint8_t const* KeyboardMovementControllerMode{ nullptr };
}

static_assert(sizeof(KeyboardMovementOverride) == 1);
static_assert(decltype(KeyboardMovementOverride)::is_always_lock_free);

namespace {
bool available{ false };

// Allocate a leaf JMP relay in CALL rel32 range; it lives with the engine hook.
void* AllocateRelay(uintptr_t site, uintptr_t otherSite = 0)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    auto granularity = static_cast<uintptr_t>(info.dwAllocationGranularity);
    if (!otherSite) otherSite = site;
    auto first = std::min(site, otherSite);
    auto last = std::max(site, otherSite);
    // Shared relays must lie in the intersection of both CALL ranges.
    auto lower = last > 0x7fff0000ull ? last - 0x7fff0000ull : granularity;
    auto upper = first + 0x7fff0000ull;
    auto address = (lower + granularity - 1) & ~(granularity - 1);
    while (address < upper) {
        MEMORY_BASIC_INFORMATION region;
        if (!VirtualQuery(reinterpret_cast<void*>(address), &region, sizeof(region))) break;
        auto end = reinterpret_cast<uintptr_t>(region.BaseAddress) + region.RegionSize;
        if (region.State == MEM_FREE && end - address >= info.dwPageSize) {
            if (auto relay = VirtualAlloc(reinterpret_cast<void*>(address), info.dwPageSize,
                MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) return relay;
        }
        if (end <= address) break;
        address = (end + granularity - 1) & ~(granularity - 1);
    }
    return nullptr;
}
}

extern "C" void KeyboardMovementModeBridge();

bool SetKeyboardMovementOverride(bool enabled)
{
    if (!available) return false;
    KeyboardMovementOverride.store(static_cast<uint8_t>(enabled), std::memory_order_relaxed);
    return true;
}

bool GetKeyboardMovementOverride()
{
    return KeyboardMovementOverride.load(std::memory_order_relaxed) != 0;
}

void InitializeKeyboardMovementOverride()
{
    if (available) return;
    auto site = GetStaticSymbols().ecl__MoveController__KeyboardModeGate;
    if (!site) return;

    // Preserve the original JE and every other eligibility check.
    int32_t modeOffset;
    memcpy(&modeOffset, site + 2, sizeof(modeOffset));
    auto mode = site + 7 + modeOffset;
    auto relay = static_cast<uint8_t*>(AllocateRelay(reinterpret_cast<uintptr_t>(site)));
    if (!relay) {
        WARN("Keyboard movement override unavailable: cannot allocate near relay");
        return;
    }
    uint8_t jump[] = { 0xff, 0x25, 0, 0, 0, 0 };
    memcpy(relay, jump, sizeof(jump));
    auto bridge = &KeyboardMovementModeBridge;
    memcpy(relay + sizeof(jump), &bridge, sizeof(bridge));
    DWORD relayProtection;
    if (!VirtualProtect(relay, 14, PAGE_EXECUTE_READ, &relayProtection)) {
        VirtualFree(relay, 0, MEM_RELEASE);
        WARN("Keyboard movement override unavailable: cannot protect relay");
        return;
    }
    FlushInstructionCache(GetCurrentProcess(), relay, 14);
    auto displacement = reinterpret_cast<intptr_t>(relay) - reinterpret_cast<intptr_t>(site + 5);
    if (displacement < INT32_MIN || displacement > INT32_MAX) {
        VirtualFree(relay, 0, MEM_RELEASE);
        return;
    }
    int32_t rel32 = static_cast<int32_t>(displacement);
    uint8_t replacement[] = { 0xe8, 0, 0, 0, 0, 0x84, 0xc0 };
    memcpy(replacement + 1, &rel32, sizeof(rel32));
    // Published before the patched CALL can execute and immutable afterward.
    KeyboardMovementControllerMode = mode;
    {
        WriteAnchor code(site, sizeof(replacement));
        memcpy(code.ptr(), replacement, sizeof(replacement));
    }
    FlushInstructionCache(GetCurrentProcess(), site, sizeof(replacement));
    available = true;
    DEBUG("Global keyboard movement gate installed; override is disabled");
}

namespace {
using InputConfigReloadProc = void(int64_t, char);
using InputConfigPathProc = uint64_t*(uint64_t*, uint32_t*, void*);
using InputConfigRemapProc = void(int64_t);
InputConfigReloadProc* reloadInputConfig{ nullptr };
InputConfigPathProc* inputConfigPath{ nullptr };
InputConfigRemapProc* inputConfigRemap{ nullptr };
std::mutex bindingMutex;
std::wstring bindingPath;
std::atomic<bool> bindingsActive{ false };

struct MovementBindingAction
{
    char const* Camera;
    char const* Character;
    char const* Defaults[3];
};

constexpr MovementBindingAction movementBindingActions[] = {
    { "CameraForward", "CharacterMoveForward", { "c:leftstick_yneg", "key:w", "key:up" } },
    { "CameraBackward", "CharacterMoveBackward", { "c:leftstick_ypos", "key:s", "key:down" } },
    { "CameraLeft", "CharacterMoveLeft", { "c:leftstick_xneg", "key:a", "key:left" } },
    { "CameraRight", "CharacterMoveRight", { "c:leftstick_xpos", "key:d", "key:right" } },
};

bool BuildMovementBindings(rapidjson::Document& document, bool& changed)
{
    using namespace rapidjson;
    changed = false;
    if (document.HasParseError() || !document.IsObject()) return false;
    for (auto const& action : movementBindingActions) {
        auto found = document.FindMember(action.Camera);
        if (found == document.MemberEnd()) continue;
        if (!found->value.IsArray()) return false;
        for (auto const& value : found->value.GetArray()) {
            if (!value.IsString()) return false;
        }
    }

    auto& allocator = document.GetAllocator();
    for (auto const& action : movementBindingActions) {
        Value bindings(kArrayType);
        auto found = document.FindMember(action.Camera);
        if (found != document.MemberEnd()) bindings.CopyFrom(found->value, allocator);
        else for (auto value : action.Defaults) {
            if (value) bindings.PushBack(Value(value, allocator), allocator);
        }
        for (auto& value : bindings.GetArray()) {
            if (!strcmp(value.GetString(), "c:leftstick_yneg")) value.SetString("c:leftstick_ypos", allocator);
            else if (!strcmp(value.GetString(), "c:leftstick_ypos")) value.SetString("c:leftstick_yneg", allocator);
        }
        auto existing = document.FindMember(action.Character);
        if (existing == document.MemberEnd()) {
            document.AddMember(Value(action.Character, allocator), bindings, allocator);
            changed = true;
        } else if (existing->value != bindings) {
            existing->value.Swap(bindings);
            changed = true;
        }
    }
    return true;
}

uint64_t* CaptureInputConfigPath(uint64_t* source, uint32_t* second, void* third)
{
    auto result = inputConfigPath(source, second, third);
    auto text = result ? reinterpret_cast<char const*>(*result) : nullptr;
    if (!text) return result;
    auto filename = strrchr(text, '/');
    auto separator = strrchr(text, '\\');
    if (separator && (!filename || separator > filename)) filename = separator;
    if (!filename || _stricmp(filename + 1, "inputconfig_p1.json")) return result;
    auto path = FromStdUTF8(text);
    if (path.empty()) return result;
    std::lock_guard _(bindingMutex);
    bindingPath = std::move(path);
    return result;
}

bool WriteMovementBindings(rapidjson::Document const& document)
{
    rapidjson::StringBuffer text;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(text);
    document.Accept(writer);
    auto temporary = bindingPath + L".movement-" + std::to_wstring(GetCurrentProcessId()) + L".tmp";
    std::ofstream output(std::filesystem::path(temporary), std::ios::binary | std::ios::trunc);
    if (!output) return false;
    output.write(text.GetString(), text.GetSize());
    output.close();
    if (output) {
        for (unsigned attempt = 0; attempt < 20; attempt++) {
            if (MoveFileExW(temporary.c_str(), bindingPath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) return true;
            auto error = GetLastError();
            if (error != ERROR_SHARING_VIOLATION && error != ERROR_ACCESS_DENIED) break;
            Sleep(25);
        }
    }
    DeleteFileW(temporary.c_str());
    return false;
}

void InputConfigRemapped(int64_t menu)
{
    inputConfigRemap(menu);
    if (bindingsActive.load(std::memory_order_acquire)) SyncKeyboardMovementBindings();
}
}

bool SyncKeyboardMovementBindings()
{
    if (!reloadInputConfig) return false;
    bool missing;
    {
        std::lock_guard _(bindingMutex);
        missing = bindingPath.empty();
    }
    // Reload discovers the engine-selected profile, including CustomProfile.
    if (missing) reloadInputConfig(0, 0);
    bool changed = false;
    bool success = false;
    {
        std::lock_guard _(bindingMutex);
        std::ifstream input(std::filesystem::path(bindingPath), std::ios::binary);
        if (input) {
            std::string contents{ std::istreambuf_iterator<char>(input), {} };
            input.close();
            rapidjson::Document document;
            document.Parse<rapidjson::kParseNoFlags>(contents.data(), contents.size());
            success = BuildMovementBindings(document, changed) && (!changed || WriteMovementBindings(document));
        }
    }
    if (!success) {
        SetKeyboardMovementOverride(false);
        WARN("Keyboard movement binding synchronization failed");
        return false;
    }
    // The path hook takes the same mutex, so release it before reloading.
    if (changed) reloadInputConfig(0, 0);
    bindingsActive.store(true, std::memory_order_release);
    return true;
}

void ResetKeyboardMovementBindings()
{
    bindingsActive.store(false, std::memory_order_release);
}

void InitializeKeyboardMovementBindings()
{
    if (reloadInputConfig) return;
    auto& sym = GetStaticSymbols();
    auto reload = sym.App__ReloadInputConfig;
    uint8_t const* sites[] = { sym.App__InputConfigPathCall, sym.gui__DCKeybindingOptions__RemapCall };
    if (!reload || !sites[0] || !sites[1]) return;
    DWORD64 imageBase;
    auto function = RtlLookupFunctionEntry(reinterpret_cast<DWORD64>(reload), &imageBase, nullptr);
    if (!function || imageBase + function->BeginAddress != reinterpret_cast<DWORD64>(reload)
        || reinterpret_cast<DWORD64>(sites[0]) < imageBase + function->BeginAddress
        || reinterpret_cast<DWORD64>(sites[0] + 5) > imageBase + function->EndAddress) return;

    uint8_t const* originals[2];
    uint8_t const pathEntry[] = { 0x48, 0x89, 0x5c, 0x24, 0x18, 0x48, 0x89, 0x6c, 0x24, 0x20, 0x48, 0x89, 0x4c, 0x24, 0x08, 0x56 };
    uint8_t const remapEntry[] = { 0x40, 0x55, 0x57, 0x48, 0x83, 0xec, 0x28, 0x48, 0x8b, 0xb9, 0x20, 0x08, 0, 0, 0x48, 0x8b };
    uint8_t const reloadEntry[] = { 0x48, 0x89, 0x5c, 0x24, 0x08, 0x48, 0x89, 0x74, 0x24, 0x10, 0x48, 0x89, 0x7c, 0x24, 0x18, 0x55 };
    if (memcmp(reload, reloadEntry, sizeof(reloadEntry))) return;
    for (unsigned i = 0; i < 2; i++) {
        if (sites[i][0] != 0xe8) return;
        int32_t offset;
        memcpy(&offset, sites[i] + 1, sizeof(offset));
        originals[i] = sites[i] + 5 + offset;
    }
    if (memcmp(originals[0], pathEntry, sizeof(pathEntry)) || memcmp(originals[1], remapEntry, sizeof(remapEntry))) return;
    auto relay = static_cast<uint8_t*>(AllocateRelay(reinterpret_cast<uintptr_t>(sites[0]), reinterpret_cast<uintptr_t>(sites[1])));
    if (!relay) return;
    void* targets[] = { reinterpret_cast<void*>(&CaptureInputConfigPath), reinterpret_cast<void*>(&InputConfigRemapped) };
    uint8_t calls[2][5];
    for (unsigned i = 0; i < 2; i++) {
        auto displacement = reinterpret_cast<intptr_t>(relay + 16 * i) - reinterpret_cast<intptr_t>(sites[i] + 5);
        if (displacement < INT32_MIN || displacement > INT32_MAX) {
            VirtualFree(relay, 0, MEM_RELEASE);
            return;
        }
        uint8_t jump[] = { 0xff, 0x25, 0, 0, 0, 0 };
        memcpy(relay + 16 * i, jump, sizeof(jump));
        memcpy(relay + 16 * i + sizeof(jump), &targets[i], sizeof(void*));
        calls[i][0] = 0xe8;
        auto rel32 = static_cast<int32_t>(displacement);
        memcpy(calls[i] + 1, &rel32, sizeof(rel32));
    }
    DWORD protection;
    if (!VirtualProtect(relay, 32, PAGE_EXECUTE_READ, &protection)) {
        VirtualFree(relay, 0, MEM_RELEASE);
        return;
    }
    FlushInstructionCache(GetCurrentProcess(), relay, 32);
    inputConfigPath = reinterpret_cast<InputConfigPathProc*>(const_cast<uint8_t*>(originals[0]));
    inputConfigRemap = reinterpret_cast<InputConfigRemapProc*>(const_cast<uint8_t*>(originals[1]));
    for (unsigned i = 0; i < 2; i++) {
        WriteAnchor code(sites[i], 5);
        memcpy(code.ptr(), calls[i], 5);
        FlushInstructionCache(GetCurrentProcess(), sites[i], 5);
    }
    reloadInputConfig = reinterpret_cast<InputConfigReloadProc*>(const_cast<uint8_t*>(reload));
}

END_NS()
