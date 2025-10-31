////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DROPSHADOWEFFECT_H__
#define __GUI_DROPSHADOWEFFECT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Effect.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

struct Color;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A bitmap effect that paints a drop shadow around the target texture.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
///        <Rectangle Width="300" Height="300" Fill="Turquoise">
///            <Rectangle.Effect>
///              <DropShadowEffect BlurRadius="20" ShadowDepth="10" Opacity="0.5"/>
///            </Rectangle.Effect>
///        </Rectangle>
///    </Grid>
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.effects.dropshadoweffect
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DropShadowEffect final: public Effect
{
public:
    DropShadowEffect();
    ~DropShadowEffect();

    /// Gets or sets a value that indicates the radius of the shadow's blur effect.
    //@{
    float GetBlurRadius();
    void SetBlurRadius(float value);
    //@}

    /// Gets or sets the color of the drop shadow.
    //@{
    const Color& GetColor() const;
    void SetColor(const Color& value);
    //@}

    /// Gets or sets the direction in degrees of the drop shadow.
    //@{
    float GetDirection() const;
    void SetDirection(float value);
    //@}

    /// Gets or sets the opacity of the drop shadow.
    //@{
    float GetOpacity() const;
    void SetOpacity(float value);
    //@}

    /// Gets or sets the distance of the drop shadow below the texture.
    //@{
    float GetShadowDepth() const;
    void SetShadowDepth(float value);
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    static const DependencyProperty* BlurRadiusProperty;
    static const DependencyProperty* ColorProperty;
    static const DependencyProperty* DirectionProperty;
    static const DependencyProperty* OpacityProperty;
    static const DependencyProperty* ShadowDepthProperty;

private:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(DropShadowEffect, Effect)
};

NS_WARNING_POP

}


#endif
