#pragma once

#include <GameDefinitions/Base/Base.h>
#include <Extender/Client/IMGUI/Backends.h>
#include <Extender/ScriptExtender.h>
#include <CoreLib/Wrappers.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui_internal.h>

BEGIN_SE()

#define VK_HOOK(name) enum class Vk##name##HookTag {}; \
    using Vk##name##HookType = WrappableFunction<Vk##name##HookTag, decltype(vk##name)>; \
    Vk##name##HookType* Vk##name##HookType::gHook;

VK_HOOK(CreateInstance)
VK_HOOK(CreateDevice)
VK_HOOK(CreatePipelineCache)
VK_HOOK(CreateSwapchainKHR)
VK_HOOK(QueuePresentKHR)

END_SE()


#define VK_ACCESS_ALL_READ_BITS                                                        \
  (VK_ACCESS_INDIRECT_COMMAND_READ_BIT | VK_ACCESS_INDEX_READ_BIT |                    \
   VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT | VK_ACCESS_UNIFORM_READ_BIT |                  \
   VK_ACCESS_INPUT_ATTACHMENT_READ_BIT | VK_ACCESS_SHADER_READ_BIT |                   \
   VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | \
   VK_ACCESS_TRANSFER_READ_BIT | VK_ACCESS_HOST_READ_BIT | VK_ACCESS_MEMORY_READ_BIT)

BEGIN_NS(extui)

class VulkanBackend : public RenderingBackend
{
public:
    VulkanBackend(IMGUIManager& ui) : ui_(ui) {}

    ~VulkanBackend() override
    {
        DestroyUI();
        DisableHooks();
    }

    void EnableHooks() override
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreateInstanceHook_.Wrap(ResolveFunctionTrampoline(&vkCreateInstance));
        CreateDeviceHook_.Wrap(ResolveFunctionTrampoline(&vkCreateDevice));
        DetourTransactionCommit();

        CreateInstanceHook_.SetPostHook(&VulkanBackend::vkCreateInstanceHooked, this);
        CreateDeviceHook_.SetPostHook(&VulkanBackend::vkCreateDeviceHooked, this);
        CreatePipelineCacheHook_.SetPostHook(&VulkanBackend::vkCreatePipelineCacheHooked, this);
        CreateSwapchainKHRHook_.SetPostHook(&VulkanBackend::vkCreateSwapchainKHRHooked, this);
        QueuePresentKHRHook_.SetPreHook(&VulkanBackend::vkQueuePresentKHRHooked, this);
    }

    void DisableHooks() override
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreateInstanceHook_.Unwrap();
        CreateDeviceHook_.Unwrap();
        CreatePipelineCacheHook_.Unwrap();
        CreateSwapchainKHRHook_.Unwrap();
        QueuePresentKHRHook_.Unwrap();
        DetourTransactionCommit();
    }

    void InitializeUI() override
    {
        if (initialized_) return;

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

        VkDescriptorPoolSize poolSize
        {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 10
        };

        VkDescriptorPoolCreateInfo createInfo
        {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets = 1,
            .poolSizeCount = 1,
            .pPoolSizes = &poolSize
        };
        auto result = vkCreateDescriptorPool(device_, &createInfo, nullptr, &descriptorPool_);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance_;
        init_info.PhysicalDevice = physicalDevice_;
        init_info.Device = device_;
        init_info.QueueFamily = queueFamily;
        init_info.Queue = queue;
        init_info.PipelineCache = pipelineCache_;
        init_info.DescriptorPool = descriptorPool_;
        init_info.Subpass = 0;
        init_info.MinImageCount = swapchain_.images_.size();
        init_info.ImageCount = swapchain_.images_.size();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = nullptr;
        // init_info.RenderPass = presentRenderPass_;
        init_info.RenderPass = swapchain_.renderPass_;
        ImGui_ImplVulkan_Init(&init_info);
        // (this gets a bit more complicated, see example app for full reference)
        //ImGui_ImplVulkan_CreateFontsTexture(YOUR_COMMAND_BUFFER);
        // (your code submit a queue)
        //ImGui_ImplVulkan_DestroyFontUploadObjects();

        initialized_ = true;
    }

    void DestroyUI() override
    {
        if (!initialized_) return;

        ImGui_ImplVulkan_Shutdown();
        initialized_ = false;
    }

    void NewFrame() override
    {
        curViewport_ = (curViewport_ + 1) % viewports_.size();
        GImGui->Viewports[0] = &viewports_[curViewport_].Viewport;
        ImGui_ImplVulkan_NewFrame();
    }

    void FinishFrame() override
    {
        auto& vp = viewports_[curViewport_].Viewport;
        auto& drawLists = viewports_[curViewport_].ClonedDrawLists;

        for (auto list : drawLists) {
            delete list;
        }

        drawLists.clear();

        for (int i = 0; i < vp.DrawDataP.CmdLists.Size; i++) {
            auto list = vp.DrawDataP.CmdLists[i];
            auto drawList = list->CloneOutput();
            vp.DrawDataP.CmdLists[i] = drawList;
            drawLists.Add(drawList);
        }

        drawViewport_ = curViewport_;
    }

    void ClearFrame() override
    {
        drawViewport_ = -1;
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
        instance_ = *pInstance;
    }

    void vkCreateDeviceHooked(
        VkPhysicalDevice physicalDevice,
        const VkDeviceCreateInfo* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDevice* pDevice,
        VkResult result)
    {
        physicalDevice_ = physicalDevice;
        device_ = *pDevice;

        auto createPipelineCache = (PFN_vkCreatePipelineCache*)vkGetDeviceProcAddr(*pDevice, "vkCreatePipelineCache");
        auto createSwapchainKHR = (PFN_vkCreatePipelineCache*)vkGetDeviceProcAddr(*pDevice, "vkCreateSwapchainKHR");
        auto queuePresentKHR = (PFN_vkQueuePresentKHR*)vkGetDeviceProcAddr(*pDevice, "vkQueuePresentKHR");

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreatePipelineCacheHook_.Wrap(ResolveFunctionTrampoline(createPipelineCache));
        CreateSwapchainKHRHook_.Wrap(ResolveFunctionTrampoline(createSwapchainKHR));
        QueuePresentKHRHook_.Wrap(ResolveFunctionTrampoline(queuePresentKHR));
        DetourTransactionCommit();
    }

    void vkCreatePipelineCacheHooked(
        VkDevice device,
        const VkPipelineCacheCreateInfo* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkPipelineCache* pPipelineCache,
        VkResult result)
    {
        pipelineCache_ = *pPipelineCache;
    }

    void vkCreateSwapchainKHRHooked(
        VkDevice device,
        const VkSwapchainCreateInfoKHR* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkSwapchainKHR* pSwapchain,
        VkResult result)
    {
        swapChain_ = *pSwapchain;
        collectSwapChainInfo(pCreateInfo);
    }

    void collectSwapChainInfo(const VkSwapchainCreateInfoKHR* pCreateInfo)
    {
        SwapchainInfo& swapInfo = swapchain_;

        swapInfo.width_ = pCreateInfo->imageExtent.width;
        swapInfo.height_ = pCreateInfo->imageExtent.height;

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

            auto vkr = vkCreateRenderPass(device_, &rpinfo, NULL, &swapInfo.renderPass_);
            // CheckVkResult(vkr);
        }

        {
            VkCommandPoolCreateInfo createInfo = {
                VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                NULL,
                VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                queueFamily_
            };
            vkCreateCommandPool(device_, &createInfo, nullptr, &swapchain_.commandPool_);
        }

        // serialise out the swap chain images
        {
            uint32_t numSwapImages;
            auto vkr = vkGetSwapchainImagesKHR(device_, swapChain_, &numSwapImages, NULL);
            // CheckVkResult(vkr);

            swapInfo.images_.resize(numSwapImages);

            Array<VkImage> images;
            images.resize(numSwapImages);

            // go through our own function so we assign these images IDs
            vkr = vkGetSwapchainImagesKHR(device_, swapChain_, &numSwapImages, images.raw_buf());
            // CheckVkResult(vkr);

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
                        VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                        NULL,
                        swapchain_.commandPool_,
                        VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                        1
                    };
                    vkAllocateCommandBuffers(device_, &allocInfo, &imInfo.commandBuffer);
                }

                {
                    VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, NULL,
                                                    VK_FENCE_CREATE_SIGNALED_BIT };

                    vkr = vkCreateFence(device_, &fenceInfo, NULL, &imInfo.fence);
                    // CheckVkResult(vkr);
                }

                {
                    VkSemaphoreCreateInfo semInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

                    vkr = vkCreateSemaphore(device_, &semInfo, NULL,
                        &imInfo.uiDoneSemaphore);
                    //CheckVkResult(vkr);
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

                    vkr = vkCreateImageView(device_, &info, NULL, &imInfo.view);
                    //CheckVkResult(vkr);

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

                    vkr = vkCreateFramebuffer(device_, &fbinfo, NULL, &imInfo.framebuffer);
                    //CheckVkResult(vkr);
                }
            }
        }
    }

    void presentPreHook(VkPresentInfoKHR* pPresentInfo)
    {
        auto& vp = viewports_[drawViewport_].Viewport;
        if (!vp.DrawDataP.Valid) return;

        auto& image = swapchain_.images_[pPresentInfo->pImageIndices[0]];

        // wait for this command buffer to be free
        // If this ring has never been used the fence is signalled on creation.
        // this should generally be a no-op because we only get here when we've acquired the image
        auto vkr = vkWaitForFences(device_, 1, &image.fence, VK_TRUE, 50000000);
        //CheckVkResult(vkr);

        vkr = vkResetFences(device_, 1, &image.fence);

        vkResetCommandBuffer(image.commandBuffer, 0);

        VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, NULL,
                                              VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };

        vkr = vkBeginCommandBuffer(image.commandBuffer, &beginInfo);
        //CheckVkResult(vkr);

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

        std::swap(bbBarrier.srcQueueFamilyIndex, bbBarrier.dstQueueFamilyIndex);
        std::swap(bbBarrier.oldLayout, bbBarrier.newLayout);
        bbBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        bbBarrier.dstAccessMask = VK_ACCESS_ALL_READ_BITS;

        vkCmdPipelineBarrier(image.commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
            VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0,
            NULL,
            0, NULL,
            1, &bbBarrier);

        vkEndCommandBuffer(image.commandBuffer);

        {
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &image.commandBuffer;

            vkr = vkQueueSubmit(renderQueue_, 1, &submitInfo, image.fence);
            // CheckVkResult(vkr);
        }

        // the next thing waits on our new semaphore - whether a subsequent overlay render or the
        // present
        const_cast<VkSemaphore*>(pPresentInfo->pWaitSemaphores)[0] = submitInfo.pSignalSemaphores[0];
        pPresentInfo->waitSemaphoreCount = 1;
    }

    void vkQueuePresentKHRHooked(
        VkQueue queue,
        const VkPresentInfoKHR* pPresentInfo)
    {
        if (pPresentInfo->swapchainCount != 1
            || pPresentInfo->pSwapchains[0] != swapChain_) {
            return;
        }

        if (!initialized_) {
            ui_.OnRenderBackendInitialized();
            for (auto i = 0; i < viewports_.size(); i++) {
                viewports_[i].Viewport = *GImGui->Viewports[0];
            }
            
            GImGui->Viewports[0] = &viewports_[0].Viewport;
        }

        if (initialized_ && drawViewport_ != -1) {
            presentPreHook(const_cast<VkPresentInfoKHR*>(pPresentInfo));
        }
    }

    IMGUIManager& ui_;
    VkInstance instance_{ nullptr };
    VkPhysicalDevice physicalDevice_{ nullptr };
    VkDevice device_{ nullptr };
    uint32_t queueFamily_{ 0 };
    VkQueue renderQueue_{ nullptr };
    VkSwapchainKHR swapChain_{ nullptr };
    VkPipelineCache pipelineCache_{ nullptr };
    VkDescriptorPool descriptorPool_{ nullptr };
    std::array<ViewportInfo, 3> viewports_;
    int32_t drawViewport_{ -1 };
    int32_t curViewport_{ 0 };

    bool initialized_{ false };

    VkCreateInstanceHookType CreateInstanceHook_;
    VkCreateDeviceHookType CreateDeviceHook_;
    VkCreatePipelineCacheHookType CreatePipelineCacheHook_;
    VkCreateSwapchainKHRHookType CreateSwapchainKHRHook_;
    VkQueuePresentKHRHookType QueuePresentKHRHook_;

    SwapchainInfo swapchain_;
};

END_NS()
