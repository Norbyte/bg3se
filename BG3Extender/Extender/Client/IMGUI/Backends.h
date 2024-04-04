#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(extui)

class PlatformBackend
{
public:
    virtual ~PlatformBackend();
    virtual void EnableHooks() = 0;
    virtual void DisableHooks() = 0;
    virtual void InitializeUI() = 0;
    virtual void DestroyUI() = 0;
    virtual void NewFrame() = 0;
};

class RenderingBackend
{
public:
    virtual ~RenderingBackend();
    virtual void EnableHooks() = 0;
    virtual void DisableHooks() = 0;
    virtual void InitializeUI() = 0;
    virtual void DestroyUI() = 0;
    virtual void NewFrame() = 0;
    virtual void RenderFrame() = 0;
    virtual void FinishFrame() = 0;
};

END_NS()
