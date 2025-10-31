////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RADIALGRADIENTBRUSH_H__
#define __GUI_RADIALGRADIENTBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/GradientBrush.h>


namespace Noesis
{

struct Point;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paints an area with a radial gradient.
///
/// A focal point defines the beginning  of the gradient, and a circle defines the end point of the
/// gradient. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.radialgradientbrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API RadialGradientBrush final: public GradientBrush
{
public:
    RadialGradientBrush();

    /// Gets or sets the center of the outermost circle of the radial gradient
    //@{
    const Point& GetCenter() const;
    void SetCenter(const Point& center);
    //@}

    /// Gets or sets the location of the two-dimensional focal point that defines the beginning of 
    /// the gradient
    //@{
    const Point& GetGradientOrigin() const;
    void SetGradientOrigin(const Point& origin);
    //@}

    /// Gets or sets the horizontal radius of the outermost circle of the radial gradient
    //@{
    float GetRadiusX() const;
    void SetRadiusX(float radius);
    //@}

    /// Gets or sets the vertical radius of the outermost circle of a radial gradient
    //@{
    float GetRadiusY() const;
    void SetRadiusY(float radius);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<RadialGradientBrush> Clone() const;
    Ptr<RadialGradientBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* CenterProperty;
    static const DependencyProperty* GradientOriginProperty;
    static const DependencyProperty* RadiusXProperty;
    static const DependencyProperty* RadiusYProperty;
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
    enum UpdateFlags
    {
        UpdateFlags_Radial = GradientBrush::UpdateFlags_Sentinel
    };

    NS_DECLARE_REFLECTION(RadialGradientBrush, GradientBrush)
};

}


#endif
