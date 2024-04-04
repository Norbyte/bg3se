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
VK_HOOK(QueuePresentKHR)
VK_HOOK(CmdEndRenderPass)
VK_HOOK(CmdBeginRenderPass)

END_SE()


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
        QueuePresentKHRHook_.SetPostHook(&VulkanBackend::vkQueuePresentKHRHooked, this);
        CmdEndRenderPassHook_.SetPreHook(&VulkanBackend::vkCmdEndRenderPassHooked, this);
        CmdBeginRenderPassHook_.SetPostHook(&VulkanBackend::vkCmdBeginRenderPassHooked, this);
    }

    void DisableHooks() override
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreateInstanceHook_.Unwrap();
        CreateDeviceHook_.Unwrap();
        CreatePipelineCacheHook_.Unwrap();
        QueuePresentKHRHook_.Unwrap();
        CmdEndRenderPassHook_.Unwrap();
        CmdBeginRenderPassHook_.Unwrap();
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
        init_info.MinImageCount = 2;
        init_info.ImageCount = 2;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = nullptr;
        init_info.RenderPass = presentRenderPass_;
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
        curViewport_ = (curViewport_++) % 2;
        GImGui->Viewports[0] = &viewports_[curViewport_];
        ImGui_ImplVulkan_NewFrame();
    }

    void FinishFrame() override
    {
        drawViewport_ = curViewport_;
    }

    void RenderFrame() override
    {
        if (drawViewport_ != -1) {
            auto& vp = viewports_[drawViewport_];
            if (vp.DrawDataP.Valid) {
                ImGui_ImplVulkan_RenderDrawData(&vp.DrawDataP, lastRenderPassCommandBuffer_);
            }
        }
    }

private:
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
        auto queuePresentKHR = (PFN_vkQueuePresentKHR*)vkGetDeviceProcAddr(*pDevice, "vkQueuePresentKHR");
        auto cmdBeginRenderPass = (PFN_vkCmdBeginRenderPass*)vkGetDeviceProcAddr(*pDevice, "vkCmdBeginRenderPass");
        auto cmdEndRenderPass = (PFN_vkCmdEndRenderPass*)vkGetDeviceProcAddr(*pDevice, "vkCmdEndRenderPass");

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreatePipelineCacheHook_.Wrap(ResolveFunctionTrampoline(createPipelineCache));
        QueuePresentKHRHook_.Wrap(ResolveFunctionTrampoline(queuePresentKHR));
        CmdEndRenderPassHook_.Wrap(ResolveFunctionTrampoline(cmdEndRenderPass));
        CmdBeginRenderPassHook_.Wrap(ResolveFunctionTrampoline(cmdBeginRenderPass));
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

    void vkQueuePresentKHRHooked(
        VkQueue queue,
        const VkPresentInfoKHR* pPresentInfo,
        VkResult result)
    {
        presentRenderPass_ = lastSeenRenderPass_;

        if (!initialized_ && lastSeenRenderPass_ != nullptr) {
            ui_.OnRenderBackendInitialized();
            viewports_[0] = *GImGui->Viewports[0];
            viewports_[1] = *GImGui->Viewports[0];
            GImGui->Viewports[0] = &viewports_[0];
        }
    }

    void vkCmdBeginRenderPassHooked(
        VkCommandBuffer commandBuffer,
        const VkRenderPassBeginInfo* pRenderPassBegin,
        VkSubpassContents contents)
    {
        lastSeenRenderPass_ = pRenderPassBegin->renderPass;
        lastRenderPassInfo_ = *pRenderPassBegin;
    }

    void vkCmdEndRenderPassHooked(VkCommandBuffer commandBuffer)
    {
        if (initialized_ && lastSeenRenderPass_ == presentRenderPass_) {
            lastRenderPassCommandBuffer_ = commandBuffer;
            RenderFrame();
        }
    }

    IMGUIManager& ui_;
    VkInstance instance_{ nullptr };
    VkPhysicalDevice physicalDevice_{ nullptr };
    VkDevice device_{ nullptr };
    VkQueue renderQueue_{ nullptr };
    VkPipelineCache pipelineCache_{ nullptr };
    VkDescriptorPool descriptorPool_{ nullptr };
    VkRenderPass presentRenderPass_{ nullptr };
    VkRenderPass lastSeenRenderPass_{ nullptr };
    VkCommandBuffer lastRenderPassCommandBuffer_{ nullptr };
    VkRenderPassBeginInfo lastRenderPassInfo_;
    ImGuiViewportP viewports_[2];
    int32_t drawViewport_{ -1 };
    int32_t curViewport_{ 0 };

    bool initialized_{ false };

    VkCreateInstanceHookType CreateInstanceHook_;
    VkCreateDeviceHookType CreateDeviceHook_;
    VkCreatePipelineCacheHookType CreatePipelineCacheHook_;
    VkQueuePresentKHRHookType QueuePresentKHRHook_;
    VkCmdEndRenderPassHookType CmdEndRenderPassHook_;
    VkCmdBeginRenderPassHookType CmdBeginRenderPassHook_;
};

END_NS()
