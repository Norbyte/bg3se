#pragma once

#include <dxgi.h>
#include <memory>

class DxgiWrapper
{
public:
	DxgiWrapper();
	~DxgiWrapper();

	HRESULT CreateDXGIFactory(REFIID riid, _Out_ void **ppFactory);
	HRESULT CreateDXGIFactory1(REFIID riid, _Out_ void **ppFactory);
	HRESULT CreateDXGIFactory2(UINT Flags, REFIID riid, _Out_ void **ppFactory);
	HRESULT DXGIDumpJournal();
	HRESULT DXGIReportAdapterConfiguration();
	HRESULT DXGID3D10CreateDevice(HMODULE hModule, IDXGIFactory * pFactory, IDXGIAdapter * pAdapter,
		UINT Flags, void * pUnknown, void ** ppDevice);
	HRESULT DXGID3D10CreateLayeredDevice(void * pUnknown1, void * pUnknown2, void * pUnknown3, void * pUnknown4,
		void * pUnknown5);
	SIZE_T DXGID3D10GetLayeredDeviceSize(const void *pLayers, UINT NumLayers);
	HRESULT DXGID3D10RegisterLayers(const void *pLayers, UINT NumLayers);

private:
	typedef HRESULT(WINAPI * CreateDXGIFactoryProc)(REFIID riid, _Out_ void **ppFactory);
	typedef HRESULT(WINAPI * CreateDXGIFactory2Proc)(UINT Flags, REFIID riid, _Out_ void **ppFactory);
	typedef HRESULT(WINAPI * DXGIDumpJournalProc)();
	typedef HRESULT(WINAPI * DXGIReportAdapterConfigurationProc)();
	typedef HRESULT(WINAPI * DXGID3D10CreateDeviceProc)(HMODULE, IDXGIFactory *, IDXGIAdapter *, UINT, void *, void **);
	typedef HRESULT(WINAPI * DXGID3D10CreateLayeredDeviceProc)(void *, void *, void *, void *, void *);
	typedef SIZE_T(WINAPI * DXGID3D10GetLayeredDeviceSizeProc)(const void *, UINT);
	typedef HRESULT(WINAPI * DXGID3D10RegisterLayersProc)(const void *, UINT);

	HMODULE dxgiHandle;
	CreateDXGIFactoryProc createDXGIFactory;
	CreateDXGIFactoryProc createDXGIFactory1;
	CreateDXGIFactory2Proc createDXGIFactory2;
	DXGIDumpJournalProc dumpJournal;
	DXGIReportAdapterConfigurationProc reportAdapterConfiguration;
	DXGID3D10CreateDeviceProc D3D10CreateDevice;
	DXGID3D10CreateLayeredDeviceProc D3D10CreateLayeredDevice;
	DXGID3D10GetLayeredDeviceSizeProc D3D10GetLayeredDeviceSize;
	DXGID3D10RegisterLayersProc D3D10RegisterLayers;
};

extern std::unique_ptr<DxgiWrapper> gDxgiWrapper;
