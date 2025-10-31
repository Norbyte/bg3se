////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IRENDERER_H__
#define __GUI_IRENDERER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

class RenderDevice;
class Matrix4;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Renders graphics on screen outputting commands to the render device abstraction. Multithreaded
/// rendering is possible by using this interface in a render thread. The render thread can either
/// operate in lock-step or independently of the main thread.
///
/// For performance reasons, various device states, such as blending modes and current rendertarget
/// are not preserved and the state of the device will be different after calling:
///
///  - Init
///  - Shutdown
///  - RenderOffscreen
///  - Render
///
/// Some applications may be adversely affected by this. The most straightforward solution is to
/// save device state before calling those functions and restore it afterwards. Greater performance
/// can be achieved by having the game engine re-initialize its required states after UI rendering.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IRenderer: public Interface
{
    /// Initializes the renderer with the given render device abstraction
    virtual void Init(RenderDevice* device) = 0;

    /// Free allocated render resources and render tree
    virtual void Shutdown() = 0;

    /// Determines the visible region. By default it is set to cover the view dimensions
    virtual void SetRenderRegion(float x, float y, float width, float height) = 0;

    /// Grabs the most recent snapshot captured by IView::Update()
    /// Returns false if no new snapshots were found
    virtual bool UpdateRenderTree() = 0;

    /// Updates internal offscreen textures needed by global opacity and shader effects. It is
    /// recommended, specially on tiled architectures, to invoke this function before binding the
    /// main render target to avoid unnecessary render target changes that may affect performance.
    /// Returns 'false' when no textures are updates and restoring the device state is not necessary
    virtual bool RenderOffscreen() = 0;

    /// RenderOffscreen overload with custom view projection matrix. Can be used for example to
    /// render each eye in VR
    virtual bool RenderOffscreen(const Matrix4& projection) = 0;

    /// Renders UI into the current render target and viewport
    virtual void Render(bool flipY = false, bool clear = false) = 0;

    /// Renders UI into the current render target and viewport. This function overrides the default
    /// view projection matrix. Can be used for example to render each eye in VR
    virtual void Render(const Matrix4& projection, bool flipY = false, bool clear = false) = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IRenderer, Interface)
};

}

#endif
