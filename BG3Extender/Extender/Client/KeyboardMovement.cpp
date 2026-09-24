#include <stdafx.h>
#include <Extender/Client/KeyboardMovement.h>
#include <Extender/ScriptExtender.h>
#include <atomic>
#include <limits>

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
void* AllocateRelay(uintptr_t site)
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    auto granularity = static_cast<uintptr_t>(info.dwAllocationGranularity);
    auto lower = site > 0x7fff0000ull ? site - 0x7fff0000ull : granularity;
    auto upper = site + 0x7fff0000ull;
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

END_NS()
