////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SOLIDCOLORBRUSH_H__
#define __GUI_SOLIDCOLORBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/Brush.h>


namespace Noesis
{

struct Color;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paints an area with a solid color.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.solidcolorbrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API SolidColorBrush final: public Brush
{
public:
    SolidColorBrush();
    SolidColorBrush(const Color& color);

    /// Gets or sets the color of this SolidColorBrush
    //@{
    const Color& GetColor() const;
    void SetColor(const Color& color);
    //@}

    /// Parses a string value to create an instance of a SolidColorBrush
    static bool TryParse(const char* str, Ptr<SolidColorBrush>& result);

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<SolidColorBrush> Clone() const;
    Ptr<SolidColorBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ColorProperty;
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

    /// From Brush
    //@{
    bool IsTransparentOverride() const override;
    //@}

private:
    enum UpdateFlags
    {
        UpdateFlags_Color = Brush::UpdateFlags_Sentinel
    };

    NS_DECLARE_REFLECTION(SolidColorBrush, Brush)
};

}


#endif
