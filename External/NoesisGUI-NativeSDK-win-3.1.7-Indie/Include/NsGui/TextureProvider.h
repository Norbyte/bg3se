////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTUREPROVIDER_H__
#define __GUI_TEXTUREPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

struct Uri;
class Texture;
class RenderDevice;
template<class T> class Ptr;

// Texture metadata returned by TextureProvider::GetTextureInfo()
// The rectangle [x, y, width, height] defines the logical surface used by the texture
// Note that the provider can reuse the same texture with different names and rectangles to
// implementing texture atlas compatible with standard XAML. For example:
//
//  - ItemA ==> Atlas.png [0,   0, 100, 100]
//  - ItemB ==> Atlas.png [0, 100, 100, 200]
//
//  <StackPanel>
//    <Image Source="ItemA.png"/>
//    <Image Source="ItemB.png"/>
//  </StackPanel>
//
// The member 'dpiScale' can be used for scaling (eg: 96 dpi = 1.0, 72 dpi = 0.75 (72/96))
struct TextureInfo
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t x = 0;
    uint32_t y = 0;

    float dpiScale = 1.0f;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for implementing providers of textures
////////////////////////////////////////////////////////////////////////////////////////////////////
class TextureProvider: public BaseComponent
{
public:
    /// Returns metadata for the texture at the given URI or empty rectangle if texture is not found
    virtual TextureInfo GetTextureInfo(const Uri& uri) = 0;

    /// Returns a texture compatible with the given device or null if texture is not found
    virtual Ptr<Texture> LoadTexture(const Uri& uri, RenderDevice* device) = 0;

    /// Delegate to notify changes to the texture file content
    typedef Delegate<void (const Uri&)> TextureChangedDelegate;
    TextureChangedDelegate& TextureChanged() { return mTextureChanged; }
    void RaiseTextureChanged(const Uri& uri) { mTextureChanged(uri); }

private:
    TextureChangedDelegate mTextureChanged;
};

}

#endif
