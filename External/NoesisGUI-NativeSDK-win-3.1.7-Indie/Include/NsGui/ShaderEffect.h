////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SHADEREFFECT_H__
#define __GUI_SHADEREFFECT_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Effect.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a custom bitmap effect by using a pixel shader.
///
/// Derive from the ShaderEffect class to implement a custom effect based on a single pixel shader.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:local="clr-namespace:Effects">
///
///      <Image Width="300" Height="300" Source="Images/image.jpg">
///        <Image.Effect>
///          <local:ThresholdEffect Threshold="0.25" />
///        </Image.Effect>
///      </Image>
///
///    </Grid>
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.media.effects.shadereffect
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ShaderEffect: public Effect
{
public:
    ShaderEffect();
    ShaderEffect(const ShaderEffect&) = delete;
    ShaderEffect& operator=(const ShaderEffect&) = delete;
    virtual ~ShaderEffect() = 0;

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

protected:
    /// Sets a value indicating that the effect's output texture is larger than its input texture
    void SetPadding(float left, float top, float right, float bottom);

    /// Sets the shader instructions for this effect
    void SetPixelShader(void* shader);

    /// Sets the constant buffer for this effect's pixel shader
    void SetConstantBuffer(const void* buffer, uint32_t size);

    /// This method must be called whenever the content of the constant buffer changes
    void InvalidateConstantBuffer();

protected:
    float mPaddingLeft, mPaddingTop, mPaddingRight, mPaddingBottom;
    const void* mCBuffer;
    uint32_t mCBufferSize;

    static const DependencyProperty* PixelShaderProperty;

    NS_DECLARE_REFLECTION(ShaderEffect, Effect)
};

NS_WARNING_POP

}


#endif
