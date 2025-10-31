////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRADIENTBRUSH_H__
#define __GUI_GRADIENTBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/Brush.h>



namespace Noesis
{

struct Color;
class GradientStopCollection;
struct GradientBrushProxy;
enum ColorInterpolationMode: int32_t;
enum BrushMappingMode: int32_t;
enum GradientSpreadMethod: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an abstract class that describes a gradient, composed of gradient stops. Classes that
/// inherit from GradientBrush describe different ways of interpreting gradient stops.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.gradientbrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API GradientBrush: public Brush
{
public:
    GradientBrush();
    GradientBrush(const GradientBrush&) = delete;
    GradientBrush& operator=(const GradientBrush&) = delete;
    virtual ~GradientBrush() = 0;

    /// Gets or sets a ColorInterpolationMode enumeration that specifies how the gradient's colors
    /// are interpolated.
    //@{
    ColorInterpolationMode GetColorInterpolationMode() const;
    void SetColorInterpolationMode(ColorInterpolationMode mode);
    //@}

    /// Gets or sets the brush's gradient stops
    //@{
    GradientStopCollection* GetGradientStops() const;
    void SetGradientStops(GradientStopCollection* stops);
    //@}

    /// Gets or sets a BrushMappingMode enumeration that specifies whether the gradient brush's 
    /// positioning coordinates are absolute or relative to the output area.
    //@{
    BrushMappingMode GetMappingMode() const;
    void SetMappingMode(BrushMappingMode mode);
    //@}

    /// Gets or sets the type of spread method that specifies how to draw a gradient that starts or
    /// ends inside the bounds of the object to be painted.
    //@{
    GradientSpreadMethod GetSpreadMethod() const;
    void SetSpreadMethod(GradientSpreadMethod spreadMethod);
    //@}

    /// Hides Freezable methods for convenience
    //@{
    Ptr<GradientBrush> Clone() const;
    Ptr<GradientBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ColorInterpolationModeProperty;
    static const DependencyProperty* GradientStopsProperty;
    static const DependencyProperty* MappingModeProperty;
    static const DependencyProperty* SpreadMethodProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    bool OnSubPropertyChanged(const DependencyProperty* dp) override;
    //@}

    /// From Brush
    //@{
    bool IsTransparentOverride() const override;
    //@}

protected:
    enum UpdateFlags
    {
        UpdateFlags_GradientStops = Brush::UpdateFlags_Sentinel,
        UpdateFlags_MappingMode,
        UpdateFlags_SpreadMethod,
        UpdateFlags_Sentinel
    };

private:
    NS_DECLARE_REFLECTION(GradientBrush, Brush)
};

}


#endif
