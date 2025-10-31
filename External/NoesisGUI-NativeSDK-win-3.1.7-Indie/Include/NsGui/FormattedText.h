////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FORMATTEDTEXT_H__
#define __GUI_FORMATTEDTEXT_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsCore/String.h>
#include <NsCore/Vector.h>
#include <NsCore/HashMap.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Freezable.h>
#include <NsGui/IRenderProxyCreator.h>
#include <NsDrawing/Rect.h>


namespace Noesis
{

class UIElement;
class TextBlock;
class Inline;
class Brush;
class FontFamily;
class Font;
class VGLFontFace;
class VGLTextLayout;
struct TextRun;
enum FontWeight: int32_t;
enum FontStretch: int32_t;
enum FontStyle: int32_t;
enum TextDecorations: int32_t;
enum TextAlignment: int32_t;
enum TextWrapping: int32_t;
enum TextTrimming: int32_t;
enum LineStackingStrategy: int32_t;

template<class T> class UICollection;
typedef UICollection<Inline> InlineCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
struct LineInfo
{
    uint32_t numGlyphs;
    float height;
    float baseline;
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides low-level control for drawing text.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.formattedtext.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FormattedText: public BaseComponent, public IRenderProxyCreator
{
public:
    FormattedText();
    ~FormattedText();

    /// Generates and keeps a collection of runs from the supplied InlineCollection and for the
    /// specified font properties
    void BuildTextRuns(const char* text, InlineCollection* inlines, FontFamily* fontFamily,
        FontWeight fontWeight, FontStretch fontStretch, FontStyle fontStyle, float fontSize,
        float strokeThickness, Brush* background, Brush* foreground, Brush* stroke,
        TextDecorations textDecorations, int32_t charSpacing);

    /// Obtains the size of the stored runs for the given constraints
    Size Measure(TextAlignment alignment, TextWrapping wrapping, TextTrimming trimming,
        float maxWidth, float maxHeight, float lineHeight, LineStackingStrategy lineStacking);

    /// Layouts and prepares text for rendering for the given constraints
    void Layout(TextAlignment alignment, TextWrapping wrapping, TextTrimming trimming,
        float maxWidth, float maxHeight, float lineHeight, LineStackingStrategy lineStacking);

    /// Gets text bounds
    Rect GetBounds() const;

    /// Gets x/y coordinates at where the specified glyph is positioned. If the glyph is outside
    /// layout limits then -10 is returned for both x/y coordinates
    void GetGlyphPosition(uint32_t chIndex, bool afterChar, float& x, float& y) const;

    /// Obtains the glyph index under the specified x/y coordinates, indicating if the point is
    /// inside the glyph bounds
    uint32_t HitTest(float x, float y, bool& isInside, bool& isTrailing) const;

    /// Gets the number of lines based on the last layout
    uint32_t GetNumLines() const;

    /// Gets information about the specified line
    const LineInfo& GetLineInfo(uint32_t index) const;

    /// Indicates if this FormattedText has no text
    bool IsEmpty() const;

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    friend class TextBlock;
    friend class TextContainer;

    void SetOwner(TextBlock* owner);

    struct FontFaces
    {
        uint32_t start;
        uint32_t numFaces;
    };

    uint32_t AddRuns(InlineCollection* inlines, FontFamily* fontFamily, FontWeight fontWeight,
        FontStretch fontStretch, FontStyle fontStyle, const FontFaces& fontFaces,
        float fontSize, float strokeThickness, int32_t backgroundIndex,
        int32_t foregroundIndex, int32_t strokeIndex, TextDecorations textDecorations,
        uint32_t numGlyphs, int32_t charSpacing);
    uint32_t AddRun(const char* text, const FontFaces& faces, float size, float strokeThickness,
        int32_t foregroundIndex, int32_t strokeIndex, int32_t backgroundIndex,
        TextDecorations textDecorations, int32_t charSpacing);
    uint32_t AddRun(UIElement* container, uint32_t numGlyphs);
    FontFaces AddFontFace(FontFamily* family, FontWeight weight, FontStretch stretch,
        FontStyle style);
    FontFaces InlineFace(Inline* inl, const FontFaces& currentFaces, FontFamily*& family,
        FontWeight& weight, FontStretch& stretch, FontStyle& style);
    TextDecorations InlineDecorations(TextDecorations currentDecorations, Inline* inl);
    float InlineSize(float currentSize, Inline* inl, const DependencyProperty* dp);
    int32_t InlineSpacing(int32_t currentSpacing, Inline* inl);
    int32_t InlineBrushIndex(int32_t currentBrushIndex, Inline* inl, const DependencyProperty* dp);
    int32_t BrushIndex(Brush* brush);

    void UnregisterBrushes();
    void RegisterBrush(Brush* brush);
    void OnBrushChanged(Freezable* sender, FreezableEventReason reason);

    bool MeasureContainers(float width, float height);
    void ArrangeContainers(float layoutHeight);

    void AddLine(uint32_t numGlyphs, float height, float baseline);

    bool IsTransparent(int32_t brushIndex) const;

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Brushes,
        UpdateFlags_TextLayout,
        UpdateFlags_Invalidate
    };

    TextBlock* mOwner;

    Vector<TextRun> mTextRuns;
    Vector<Ptr<VGLFontFace>> mFontFaces;
    Vector<Ptr<Brush>> mBrushes;

    typedef HashSet<Brush*> RegisteredBrushes;
    RegisteredBrushes mRegisteredBrushes;

    struct ContainerInfo
    {
        Ptr<UIElement> container;
        uint32_t runIndex;
        uint32_t glyphPosition;
    };

    Vector<ContainerInfo> mContainers;

    Ptr<VGLTextLayout> mTextLayout;
    Vector<LineInfo> mLines;
    bool mUseMeasureRuns;

    NS_DECLARE_REFLECTION(FormattedText, BaseComponent)
};

NS_WARNING_POP

}


#endif
