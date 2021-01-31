#include "stdafx.h"
#include "DxgiWrapper.h"
#include "ErrorUtils.h"

std::unique_ptr<DxgiWrapper> gDxgiWrapper;

DxgiWrapper::DxgiWrapper()
{
	TCHAR dxgiPath[1024 + 64];
	DWORD dxgiPathSize = GetSystemDirectory(dxgiPath, 1024);
	if (dxgiPathSize == 0)
	{
		Fail("Could not get system directory path");
	}

	wcscpy_s(dxgiPath + dxgiPathSize, std::size(dxgiPath) - dxgiPathSize, L"\\dxgi.dll");

	dxgiHandle = LoadLibrary(dxgiPath);
	if (dxgiHandle == NULL)
	{
		Fail("Could not load dxgi.dll");
	}

	createDXGIFactory = (CreateDXGIFactoryProc)GetProcAddress(dxgiHandle, "CreateDXGIFactory");
	createDXGIFactory1 = (CreateDXGIFactoryProc)GetProcAddress(dxgiHandle, "CreateDXGIFactory1");
	createDXGIFactory2 = (CreateDXGIFactory2Proc)GetProcAddress(dxgiHandle, "CreateDXGIFactory2");
	dumpJournal = (DXGIDumpJournalProc)GetProcAddress(dxgiHandle, "DXGIDumpJournal");
	reportAdapterConfiguration = (DXGIReportAdapterConfigurationProc)GetProcAddress(dxgiHandle, "DXGIReportAdapterConfiguration");
	D3D10CreateDevice = (DXGID3D10CreateDeviceProc)GetProcAddress(dxgiHandle, "DXGID3D10CreateDevice");
	D3D10CreateLayeredDevice = (DXGID3D10CreateLayeredDeviceProc)GetProcAddress(dxgiHandle, "DXGID3D10CreateLayeredDevice");
	D3D10GetLayeredDeviceSize = (DXGID3D10GetLayeredDeviceSizeProc)GetProcAddress(dxgiHandle, "DXGID3D10GetLayeredDeviceSize");
	D3D10RegisterLayers = (DXGID3D10RegisterLayersProc)GetProcAddress(dxgiHandle, "DXGID3D10RegisterLayers");

	if (createDXGIFactory == NULL)
	{
		Fail("Could not locate CreateDXGIFactory in dxgi.dll");
	}

	DEBUG("DXGI exports successfully mapped");
}

DxgiWrapper::~DxgiWrapper()
{
	FreeLibrary(dxgiHandle);
}

HRESULT DxgiWrapper::CreateDXGIFactory(REFIID riid, _Out_ void **ppFactory)
{
	return createDXGIFactory(riid, ppFactory);
}

HRESULT DxgiWrapper::CreateDXGIFactory1(REFIID riid, _Out_ void **ppFactory)
{
	if (createDXGIFactory1 == NULL)
	{
		Fail("CreateDXGIFactory1() called, but the original entry point could not be located in dxgi.dll");
	}

	return createDXGIFactory1(riid, ppFactory);
}

HRESULT DxgiWrapper::CreateDXGIFactory2(UINT Flags, REFIID riid, _Out_ void **ppFactory)
{
	if (createDXGIFactory2 == NULL)
	{
		Fail("CreateDXGIFactory2() called, but the original entry point could not be located in dxgi.dll");
	}

	return createDXGIFactory2(Flags, riid, ppFactory);
}

HRESULT DxgiWrapper::DXGIDumpJournal()
{
	return dumpJournal();
}

HRESULT DxgiWrapper::DXGIReportAdapterConfiguration()
{
	return reportAdapterConfiguration();
}

HRESULT DxgiWrapper::DXGID3D10CreateDevice(HMODULE hModule, IDXGIFactory *pFactory, IDXGIAdapter *pAdapter, UINT Flags, void *pUnknown, void **ppDevice)
{
	return D3D10CreateDevice(hModule, pFactory, pAdapter, Flags, pUnknown, ppDevice);
}

HRESULT DxgiWrapper::DXGID3D10CreateLayeredDevice(void *pUnknown1, void *pUnknown2, void *pUnknown3, void *pUnknown4, void *pUnknown5)
{
	return D3D10CreateLayeredDevice(pUnknown1, pUnknown2, pUnknown3, pUnknown4, pUnknown5);
}

SIZE_T DxgiWrapper::DXGID3D10GetLayeredDeviceSize(const void *pLayers, UINT NumLayers)
{
	return D3D10GetLayeredDeviceSize(pLayers, NumLayers);
}

HRESULT DxgiWrapper::DXGID3D10RegisterLayers(const void *pLayers, UINT NumLayers)
{
	return D3D10RegisterLayers(pLayers, NumLayers);
}

extern "C"
{
	HRESULT WINAPI WrappedCreateDXGIFactory(REFIID riid, _Out_ void **ppFactory)
	{
		return gDxgiWrapper->CreateDXGIFactory(riid, ppFactory);
	}

	HRESULT WINAPI WrappedCreateDXGIFactory1(REFIID riid, _Out_ void **ppFactory)
	{
		return gDxgiWrapper->CreateDXGIFactory1(riid, ppFactory);
	}

	HRESULT WINAPI WrappedCreateDXGIFactory2(UINT Flags, REFIID riid, _Out_ void **ppFactory)
	{
		return gDxgiWrapper->CreateDXGIFactory2(Flags, riid, ppFactory);
	}

	HRESULT WINAPI WrappedDXGIDumpJournal()
	{
		return gDxgiWrapper->DXGIDumpJournal();
	}

	HRESULT WINAPI WrappedDXGIReportAdapterConfiguration()
	{
		return gDxgiWrapper->DXGIReportAdapterConfiguration();
	}

	HRESULT WINAPI WrappedDXGID3D10CreateDevice(HMODULE hModule, IDXGIFactory * pFactory, IDXGIAdapter * pAdapter, 
		UINT Flags, void * pUnknown, void ** ppDevice)
	{
		return gDxgiWrapper->DXGID3D10CreateDevice(hModule, pFactory, pAdapter, Flags, pUnknown, ppDevice);
	}

	HRESULT WINAPI WrappedDXGID3D10CreateLayeredDevice(void * pUnknown1, void * pUnknown2, void * pUnknown3, void * pUnknown4,
		void * pUnknown5)
	{
		return gDxgiWrapper->DXGID3D10CreateLayeredDevice(pUnknown1, pUnknown2, pUnknown3, pUnknown4, pUnknown5);
	}

	SIZE_T WINAPI WrappedDXGID3D10GetLayeredDeviceSize(const void *pLayers, UINT NumLayers)
	{
		return gDxgiWrapper->DXGID3D10GetLayeredDeviceSize(pLayers, NumLayers);
	}

	HRESULT WINAPI WrappedDXGID3D10RegisterLayers(const void *pLayers, UINT NumLayers)
	{
		return gDxgiWrapper->DXGID3D10RegisterLayers(pLayers, NumLayers);
	}
}
