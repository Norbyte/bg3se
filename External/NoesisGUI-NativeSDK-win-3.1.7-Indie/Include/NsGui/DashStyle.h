////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DASHSTYLE_H__
#define __GUI_DASHSTYLE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Animatable.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the sequence of dashes and gaps that will be applied by a Pen.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.dashstyle.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DashStyle final: public Animatable, public IRenderProxyCreator
{
public:
    DashStyle();
    ~DashStyle();

    /// Gets or sets the collection of dashes and gaps in this DashStyle
    //@{
    const char* GetDashes() const;
    void SetDashes(const char* dashes);
    //@}

    /// Gets or sets how far in the dash sequence the stroke will start
    //@{
    float GetOffset() const;
    void SetOffset(float offset);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<DashStyle> Clone() const;
    Ptr<DashStyle> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* DashesProperty;
    static const DependencyProperty* OffsetProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Dashes,
        UpdateFlags_Offset
    };

    NS_DECLARE_REFLECTION(DashStyle, Animatable)
};

NS_WARNING_POP

}


#endif
