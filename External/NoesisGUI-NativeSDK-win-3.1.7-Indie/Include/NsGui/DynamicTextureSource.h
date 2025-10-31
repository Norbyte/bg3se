////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DYNAMICTEXTURESOURCE_H__
#define __GUI_DYNAMICTEXTURESOURCE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ImageSource.h>


namespace Noesis
{

class Texture;
class RenderDevice;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an ImageSource implementation based on a dynamic texture.
///
/// Dynamic textures are regenerated per frame. On each frame, this class will invoke the given
/// callback to get a new texture. The callback is always invoked from the render thread.
///
/// This source is appropriate for dynamic content like Video.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DynamicTextureSource: public ImageSource
{
public:
    typedef Texture* (*TextureRenderCallback)(RenderDevice* device, void* user);
    DynamicTextureSource(uint32_t width, uint32_t height, TextureRenderCallback callback, void* user);

    ~DynamicTextureSource();

    /// Resizes the dynamic texture
    void Resize(uint32_t width, uint32_t height);

    /// Gets the width of the texture in pixels
    /// \prop
    uint32_t GetPixelWidth() const;

    /// Gets the height of the texture in pixels
    /// \prop
    uint32_t GetPixelHeight() const;

private:
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

    /// From ImageSource
    //@{
    float GetWidthCore() const override;
    float GetHeightCore() const override;
    //@}

private:
    RenderProxyCreatorFlags mUpdateFlags;

    TextureRenderCallback mCallback;
    void* mUser;

    uint32_t mPixelWidth;
    uint32_t mPixelHeight;

    NS_DECLARE_REFLECTION(DynamicTextureSource, ImageSource)
};

NS_WARNING_POP

}


#endif
