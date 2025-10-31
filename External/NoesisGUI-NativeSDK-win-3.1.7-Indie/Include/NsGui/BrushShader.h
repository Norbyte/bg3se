////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BRUSHSHADER_H__
#define __GUI_BRUSHSHADER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Animatable.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

class ImageSource;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a custom brush by using a pixel shader.
///
/// Derive from the BrushShader class to implement a custom brush based on a pixel shader.
///
/// In comparison with ShaderEffect, a brush shader is more efficient because it doesn't need an
/// extra render pass to an offscreen.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///      xmlns:noesis="clr-namespace:NoesisGUIExtensions;assembly=Noesis.GUI.Extensions"
///      xmlns:local="clr-namespace:CustomBrushes">
///
///      <Rectangle Width="300" Height="300">
///        <Rectangle.Fill>
///          <ImageBrush ImageSource="Images/image.jpg">
///            <noesis:Brush.Shader>
///              <local:WaveBrushShader Frequency="0.7" />
///            </noesis:Brush.Shader>
///          </ImageBrush>
///        </Rectangle.Fill>
///      </Rectangle>
///
///    </Grid>
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BrushShader: public Animatable, public IRenderProxyCreator
{
public:
    BrushShader();
    BrushShader(const BrushShader&) = delete;
    BrushShader& operator=(const BrushShader&) = delete;
    virtual ~BrushShader() = 0;

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    enum class Target: uint8_t
    {
        Path,
        Path_AA,
        SDF,
        SDF_LCD,
        Opacity
    };

    /// Sets the shader instructions for this brush for the specified target
    void SetPixelShader(void* shader, Target target);

    /// Sets extra texture at the given index
    void SetTexture(ImageSource* texture, uint32_t index);

    /// Sets the constant buffer for this brush's pixel shader
    void SetConstantBuffer(const void* buffer, uint32_t size);

    /// This method must be called whenever the content of the constant buffer changes
    void InvalidateConstantBuffer();

protected:
    RenderProxyCreatorFlags mUpdateFlags;

    void* mPixelShaders[5];
    ImageSource* mTextures[4];

    const void* mCBuffer;
    uint32_t mCBufferSize;

    NS_DECLARE_REFLECTION(BrushShader, Animatable)
};

NS_WARNING_POP

}


#endif
