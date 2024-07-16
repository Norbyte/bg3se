#pragma once

#include <GameDefinitions/Base/Base.h>
#include <Extender/Client/IMGUI/Backends.h>
#include <Extender/ScriptExtender.h>
#include <CoreLib/Wrappers.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <imgui_internal.h>
#include <dxgi1_2.h>
#include <d3d11.h>

BEGIN_SE()

using D3D11CreateDeviceProc = HRESULT WINAPI (_In_opt_ IDXGIAdapter*,
    D3D_DRIVER_TYPE, HMODULE, UINT,
    _In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL*,
    UINT FeatureLevels, UINT, _COM_Outptr_opt_ ID3D11Device**,
    _Out_opt_ D3D_FEATURE_LEVEL*, _COM_Outptr_opt_ ID3D11DeviceContext**);

using CreateDXGIFactory1Proc = HRESULT WINAPI (REFIID riid, _COM_Outptr_ void** ppFactory);

using DXGICreateSwapChainForHwndProc = HRESULT STDMETHODCALLTYPE (
    IDXGIFactory2* pFactory,
    _In_  IUnknown* pDevice,
    _In_  HWND hWnd,
    _In_  const DXGI_SWAP_CHAIN_DESC1* pDesc,
    _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
    _In_opt_  IDXGIOutput* pRestrictToOutput,
    _COM_Outptr_  IDXGISwapChain1** ppSwapChain);

using DXGISwapChainPresentProc = HRESULT STDMETHODCALLTYPE(
    IDXGISwapChain* pSwapChain,
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT Flags);

enum class D3D11CreateDeviceHookTag{};
using D3D11CreateDeviceHookType = WrappableFunction<D3D11CreateDeviceHookTag, D3D11CreateDeviceProc>;
D3D11CreateDeviceHookType* D3D11CreateDeviceHookType::gHook;

enum class CreateDXGIFactory1HookTag{};
using CreateDXGIFactory1HookType = WrappableFunction<CreateDXGIFactory1HookTag, CreateDXGIFactory1Proc>;
CreateDXGIFactory1HookType* CreateDXGIFactory1HookType::gHook;

enum class DXGICreateSwapChainForHwndHookTag{};
using DXGICreateSwapChainForHwndHookType = WrappableFunction<DXGICreateSwapChainForHwndHookTag, DXGICreateSwapChainForHwndProc>;
DXGICreateSwapChainForHwndHookType* DXGICreateSwapChainForHwndHookType::gHook;

enum class DXGISwapChainPresentHookTag{};
using DXGISwapChainPresentHookType = WrappableFunction<DXGISwapChainPresentHookTag, DXGISwapChainPresentProc>;
DXGISwapChainPresentHookType* DXGISwapChainPresentHookType::gHook;

END_SE()

BEGIN_NS(extui)

class DX11Backend : public RenderingBackend
{
public:
    DX11Backend(IMGUIManager& ui) : ui_(ui) {}

    ~DX11Backend() override
    {
        IMGUI_DEBUG("Destroying DX backend");
        DestroyUI();
        DisableHooks();
    }

    void EnableHooks() override
    {
        if (CreateDeviceHook_.IsWrapped()) return;

        IMGUI_DEBUG("DX11Backend::EnableHooks()");

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        auto dx11Lib = LoadLibraryW(L"d3d11.dll");
        auto createDevice = (D3D11CreateDeviceProc*)GetProcAddress(dx11Lib, "D3D11CreateDevice");

        auto dxgiLib = LoadLibraryW(L"dxgi.dll");
        auto createDxgiFactory = (CreateDXGIFactory1Proc*)GetProcAddress(dxgiLib, "CreateDXGIFactory1");

        CreateDeviceHook_.Wrap(ResolveFunctionTrampoline(createDevice));
        CreateDxgiFactoryHook_.Wrap(ResolveFunctionTrampoline(createDxgiFactory));
        DetourTransactionCommit();

        CreateDeviceHook_.SetPostHook(&DX11Backend::D3D11CreateDeviceHooked, this);
        CreateDxgiFactoryHook_.SetPostHook(&DX11Backend::CreateDXGIFactory1Hooked, this);
        DXGICreateSwapChainForHwndHook_.SetPostHook(&DX11Backend::DXGICreateSwapChainForHwndHooked, this);
        DXGISwapChainPresentHook_.SetPreHook(&DX11Backend::DXGISwapChainPresentHooked, this);
    }

    void DisableHooks() override
    {
        if (!CreateDeviceHook_.IsWrapped()) return;

        IMGUI_DEBUG("DX11Backend::DisableHooks()");

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        CreateDeviceHook_.Unwrap();
        CreateDxgiFactoryHook_.Unwrap();
        DetourTransactionCommit();
    }

    void InitializeUI() override
    {
        if (initialized_) return;

        IMGUI_DEBUG("DX11Backend::InitializeUI()");
        ImGui_ImplDX11_Init(device_, context_);

        initializationFailed_ = false;
        initialized_ = true;
    }

    void DestroyUI() override
    {
        if (!initialized_) return;

        IMGUI_DEBUG("DX11Backend::DestroyUI()");
        ImGui_ImplDX11_Shutdown();
        initialized_ = false;
    }

    void NewFrame() override
    {
        curViewport_ = (curViewport_ + 1) % viewports_.size();
        GImGui->Viewports[0] = &viewports_[curViewport_].Viewport;

        if (requestReloadFonts_) {
            IMGUI_DEBUG("Rebuilding font atlas");
            ImGui_ImplDX11_InvalidateDeviceObjects();
            requestReloadFonts_ = false;
            initializationFailed_ = false;
        }

        if (!initializationFailed_ && !ImGui_ImplDX11_RenderObjectsInitialized()) {
            IMGUI_DEBUG("Re-initializing DX11 render objects");
            initializationFailed_ = !ImGui_ImplDX11_CreateDeviceObjects();
            if (initializationFailed_) {
                ERR("Failed to initialize IMGUI DX11 backend!");
            }
        }

        if (!initializationFailed_) {
            ImGui_ImplDX11_NewFrame();
        }
    }

    void FinishFrame() override
    {
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
    }

    void ClearFrame() override
    {
        drawViewport_ = -1;
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
        return glm::ivec2(width_, height_);
    }

    std::optional<TextureLoadResult> RegisterTexture(TextureDescriptor* descriptor) override
    {
        auto view = descriptor->DX11.Views[0].View;
        if (!view) return {};

        return TextureLoadResult{ view, descriptor->DX11.Width, descriptor->DX11.Height };
    }

    void UnregisterTexture(ImTextureID id) override
    {
    }

private:
    struct ViewportInfo
    {
        ImGuiViewportP Viewport;
        Array<ImDrawList*> ClonedDrawLists;
    };

    void WINAPI D3D11CreateDeviceHooked(IDXGIAdapter* pAdapter,
        D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels,
        UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice,
        D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext, HRESULT result)
    {
        IMGUI_DEBUG("D3D11CreateDevice -> %p, %d", *ppDevice, result);
        IMGUI_DEBUG("DriverType %x, Software %p, Flags %x, FeatureLevels %d, SDKVersion %x, ppImmediateContext %p", DriverType, Software, Flags, FeatureLevels, SDKVersion, ppImmediateContext);

        for (uint32_t i = 0; i < FeatureLevels; i++) {
            IMGUI_DEBUG("FeatureLevel=%x", pFeatureLevels[i]);
        }

        if (!SUCCEEDED(result)) return;

        if (device_ == nullptr && ppImmediateContext != nullptr 
            // Filter out extra device created by nvidia overlay
            && FeatureLevels == 1
            && (*pFeatureLevels == D3D_FEATURE_LEVEL_11_1 || *pFeatureLevels == D3D_FEATURE_LEVEL_11_0)) {
            adapter_ = pAdapter;
            context_ = *ppImmediateContext;
            device_ = *ppDevice;
            IMGUI_DEBUG("Bind: Adapter %p, Context %p, Device %p", adapter_, context_, device_);
        }
    }

    void WINAPI CreateDXGIFactory1Hooked(REFIID riid, _COM_Outptr_ void** ppFactory, HRESULT result)
    {
        IMGUI_DEBUG("CreateDXGIFactory1 -> %p, %d", *ppFactory, result);
        IMGUI_DEBUG("RIID: %08x %04x %04x %16llx", riid.Data1, riid.Data2, riid.Data3, *(uint64_t*)&riid.Data4[0]);

        if (!SUCCEEDED(result)) return;
        if (riid != IID_IDXGIFactory1) return;

        if (DXGICreateSwapChainForHwndHook_.IsWrapped()) return;

        factory_ = (IDXGIFactory2*)*ppFactory;
        auto createSwapChain = (*(void***)factory_)[15];

        IMGUI_DEBUG("Hooking DXGICreateSwapChainForHwnd");
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DXGICreateSwapChainForHwndHook_.Wrap(ResolveFunctionTrampoline(createSwapChain));
        DetourTransactionCommit();
    }

    void STDMETHODCALLTYPE DXGICreateSwapChainForHwndHooked(
        IDXGIFactory2* pFactory,
        _In_  IUnknown* pDevice,
        _In_  HWND hWnd,
        _In_  const DXGI_SWAP_CHAIN_DESC1* pDesc,
        _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
        _In_opt_  IDXGIOutput* pRestrictToOutput,
        _COM_Outptr_  IDXGISwapChain1** ppSwapChain, 
        HRESULT result)
    {
        IMGUI_DEBUG("DXGICreateSwapChainForHwnd -> %p, %d", *ppSwapChain, result);
        IMGUI_DEBUG("Device %p, HWND %d, %d x %d, fmt %d, usage %d, flags %x", pDevice, hWnd, pDesc->Width, pDesc->Height,
            pDesc->Format, pDesc->BufferUsage, pDesc->Flags);

        if (!SUCCEEDED(result)) return;
        
        swapChain_ = *ppSwapChain;
        width_ = pDesc->Width;
        height_ = pDesc->Height;

        if (DXGISwapChainPresentHook_.IsWrapped()) return;

        auto createSwapChain = (*(void***)swapChain_)[8];

        IMGUI_DEBUG("Hooking DXGISwapChainPresent");
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DXGISwapChainPresentHook_.Wrap(ResolveFunctionTrampoline(createSwapChain));
        DetourTransactionCommit();

        if (swapChain_ != nullptr && device_ != nullptr) {
            ui_.OnRenderBackendInitialized();
        }
    }

    void STDMETHODCALLTYPE DXGISwapChainPresentHooked(
        IDXGISwapChain* pSwapChain,
        /* [in] */ UINT SyncInterval,
        /* [in] */ UINT Flags)
    {
        if (pSwapChain != swapChain_ || !initialized_ || drawViewport_ == -1) return;

        auto& vp = viewports_[drawViewport_].Viewport;
        if (!vp.DrawDataP.Valid || initializationFailed_) return;

        ImGui_ImplDX11_RenderDrawData(&vp.DrawDataP);
    }


    IMGUIManager& ui_;
    std::array<ViewportInfo, 3> viewports_;
    int32_t drawViewport_{ -1 };
    int32_t curViewport_{ 0 };

    IDXGIAdapter* adapter_{ nullptr };
    ID3D11DeviceContext* context_{ nullptr };
    ID3D11Device* device_{ nullptr };
    IDXGIFactory1* factory_{ nullptr };
    IDXGISwapChain* swapChain_{ nullptr };

    bool initialized_{ false };
    bool initializationFailed_{ false };
    bool requestReloadFonts_{ false };
    uint32_t width_{ 0 };
    uint32_t height_{ 0 };

    D3D11CreateDeviceHookType CreateDeviceHook_;
    CreateDXGIFactory1HookType CreateDxgiFactoryHook_;
    DXGICreateSwapChainForHwndHookType DXGICreateSwapChainForHwndHook_;
    DXGISwapChainPresentHookType DXGISwapChainPresentHook_;
};

END_NS()
