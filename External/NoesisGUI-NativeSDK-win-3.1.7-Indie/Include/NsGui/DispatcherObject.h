////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DISPATCHEROBJECT_H__
#define __GUI_DISPATCHEROBJECT_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
const uint32_t NoThreadId = static_cast<uint32_t>(-1);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an object with thread affinity.
///
/// This object can be accessed only from the thread on which it was created. Subclasses of
/// DispatcherObject that need to enforce thread safety can do so by calling *VerifyAccess* on all
/// public methods. This guarantees the calling thread is the thread that the DispatcherObject was
/// created on.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.threading.dispatcherobject.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API DispatcherObject: public BaseComponent
{
public:
    DispatcherObject(const DispatcherObject&) = delete;
    DispatcherObject& operator=(const DispatcherObject&) = delete;
    virtual ~DispatcherObject() = 0;

    /// Gets the thread this DispatcherObject is associated with.
    /// Returns NoThreadId when this object is not attached to any thread.
    uint32_t GetThreadId() const;

    /// Determines whether the calling thread has access to this DispatcherObject
    bool CheckAccess() const;

    /// Enforces that the calling thread has access to this DispatcherObject
    void VerifyAccess() const;

protected:
    DispatcherObject();

    // Allows certain derived classes to break the dispatcher affinity
    void DetachFromDispatcher();

private:
    uint32_t mThreadId;

    NS_DECLARE_REFLECTION(DispatcherObject, BaseComponent)
};

}

#endif
