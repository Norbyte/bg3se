////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ICOMPONENTINITIALIZER_H__
#define __GUI_ICOMPONENTINITIALIZER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/DependencySystemApi.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// IComponentInitializer. Identifies components that can be initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IComponentInitializer: public Interface
{
    /// Indicates if component is already initialized
    virtual bool IsInitialized() const = 0;

    /// Initializes the component
    virtual void Init() = 0;

    /// Signals the component that initialization is starting. Any calls to Init() after BeginInit
    /// will be skipped until EndInit gets called
    virtual void BeginInit() = 0;

    /// Signals the component that initialization is complete
    virtual void EndInit() = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IComponentInitializer, Interface)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void InitComponent(IComponentInitializer* component, bool doInit)
{
    if (doInit && component && !component->IsInitialized())
    {
        component->Init();
    }
}

}

#endif
