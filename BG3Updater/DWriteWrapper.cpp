#include "stdafx.h"
#include "DWriteWrapper.h"
#include "GameHelpers.h"

BEGIN_SE()

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

    if (CreateFactory == NULL) {
        Fail("Could not locate DWriteCreateFactory in DWrite.dll");
    }
}

DWriteWrapper::~DWriteWrapper()
{
    FreeLibrary(dllHandle);
}

HRESULT DWriteWrapper::DWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory)
{
    return CreateFactory(factoryType, iid, factory);
}

END_SE()

extern "C"
{
    HRESULT WINAPI WrappedDWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory)
    {
        return bg3se::gDWriteWrapper->DWriteCreateFactory(factoryType, iid, factory);
    }
}
