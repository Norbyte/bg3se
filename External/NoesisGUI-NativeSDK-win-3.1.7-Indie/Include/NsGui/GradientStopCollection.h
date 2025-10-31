////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRADIENTSTOPCOLLECTION_H__
#define __GUI_GRADIENTSTOPCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/FreezableCollection.h>
#include <NsGui/GradientStop.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of GradientStop objects that can be individually accessed by index.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.gradientstopcollection.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API GradientStopCollection final: public FreezableCollection<GradientStop>,
    public IRenderProxyCreator
{
public:
    GradientStopCollection();
    ~GradientStopCollection();

    // Hides Freezable methods for convenience
    //@{
    Ptr<GradientStopCollection> Clone() const;
    Ptr<GradientStopCollection> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From FreezableCollection
    //@{
    void InsertItem(uint32_t index, BaseComponent* item) override;
    void SetItem(uint32_t index, BaseComponent* item) override;
    void RemoveItem(uint32_t index) override;
    void ClearItems() override;
    //@}

private:
    void RegisterStop(GradientStop* stop);
    void UnregisterStop(GradientStop* stop);
    void UnregisterStops();
    void OnStopChanged(Freezable* freezable, FreezableEventReason reason);

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Stops
    };

    NS_DECLARE_REFLECTION(GradientStopCollection, BaseFreezableCollection)
};

NS_WARNING_POP

}


#endif
