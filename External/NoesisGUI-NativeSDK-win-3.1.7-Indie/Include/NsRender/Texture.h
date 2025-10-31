////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_TEXTURE_H__
#define __RENDER_TEXTURE_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsRender/RenderDeviceApi.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for 2D textures
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_RENDER_RENDERDEVICE_API Texture: public BaseComponent
{
public:
    /// Returns the width of the texture, in pixels
    virtual uint32_t GetWidth() const = 0;

    /// Returns the height of the texture, in pixels
    virtual uint32_t GetHeight() const = 0;

    /// Returns true if the texture has mipmaps
    virtual bool HasMipMaps() const = 0;

    /// Returns true when texture must be vertically inverted when mapped. This is true for surfaces
    /// on platforms (OpenGL) where texture V coordinate is zero at the "bottom of the texture"
    virtual bool IsInverted() const = 0;

    /// Returns true if the texture has an alpha channel that is not completely white. Just a hint
    /// to optimize rendering as alpha blending can be disabled when there is no transparency.
    virtual bool HasAlpha() const = 0;

    /// Stores custom private data
    void SetPrivateData(BaseComponent* data);

private:
    Ptr<BaseComponent> mData;

    NS_DECLARE_REFLECTION(Texture, BaseComponent)
};

NS_WARNING_POP

}

#endif
