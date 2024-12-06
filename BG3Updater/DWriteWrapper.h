#pragma once

#include <dwrite.h>
#include <memory>

BEGIN_SE()

class DWriteWrapper
{
public:
    DWriteWrapper();
    ~DWriteWrapper();

    HRESULT DWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory);

private:
    typedef HRESULT(WINAPI * DWriteCreateFactoryProc)(DWRITE_FACTORY_TYPE factoryType, REFIID iid, IUnknown** factory);

    HMODULE dllHandle;
    DWriteCreateFactoryProc CreateFactory;
};

extern std::unique_ptr<DWriteWrapper> gDWriteWrapper;

END_SE()
