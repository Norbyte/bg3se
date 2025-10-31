////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTURESOURCE_H__
#define __GUI_TEXTURESOURCE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/BitmapSource.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class Texture;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a BitmapSource constructed from a Texture.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TextureSource: public BitmapSource
{
public:
    /// Constructor
    TextureSource();
    TextureSource(Texture* texture);

    /// Destructor
    ~TextureSource();

    /// Gets or sets texture
    //@{
    Texture* GetTexture() const;
    void SetTexture(Texture* texture);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<TextureSource> Clone() const;
    Ptr<TextureSource> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

protected:
    /// From Freezable
    //@{
    void CloneCommonCore(const Freezable* source) override;
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From BitmapSource
    //@{
    float GetDpiXCore() const override;
    float GetDpiYCore() const override;
    int32_t GetPixelWidthCore() const override;
    int32_t GetPixelHeightCore() const override;
    //@}

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Texture
    };

    Ptr<Texture> mTexture;

    NS_DECLARE_REFLECTION(TextureSource, BitmapSource)
};

NS_WARNING_POP

}


#endif
