#include "stdafx.h"
#include "DWriteWrapper.h"

std::unique_ptr<DWriteWrapper> gDWriteWrapper;

DWriteWrapper::DWriteWrapper()
{
	TCHAR dllPath[1024 + 64];
	DWORD dllPathSize = GetSystemDirectory(dllPath, 1024);
	if (dllPathSize == 0)
	{
		Fail("Could not get system directory path");
	}

	wcscpy_s(dllPath + dllPathSize, std::size(dllPath) - dllPathSize, L"\\DWrite.dll");

	dllHandle = LoadLibrary(dllPath);
	if (dllHandle == NULL) {
		Fail("Could not load DWrite.dll");
	}

	CreateFactory = (DWriteCreateFactoryProc)GetProcAddress(dllHandle, "DWriteCreateFactory");

	if (CreateFactory == NULL)
	{
		Fail("Could not locate DWriteCreateFactory in DWrite.dll");
	}

	DEBUG("DWrite exports successfully mapped");
}

DWriteWrapper::~DWriteWrapper()
{
	FreeLibrary(dllHandle);
}

HRESULT DWriteWrapper::DWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory)
{
	return CreateFactory(factoryType, iid, factory);
}

extern "C"
{
	HRESULT WINAPI WrappedDWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory)
	{
		return gDWriteWrapper->DWriteCreateFactory(factoryType, iid, factory);
	}
}
