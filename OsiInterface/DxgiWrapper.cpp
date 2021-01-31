#include "stdafx.h"
#include "DxgiWrapper.h"

std::unique_ptr<DxgiWrapper> gDxgiWrapper;

DxgiWrapper::DxgiWrapper()
{
	TCHAR dxgiPath[1024 + 64];
	DWORD dxgiPathSize = GetSystemDirectory(dxgiPath, 1024);
	if (dxgiPathSize == 0)
	{
		Fail("Could not get system directory path");
	}

	wcscpy_s(dxgiPath + dxgiPathSize, 1024 + 64 - dxgiPathSize, L"\\dxgi.dll");

	dxgiHandle = LoadLibrary(dxgiPath);
	if (dxgiHandle == NULL)
	{
		Fail("Could not load dxgi.dll");
	}

	createDXGIFactory = (CreateDXGIFactoryProc)GetProcAddress(dxgiHandle, "CreateDXGIFactory");
	createDXGIFactory1 = (CreateDXGIFactoryProc)GetProcAddress(dxgiHandle, "CreateDXGIFactory1");
	createDXGIFactory2 = (CreateDXGIFactory2Proc)GetProcAddress(dxgiHandle, "CreateDXGIFactory2");

	if (createDXGIFactory == NULL)
	{
		Fail("Could not locate CreateDXGIFactory in dxgi.dll");
	}
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
}
