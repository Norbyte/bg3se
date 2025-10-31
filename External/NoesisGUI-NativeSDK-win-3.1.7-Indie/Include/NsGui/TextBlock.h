////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTBLOCK_H__
#define __GUI_TEXTBLOCK_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsCore/Vector.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class FormattedText;
class Brush;
class FontFamily;
class Inline;
class RectangleGeometry;
struct NotifyCollectionChangedEventArgs;
struct Uri;

template<class T> class UICollection;
typedef UICollection<Inline> InlineCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a lightweight control for displaying small amounts of text content.
///
/// Example:
///
/// .. code-block:: xml
///
///     <Grid
///       xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
///       xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
///         <StackPanel HorizontalAlignment="Center" VerticalAlignment="Center">
///             <TextBlock Text="Normal" FontStyle="Normal" FontWeight="Normal"/>
///             <TextBlock Text="Italic" FontStyle="Italic" FontWeight="Normal"/>
///             <TextBlock Text="Bold" FontStyle="Normal" FontWeight="Bold"/>
///             <TextBlock Text="Bold Italic" FontStyle="Italic" FontWeight="Bold"/>
///         </StackPanel>
///     </Grid>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.textblock.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TextBlock: public FrameworkElement
{
public:
    /// Default constructor
    TextBlock();

    /// Constructor
    TextBlock(const char* text);

    /// Destructor
    ~TextBlock();

    /// Gets or sets the Brush used to fill the background of content area.
    //@{
    Brush* GetBackground() const;
    void SetBackground(Brush* brush);
    //@}

    /// Gets or sets the uniform spacing between characters, in units of 1/1000 of an em.
    //@{
    int32_t GetCharacterSpacing() const;
    void SetCharacterSpacing(int32_t spacing);
    //@}

    /// Gets or sets the preferred top-level font family for the TextBlock.
    //@{
    FontFamily* GetFontFamily() const;
    void SetFontFamily(FontFamily* fontFamily);
    //@}

    /// Gets or sets the top-level font size for the TextBlock.
    //@{
    float GetFontSize() const;
    void SetFontSize(float size);
    //@}

    /// Gets or sets the top-level font stretch for the TextBlock.
    //@{
    FontStretch GetFontStretch() const;
    void SetFontStretch(FontStretch stretch);
    //@}

    /// Gets or sets the top-level font style for the TextBlock.
    //@{
    FontStyle GetFontStyle() const;
    void SetFontStyle(FontStyle style);
    //@}

    /// Gets or sets the top-level font weight for the TextBlock.
    //@{
    FontWeight GetFontWeight() const;
    void SetFontWeight(FontWeight weight);
    //@}

    /// Gets or sets the Brush to apply to the text contents of the TextBlock.
    //@{
    Brush* GetForeground() const;
    void SetForeground(Brush* foreground);
    //@}

    /// Gets an InlineCollection containing the top-level Inline elements that comprise the
    /// contents of the TextBlock
    InlineCollection* GetInlines() const;

    /// Indicates if this TextBlock contains any inlines (avoids creating Inlines if not needed)
    bool HasInlines() const;

    /// Gets or sets the height of each line of content
    //@{
    float GetLineHeight() const;
    void SetLineHeight(float value);
    //@}

    /// Gets or sets the mechanism by which a line box is determined for each line of text
    //@{
    LineStackingStrategy GetLineStackingStrategy() const;
    void SetLineStackingStrategy(LineStackingStrategy value);
    //@}

    /// Gets or sets the padding inside a control
    //@{
    const Thickness& GetPadding() const;
    void SetPadding(const Thickness& padding);
    //@}

    /// Gets or sets the Brush used to stroke the text
    //@{
    Brush* GetStroke() const;
    void SetStroke(Brush* stroke);
    //@}

    /// Gets or sets the width (in pixels) of the text stroke
    //@{
    float GetStrokeThickness() const;
    void SetStrokeThickness(float strokeThickness);
    //@}

    /// Gets or sets text alignment
    //@{
    TextAlignment GetTextAlignment() const;
    void SetTextAlignment(TextAlignment align);
    //@}

    /// Gets or sets the text decoration to apply to the text
    //@{
    TextDecorations GetTextDecorations() const;
    void SetTextDecorations(TextDecorations decorations);
    //@}

    /// Gets or sets text string
    //@{
    const char* GetText() const;
    void SetText(const char* text);
    //@}

    /// Gets or sets the text trimming behavior to employ when content overflows the content area
    //@{
    TextTrimming GetTextTrimming() const;
    void SetTextTrimming(TextTrimming trimming);
    //@}

    /// Gets or sets text wrapping
    //@{
    TextWrapping GetTextWrapping() const;
    void SetTextWrapping(TextWrapping textWrap);
    //@}

    /// Returns the object used to layout and render text for this TextBlock
    FormattedText* GetFormattedText() const;

    /// From BaseObject
    //@{
    String ToString() const override;
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* BackgroundProperty;
    static const DependencyProperty* CharacterSpacingProperty;
    static const DependencyProperty* FontFamilyProperty;
    static const DependencyProperty* FontSizeProperty;
    static const DependencyProperty* FontStretchProperty;
    static const DependencyProperty* FontStyleProperty;
    static const DependencyProperty* FontWeightProperty;
    static const DependencyProperty* ForegroundProperty;
    static const DependencyProperty* LineHeightProperty;
    static const DependencyProperty* LineStackingStrategyProperty;
    static const DependencyProperty* PaddingProperty;
    static const DependencyProperty* StrokeProperty;
    static const DependencyProperty* StrokeThicknessProperty;
    static const DependencyProperty* TextAlignmentProperty;
    static const DependencyProperty* TextDecorationsProperty;
    static const DependencyProperty* TextProperty;
    static const DependencyProperty* TextTrimmingProperty;
    static const DependencyProperty* TextWrappingProperty;
    //@}

protected:
    Ptr<FormattedText> mFormattedText;

protected:
    void UpdateTextRuns();
    void UpdateTextLayout(const Size& finalSize);

    void InvalidateTextMeasure();
    void InvalidateTextRuns();
    void InvalidateTextLayout();

    /// From DependencyObject
    //@{
    void OnPostInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From Visual
    //@{
    void OnConnectToViewChildren() override;
    void OnDisconnectFromViewChildren() override;
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    //@}

    /// From UIElement
    //@{
    void OnRender(DrawingContext* context) override;
    HitTestResult HitTestCore(const Point& point) override;
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const override;
    uint32_t GetLogicalChildrenCount() const override;
    Ptr<BaseComponent> GetLogicalChild(uint32_t index) const override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    void OnTextChanged(const char* newText);
    void OnInlinesChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);

    void OnInnerInlinesChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);

    void SyncTextAndInlines();
    void DoSyncTextAndInlines();
    void ConnectInlines();

    void DisconnectInlineContainers();

    void BuildInlinesInfo(FixedString<4096>& text);
    void BuildInlinesInfo(InlineCollection* inlines, FixedString<4096>& text);

    void RegisterInlines(InlineCollection* inlines);
    void UnregisterInlines(InlineCollection* inlines, bool removeLogical);
    void RegisterInline(Inline* inline_);
    void UnregisterInline(Inline* inline_);

    void RegisterInnerInlines(InlineCollection* inlines);
    void UnregisterInnerInlines(InlineCollection* inlines);

    void OnInlineChanged(BaseComponent* sender, const DependencyPropertyChangedEventArgs& e);
    void OnInlineTextChanged();

    void EnsureInlines();
    void EnsureFormattedText();

    struct InlineInfo;
    const InlineInfo* FindInline(uint32_t position) const;

    static void OnFontChanged(const Uri&, const char*, FontWeight, FontStretch, FontStyle);

private:
    friend class TextBlockTest;

    Ptr<InlineCollection> mInlines;
    Ptr<RectangleGeometry> mBackgroundGeometry;
    Size mPreviousConstraint;
    uint32_t mTextFlags;

    struct InlineInfo
    {
        uint32_t start;
        uint32_t end;
        Inline* inl;
    };

    typedef Vector<InlineInfo> InlineInfoList;
    InlineInfoList mInlineInfo;
    Vector<uint32_t> mInlineContainers;

    NS_DECLARE_REFLECTION(TextBlock, FrameworkElement)
};

NS_WARNING_POP

}


#endif
