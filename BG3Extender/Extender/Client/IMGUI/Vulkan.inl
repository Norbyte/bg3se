#pragma once
#define SL_ENABLE_DEPRECATED_API 1
#include <GameDefinitions/Base/Base.h>
#include <Extender/Client/IMGUI/Backends.h>
#include <Extender/ScriptExtender.h>
#include <CoreLib/Wrappers.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui_internal.h>
#include <cstring>
#include <Extender/Client/IMGUI/sl/sl.h>
#include <Extender/Client/IMGUI/sl/sl_dlss_g.h>
#include <Extender/Client/IMGUI/sl/sl_core_types.h>
#include <Extender/Client/IMGUI/sl/sl_helpers.h>
#include <Extender/Client/IMGUI/sl/sl_core_api.h>


BEGIN_SE()

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    // ... other fields we don't need
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;
    BOOLEAN ReadImageFileExecOptions;
    BOOLEAN BeingDebugged;
    BOOLEAN SpareBool;
    HANDLE Mutant;
    PVOID ImageBaseAddress;
    PPEB_LDR_DATA Ldr;
    // ... other fields we don't need
} PEB, *PPEB;

#define VK_HOOK(name) enum class Vk##name##HookTag {}; \
    using Vk##name##HookType = WrappableFunction<Vk##name##HookTag, decltype(vk##name)>; \
    Vk##name##HookType* Vk##name##HookType::gHook;

VK_HOOK(CreateInstance)
VK_HOOK(CreateDevice)
VK_HOOK(DestroyDevice)
VK_HOOK(CreatePipelineCache)
VK_HOOK(CreateSwapchainKHR)
VK_HOOK(DestroySwapchainKHR)
VK_HOOK(QueuePresentKHR)
enum class SlDLSSGSetOptionsHookTag {};
using SlDLSSGSetOptionsHookType = WrappableFunction<SlDLSSGSetOptionsHookTag, sl::Result(sl::ViewportHandle, const sl::DLSSGOptions&)>;
template<> SlDLSSGSetOptionsHookType* SlDLSSGSetOptionsHookType::gHook = nullptr;

END_SE()


#define VK_ACCESS_ALL_READ_BITS                                                        \
  (VK_ACCESS_INDIRECT_COMMAND_READ_BIT | VK_ACCESS_INDEX_READ_BIT |                    \
   VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT |                  \
   VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT |                   \
   VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | \
   VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_HOST_READ_BIT | VK_ACCESS_MEMORY_READ_BIT)

#define VK_CHECK(expr) { auto _rval = (expr); if (_rval != VK_SUCCESS) { ERR(#expr " failed: %d", _rval); } }

BEGIN_NS(extui)

namespace {
    thread_local bool makingOwnDLSSGCall_ = false;
}


class VulkanBackend : public RenderingBackend
{
public:
    static constexpr unsigned TextureSoftCap = 2000;
    static constexpr unsigned TextureHardCap = 2048;

    VulkanBackend(IMGUIManager& ui) : ui_(ui) {}

    ~VulkanBackend() override
    {
        ERR("Destroying VK backend");
        releaseSwapChain(swapchain_);
        DestroyUI();
        DisableHooks();
    }

    void EnableHooks() override
    {
        if (CreateInstanceHook_.IsWrapped()) return;

        ERR("VulkanBackend::EnableHooks() CACA");

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        auto createInstance = vkGetInstanceProcAddr(nullptr, "vkCreateInstance");
        CreateInstanceHook_.Wrap(ResolveFunctionTrampoline(createInstance));
        DetourTransactionCommit();

        CreateInstanceHook_.SetPostHook(&VulkanBackend::vkCreateInstanceHooked, this);
        CreateDeviceHook_.SetPostHook(&VulkanBackend::vkCreateDeviceHooked, this);
        DestroyDeviceHook_.SetPreHook(&VulkanBackend::vkDestroyDeviceHooked, this);
        CreatePipelineCacheHook_.SetPostHook(&VulkanBackend::vkCreatePipelineCacheHooked, this);
        CreateSwapchainKHRHook_.SetPostHook(&VulkanBackend::vkCreateSwapchainKHRHooked, this);
        DestroySwapchainKHRHook_.SetPreHook(&VulkanBackend::vkDestroySwapchainKHRHooked, this);
        QueuePresentKHRHook_.SetPreHook(&VulkanBackend::vkQueuePresentKHRHooked, this);


        ///HERE

        hSL_  = LoadLibraryW(L"upscaler.dll");
        if (hSL_) {
            ERR("[EXTUI_HOOK_SETUP] upscaler.dll loaded");
        } else {
            ERR("[EXTUI_HOOK_SETUP] WARNING: upscaler.dll not found!");
        }
        sl_ = GetModuleHandleW(L"sl.interposer.dll");
        ERR("[EXTUI_HOOK_SETUP] sl.interposer.dll = %p", sl_);
        if (!sl_) 
        {
            ERR("[EXTUI_HOOK_SETUP] sl.interposer.dll not found");
            sl_ = LoadLibraryW(L"sl.interposer.dll");
            ERR("[EXTUI_HOOK_SETUP] sl.interposer.dll = %p", sl_);
        }
        if (!sl_) {
            ERR("[EXTUI_HOOK_SETUP] sl.interposer.dll not found — DLSS-FG hook skipped");
        }
        else {
            auto slQueuePresent = reinterpret_cast<PFN_vkQueuePresentKHR>(
                GetProcAddress(sl_, "vkQueuePresentKHR")
            );
            ERR("[EXTUI_HOOK_SETUP] sl.interposer.dll vkQueuePresentKHR = %p", slQueuePresent);
            if (!slQueuePresent) {
                ERR("[EXTUI_HOOK_SETUP] sl.interposer.dll missing vkQueuePresentKHR export");
            }
            else {
                // Store DLSSG's present function for later use
                dlssgPresentFunction_ = slQueuePresent;
                ERR("[EXTUI_HOOK_SETUP] Stored DLSSG vkQueuePresentKHR @ %p", dlssgPresentFunction_);
                // DO NOT re-hook! We keep our original hook on the game's vkQueuePresentKHR
                
                // Get all other DLSS-FG Vulkan functions we need
                dlssgCreateSwapchainKHR_ = reinterpret_cast<PFN_vkCreateSwapchainKHR>(
                    GetProcAddress(sl_, "vkCreateSwapchainKHR"));
                    
                ERR("[EXTUI_HOOK_SETUP] Retrieved DLSS-FG Vulkan functions:");
                ERR("vkCreateSwapchainKHR: %p", dlssgCreateSwapchainKHR_);
            }
        }
    }

    void DisableHooks() override
    {
        if (!CreateInstanceHook_.IsWrapped()) return;

        ERR("VulkanBackend::DisableHooks()");

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreateInstanceHook_.Unwrap();
        CreateDeviceHook_.Unwrap();
        DestroyDeviceHook_.Unwrap();
        CreatePipelineCacheHook_.Unwrap();
        CreateSwapchainKHRHook_.Unwrap();
        DestroySwapchainKHRHook_.Unwrap();
        QueuePresentKHRHook_.Unwrap();
        if (slDLSSGSetOptionsHook_.IsWrapped()) {
            slDLSSGSetOptionsHook_.Unwrap();
        }
        DetourTransactionCommit();
    }

    void InitializeUI() override
    {
        if (initialized_) return;

        ERR("VulkanBackend::InitializeUI() - Entry - drawViewport_: %d", drawViewport_);

        VkDescriptorPoolSize poolSize
        {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = TextureHardCap,
        };

        VkDescriptorPoolCreateInfo createInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets = TextureHardCap,
            .poolSizeCount = 1,
            .pPoolSizes = &poolSize
        };
        VK_CHECK(vkCreateDescriptorPool(device_, &createInfo, nullptr, &descriptorPool_));

        VkSamplerCreateInfo samplerInfo
        {
            .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter = VK_FILTER_LINEAR,
            .minFilter = VK_FILTER_LINEAR,
            .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .maxLod = VK_LOD_CLAMP_NONE,
        };
        VK_CHECK(vkCreateSampler(device_, &samplerInfo, nullptr, &sampler_));
        ERR("VK initialization: desc pool %p, sampler %p", descriptorPool_, sampler_);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance_;
        init_info.PhysicalDevice = physicalDevice_;
        init_info.Device = device_;
        init_info.QueueFamily = queueFamily_;
        init_info.Queue = renderQueue_;
        init_info.PipelineCache = pipelineCache_;
        init_info.DescriptorPool = descriptorPool_;
        init_info.Subpass = 0;
        init_info.MinImageCount = swapchain_.images_.size();
        init_info.ImageCount = swapchain_.images_.size();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = [](VkResult err) {
            if (err != VK_SUCCESS) {
                ERR("[IMGUI] VK operation failed: %d", err);
                TryDebugBreak();
            }
        };
        // init_info.RenderPass = presentRenderPass_;
        init_info.RenderPass = swapchain_.renderPass_;
        ImGui_ImplVulkan_Init(&init_info);
        ImGui_ImplVulkan_CreateFontsTexture();

        // Ensure drawViewport_ is set so first frame after initialization can render
        // Otherwise presentPreHook won't be called due to drawViewport_ being -1
        //drawViewport_ = 0;
        
        initialized_ = true;

        ERR("VK POSTINIT - initialized: %d, drawViewport_: %d, uiFrameworkStarted: %d", 
            initialized_ ? 1 : 0, drawViewport_, uiFrameworkStarted_ ? 1 : 0);
        ERR("    Instance %p, PhysicalDevice %p, Device %p", instance_, physicalDevice_, device_);
        ERR("    QueueFamily %d, RenderQueue %p", queueFamily_, renderQueue_, swapChain_);
        ERR("    PipelineCache %p, DescriptorPool %p, Sampler %p", pipelineCache_, descriptorPool_, sampler_);
        ERR("VK SWAPCHAIN");
        ERR("    SwapChain %p, RenderPass %p, CommandPool %p", swapChain_, swapchain_.renderPass_, swapchain_.commandPool_);
        ERR("VK FRAMEBUFFERS");
        for (auto const& image : swapchain_.images_) {
            ERR("    Image %p, FB %p, View %p, Fence %p", image.image, image.framebuffer, image.view, image.fence);
        }
    }

    void DestroyUI() override
    {
        if (!initialized_) return;

        ERR("VK shutdown");

        ImGui_ImplVulkan_Shutdown();
        drawViewport_ = -1;
        initialized_ = false;
    }

    void slDLSSGSetOptionsHooked(
        sl::ViewportHandle viewport,
        const sl::DLSSGOptions& options,
        sl::Result result)
    {
        dlssgViewport_ = viewport;  // Store the viewport DLSS-G is using
        //ERR("[DLSSG HOOK] DLSS-G using viewport: %u", viewport);
        //ERR("[DLSSG HOOK] === POST-HOOK ENTRY ===");
        //ERR("[DLSSG HOOK] viewport: %u, mode: %d, flags: %d, result: %s", 
            //viewport, static_cast<int>(options.mode), static_cast<int>(options.flags), sl::getResultAsStr(result));
        
        // Skip our own calls to avoid infinite recursion
        if (makingOwnDLSSGCall_) {
            //ERR("[DLSSG HOOK] Skipping our own call to slDLSSGSetOptions");
            return;
        }
        
        //ERR("[DLSSG HOOK] External slDLSSGSetOptions intercepted - viewport: %u, mode: %d, result: %s", 
            //viewport, static_cast<int>(options.mode), sl::getResultAsStr(result));
        
        // Only proceed if the original call was successful
        if (result != sl::Result::eOk) {
            //ERR("[DLSSG HOOK] Original slDLSSGSetOptions failed, skipping override");
            return;
        }
        
        //ERR("[DLSSG HOOK] Checking conditions: isDlssgActive=%s, okayToModifyFG=%s", 
            //isDlssgActive() ? "true" : "false", okayToModifyFG_ ? "true" : "false");
        
        // Now apply our override logic - we want to override external calls with our menu state
        if (isDlssgActive() && okayToModifyFG_)
        {
            //ERR("[DLSSG HOOK] Conditions met, proceeding with override logic");
            
            bool wantPause = menuVisible_;
            //ERR("[DLSSG HOOK] menuVisible_: %s, wantPause: %s", menuVisible_ ? "true" : "false", wantPause ? "true" : "false");
            
            // Always override external calls with our desired state
            sl::DLSSGOptions overrideOptions = {};
            overrideOptions.flags = sl::DLSSGFlags::eRequestVRAMEstimate;   // ask plugin to fill VRAM
            overrideOptions.mode = wantPause ? sl::DLSSGMode::eOn : sl::DLSSGMode::eOn;
            
            //ERR("[DLSSG HOOK] Created override options: mode=%d, flags=%d", 
            //    static_cast<int>(overrideOptions.mode), static_cast<int>(overrideOptions.flags));
            
            // Only override if our desired state is different from what was requested
            if (static_cast<int>(options.mode) != static_cast<int>(overrideOptions.mode))
            {
                //ERR("[DLSSG HOOK] Overriding external DLSS-G mode from %d to %d (menu visible: %s)", 
                    //static_cast<int>(options.mode), static_cast<int>(overrideOptions.mode), 
                    //wantPause ? "true" : "false");
                
                //ERR("[DLSSG HOOK] Checking originalSlDLSSGSetOptions_: %p", originalSlDLSSGSetOptions_);
                if (originalSlDLSSGSetOptions_) 
                {
                    //ERR("[DLSSG HOOK] Setting makingOwnDLSSGCall_ = true");
                    // Set flag to indicate we're making our own call
                    makingOwnDLSSGCall_ = true;
                    
                    //ERR("[DLSSG HOOK] About to call originalSlDLSSGSetOptions_ with viewport: %u, mode: %d", 
                        //viewport, static_cast<int>(overrideOptions.mode));
                    
                    sl::Result overrideResult = originalSlDLSSGSetOptions_(viewport, overrideOptions);
                    sl::DLSSGState st{};
                    sl::Result r = slDLSSGGetState_(0, st, nullptr);
                    auto text= sl::getResultAsStr(r);
                    if (r == sl::Result::eOk)
                    {
                        //HERE GOOD
                        //DumpDLSSGState(st);
                    }
                    
                    //ERR("[DLSSG HOOK] originalSlDLSSGSetOptions_ returned, setting makingOwnDLSSGCall_ = false");
                    makingOwnDLSSGCall_ = false;
                    
                    auto text2 = sl::getResultAsStr(overrideResult);
                    //ERR("[DLSSG HOOK] Override slDLSSGSetOptions result: %s", text2);
                    if (overrideResult == sl::Result::eOk) {
                        fgPaused_ = wantPause;
                        //ERR("[DLSSG HOOK] Frame-Generation %s (overrode external call)", fgPaused_ ? "PAUSED" : "RESUMED");
                    }
                    else
                    {
                        //ERR("[DLSSG HOOK] Failed to override DLSS-G mode: %s", text2);
                    }
                }
                else
                {
                    //ERR("[DLSSG HOOK] originalSlDLSSGSetOptions_ is null!");
                }
            }
            else
            {
                //ERR("[DLSSG HOOK] External call matches our desired state, no override needed");
                fgPaused_ = wantPause; // Update our state to match
            }
        }
        else
        {
            //ERR("[DLSSG HOOK] Conditions not met for override");
        }
        
        //ERR("[DLSSG HOOK] === POST-HOOK EXIT ===");
    }

    HMODULE linkStreamline() {
        


        wchar_t exePath[MAX_PATH] = {};
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);

        // Trim filename → leave just the folder
        wchar_t* lastBS = wcsrchr(exePath, L'\\');
        if (lastBS) *lastBS = L'\0';

        std::wstring imguiPath = std::wstring(exePath) +
            L"\\mods\\UpscalerBasePlugin\\Streamline\\sl.imgui.dll";

        slImGui_ = LoadLibraryExW(
            imguiPath.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        if (slImGui_)
            ERR("[STREAMLINE] sl.imgui.dll loaded from \"%ls\" (%p)",
                imguiPath.c_str(), slImGui_);
        else
            ERR("[STREAMLINE] sl.imgui.dll NOT found at \"%ls\" (GLE=%lu)",
                imguiPath.c_str(), GetLastError());




        ERR("[STREAMLINE] Searching for real Streamline interposer...");
        
        // Get PEB (Process Environment Block) to walk loaded modules
        PPEB peb = reinterpret_cast<PPEB>(__readgsqword(0x60)); // x64 only
        PPEB_LDR_DATA ldr = peb->Ldr;
        
        PLIST_ENTRY head = &ldr->InLoadOrderModuleList;
        PLIST_ENTRY entry = head->Flink;
        
        int count = 0;
        while (entry != head && count < 1000) { // Safety limit
            PLDR_DATA_TABLE_ENTRY dataEntry = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
            
            if (dataEntry->FullDllName.Buffer && dataEntry->DllBase) {
                wchar_t* dllName = dataEntry->FullDllName.Buffer;
                HMODULE baseAddress = reinterpret_cast<HMODULE>(dataEntry->DllBase);
                
                if(wcsstr(dllName, L"sl.imgui.dll"))
                {
                    ERR("[STREAMLINE] Found sl.imgui.dll candidate: %ls (%p)", dllName, baseAddress);
                }
                // Check if this looks like a Streamline interposer
                if (wcsstr(dllName, L"sl.interposer.dll")) {
                    ERR("[STREAMLINE] Found sl.interposer.dll candidate: %ls (%p)", dllName, baseAddress);
                    
                    // Test if it has the Streamline functions we need
                    slGetFeatureFunction_ = reinterpret_cast<PFun_slGetFeatureFunction*>(
                        GetProcAddress(baseAddress, "slGetFeatureFunction"));
                        if (slGetFeatureFunction_) {
                            ERR("[STREAMLINE] Got slGetFeatureFunction: %p", slGetFeatureFunction_);
                            
                            void* funcPtr = nullptr;
                            sl::Result result = slGetFeatureFunction_(sl::kFeatureDLSS_G, "slDLSSGSetOptions", funcPtr);
                            if (result == sl::Result::eOk && funcPtr) {
                                slDLSSGSetOptions_ = reinterpret_cast<PFun_slDLSSGSetOptions*>(funcPtr);
                                originalSlDLSSGSetOptions_ = slDLSSGSetOptions_; // Store original
                                okayToModifyFG_ = true;
                                ERR("[STREAMLINE] ✓ Got slDLSSGSetOptions: %p", slDLSSGSetOptions_);

                                slSetTag_ = reinterpret_cast<PFun_slSetTag*>(GetProcAddress(baseAddress, "slSetTag"));
                                slSetTagForFrame_ = reinterpret_cast<PFun_slSetTagForFrame*>(GetProcAddress(baseAddress, "slSetTagForFrame"));
                                slGetNewFrameToken_ = reinterpret_cast<PFun_slGetNewFrameToken*>(GetProcAddress(baseAddress, "slGetNewFrameToken"));
                                void* pFn = nullptr;
                                if (slGetFeatureFunction_(sl::kFeatureDLSS_G, "slDLSSGGetState", pFn) == sl::Result::eOk && pFn)
                                {
                                    slDLSSGGetState_ = reinterpret_cast<PFun_slDLSSGGetState*>(pFn);
                                    ERR("[STREAMLINE] ✓ Got slDLSSGGetState: %p", slDLSSGGetState_);
                                }
                                else
                                {
                                    ERR("[STREAMLINE] ✗ Could not get slDLSSGGetState");
                                }
                                
                                if (slSetTag_) {
                                    ERR("[STREAMLINE] ✓ Got slSetTag: %p", slSetTag_);
                                } else {
                                    ERR("[STREAMLINE] ✗ Failed to get slSetTag");
                                }
                                
                                if (slSetTagForFrame_) {
                                    ERR("[STREAMLINE] ✓ Got slSetTagForFrame: %p", slSetTagForFrame_);
                                } else {
                                    ERR("[STREAMLINE] ✗ Failed to get slSetTagForFrame");
                                }
                                
                                if (slGetNewFrameToken_) {
                                    ERR("[STREAMLINE] ✓ Got slGetNewFrameToken: %p", slGetNewFrameToken_);
                                } else {
                                    ERR("[STREAMLINE] ✗ Failed to get slGetNewFrameToken");
                                }
                                sl::Result result = slGetFeatureFunction_(sl::kFeatureCommon, "slSetConstants", funcPtr);
                                if (result == sl::Result::eOk && funcPtr) {
                                    slSetConstants_ = reinterpret_cast<PFun_slSetConstants*>(funcPtr);
                                    ERR("[STREAMLINE] ✓ Got slSetConstants: %p", slSetConstants_);
                                } else {
                                    ERR("[STREAMLINE] ✗ Failed to get slSetConstants: %s", sl::getResultAsStr(result));
                                }

                                result = slGetFeatureFunction_(sl::kFeatureCommon, "slEvaluateFeature", funcPtr);
                                if (result == sl::Result::eOk && funcPtr) {
                                    slEvaluateFeature_ = reinterpret_cast<PFun_slEvaluateFeature*>(funcPtr);
                                    ERR("[STREAMLINE] ✓ Got slEvaluateFeature: %p", slEvaluateFeature_);
                                } else {
                                    ERR("[STREAMLINE] ✗ Failed to get slEvaluateFeature: %s", sl::getResultAsStr(result));
                                }
                                
                                // Hook slDLSSGSetOptions
                                DetourTransactionBegin();
                                DetourUpdateThread(GetCurrentThread());
                                slDLSSGSetOptionsHook_.Wrap(ResolveFunctionTrampoline(slDLSSGSetOptions_));
                                DetourTransactionCommit();
                                
                                slDLSSGSetOptionsHook_.SetPostHook(&VulkanBackend::slDLSSGSetOptionsHooked, this);
                                ERR("[STREAMLINE] ✓ Hooked slDLSSGSetOptions");
                                
                            } else {
                                ERR("[STREAMLINE] ✗ Failed to get slDLSSGSetOptions, result: %d", static_cast<int>(result));
                            }
                    }
                    auto slInit = GetProcAddress(baseAddress, "slInit");
                    slShutdown_ = reinterpret_cast<PFun_slShutdown*>(GetProcAddress(baseAddress, "slShutdown"));
                    ERR("[STREAMLINE] Testing exports - slGetFeatureFunction: %p, slInit: %p, slShutdown: %p", 
                        slGetFeatureFunction_, slInit, slShutdown_);
                    
                    if (slGetFeatureFunction_ && slInit && slShutdown_) {
                        ERR("[STREAMLINE] ✓ Found REAL Streamline interposer: %ls (%p)", dllName, baseAddress);
                        return baseAddress;
                    } else {
                        ERR("[STREAMLINE] ✗ This is a DXGI proxy, not real Streamline: %ls", dllName);
                    }
                }
            }
            
            entry = entry->Flink;
            count++;
        }
        
        ERR("[STREAMLINE] No real Streamline interposer found after checking %d modules", count);
        return nullptr;
    }

    void NewFrame() override
    {
        // Speculative check to avoid unnecessary locking
        if (!initialized_) return;

        std::lock_guard _(globalResourceLock_);

        // Locked check (at this point we're certain noone is manipulating the initialized flag)
        if (!initialized_) return;

        curViewport_ = (curViewport_ + 1) % viewports_.size();
        GImGui->Viewports[0] = &viewports_[curViewport_].Viewport;

        if (requestReloadFonts_) {
            ERR("Rebuilding font atlas");
            ImGui_ImplVulkan_DestroyFontsTexture();
            requestReloadFonts_ = false;
            if(!okayToModifyFG_)
            {
                HMODULE hDLSSG = GetModuleHandleW(L"sl.dlss_g.dll");
                if (hDLSSG && reinterpret_cast<uintptr_t>(hDLSSG) != 0) 
                {
                    sl_=linkStreamline();
                }
            }
        }
        ImGui_ImplVulkan_NewFrame();
        //ERR("VK: NewFrame");
    }

    void FinishFrame() override
    {
        // Speculative check to avoid unnecessary locking
        if (!initialized_) {
            ERR("VK: FinishFrame - skipped (not initialized)");
            return;
        }

        std::lock_guard _(globalResourceLock_);

        // Locked check (at this point we're certain noone is manipulating the initialized flag)
        if (!initialized_) {
            ERR("VK: FinishFrame - skipped after lock (not initialized)");
            return;
        }
        
        //ERR("VK: FinishFrame - initializing with curViewport: %d, drawViewport_ before: %d", curViewport_, drawViewport_);

        auto& vp = viewports_[curViewport_].Viewport;
        auto& drawLists = viewports_[curViewport_].ClonedDrawLists;

        for (auto list : drawLists) {
            list->~ImDrawList();
            IM_FREE(list);
        }

        drawLists.clear();

        for (int i = 0; i < vp.DrawDataP.CmdLists.Size; i++) {
            auto list = vp.DrawDataP.CmdLists[i];
            auto drawList = list->CloneOutput();
            vp.DrawDataP.CmdLists[i] = drawList;
            drawLists.Add(drawList);
        }

        drawViewport_ = curViewport_;
        menuVisible_ = (vp.DrawDataP.CmdLists.Size > 0);   // simple heuristic
        //ERR("[MENU DEBUG] FinishFrame: CmdLists.Size=%d, menuVisible_=%s, drawViewport_=%d", 
            //vp.DrawDataP.CmdLists.Size, menuVisible_ ? "true" : "false", drawViewport_);
    }

    void ClearFrame() override
    {
        if (!initialized_) {
            ERR("VK: ClearFrame - skipped (not initialized)");
            return;
        }

        ERR("VK: ClearFrame - setting drawViewport_ from %d to -1", drawViewport_);
        drawViewport_ = -1;
    }

    std::optional<TextureLoadResult> RegisterTexture(TextureDescriptor* descriptor) override
    {
        if (!initialized_) return {};

        if (descriptor->Vulkan.Views.size() != 1 || !descriptor->Vulkan.Views[0]->View) {
            ERR("VK texture has no render view?");
            return {};
        }

        auto view = descriptor->Vulkan.Views[0]->View;
        if (!view) return {};

        if (textures_ > TextureSoftCap) {
            if (!textureLimitWarningShown_) {
                ERR("UI texture limit reached. Newly loaded textures may not load or render correctly");
                textureLimitWarningShown_ = true;
            }
            return {};
        }

        textures_++;
        return TextureLoadResult{ 
            TextureOpaqueHandle(view),
            descriptor->Vulkan.ImageData.Width, 
            descriptor->Vulkan.ImageData.Height
        };
    }

    void UnregisterTexture(TextureOpaqueHandle opaqueHandle) override
    {
        if (!initialized_) return;

        auto imageView = reinterpret_cast<VkImageView>(opaqueHandle);
        auto desc = textureDescriptors_.get_or_default(imageView, 0);
        if (desc) {
            ImGui_ImplVulkan_RemoveTexture(desc);
            textureDescriptors_.remove(imageView);
        }

        textures_--;
    }

    std::optional<ImTextureID> BindTexture(TextureOpaqueHandle opaqueHandle) override
    {
        auto imageView = reinterpret_cast<VkImageView>(opaqueHandle);
        auto desc = textureDescriptors_.get_or_default(imageView, 0);
        if (!desc) {
            desc = ImGui_ImplVulkan_AddTexture(sampler_, imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            if (desc) {
                textureDescriptors_.set(imageView, desc);
            }
        }
        
        return (ImTextureID)desc;
    }

    bool IsInitialized() override
    {
        return initialized_;
    }

    void ReloadFonts() override
    {
        requestReloadFonts_ = true;
    }

    glm::ivec2 GetViewportSize() override
    {
        return glm::ivec2(swapchain_.width_, swapchain_.height_);
    }

private:
    struct SwapchainImageInfo
    {
        VkImage image{ VK_NULL_HANDLE };
        VkFramebuffer framebuffer{ VK_NULL_HANDLE };
        VkImageView view{ VK_NULL_HANDLE };
        VkFence fence{ VK_NULL_HANDLE };
        VkSemaphore uiDoneSemaphore{ VK_NULL_HANDLE };
        VkCommandBuffer commandBuffer{ VK_NULL_HANDLE };
    };

    struct SwapchainInfo
    {
        VkRenderPass renderPass_{ VK_NULL_HANDLE };
        VkCommandPool commandPool_{ VK_NULL_HANDLE };
        Array<SwapchainImageInfo> images_;
        uint32_t width_;
        uint32_t height_;
    };

    struct ViewportInfo
    {
        ImGuiViewportP Viewport;
        Array<ImDrawList*> ClonedDrawLists;
    };

    void vkCreateInstanceHooked(
        const VkInstanceCreateInfo* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkInstance* pInstance,
        VkResult result)
    {
        ERR("vkCreateInstance -> %p, %d", *pInstance, result);
        if (pCreateInfo->pApplicationInfo != nullptr && pCreateInfo->pApplicationInfo->pApplicationName != nullptr) {
            ERR("App=%s", pCreateInfo->pApplicationInfo->pApplicationName);
        }

        for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount; i++) {
            ERR("Layer=%s", pCreateInfo->ppEnabledLayerNames[i]);
        }

        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            ERR("Extension=%s", pCreateInfo->ppEnabledExtensionNames[i]);
        }

        if (result != VK_SUCCESS) return;

        instance_ = *pInstance;

        if (!CreateDeviceHook_.IsWrapped()) {
            ERR("Hooking CreateDevice()");

            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            auto createDevice = vkGetInstanceProcAddr(instance_, "vkCreateDevice");
            auto destroyDevice = vkGetInstanceProcAddr(instance_, "vkDestroyDevice");
            CreateDeviceHook_.Wrap(ResolveFunctionTrampoline(createDevice));
            DestroyDeviceHook_.Wrap(ResolveFunctionTrampoline(destroyDevice));
            DetourTransactionCommit();
        }
    }

// ---------------------------------------------------------------------------
// VulkanBackend::vkCreateDeviceHooked
// ---------------------------------------------------------------------------
void vkCreateDeviceHooked(
    VkPhysicalDevice               physicalDevice,
    const VkDeviceCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*   pAllocator,
    VkDevice*                      pDevice,
    VkResult                       result)
{
// ── basic diagnostics ───────────────────────────────────────────────────
ERR("vkCreateDevice flags=%x -> %p, %d",
            pCreateInfo->flags, *pDevice, result);

for (uint32_t i = 0; i < pCreateInfo->queueCreateInfoCount; ++i) {
    const auto& q = pCreateInfo->pQueueCreateInfos[i];
    ERR("  Queue[%u] flags=%x fam=%u cnt=%u",
                i, q.flags, q.queueFamilyIndex, q.queueCount);
}
for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount; ++i)
    ERR("  Layer  : %s", pCreateInfo->ppEnabledLayerNames[i]);
for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i)
    ERR("  Ext    : %s", pCreateInfo->ppEnabledExtensionNames[i]);

// bail out if creation failed
if (result != VK_SUCCESS)
    return;

// ── remember device / queue family / graphics queue ────────────────────
{
    std::lock_guard _(globalResourceLock_);

    physicalDevice_ = physicalDevice;
    device_         = *pDevice;

    uint32_t numFamilies = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &numFamilies, nullptr);

    Array<VkQueueFamilyProperties> families;
    families.resize(numFamilies);
    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice_, &numFamilies, families.raw_buf());

    for (uint32_t fam = 0; fam < numFamilies; ++fam)
    {
        if (families[fam].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            queueFamily_ = fam;
            vkGetDeviceQueue(device_, fam, 0, &renderQueue_);
            ERR("Detected graphics queue family %u, queue=%p", fam, renderQueue_);
            break;
        }
    }
}

// ── resolve original Vulkan entry points we want to wrap ───────────────
auto  vkCreatePipelineCache = reinterpret_cast<PFN_vkCreatePipelineCache>(
    vkGetDeviceProcAddr(*pDevice, "vkCreatePipelineCache"));

auto  vkCreateSwapchainKHR  = reinterpret_cast<PFN_vkCreateSwapchainKHR>(
    vkGetDeviceProcAddr(*pDevice, "vkCreateSwapchainKHR"));

auto  vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(
    vkGetDeviceProcAddr(*pDevice, "vkDestroySwapchainKHR"));

auto  gamePresent           = reinterpret_cast<PFN_vkQueuePresentKHR>(
    vkGetDeviceProcAddr(*pDevice, "vkQueuePresentKHR"));

    // Decide which functions we should hook based on DLSS-FG availability
    PFN_vkQueuePresentKHR nextPresent =
        dlssgPresentFunction_ ? dlssgPresentFunction_ : gamePresent;
    
    // For vkCreateSwapchainKHR, use DLSS-FG's version if available
    PFN_vkCreateSwapchainKHR nextCreateSwapchain =
        dlssgCreateSwapchainKHR_ ? dlssgCreateSwapchainKHR_ : vkCreateSwapchainKHR;

    // ── install detours exactly once ────────────────────────────────────────
    if (!CreatePipelineCacheHook_.IsWrapped())
    {
        ERR("Hooking Vulkan device-level functions");
        if (dlssgCreateSwapchainKHR_) {
            ERR("DLSS-FG detected - hooking DLSS-FG's vkCreateSwapchainKHR");
        }

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        CreatePipelineCacheHook_.Wrap(ResolveFunctionTrampoline(vkCreatePipelineCache));
        CreateSwapchainKHRHook_.Wrap (ResolveFunctionTrampoline(nextCreateSwapchain));
        DestroySwapchainKHRHook_.Wrap(ResolveFunctionTrampoline(vkDestroySwapchainKHR));
        QueuePresentKHRHook_.Wrap    (ResolveFunctionTrampoline(nextPresent));

        // our PreHook was already registered in EnableHooks()
        DetourTransactionCommit();
    }
}


    void vkDestroyDeviceHooked(VkDevice device, const VkAllocationCallbacks* pAllocator)
    {
        if (device != device_) return;

        std::lock_guard _(globalResourceLock_);
        ERR("VK device destroyed");

        DestroyUI();
        releaseSwapChain(swapchain_);

        physicalDevice_ = VK_NULL_HANDLE;
        device_ = VK_NULL_HANDLE;
        queueFamily_ = 0;
        renderQueue_ = VK_NULL_HANDLE;

        if (descriptorPool_ != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);
            descriptorPool_ = VK_NULL_HANDLE;
            textureDescriptors_.clear();
        }

        if (sampler_ != VK_NULL_HANDLE) {
            vkDestroySampler(device_, sampler_, nullptr);
            sampler_ = VK_NULL_HANDLE;
        }
    }

    void vkCreatePipelineCacheHooked(
        VkDevice device,
        const VkPipelineCacheCreateInfo* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkPipelineCache* pPipelineCache,
        VkResult result)
    {
        if (result != VK_SUCCESS) return;

        ERR("VK pipeline cache created: %p", *pPipelineCache);
        pipelineCache_ = *pPipelineCache;
    }

    void vkCreateSwapchainKHRHooked(
        VkDevice device,
        const VkSwapchainCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSwapchainKHR* pSwapchain,
        VkResult result)
    {
        if (result != VK_SUCCESS) return;
        std::lock_guard _(globalResourceLock_);
        ERR("VK swap chain created: %p (old swapchain was %p)", *pSwapchain, swapChain_);
        
        // Update our swapchain reference - this could be either the game's swapchain
        // or DLSS-FG's swapchain, we need to track the latest one
        swapChain_ = *pSwapchain;
        
        if (isDlssgActive()) {
            ERR("DLSS-FG is active - this might be DLSS-FG's internal swapchain");
        }
        collectSwapChainInfo(pCreateInfo);
    }

    void vkDestroySwapchainKHRHooked(
        VkDevice device,
        VkSwapchainKHR swapchain,
        const VkAllocationCallbacks* pAllocator)
    {
        if (device != device_ || swapchain != swapChain_) return;

        std::lock_guard _(globalResourceLock_);
        ERR("VK swap chain destroyed");
        DestroyUI();
        releaseSwapChain(swapchain_);

        swapChain_ = VK_NULL_HANDLE;
        pipelineCache_ = VK_NULL_HANDLE;
    }

    void releaseSwapChain(SwapchainInfo& swapchain)
    {
        for (auto& image : swapchain.images_) {
            vkFreeCommandBuffers(device_, swapchain.commandPool_, 1, &image.commandBuffer);
            vkDestroySemaphore(device_, image.uiDoneSemaphore, nullptr);
            vkDestroyFence(device_, image.fence, nullptr);
            vkDestroyFramebuffer(device_, image.framebuffer, nullptr);
            vkDestroyImageView(device_, image.view, nullptr);
        }

        swapchain.images_.clear();

        if (swapchain.renderPass_ != VK_NULL_HANDLE) {
            vkDestroyRenderPass(device_, swapchain.renderPass_, nullptr);
            swapchain.renderPass_ = VK_NULL_HANDLE;
        }

        if (swapchain.commandPool_ != VK_NULL_HANDLE) {
            vkDestroyCommandPool(device_, swapchain.commandPool_, nullptr);
            swapchain.commandPool_ = VK_NULL_HANDLE;
        }
    }

    void collectSwapChainInfo(const VkSwapchainCreateInfoKHR* pCreateInfo)
    {
        SwapchainInfo& swapInfo = swapchain_;

        swapInfo.width_ = pCreateInfo->imageExtent.width;
        swapInfo.height_ = pCreateInfo->imageExtent.height;
        
        ERR("=== SWAPCHAIN ANALYSIS ===");
        ERR("Swap chain size: %d x %d", swapInfo.width_, swapInfo.height_);
        ERR("Format: %d, ColorSpace: %d", pCreateInfo->imageFormat, pCreateInfo->imageColorSpace);
        ERR("Usage: 0x%x, Transform: %d", pCreateInfo->imageUsage, pCreateInfo->preTransform);
        ERR("Present mode: %d, Clipped: %d", pCreateInfo->presentMode, pCreateInfo->clipped);
        ERR("Min images: %d, Array layers: %d", pCreateInfo->minImageCount, pCreateInfo->imageArrayLayers);
        ERR("Old swapchain: %p", pCreateInfo->oldSwapchain);
        ERR("DLSS-G active: %s", isDlssgActive() ? "YES" : "NO");

        {
            VkAttachmentDescription attDesc = {
                0,
                pCreateInfo->imageFormat,
                VK_SAMPLE_COUNT_1_BIT,
                VK_ATTACHMENT_LOAD_OP_LOAD,
                VK_ATTACHMENT_STORE_OP_STORE,
                VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                VK_ATTACHMENT_STORE_OP_DONT_CARE,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            };

            VkAttachmentReference attRef = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

            VkSubpassDescription sub = {
                0,    VK_PIPELINE_BIND_POINT_GRAPHICS,
                0,    NULL,       // inputs
                1,    &attRef,    // color
                NULL,             // resolve
                NULL,             // depth-stencil
                0,    NULL,       // preserve
            };

            VkRenderPassCreateInfo rpinfo = {
                VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                NULL,
                0,
                1,
                &attDesc,
                1,
                &sub,
                0,
                NULL,    // dependencies
            };

            VK_CHECK(vkCreateRenderPass(device_, &rpinfo, NULL, &swapInfo.renderPass_));
        }

        {
            VkCommandPoolCreateInfo createInfo = {
                VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                NULL,
                VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                queueFamily_
            };
            VK_CHECK(vkCreateCommandPool(device_, &createInfo, nullptr, &swapchain_.commandPool_));
        }

        // serialise out the swap chain images
        {
            uint32_t numSwapImages{ 0 };
            VK_CHECK(vkGetSwapchainImagesKHR(device_, swapChain_, &numSwapImages, NULL));

            swapInfo.images_.resize(numSwapImages);

            Array<VkImage> images;
            images.resize(numSwapImages);

            // go through our own function so we assign these images IDs
            VK_CHECK(vkGetSwapchainImagesKHR(device_, swapChain_, &numSwapImages, images.raw_buf()));

            for (uint32_t i = 0; i < numSwapImages; i++)
            {
                auto& imInfo = swapInfo.images_[i];

                // memory doesn't exist for genuine WSI created images
                imInfo.image = images[i];

                VkImageSubresourceRange range;
                range.baseMipLevel = range.baseArrayLayer = 0;
                range.levelCount = 1;
                range.layerCount = pCreateInfo->imageArrayLayers;
                range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

                {
                    VkCommandBufferAllocateInfo allocInfo = {
                        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                        NULL,
                        swapchain_.commandPool_,
                        VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        1
                    };
                    VK_CHECK(vkAllocateCommandBuffers(device_, &allocInfo, &imInfo.commandBuffer));
                }

                {
                    VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, NULL,
                                                    VK_FENCE_CREATE_SIGNALED_BIT };

                    VK_CHECK(vkCreateFence(device_, &fenceInfo, NULL, &imInfo.fence));
                }

                {
                    VkSemaphoreCreateInfo semInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

                    VK_CHECK(vkCreateSemaphore(device_, &semInfo, NULL, &imInfo.uiDoneSemaphore));
                }

                {
                    VkImageViewCreateInfo info = {
                        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                        NULL,
                        0,
                        images[i],
                        VK_IMAGE_VIEW_TYPE_2D,
                        pCreateInfo->imageFormat,
                        {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
                        {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
                    };

                    VK_CHECK(vkCreateImageView(device_, &info, NULL, &imInfo.view));

                    VkFramebufferCreateInfo fbinfo = {
                        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                        NULL,
                        0,
                        swapchain_.renderPass_,
                        1,
                        &imInfo.view,
                        (uint32_t)pCreateInfo->imageExtent.width,
                        (uint32_t)pCreateInfo->imageExtent.height,
                        1,
                    };

                    VK_CHECK(vkCreateFramebuffer(device_, &fbinfo, NULL, &imInfo.framebuffer));
                }
            }
        }
    }

    bool createSLResource(SwapchainImageInfo& image, sl::Resource& outResource)
{
    try {
        outResource.type = sl::ResourceType::eTex2d;
        outResource.native = image.image;
        outResource.view = image.view;  // Try this field name first
        outResource.memory = nullptr;
        outResource.state = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        // Try to set dimensions if they exist
        outResource.width = swapchain_.width_;
        outResource.height = swapchain_.height_;
        outResource.mipLevels = 1;
        outResource.arrayLayers = 1;
        
        return true;
    } catch (...) {
        ERR("Failed to create sl::Resource - trying fallback field names");
        
        // Fallback: try different field names
        try {
            outResource.type = sl::ResourceType::eTex2d;
            outResource.native = image.image;
            // Skip view field if it doesn't exist
            outResource.memory = nullptr;
            outResource.state = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            return true;
        } catch (...) {
            ERR("Failed to create sl::Resource - struct layout incompatible");
            return false;
        }
    }
}

// Helper function to create sl::Extent with error checking
bool createSLExtent(sl::Extent& outExtent)
{
    try {
        outExtent.width = swapchain_.width_;
        outExtent.height = swapchain_.height_;
        // Remove depth field - it doesn't exist in your version
        return true;
    } catch (...) {
        ERR("Failed to create sl::Extent - struct layout incompatible");
        return false;
    }
}

// APPROACH 1: Using slSetTag (simpler, older API)
void tagUIWithSlSetTag(SwapchainImageInfo& image)
{
    if (!slSetTag_ || !isDlssgActive()) return;
    
    ERR("[UI TAG] Attempting slSetTag approach");
    
    sl::Resource uiResource;
    if (!createSLResource(image, uiResource)) {
        ERR("[UI TAG] Failed to create sl::Resource for slSetTag");
        return;
    }
    
    sl::Extent uiExtent;
    if (!createSLExtent(uiExtent)) {
        ERR("[UI TAG] Failed to create sl::Extent for slSetTag");
        return;
    }
    
    sl::ResourceTag uiTag;
    uiTag.resource = &uiResource;
    uiTag.type = sl::kBufferTypeHUDLessColor;  // Changed to HUD-less for DLSS-G UI handling
    uiTag.lifecycle = sl::ResourceLifecycle::eOnlyValidNow;  // Copy now, as buffer will be modified by ImGui render
    uiTag.extent = uiExtent;
    sl::ViewportHandle vp = 0;  // DLSS-G requires viewport 0 (docs specify no multi-viewport support)
    sl::Result result = slSetTag_(vp, &uiTag, 1, reinterpret_cast<sl::CommandBuffer*>(image.commandBuffer));

    sl::ViewportHandle vp1 = dlssgViewport_;  // DLSS-G requires viewport 0 (docs specify no multi-viewport support)
    sl::Result result1 = slSetTag_(vp, &uiTag, 1, reinterpret_cast<sl::CommandBuffer*>(image.commandBuffer));

    ERR("[UI TAG] vp=%d slSetTag result: %s", vp, sl::getResultAsStr(result));
    ERR("[UI TAG] vp=%d slSetTag result: %s", vp, sl::getResultAsStr(result1));
}

void tagUIColorBuffer(SwapchainImageInfo& image)
{
    // Try approach 1: slSetTag
    if (slSetTag_) {
        tagUIWithSlSetTag(image);
        return;
    }
}
void presentPreHook(VkPresentInfoKHR* pPresentInfo)
{
    //ERR("presentPreHook: Called for viewport %d, swapchainCount=%d", drawViewport_, pPresentInfo->swapchainCount);

    if (pPresentInfo->swapchainCount > 0) {
        //ERR("presentPreHook: pSwapchains[0]=%p, our swapChain_=%p",
        //    pPresentInfo->pSwapchains[0], swapChain_);
    }

    auto& vp = viewports_[drawViewport_].Viewport;

    //ERR("presentPreHook: DrawData valid=%d, CmdListsCount=%d", vp.DrawDataP.Valid, vp.DrawDataP.CmdListsCount);

    if (!vp.DrawDataP.Valid || vp.DrawDataP.CmdListsCount == 0) {
        //ERR("presentPreHook: DrawData valid=%d, CmdListsCount=%d", vp.DrawDataP.Valid, vp.DrawDataP.CmdListsCount);
        return;
    }

    //ERR("presentPreHook: DLSS-FG active: %s", isDlssgActive() ? "YES" : "NO");

    if (!swapchain_.images_.size()) {
        //ERR("presentPreHook: No swapchain images available!");
        return;
    }

    uint32_t imageIdx = pPresentInfo->pImageIndices[0];
    if (imageIdx >= swapchain_.images_.size()) {
        //ERR("presentPreHook: Invalid image index %d (max %d)", imageIdx, (uint32_t)swapchain_.images_.size());
        return;
    }

    auto& image = swapchain_.images_[imageIdx];

    // wait for this command buffer to be free
    VK_CHECK(vkWaitForFences(device_, 1, &image.fence, VK_TRUE, UINT64_MAX));
    VK_CHECK(vkResetFences(device_, 1, &image.fence));
    VK_CHECK(vkResetCommandBuffer(image.commandBuffer, 0));

    VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VK_CHECK(vkBeginCommandBuffer(image.commandBuffer, &beginInfo));

    VkImageMemoryBarrier bbBarrier = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr,
        0,
        0,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        queueFamily_,
        queueFamily_,
        image.image,
        { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 }
    };
    bbBarrier.srcAccessMask = VK_ACCESS_ALL_READ_BITS;
    bbBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    vkCmdPipelineBarrier(image.commandBuffer,
                         VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &bbBarrier);

    //------------------------------------------------------------------
    // **FIXED SECTION** — get frame token correctly
    //------------------------------------------------------------------
    sl::FrameToken* frameToken = nullptr;
    if (isDlssgActive() && slGetNewFrameToken_ && slSetConstants_) {
        // PASS THE POINTER BY REFERENCE (no extra &)
        sl::Result tokenRes = slGetNewFrameToken_(frameToken, nullptr);
        ERR("[FG] slGetNewFrameToken: %s", sl::getResultAsStr(tokenRes));

        if (tokenRes == sl::Result::eOk && frameToken) {
            sl::Constants consts{};
            consts.reset = sl::Boolean::eFalse;
            sl::Result constsRes = slSetConstants_(consts, *frameToken, dlssgViewport_);
            ERR("[FG] slSetConstants: %s", sl::getResultAsStr(constsRes));
        }
    }
    //------------------------------------------------------------------

    VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
    VkPipelineStageFlags waitStages[3] = {
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
    };

    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.pWaitSemaphores   = pPresentInfo->pWaitSemaphores;
    submitInfo.waitSemaphoreCount = pPresentInfo->waitSemaphoreCount;
    submitInfo.pSignalSemaphores  = &image.uiDoneSemaphore;
    submitInfo.signalSemaphoreCount = 1;

    VkClearValue clearval{};
    VkRenderPassBeginInfo rpbegin = {
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        nullptr,
        swapchain_.renderPass_,
        image.framebuffer,
        { { 0, 0 }, { swapchain_.width_, swapchain_.height_ } },
        1,
        &clearval
    };
    vkCmdBeginRenderPass(image.commandBuffer, &rpbegin, VK_SUBPASS_CONTENTS_INLINE);
    ImGui_ImplVulkan_RenderDrawData(&vp.DrawDataP, image.commandBuffer);
    vkCmdEndRenderPass(image.commandBuffer);

    // Evaluate DLSS-FG after UI draw so HUD is included
    if (isDlssgActive() && slEvaluateFeature_ && frameToken) {
        const sl::BaseStructure* inputs = nullptr;
        uint32_t numInputs = 0;
        sl::Result evalRes = slEvaluateFeature_(sl::kFeatureDLSS_G,
                                                *frameToken,
                                                &inputs,
                                                numInputs,
                                                reinterpret_cast<sl::CommandBuffer*>(image.commandBuffer));
        ERR("[FG] slEvaluateFeature for DLSS-G: %s", sl::getResultAsStr(evalRes));
    }

    VkImageMemoryBarrier bbBarrier2 = bbBarrier;
    bbBarrier2.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    bbBarrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    bbBarrier2.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    bbBarrier2.dstAccessMask = VK_ACCESS_ALL_READ_BITS;

    vkCmdPipelineBarrier(image.commandBuffer,
                         VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         0, 0, nullptr, 0, nullptr, 1, &bbBarrier2);

    VK_CHECK(vkEndCommandBuffer(image.commandBuffer));

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &image.commandBuffer;
    VK_CHECK(vkQueueSubmit(renderQueue_, 1, &submitInfo, image.fence));

    // Append our semaphore to the present-wait list
    static thread_local VkSemaphore semBuffer[8];
    uint32_t orig = pPresentInfo->waitSemaphoreCount;
    std::memcpy(semBuffer, pPresentInfo->pWaitSemaphores, orig * sizeof(VkSemaphore));
    semBuffer[orig] = image.uiDoneSemaphore;
    pPresentInfo->pWaitSemaphores  = semBuffer;
    pPresentInfo->waitSemaphoreCount = orig + 1;
}

    static void DumpDLSSGState(const sl::DLSSGState& s)
    {
        ERR("┌─ DLSS-G State ───────────────────────────────────┐");
        ERR("│ status                     : 0x%08X",  s.status);
        ERR("│ bReserved4                       : %s",     (uint32_t)s.bReserved4);
        ERR("│ VRAM (MiB)                 : %.2f",     s.estimatedVRAMUsageInBytes / (1024.0*1024.0));
        ERR("│ minWidthOrHeight           : %u",       s.minWidthOrHeight);
        ERR("│ numFramesActuallyPresented : %u",       s.numFramesActuallyPresented);
        ERR("│ maxFramesToGenerate        : %u",       s.numFramesToGenerateMax);
        ERR("│ vsyncSupport               : %u",       (uint32_t)s.bIsVsyncSupportAvailable);
        if(s.inputsProcessingCompletionFence)
        {
            ERR("│ inputFence                 : %p",       s.inputsProcessingCompletionFence);
            ERR("│ lastPresentInputsProcessingFenceValue  : %llu", s.lastPresentInputsProcessingCompletionFenceValue);
        }
        ERR("└──────────────────────────────────────────────────┘");
    }

    void vkQueuePresentKHRHooked(
        VkQueue queue,
        const VkPresentInfoKHR* pPresentInfo)
    {
        //ERR("[PRESENT] vkQueuePresentKHR called - queue=%p, isDlssgActive=%s", 
        //    queue, isDlssgActive() ? "YES" : "NO");
        //ERR("[PRESENT] dlssgPresentFunction_=%p, this call from=%p", 
        //    dlssgPresentFunction_, _ReturnAddress());

        std::lock_guard _(globalResourceLock_);
        if (!initialized_) {
            //ERR("vkQueuePresentKHR: Render backend not initialized yet - drawViewport_=%d", drawViewport_);
            frameNo_ = 0;

            if (!uiFrameworkStarted_) {
                ui_.OnRenderBackendInitialized();
                uiFrameworkStarted_ = true;
                //ERR("vkQueuePresentKHR: uiFrameworkStarted_ = true");

                for (auto i = 0; i < viewports_.size(); i++) {
                    viewports_[i].Viewport = *GImGui->Viewports[0];
                }
            
                GImGui->Viewports[0] = &viewports_[0].Viewport;
            } else {
                ui_.OnRenderBackendInitialized();
                InitializeUI();
            }
        }

        if (initialized_ && drawViewport_ != -1) {
            if (pPresentInfo->swapchainCount > 0) {
                //ERR("vkQueuePresentKHR: Presenting swapchain %p (our tracked swapchain: %p)",
                //    pPresentInfo->pSwapchains[0], swapChain_);
            }
            //ERR("vkQueuePresentKHR: Calling presentPreHook - initialized %d, drawViewport %d",
            //    initialized_ ? 1 : 0, drawViewport_);
            //ERR("vkQueuePresentKHR: present queue = %p graphics queue = %p", queue, renderQueue_);
            presentPreHook(const_cast<VkPresentInfoKHR*>(pPresentInfo));
        } else {
            //ERR("vkQueuePresentKHR: Cannot append command buffer - initialized %d, drawViewport %d",
                //    initialized_ ? 1 : 0, drawViewport_);
        }

        frameNo_++;
    }

    IMGUIManager& ui_;
    VkInstance instance_{ VK_NULL_HANDLE };
    VkPhysicalDevice physicalDevice_{ VK_NULL_HANDLE };
    VkDevice device_{ VK_NULL_HANDLE };
    uint32_t queueFamily_{ 0 };
    VkQueue renderQueue_{ VK_NULL_HANDLE };
    VkSwapchainKHR swapChain_{ VK_NULL_HANDLE };
    VkPipelineCache pipelineCache_{ VK_NULL_HANDLE };
    VkDescriptorPool descriptorPool_{ VK_NULL_HANDLE };
    VkSampler sampler_{ VK_NULL_HANDLE };
    std::array<ViewportInfo, 3> viewports_;
    int32_t drawViewport_{ -1 };
    int32_t curViewport_{ 0 };
    int32_t frameNo_{ 0 };
    bool textureLimitWarningShown_{ false };

    bool initialized_{ false };
    bool uiFrameworkStarted_{ false };
    bool requestReloadFonts_{ false };
    std::mutex globalResourceLock_;

    HashMap<VkImageView, VkDescriptorSet> textureDescriptors_;

    VkCreateInstanceHookType CreateInstanceHook_;
    VkCreateDeviceHookType CreateDeviceHook_;
    VkDestroyDeviceHookType DestroyDeviceHook_;
    VkCreatePipelineCacheHookType CreatePipelineCacheHook_;
    VkCreateSwapchainKHRHookType CreateSwapchainKHRHook_;
    VkDestroySwapchainKHRHookType DestroySwapchainKHRHook_;
    VkQueuePresentKHRHookType QueuePresentKHRHook_;

    SwapchainInfo swapchain_;
    uint32_t textures_{ 0 };
    // Store DLSSG's present function separately
    PFN_vkQueuePresentKHR dlssgPresentFunction_{ nullptr };
    // Store DLSS-FG's vkCreateSwapchainKHR for swapchain recreation
    PFN_vkCreateSwapchainKHR dlssgCreateSwapchainKHR_{ nullptr };
    bool okayToModifyFG_{ false };
    HMODULE hSL_{ nullptr };
    HMODULE sl_{ nullptr };
    HMODULE slImGui_{ nullptr };
    bool                  fgPaused_          { true };   // our FG state
    bool                  menuVisible_       { true };   // set each frame1
    //auto slInit{nullptr};
    //auto slShutdown{nullptr};
    PFun_slGetFeatureFunction* slGetFeatureFunction_{ nullptr };
    PFun_slDLSSGSetOptions* slDLSSGSetOptions_{ nullptr };
    PFun_slDLSSGGetState* slDLSSGGetState_{ nullptr };
    PFun_slShutdown* slShutdown_{ nullptr };
    SlDLSSGSetOptionsHookType slDLSSGSetOptionsHook_;
    PFun_slDLSSGSetOptions* originalSlDLSSGSetOptions_{ nullptr };
    PFun_slSetTag* slSetTag_{ nullptr };
    PFun_slSetTagForFrame* slSetTagForFrame_{ nullptr };
    PFun_slGetNewFrameToken* slGetNewFrameToken_{ nullptr };
    PFun_slSetConstants* slSetConstants_{ nullptr };
    PFun_slEvaluateFeature* slEvaluateFeature_{ nullptr };
    sl::ViewportHandle dlssgViewport_{ 0 };



    
    // Store the original game's present function
    //PFN_vkQueuePresentKHR originalGamePresentFunction_{ nullptr };
    
    // Helper functions to route Vulkan calls through DLSS-FG when active
    inline bool isDlssgActive() const { return dlssgPresentFunction_ != nullptr; }
};

END_NS()