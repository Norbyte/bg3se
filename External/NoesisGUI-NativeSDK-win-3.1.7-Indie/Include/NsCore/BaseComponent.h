////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_BASECOMPONENT_H__
#define __CORE_BASECOMPONENT_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>
#include <NsCore/BaseRefCounted.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for components
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_CORE_KERNEL_API BaseComponent: public BaseRefCounted
{
public:
    BaseComponent();
    BaseComponent(const BaseComponent&) = delete;
    BaseComponent& operator=(const BaseComponent&) = delete;
    virtual ~BaseComponent() = 0;

private:
    NS_DECLARE_REFLECTION(BaseComponent, BaseRefCounted)
};

}

#endif
