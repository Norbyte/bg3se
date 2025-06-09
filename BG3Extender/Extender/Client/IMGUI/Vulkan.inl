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
        IMGUI_DEBUG("Destroying VK backend");
        releaseSwapChain(swapchain_);
        DestroyUI();
        DisableHooks();
    }

    void EnableHooks() override
    {
        if (CreateInstanceHook_.IsWrapped()) return;

        IMGUI_DEBUG("VulkanBackend::EnableHooks()");

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
            }
        }
    }

    void DisableHooks() override
    {
        if (!CreateInstanceHook_.IsWrapped()) return;

        IMGUI_DEBUG("VulkanBackend::DisableHooks()");

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

        IMGUI_DEBUG("VulkanBackend::InitializeUI()");

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

        IMGUI_DEBUG("VK initialization: desc pool %p, sampler %p", descriptorPool_, sampler_);

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

        initialized_ = true;
        
        // Ensure drawViewport_ is set to a valid value after initialization
        drawViewport_ = 0;

        IMGUI_DEBUG("VK POSTINIT - uiFrameworkStarted %d", uiFrameworkStarted_ ? 1 : 0);
        IMGUI_DEBUG("    Instance %p, PhysicalDevice %p, Device %p", instance_, physicalDevice_, device_);
        IMGUI_DEBUG("    QueueFamily %d, RenderQueue %p, CommandPool %p", queueFamily_, renderQueue_, swapchain_.commandPool_);
        IMGUI_DEBUG("    PipelineCache %p, DescriptorPool %p, Sampler %p", pipelineCache_, descriptorPool_, sampler_);
        IMGUI_DEBUG("VK SWAPCHAIN");
        IMGUI_DEBUG("    SwapChain %p, RenderPass %p, CommandPool %p", swapChain_, swapchain_.renderPass_, swapchain_.commandPool_);
        IMGUI_DEBUG("VK FRAMEBUFFERS");
        for (auto const& image : swapchain_.images_) {
            IMGUI_DEBUG("    Image %p, FB %p, View %p, Fence %p", image.image, image.framebuffer, image.view, image.fence);
        }
    }

    void DestroyUI() override
    {
        if (!initialized_) return;

        IMGUI_DEBUG("VK shutdown");

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

        // Always use viewport 0 - no cycling
        curViewport_ = 0;
        GImGui->Viewports[0] = &viewports_[0].Viewport;
        ERR("VK: NewFrame - Using viewport 0");

        if (requestReloadFonts_) {
            IMGUI_DEBUG("Rebuilding font atlas");
            ImGui_ImplVulkan_DestroyFontsTexture();
            requestReloadFonts_ = false;
        }

        ImGui_ImplVulkan_NewFrame();
        ERR("VK: NewFrame");
    }

    void FinishFrame() override
    {
        // FinishFrame is called by the IMGUI framework but we handle all rendering in presentPreHook
        // Just return early to avoid conflicts
        return;
    }

    void ClearFrame() override
    {
        if (!initialized_) return;

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
        IMGUI_DEBUG("vkCreateInstance -> %p, %d", *pInstance, result);
        if (pCreateInfo->pApplicationInfo != nullptr && pCreateInfo->pApplicationInfo->pApplicationName != nullptr) {
            IMGUI_DEBUG("App=%s", pCreateInfo->pApplicationInfo->pApplicationName);
        }

        for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount; i++) {
            IMGUI_DEBUG("Layer=%s", pCreateInfo->ppEnabledLayerNames[i]);
        }

        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            IMGUI_DEBUG("Extension=%s", pCreateInfo->ppEnabledExtensionNames[i]);
        }

        if (result != VK_SUCCESS) return;

        instance_ = *pInstance;

        if (!CreateDeviceHook_.IsWrapped()) {
            IMGUI_DEBUG("Hooking CreateDevice()");

            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            auto createDevice = vkGetInstanceProcAddr(instance_, "vkCreateDevice");
            auto destroyDevice = vkGetInstanceProcAddr(instance_, "vkDestroyDevice");
            CreateDeviceHook_.Wrap(ResolveFunctionTrampoline(createDevice));
            DestroyDeviceHook_.Wrap(ResolveFunctionTrampoline(destroyDevice));
            DetourTransactionCommit();
        }
    }

    void vkCreateDeviceHooked(
        VkPhysicalDevice physicalDevice,
        const VkDeviceCreateInfo* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDevice* pDevice,
        VkResult result)
    {
        IMGUI_DEBUG("vkCreateDevice flags=%x -> %p, %d", pCreateInfo->flags, *pDevice, result);

        for (uint32_t i = 0; i < pCreateInfo->queueCreateInfoCount; i++) {
            auto const& queue = pCreateInfo->pQueueCreateInfos[i];
            IMGUI_DEBUG("Queue=%x, %d, %d", queue.flags, queue.queueFamilyIndex, queue.queueCount);
        }

        for (uint32_t i = 0; i < pCreateInfo->enabledLayerCount; i++) {
            IMGUI_DEBUG("Layer=%s", pCreateInfo->ppEnabledLayerNames[i]);
        }

        for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; i++) {
            IMGUI_DEBUG("Extension=%s", pCreateInfo->ppEnabledExtensionNames[i]);
        }

        if (result != VK_SUCCESS) return;

        std::lock_guard _(globalResourceLock_);
        physicalDevice_ = physicalDevice;
        device_ = *pDevice;
        
        uint32_t numFamilies;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &numFamilies, nullptr);
        Array<VkQueueFamilyProperties> families;
        families.resize(numFamilies);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &numFamilies, families.raw_buf());

        uint32_t queueFamily{ 0 };
        VkQueue queue{ nullptr };
        for (auto const& family : families) {
            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                vkGetDeviceQueue(device_, queueFamily, 0, &queue);
                queueFamily_ = queueFamily;
                renderQueue_ = queue;
                break;
            }

            queueFamily++;
        }

        IMGUI_DEBUG("Detected graphics queue family %d, %p", queueFamily, queue);

        auto createPipelineCache = (PFN_vkCreatePipelineCache)vkGetDeviceProcAddr(*pDevice, "vkCreatePipelineCache");
        auto createSwapchainKHR = (PFN_vkCreateSwapchainKHR)vkGetDeviceProcAddr(*pDevice, "vkCreateSwapchainKHR");
        auto destroySwapchainKHR = (PFN_vkDestroySwapchainKHR)vkGetDeviceProcAddr(*pDevice, "vkDestroySwapchainKHR");
        auto queuePresentKHR = (PFN_vkQueuePresentKHR)vkGetDeviceProcAddr(*pDevice, "vkQueuePresentKHR");

        if (!CreatePipelineCacheHook_.IsWrapped()) {
            IMGUI_DEBUG("Hooking CreatePipelineCache()");

            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            CreatePipelineCacheHook_.Wrap(ResolveFunctionTrampoline(createPipelineCache));
            CreateSwapchainKHRHook_.Wrap(ResolveFunctionTrampoline(createSwapchainKHR));
            DestroySwapchainKHRHook_.Wrap(ResolveFunctionTrampoline(destroySwapchainKHR));
            originalGamePresentFunction_ = queuePresentKHR;
            QueuePresentKHRHook_.Wrap(ResolveFunctionTrampoline(queuePresentKHR));
            DetourTransactionCommit();
            
            IMGUI_DEBUG("Hooked game's vkQueuePresentKHR at %p", queuePresentKHR);
        }
    }

    void vkDestroyDeviceHooked(VkDevice device, const VkAllocationCallbacks* pAllocator)
    {
        if (device != device_) return;

        std::lock_guard _(globalResourceLock_);
        IMGUI_DEBUG("VK device destroyed");

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

        IMGUI_DEBUG("VK pipeline cache created: %p", *pPipelineCache);
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
        IMGUI_DEBUG("VK swap chain created: %p", *pSwapchain);
        swapChain_ = *pSwapchain;
        collectSwapChainInfo(pCreateInfo);
    }

    void vkDestroySwapchainKHRHooked(
        VkDevice device,
        VkSwapchainKHR swapchain,
        const VkAllocationCallbacks* pAllocator)
    {
        if (device != device_ || swapchain != swapChain_) return;

        std::lock_guard _(globalResourceLock_);
        IMGUI_DEBUG("VK swap chain destroyed");
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
        IMGUI_DEBUG("Swap chain size: %d x %d", swapInfo.width_, swapInfo.height_);

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
        // Extended start-of-function diagnostics
        ERR("presentPreHook start: frameNo=%llu drawViewport=%d pImageIndices=%p", 
            (unsigned long long)frameNo_, drawViewport_, pPresentInfo->pImageIndices);

        // Some engines leave pImageIndices null and rely on implicit acquisition; bail out if that happens
        if (pPresentInfo->pImageIndices == nullptr) {
            ERR("presentPreHook early exit: pImageIndices is NULL (implicit acquire path)");
            return;
        }

        // CRITICAL FIX: Always use viewport 0 for consistent IMGUI rendering
        // The viewport cycling (0,1,2) was causing IMGUI to only appear 1/3 of the time
        const int32_t renderViewport = 0;
        
        // Log the fix in action
        if (renderViewport != drawViewport_) {
            ERR("presentPreHook: VIEWPORT FIX - Using viewport %d instead of drawViewport_ %d", 
                renderViewport, drawViewport_);
        }
        
        auto& vp = viewports_[renderViewport].Viewport;
        if (!vp.DrawDataP.Valid) {
            ERR("presentPreHook: Viewport %d has no valid draw data", renderViewport);
            return;
        }
    
        // Get the current swapchain image
        uint32_t imageIndex = pPresentInfo->pImageIndices[0];
        auto& image = swapchain_.images_[imageIndex];
        ERR("vkQueuePresentKHR: Swap chain image #%d (%p), using viewport %d", 
            imageIndex, image.image, renderViewport);
    
        // Track frame timing to detect DLSS frame generation patterns
        static std::chrono::high_resolution_clock::time_point lastFrameTime;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto frameDelta = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastFrameTime).count();
        
        // Track semaphore patterns more comprehensively
        static VkSemaphore lastSemaphore = VK_NULL_HANDLE;
        static uint32_t sameSemasphoreCount = 0;
        static std::unordered_map<VkSemaphore, uint32_t> semaphoreFrequency;
        
        bool semaphoreChanged = false;
        VkSemaphore currentSemaphore = VK_NULL_HANDLE;
        
        // Update frame time
        lastFrameTime = currentTime;

        // For each swapchain image, ensure we have valid rendering state
        // This is important because DLSS might present images out of order
        static std::array<bool, 16> imageRenderedFlags = {}; // Track if image has current frame UI
        static uint64_t lastFrameNumber = 0;
        
        // If frame number changed, reset all flags
        if (frameNo_ != lastFrameNumber) {
            std::fill(imageRenderedFlags.begin(), imageRenderedFlags.end(), false);
            lastFrameNumber = frameNo_;
        }
        
        // Check if this image already has UI rendered for this frame
        if (imageIndex < imageRenderedFlags.size() && imageRenderedFlags[imageIndex]) {
            ERR("Image %u already has UI for frame %llu, skipping render", 
                imageIndex, (unsigned long long)frameNo_);
            return;
        }

        // Diagnostic: check fence status before waiting
        VkResult fenceStatusBefore = vkGetFenceStatus(device_, image.fence);
        ERR("presentPreHook: fence status before wait = %d", fenceStatusBefore);

        // Wait for the fence from the previous frame to ensure command buffer is available
        VkResult waitRes = vkWaitForFences(device_, 1, &image.fence, VK_TRUE, 50000000);
        if (waitRes != VK_SUCCESS) {
            ERR("vkWaitForFences returned %d (timeout / error)", waitRes);
            return; // Don't proceed if fence wait failed
        } else {
            ERR("presentPreHook: fence wait succeeded");
        }

        VkResult resetRes = vkResetFences(device_, 1, &image.fence);
        if (resetRes != VK_SUCCESS) {
            ERR("vkResetFences failed: %d", resetRes);
            return; // Don't proceed if fence reset failed
        }

        // Begin recording command buffer
        VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        VK_CHECK(vkBeginCommandBuffer(image.commandBuffer, &beginInfo));
    
        // Transition image layout to color attachment
        VkImageMemoryBarrier barrierToRender = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .srcAccessMask = VK_ACCESS_MEMORY_READ_BIT, // Ensure any previous memory reads (e.g. by the presentation engine) are complete
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .srcQueueFamilyIndex = queueFamily_,
            .dstQueueFamilyIndex = queueFamily_,
            .image = image.image,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        vkCmdPipelineBarrier(
            image.commandBuffer,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // Per spec, transitioning from PRESENT_SRC should wait for BOTTOM_OF_PIPE
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrierToRender
        );
    
        // Begin render pass
        VkClearValue clearValue = {};
        VkRenderPassBeginInfo renderPassInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = swapchain_.renderPass_,
            .framebuffer = image.framebuffer,
            .renderArea = {
                .offset = {0, 0},
                .extent = {swapchain_.width_, swapchain_.height_}
            },
            .clearValueCount = 1,
            .pClearValues = &clearValue
        };
        vkCmdBeginRenderPass(image.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    
        // Render ImGui draw data
        ImGui_ImplVulkan_RenderDrawData(&vp.DrawDataP, image.commandBuffer);
    
        // End render pass
        vkCmdEndRenderPass(image.commandBuffer);
    
        // Transition layout back to present
        VkImageMemoryBarrier barrierToPresent = barrierToRender;
        std::swap(barrierToPresent.oldLayout, barrierToPresent.newLayout);
        barrierToPresent.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrierToPresent.dstAccessMask = 0; // none required for presentation
        vkCmdPipelineBarrier(
            image.commandBuffer,
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrierToPresent
        );
    
        // End command buffer
        VK_CHECK(vkEndCommandBuffer(image.commandBuffer));
    
        // Submit command buffer WITHOUT waiting on original semaphores
        // The present operation will handle all semaphore waiting
        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount = 0,  // Don't wait on any semaphores
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &image.commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &image.uiDoneSemaphore
        };
    
        VkResult submitResult = vkQueueSubmit(renderQueue_, 1, &submitInfo, image.fence);
        if (submitResult != VK_SUCCESS) {
            ERR("vkQueueSubmit failed: %d", submitResult);
            return;
        }
        
        ERR("presentPreHook: submitted cmd; fence=%p, uiDoneSemaphore=%p", image.fence, image.uiDoneSemaphore);
    
        // Mark this image as rendered for this frame
        if (imageIndex < imageRenderedFlags.size()) {
            imageRenderedFlags[imageIndex] = true;
        }

        // Preserve original semaphores and add our UI done semaphore for DLSS Frame Generation compatibility.
        // This ensures the present waits for both original operations and IMGUI rendering to complete.
        static std::vector<VkSemaphore> combinedSemaphores; // Static to reuse allocated memory and keep pointer valid.
        combinedSemaphores.clear(); // Clear for current frame's semaphores.

        // Add all original wait semaphores from pPresentInfo.
        if (pPresentInfo->waitSemaphoreCount > 0 && pPresentInfo->pWaitSemaphores != nullptr) {
            for (uint32_t i = 0; i < pPresentInfo->waitSemaphoreCount; ++i) {
                combinedSemaphores.push_back(pPresentInfo->pWaitSemaphores[i]);
            }
        }

        // Add our UI-specific semaphore, signaled after UI rendering.
        combinedSemaphores.push_back(image.uiDoneSemaphore);

        // Update VkPresentInfoKHR with the combined list.
        // const_cast is used here because pPresentInfo is const but we need to modify its members for the hook.
        // This is a common pattern in Vulkan hooking scenarios where the hooked function receives a const struct
        // but needs to alter it before passing it to the original function.
        // The `combinedSemaphores` vector is static, so its data pointer remains valid for the duration of the
        // vkQueuePresentKHR call.
        const_cast<VkPresentInfoKHR*>(pPresentInfo)->waitSemaphoreCount = static_cast<uint32_t>(combinedSemaphores.size());
        const_cast<VkPresentInfoKHR*>(pPresentInfo)->pWaitSemaphores = combinedSemaphores.data();
        
        ERR("presentPreHook: Chained UI semaphore. Total wait semaphores: %u. UI done: %p. First original (if any): %p",
            static_cast<uint32_t>(combinedSemaphores.size()), 
            image.uiDoneSemaphore,
            (pPresentInfo->waitSemaphoreCount > 0 && combinedSemaphores.size() > 1 ? combinedSemaphores[0] : nullptr)
        );
    }
    

    void vkQueuePresentKHRHooked(
        VkQueue queue,
        const VkPresentInfoKHR* pPresentInfo)
    {
        // This is a pre-hook - it runs before the original function
        // We can only add our rendering, not control what happens after
        
        // Add detailed logging for debugging
        ERR("[IMGUI] vkQueuePresentKHRHooked called - swapchainCount: %d, swapChain_: %p", 
            pPresentInfo->swapchainCount, swapChain_);

        // Record image index pointer information as some engines may pass NULL
        ERR("[IMGUI] vkQueuePresentKHRHooked: pImageIndices=%p", pPresentInfo->pImageIndices);
        if (pPresentInfo->pImageIndices != nullptr && pPresentInfo->swapchainCount > 0) {
            ERR("[IMGUI] vkQueuePresentKHRHooked: first imageIndex=%u", pPresentInfo->pImageIndices[0]);
        }

        // If we don't have a swapchain yet, we need to capture it from the present info
        if (swapChain_ == VK_NULL_HANDLE && pPresentInfo->swapchainCount > 0) {
            ERR("[IMGUI] vkQueuePresentKHRHooked: Attempting to acquire globalResourceLock_ for swapchain capture...");
            std::lock_guard _(globalResourceLock_);
            ERR("[IMGUI] vkQueuePresentKHRHooked: Successfully acquired globalResourceLock_ for swapchain capture.");
            swapChain_ = pPresentInfo->pSwapchains[0];
            ERR("[IMGUI] Captured swapchain from present info: %p", swapChain_);
            return; // Don't render yet
        }
        
        if (pPresentInfo->swapchainCount != 1
            || pPresentInfo->pSwapchains[0] != swapChain_) {
            ERR("vkQueuePresentKHR: Bad swapchain (%d: %p vs. %p)",
                pPresentInfo->swapchainCount, 
                pPresentInfo->swapchainCount ? pPresentInfo->pSwapchains[0] : nullptr,
                swapChain_);
            frameNo_++;
            return;
        }

        std::lock_guard _(globalResourceLock_);
        if (!initialized_) {
            ERR("[IMGUI] vkQueuePresentKHR: Render backend not initialized yet");
            frameNo_ = 0;

            if (!uiFrameworkStarted_) {
                ui_.OnRenderBackendInitialized();
                uiFrameworkStarted_ = true;
                IMGUI_DEBUG("vkQueuePresentKHR: uiFrameworkStarted_ = true");

                for (auto i = 0; i < viewports_.size(); i++) {
                    viewports_[i].Viewport = *GImGui->Viewports[0];
                }
            
                GImGui->Viewports[0] = &viewports_[0].Viewport;
            } else {
                ui_.OnRenderBackendInitialized();
                InitializeUI();
            }
            
            // Don't try to render on the initialization frame
            ERR("[IMGUI] Initialization complete, skipping render for this frame");
            frameNo_++;
            return;
        }

        // Now we're initialized and ready to render
        // Always ensure we have a valid viewport
        if (drawViewport_ < 0 || drawViewport_ >= viewports_.size()) {
            drawViewport_ = 0;
            ERR("[IMGUI] vkQueuePresentKHRHooked: Reset drawViewport_ to 0");
        }
        
        // Call our pre-hook to do the IMGUI rendering
        presentPreHook(const_cast<VkPresentInfoKHR*>(pPresentInfo));
        
        ERR("[IMGUI] MCM rendering complete for frame %d", frameNo_);
        frameNo_++;
        
        // The original present function (either DLSSG's or the game's) will be called automatically after this
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
    int32_t drawViewport_{ 0 };  // Changed from -1 to 0 to always have a valid viewport
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
    
    // Store the original game's present function
    PFN_vkQueuePresentKHR originalGamePresentFunction_{ nullptr };
};

END_NS()
