////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __RENDER_RENDERDEVICE_H__
#define __RENDER_RENDERDEVICE_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsCore/BaseComponent.h>
#include <NsRender/RenderDeviceApi.h>


namespace Noesis
{

class RenderTarget;
class Texture;

// Texture formats enumeration
struct TextureFormat
{
    enum Enum
    {
        // A four-component format that supports 8 bits per channel including alpha
        RGBA8,
        // A four-component format that supports 8 bits for each color channel and 8 bits unused
        RGBX8,
        // A single-component format that supports 8 bits for the red channel
        R8,

        Count
    };
};

// Render device capabilities
struct DeviceCaps
{
    // Offset in pixel units from top-left corner to center of pixel
    float centerPixelOffset = 0.0f;

    // When this flag is enabled the device works in 'Linear' mode. All internal textures and
    // offscreens are created in 'sRGB' format. In this mode, the device also expects colors
    // (like the ones in the vertex buffer) in 'sRGB' format. It also indicates that the device
    // writes to the render target in linear space
    bool linearRendering = false;

    // This flag is enabled to indicate that the device supports LCD subpixel rendering. Extra
    // shaders and dual source blending are needed for this feature
    bool subpixelRendering = false;
};

NS_WARNING_PUSH
NS_CLANG_WARNING_DISABLE("-Wshadow")

// Shader effects
struct Shader
{
    // This is the high-level shader description that gets sent in each draw call.
    // Implementations must map this to corresponding low-level shaders and layouts
    enum Enum
    {
        // Shader for debug modes
        RGBA,

        // Stencil-only rendering for masks
        Mask,

        // Shader used for clearing render target
        Clear,

        // Shaders for rendering geometry paths without PPAA
        Path_Solid,
        Path_Linear,
        Path_Radial,
        Path_Pattern,
        Path_Pattern_Clamp,
        Path_Pattern_Repeat,
        Path_Pattern_MirrorU,
        Path_Pattern_MirrorV,
        Path_Pattern_Mirror,

        // Shaders for rendering geometry paths with PPAA
        Path_AA_Solid,
        Path_AA_Linear,
        Path_AA_Radial,
        Path_AA_Pattern,
        Path_AA_Pattern_Clamp,
        Path_AA_Pattern_Repeat,
        Path_AA_Pattern_MirrorU,
        Path_AA_Pattern_MirrorV,
        Path_AA_Pattern_Mirror,

        // Shaders for rendering distance fields
        SDF_Solid,
        SDF_Linear,
        SDF_Radial,
        SDF_Pattern,
        SDF_Pattern_Clamp,
        SDF_Pattern_Repeat,
        SDF_Pattern_MirrorU,
        SDF_Pattern_MirrorV,
        SDF_Pattern_Mirror,

        // Shaders for rendering distance fields with subpixel rendering.
        // These shaders are only used when the device reports support for
        // subpixel rendering in DeviceCaps. Otherwise SDF_* shaders are used
        SDF_LCD_Solid,
        SDF_LCD_Linear,
        SDF_LCD_Radial,
        SDF_LCD_Pattern,
        SDF_LCD_Pattern_Clamp,
        SDF_LCD_Pattern_Repeat,
        SDF_LCD_Pattern_MirrorU,
        SDF_LCD_Pattern_MirrorV,
        SDF_LCD_Pattern_Mirror,

        // Shaders for offscreen rendering
        Opacity_Solid,
        Opacity_Linear,
        Opacity_Radial,
        Opacity_Pattern,
        Opacity_Pattern_Clamp,
        Opacity_Pattern_Repeat,
        Opacity_Pattern_MirrorU,
        Opacity_Pattern_MirrorV,
        Opacity_Pattern_Mirror,

        Upsample,
        Downsample,

        Shadow,
        Blur,
        Custom_Effect,

        Count
    };

    uint8_t v;

    // Vertex shaders for each Noesis::Shader (see table VertexForShader[])
    struct Vertex
    {
        enum Enum
        {
            Pos,
            PosColor,
            PosTex0,
            PosTex0Rect,
            PosTex0RectTile,
            PosColorCoverage,
            PosTex0Coverage,
            PosTex0CoverageRect,
            PosTex0CoverageRectTile,
            PosColorTex1_SDF,
            PosTex0Tex1_SDF,
            PosTex0Tex1Rect_SDF,
            PosTex0Tex1RectTile_SDF,
            PosColorTex1,
            PosTex0Tex1,
            PosTex0Tex1Rect,
            PosTex0Tex1RectTile,
            PosColorTex0Tex1,
            PosTex0Tex1_Downsample,
            PosColorTex1Rect,
            PosColorTex0RectImagePos,

            Count
        };

        // Vertex Formats for each vertex shader (see table FormatForVertex[])
        struct Format
        {
            enum Enum
            {
                Pos,
                PosColor,
                PosTex0,
                PosTex0Rect,
                PosTex0RectTile,
                PosColorCoverage,
                PosTex0Coverage,
                PosTex0CoverageRect,
                PosTex0CoverageRectTile,
                PosColorTex1,
                PosTex0Tex1,
                PosTex0Tex1Rect,
                PosTex0Tex1RectTile,
                PosColorTex0Tex1,
                PosColorTex1Rect,
                PosColorTex0RectImagePos,

                Count
            };

            // Attributes for each vertex format (see tables AttributesForFormat[] and SizeForFormat[])
            struct Attr
            {
                enum Enum
                {
                //  --------------------------------------------------------------------------------
                //  Attr                  Interpolation      Semantic
                //  --------------------------------------------------------------------------------
                    Pos,             //   linear             Position (xy)
                    Color,           //   nointerpolation    sRGB Color (rgba)
                    Tex0,            //   linear             TexCoord0 (uv)
                    Tex1,            //   linear             TexCoord1 (uv)
                    Coverage,        //   linear             Coverage (alpha)
                    Rect,            //   nointerpolation    Rect (x0, y0, x1, y1)
                    Tile,            //   nointerpolation    Rect (x, y, width, height)
                    ImagePos,        //   linear             Position (xy) - Scale(zw)

                    Count
                };

                // Types for each vertex format attribute (see tables TypeForAttr[] and SizeForType[])
                struct Type
                {
                    enum Enum
                    {
                        // One 32-bit floating-point component
                        Float,
                        // Two 32-bit floating-point components
                        Float2,
                        // Four 32-bit floating-point components
                        Float4,
                        // Four 8-bit normalized unsigned integer components
                        UByte4Norm,
                        // Four 16-bit normalized unsigned integer components
                        UShort4Norm,

                        Count
                    };
                };
            };
        };
    };
};

NS_WARNING_POP

// Table for getting the vertex shader corresponding to each Noesis::Shader
// Noesis::Shader => Noesis::Shader::Vertex
static constexpr const uint8_t VertexForShader[Noesis::Shader::Count] =
{
    0, 0, 0, 1, 2, 2, 2, 3, 4, 4, 4, 4, 5, 6, 6, 6, 7, 8, 8, 8, 8, 9, 10, 10, 10, 11, 12, 12, 12,
    12, 9, 10, 10, 10, 11, 12, 12, 12, 12, 13, 14, 14, 14, 15, 16, 16, 16, 16, 17, 18, 19, 13, 20
};

// Table for getting the vertex format corresponding to each vertex shader
// Noesis::Shader::Vertex => Noesis::Shader::Vertex::Format
static constexpr const uint8_t FormatForVertex[Noesis::Shader::Vertex::Count] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 9, 10, 11, 12, 13, 10, 14, 15
};

// Size in bytes for each vertex format
static constexpr const uint8_t SizeForFormat[Noesis::Shader::Vertex::Format::Count] = 
{
    8, 12, 16, 24, 40, 16, 20, 28, 44, 20, 24, 32, 48, 28, 28, 44
};

// Table for getting the attribute bitmask corresponding to each vertex format
static constexpr const uint8_t AttributesForFormat[Noesis::Shader::Vertex::Format::Count] =
{
    1, 3, 5, 37, 101, 19, 21, 53, 117, 11, 13, 45, 109, 15, 43, 167
};

// Table for getting the type corresponding to each vertex format attribute
// Noesis::Shader::Vertex::Format::Attr => Noesis::Shader::Vertex::Format::Attr::Type
static constexpr const uint8_t TypeForAttr[Noesis::Shader::Vertex::Format::Attr::Count] =
{
    1, 3, 1, 1, 0, 4, 2, 2
};

// Table for getting the size in bytes corresponding to each vertex format attribute type
static constexpr const uint8_t SizeForType[Noesis::Shader::Vertex::Format::Attr::Type::Count]
{
    4, 8, 16, 4, 8
};

// Alpha blending mode
struct BlendMode
{
    enum Enum
    {
                                 //  ----------------------------------------------------
                                 //  COLOR                       ALPHA
                                 //  ----------------------------------------------------
        Src,                     //  cs                          as
        SrcOver,                 //  cs + cd * (1 - as)          as + ad * (1 - as)
        SrcOver_Multiply,        //  cs * cd + cd * (1 - as)     as + ad * (1 - as)
        SrcOver_Screen,          //  cs + cd * (1 - cs)          as + ad * (1 - as)
        SrcOver_Additive,        //  cs + cs                     as + ad * (1 - as)
        SrcOver_Dual,            //  cs + cd * (1 - as) [RGB]    as + ad * (1 - as) [RGB]
                                 //  ----------------------------------------------------

        Count
    };
};

// Stencil buffer mode
struct StencilMode
{
    enum Enum
    {
        // Stencil testing disabled
        Disabled,
        // Stencil testing enabled
        Equal_Keep,
        // Stencil enabled and incremented
        Equal_Incr,
        // Stencil enabled and decremented
        Equal_Decr,
        // Set the stencil data to 0
        Clear,

        Count
    };
};

// Render state
union RenderState
{
    struct
    {
        uint8_t colorEnable:1;
        uint8_t blendMode:3;
        uint8_t stencilMode:3;
        uint8_t wireframe:1;
    } f;

    uint8_t v;
};

// Texture wrapping mode
struct WrapMode
{
    enum Enum
    {
        // Clamp between 0.0 and 1.0
        ClampToEdge,
        // Out of range texture coordinates return transparent zero (0,0,0,0)
        ClampToZero,
        // Wrap to the other side of the texture
        Repeat,
        // The same as repeat but flipping horizontally
        MirrorU,
        // The same as repeat but flipping vertically
        MirrorV,
        // The combination of MirrorU and MirrorV
        Mirror,

        Count
    };
};

// Texture minification and magnification filter
struct MinMagFilter
{
    enum Enum
    {
        // Select the single pixel nearest to the sample point
        Nearest,
        // Select two pixels in each dimension and interpolate linearly between them
        Linear,

        Count
    };
};

// Texture Mipmap filter
struct MipFilter
{
    enum Enum
    {
        // Texture sampled from mipmap level 0
        Disabled,
        // The nearest mipmap level is selected
        Nearest,
        // Both nearest levels are sampled and linearly interpolated
        Linear,

        Count
    };
};

// Texture sampler state
union SamplerState
{
    struct
    {
        uint8_t wrapMode:3;
        uint8_t minmagFilter:1;
        uint8_t mipFilter:2;
        uint8_t unused:2;
    } f;

    uint8_t v;
};

// Uniform shader values. For unused buffers, 'values' is set to null and 'numDwords' is zero.
// A hash for the content is given to avoid unnecessary constant buffer updates
struct UniformData
{
    const void* values;
    uint32_t numDwords;
    uint32_t hash;
};

// Render batch information
struct Batch
{
    // Render state
    Shader shader;
    RenderState renderState;
    uint8_t stencilRef;

    // Information about the list of indexed triangles to be rendered. The offset in bytes to the
    // last pointer returned by MapVertices() is stored in 'vertexOffset'. Indices are 16 bits each
    // and they start at the last pointer returned by MapIndices() plus 'startIndex' x 2 bytes
    uint32_t vertexOffset;
    uint32_t numVertices;
    uint32_t startIndex;
    uint32_t numIndices;

    // Textures. Unused textures are set to null
    Texture* pattern;
    Texture* ramps;
    Texture* image;
    Texture* glyphs;
    Texture* shadow;

    // Sampler states corresponding to each texture
    SamplerState patternSampler;
    SamplerState rampsSampler;
    SamplerState imageSampler;
    SamplerState glyphsSampler;
    SamplerState shadowSampler;

    // Vertex and pixel shader uniform buffers
    UniformData vertexUniforms[2];
    UniformData pixelUniforms[2];

    // Custom pointer set in ShaderEffect::SetPixelShader() or BrushShader::SetPixelShader()
    void* pixelShader;
};

// A region of the render target with origin located at the lower left corner
struct Tile
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

// Max requested bytes for MapVertices
#ifndef DYNAMIC_VB_SIZE
    #define DYNAMIC_VB_SIZE 512 * 1024
#endif

// Max requested bytes for MapIndices
#ifndef DYNAMIC_IB_SIZE
    #define DYNAMIC_IB_SIZE 128 * 1024
#endif

// Max requested bytes for UpdateTexture
#ifndef DYNAMIC_TEX_SIZE
    #define DYNAMIC_TEX_SIZE 128 * 1024
#endif

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstraction of a graphics rendering device
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_RENDER_RENDERDEVICE_API RenderDevice: public BaseComponent
{
public:
    RenderDevice();
    RenderDevice(const RenderDevice&) = delete;
    RenderDevice& operator=(const RenderDevice&) = delete;
    ~RenderDevice();

    /// Retrieves device render capabilities
    virtual const DeviceCaps& GetCaps() const = 0;

    /// Creates render target surface with given dimensions, samples and optional stencil buffer
    virtual Ptr<RenderTarget> CreateRenderTarget(const char* label, uint32_t width, uint32_t height,
        uint32_t sampleCount, bool needsStencil) = 0;

    /// Creates render target sharing transient (stencil, colorAA) buffers with the given surface
    virtual Ptr<RenderTarget> CloneRenderTarget(const char* label, RenderTarget* surface) = 0;

    /// Creates texture with given dimensions and format. For immutable textures, the content of
    /// each mipmap is given in 'data'. The passed data is tightly packed (no extra pitch). When
    /// 'data' is null the texture is considered dynamic and will be updated using UpdateTexture()
    virtual Ptr<Texture> CreateTexture(const char* label, uint32_t width, uint32_t height,
        uint32_t numLevels, TextureFormat::Enum format, const void** data) = 0;

    /// Updates texture mipmap copying the given data to desired position. The passed data is
    /// tightly packed (no extra pitch) and is never greater than DYNAMIC_TEX_SIZE bytes.
    /// Origin is located at the left of the first scanline
    virtual void UpdateTexture(Texture* texture, uint32_t level, uint32_t x, uint32_t y,
        uint32_t width, uint32_t height, const void* data) = 0;

    /// Begins rendering offscreen commands
    virtual void BeginOffscreenRender() = 0;

    /// Ends rendering offscreen commands
    virtual void EndOffscreenRender() = 0;

    /// Begins rendering onscreen commands
    virtual void BeginOnscreenRender() = 0;

    /// Ends rendering onscreen commands
    virtual void EndOnscreenRender() = 0;

    /// Binds render target and sets viewport to cover the entire surface. The existing contents of
    /// the surface are discarded and replaced with arbitrary data. Surface is not cleared
    virtual void SetRenderTarget(RenderTarget* surface) = 0;

    /// Resolves multisample render target. Transient surfaces (stencil, colorAA) are discarded.
    /// Only the specified list of surface regions are resolved
    virtual void ResolveRenderTarget(RenderTarget* surface, const Tile* tiles, uint32_t numTiles) = 0;

    /// Gets a pointer to stream vertices (bytes <= DYNAMIC_VB_SIZE)
    virtual void* MapVertices(uint32_t bytes) = 0;

    /// Invalidates the pointer previously mapped
    virtual void UnmapVertices() = 0;

    /// Gets a pointer to stream 16-bit indices (bytes <= DYNAMIC_IB_SIZE)
    virtual void* MapIndices(uint32_t bytes) = 0;

    /// Invalidates the pointer previously mapped
    virtual void UnmapIndices() = 0;

    /// Draws primitives for the given batch
    virtual void DrawBatch(const Batch& batch) = 0;

    /// Width of offscreen textures (0 = automatic). Default is automatic
    void SetOffscreenWidth(uint32_t width);
    uint32_t GetOffscreenWidth() const;

    /// Height of offscreen textures (0 = automatic). Default is automatic
    void SetOffscreenHeight(uint32_t height);
    uint32_t GetOffscreenHeight() const;

    /// Multisampling of offscreen textures. Default is 1x
    void SetOffscreenSampleCount(uint32_t sampleCount);
    uint32_t GetOffscreenSampleCount() const;

    /// Number of offscreen textures created at startup. Default is 0
    void SetOffscreenDefaultNumSurfaces(uint32_t numSurfaces);
    uint32_t GetOffscreenDefaultNumSurfaces() const;

    /// Maximum number of offscreen textures (0 = unlimited). Default is unlimited
    void SetOffscreenMaxNumSurfaces(uint32_t numSurfaces);
    uint32_t GetOffscreenMaxNumSurfaces() const;

    /// Width of texture used to cache glyphs. Default is 1024
    void SetGlyphCacheWidth(uint32_t width);
    uint32_t GetGlyphCacheWidth() const;

    /// Height of texture used to cache glyphs. Default is 1024
    void SetGlyphCacheHeight(uint32_t height);
    uint32_t GetGlyphCacheHeight() const;

    /// Vector graphics context
    void SetVGContext(BaseComponent* context);
    BaseComponent* GetVGContext() const;

protected:
    /// Returns whether the passed render state is compatible with the given shader
    bool IsValidState(Shader shader, RenderState state) const;

    int32_t OnDestroy() override;

private:
    uint32_t mOffscreenWidth;
    uint32_t mOffscreenHeight;
    uint32_t mOffscreenSampleCount;
    uint32_t mOffscreenDefaultNumSurfaces;
    uint32_t mOffscreenMaxNumSurfaces;
    uint32_t mGlyphCacheWidth;
    uint32_t mGlyphCacheHeight;

    Ptr<BaseComponent> mVGContext;

    NS_DECLARE_REFLECTION(RenderDevice, BaseComponent)
};

NS_WARNING_POP

}

#endif
