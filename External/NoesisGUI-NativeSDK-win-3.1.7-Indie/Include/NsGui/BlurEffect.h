////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BLUREFFECT_H__
#define __GUI_BLUREFFECT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Effect.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A bitmap effect that blurs the target texture.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
///        <Image Width="300" Height="300" Source="Images/image.jpg">
///            <Image.Effect>
///              <BlurEffect Radius="20"/>
///            </Image.Effect>
///        </Image>
///    </Grid>
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.effects.blureffect
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BlurEffect final: public Effect
{
public:
    BlurEffect();
    ~BlurEffect();

    /// Gets or sets a value that indicates the radius of the blur effect.
    //@{
    float GetRadius();
    void SetRadius(float value);
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

public:
    static const DependencyProperty* RadiusProperty;

private:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_DECLARE_REFLECTION(BlurEffect, Effect)
};

NS_WARNING_POP

}


#endif
