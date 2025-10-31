////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IRENDERPROXYCREATOR_H__
#define __GUI_IRENDERPROXYCREATOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsCore/Ptr.h>
#include <NsCore/HashMap.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class BaseComponent;
class RenderTreeUpdater;
typedef void* ViewId;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Creates and updates the proxy used in render thread.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IRenderProxyCreator: public Interface
{
    /// Creates the corresponding render proxy for this object
    virtual void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) = 0;

    /// Updates the corresponding render proxy for this object
    virtual void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) = 0;

    /// Unregisters the specified view
    virtual void UnregisterRenderer(ViewId viewId) = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IRenderProxyCreator, Interface)
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Manages dirty flags of a render proxy creator object
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RenderProxyCreatorFlags
{
public:
    /// Indicates if the specified flag is set for the view
    bool Check(uint32_t flag, ViewId viewId);

    /// Modifies a flag of the creator for the specified view
    //@{
    void Set(uint32_t flag, ViewId viewId);
    void Clear(uint32_t flag, ViewId viewId);
    //@}

    /// Modifies a flag of the creator for all the registered views
    //@{
    void Set(uint32_t flag);
    void Clear(uint32_t flag);
    //@}

    /// Registers the resource in a view
    void RegisterView(ViewId viewId);

    /// Unregisters the resource from the specified view
    void UnregisterRenderer(ViewId viewId);

    /// Notifies all registered views that resource was destroyed
    void NotifyDestroyed(BaseComponent* resource);

private:
    typedef HashMap<ViewId, uint32_t> Flags;
    Flags mFlags;
};

NS_WARNING_POP

}


#endif
