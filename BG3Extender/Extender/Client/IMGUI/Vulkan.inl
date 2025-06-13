#pragma once

#include <GameDefinitions/Base/Base.h>
#include <Extender/Client/IMGUI/Backends.h>
#include <Extender/ScriptExtender.h>
#include <CoreLib/Wrappers.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui_internal.h>
#include <cstring>


BEGIN_SE()

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

END_SE()


#define VK_ACCESS_ALL_READ_BITS                                                        \
  (VK_ACCESS_INDIRECT_COMMAND_READ_BIT | VK_ACCESS_INDEX_READ_BIT |                    \
   VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT |                  \
   VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT |                   \
   VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | \
   VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_HOST_READ_BIT | VK_ACCESS_MEMORY_READ_BIT)

#define VK_CHECK(expr) { auto _rval = (expr); if (_rval != VK_SUCCESS) { ERR(#expr " failed: %d", _rval); } }

BEGIN_NS(extui)

class VulkanBackend : public RenderingBackend
{
public:
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

        ERR("VulkanBackend::EnableHooks()");

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

        HMODULE hUpscaler = LoadLibraryW(L"upscaler.dll");
        if (hUpscaler)
            ERR("[EXTUI_HOOK_SETUP] upscaler.dll loaded");
        else
            ERR("[EXTUI_HOOK_SETUP] WARNING: upscaler.dll not found!");
        HMODULE sl = GetModuleHandleW(L"sl.interposer.dll");
        if (!sl) sl = LoadLibraryW(L"sl.interposer.dll");
        if (!sl) {
            ERR("[EXTUI_HOOK_SETUP] sl.interposer.dll not found — DLSS-FG hook skipped");
        }
        else {
            auto slQueuePresent = reinterpret_cast<PFN_vkQueuePresentKHR>(
                GetProcAddress(sl, "vkQueuePresentKHR")
            );
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
                    GetProcAddress(sl, "vkCreateSwapchainKHR"));
                    
                ERR("[EXTUI_HOOK_SETUP] Retrieved DLSS-FG Vulkan functions:");
                ERR("  vkCreateSwapchainKHR: %p", dlssgCreateSwapchainKHR_);
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
        DetourTransactionCommit();
    }

    void InitializeUI() override
    {
        if (initialized_) return;

        ERR("VulkanBackend::InitializeUI() - Entry - drawViewport_: %d", drawViewport_);

        VkDescriptorPoolSize poolSize
        {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 2048,
        };

        VkDescriptorPoolCreateInfo createInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets = 2048,
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
        }

        ImGui_ImplVulkan_NewFrame();
        ERR("VK: NewFrame");
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
        
        ERR("VK: FinishFrame - initializing with curViewport: %d, drawViewport_ before: %d", curViewport_, drawViewport_);

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
        ERR("VK: FinishFrame - set drawViewport_ to %d, have %d draw lists", 
            drawViewport_, vp.DrawDataP.CmdLists.Size);
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

        if (textures_ > 2000) {
            if (!textureLimitWarningShown_) {
                ERR("UI texture limit reached. Newly loaded textures may not load or render correctly");
                textureLimitWarningShown_ = true;
            }
            return {};
        }

        textures_++;
        auto desc = ImGui_ImplVulkan_AddTexture(sampler_, VkImageView(view), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        return TextureLoadResult{ reinterpret_cast<ImTextureID>(desc), descriptor->Vulkan.ImageData.Width, descriptor->Vulkan.ImageData.Height };
    }

    void UnregisterTexture(ImTextureID id) override
    {
        if (!initialized_) return;

        auto desc = reinterpret_cast<VkDescriptorSet>(id);
        ImGui_ImplVulkan_RemoveTexture(desc);
        textures_--;
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
        ERR("Swap chain size: %d x %d", swapInfo.width_, swapInfo.height_);

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

    void presentPreHook(VkPresentInfoKHR* pPresentInfo)
    {
        ERR("presentPreHook: Called for viewport %d", drawViewport_);
        auto& vp = viewports_[drawViewport_].Viewport;
        
        if (!vp.DrawDataP.Valid || vp.DrawDataP.CmdListsCount == 0) {
            ERR("presentPreHook: DrawData valid=%d, CmdListsCount=%d", vp.DrawDataP.Valid, vp.DrawDataP.CmdListsCount);
            return;
        }
        
        ERR("presentPreHook: DrawData valid, CmdListsCount=%d", vp.DrawDataP.CmdListsCount);
        
        ERR("presentPreHook: DLSS-FG active: %s", isDlssgActive() ? "YES" : "NO");
        
        if (!swapchain_.images_.size()) {
            ERR("presentPreHook: No swapchain images available!");
            return;
        }
        
        uint32_t imageIdx = pPresentInfo->pImageIndices[0];
        if (imageIdx >= swapchain_.images_.size()) {
            ERR("presentPreHook: Invalid image index %d (max %d)", imageIdx, (uint32_t)swapchain_.images_.size());
            return;
        }
        
        auto& image = swapchain_.images_[imageIdx];
        
        ERR("presentPreHook: Processing swapchain image #%d (%p)", imageIdx, image.image);
        ERR("presentPreHook: Our swapchain=%p, pSwapchains[0]=%p", swapChain_, pPresentInfo->pSwapchains[0]);
        ERR("presentPreHook: Image dimensions: %dx%d", swapchain_.width_, swapchain_.height_);
        
        // wait for this command buffer to be free
        // If this ring has never been used the fence is signalled on creation.
        // this should generally be a no-op because we only get here when we've acquired the image
        VK_CHECK(vkWaitForFences(device_, 1, &image.fence, VK_TRUE, UINT64_MAX));
        VK_CHECK(vkResetFences(device_, 1, &image.fence));
        VK_CHECK(vkResetCommandBuffer(image.commandBuffer, 0));

        VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, NULL,
                                              VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };

        VK_CHECK(vkBeginCommandBuffer(image.commandBuffer, &beginInfo));

        VkImageMemoryBarrier bbBarrier = {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            NULL,
            0,
            0,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            queueFamily_,
            queueFamily_,
            image.image,
            {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
        };

        bbBarrier.srcAccessMask = VK_ACCESS_ALL_READ_BITS;
        bbBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        vkCmdPipelineBarrier(image.commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0,
            NULL,
            0, NULL,
            1, &bbBarrier);

        uint32_t ringIdx = 0;

        VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };

        VkPipelineStageFlags waitStages[3] = { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };

        // wait on the present's semaphores
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.pWaitSemaphores = pPresentInfo->pWaitSemaphores;
        submitInfo.waitSemaphoreCount = pPresentInfo->waitSemaphoreCount;

        // and signal overlaydone
        submitInfo.pSignalSemaphores = &image.uiDoneSemaphore;
        submitInfo.signalSemaphoreCount = 1;

        {
            VkClearValue clearval = {};
            VkRenderPassBeginInfo rpbegin = {
                VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                NULL,
                swapchain_.renderPass_,
                image.framebuffer,
                {{
                     0,
                     0,
                 },
                 {swapchain_.width_, swapchain_.height_}},
                1,
                &clearval,
            };
            vkCmdBeginRenderPass(image.commandBuffer, &rpbegin, VK_SUBPASS_CONTENTS_INLINE);
        }

        ImGui_ImplVulkan_RenderDrawData(&vp.DrawDataP, image.commandBuffer);

        vkCmdEndRenderPass(image.commandBuffer);

        VkImageMemoryBarrier bbBarrier2 = {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            NULL,
            0,
            0,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            queueFamily_,
            queueFamily_,
            image.image,
            {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1},
        };

        bbBarrier2.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        bbBarrier2.dstAccessMask = VK_ACCESS_ALL_READ_BITS;

        vkCmdPipelineBarrier(image.commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0,
            NULL,
            0, NULL,
            1, &bbBarrier2);

        VK_CHECK(vkEndCommandBuffer(image.commandBuffer));

        {
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &image.commandBuffer;

            VK_CHECK(vkQueueSubmit(renderQueue_, 1, &submitInfo, image.fence));
        }

        // the next thing waits on our new semaphore - whether a subsequent overlay render or the
        // present
        //const_cast<VkSemaphore*>(pPresentInfo->pWaitSemaphores)[0] = submitInfo.pSignalSemaphores[0];
        //pPresentInfo->waitSemaphoreCount = 1;
        // append uiDone instead of overwriting the game's list (works even if list is empty)
        static thread_local VkSemaphore semBuffer[8];
        uint32_t orig = pPresentInfo->waitSemaphoreCount;
        std::memcpy(semBuffer, pPresentInfo->pWaitSemaphores, orig * sizeof(VkSemaphore));
        semBuffer[orig]           = image.uiDoneSemaphore;
        pPresentInfo->pWaitSemaphores  = semBuffer;
        pPresentInfo->waitSemaphoreCount = orig + 1;
    }

    void vkQueuePresentKHRHooked(
        VkQueue queue,
        const VkPresentInfoKHR* pPresentInfo)
    {
        if (pPresentInfo->swapchainCount > 0) {
            ERR("vkQueuePresentKHR: Presenting swapchain %p (our tracked swapchain: %p)",
                pPresentInfo->pSwapchains[0], swapChain_);
        }

        std::lock_guard _(globalResourceLock_);
        if (!initialized_) {
            ERR("vkQueuePresentKHR: Render backend not initialized yet - drawViewport_=%d", drawViewport_);
            frameNo_ = 0;

            if (!uiFrameworkStarted_) {
                ui_.OnRenderBackendInitialized();
                uiFrameworkStarted_ = true;
                ERR("vkQueuePresentKHR: uiFrameworkStarted_ = true");

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
            ERR("vkQueuePresentKHR: Calling presentPreHook - initialized %d, drawViewport %d",
                initialized_ ? 1 : 0, drawViewport_);
            ERR("vkQueuePresentKHR: present queue = %p graphics queue = %p", queue, renderQueue_);
            
            presentPreHook(const_cast<VkPresentInfoKHR*>(pPresentInfo));
        } else {
            ERR("vkQueuePresentKHR: Cannot append command buffer - initialized %d, drawViewport %d",
                initialized_ ? 1 : 0, drawViewport_);
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
    
    // Store the original game's present function
    //PFN_vkQueuePresentKHR originalGamePresentFunction_{ nullptr };
    
    // Helper functions to route Vulkan calls through DLSS-FG when active
    inline bool isDlssgActive() const { return dlssgPresentFunction_ != nullptr; }
};

END_NS()