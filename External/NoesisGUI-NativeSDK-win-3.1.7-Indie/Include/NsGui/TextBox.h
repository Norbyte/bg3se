////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTBOX_H__
#define __GUI_TEXTBOX_H__


#include <NsCore/Noesis.h>
#include <NsGui/BaseTextBox.h>
#include <NsGui/CompositionUnderline.h>
#include <NsCore/Vector.h>


namespace Noesis
{

class ScrollViewer;
class TextBoxTextContainer;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that can be used to display or edit unformatted text.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.textbox.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TextBox: public BaseTextBox
{
public:
    TextBox();
    ~TextBox();

    /// Gets or sets the insertion position index of the caret
    //@{
    int32_t GetCaretIndex() const;
    void SetCaretIndex(int32_t caretIndex);
    //@}

    /// Gets or sets the maximum number of characters that can be manually entered into the text box
    //@{
    int32_t GetMaxLength() const;
    void SetMaxLength(int32_t length);
    //@}

    /// Gets or sets the maximum number of visible lines
    //@{
    int32_t GetMaxLines() const;
    void SetMaxLines(int32_t lines);
    //@}

    /// Gets or sets the minimum number of visible lines
    //@{
    int32_t GetMinLines() const;
    void SetMinLines(int32_t lines);
    //@}

    /// Gets or sets the placeholder text
    //@{
    const char* GetPlaceholder() const;
    void SetPlaceholder(const char* text);
    //@}

    /// Gets or sets the content of the current selection in the text box
    /// \remarks Returned string must be copied becuase it will be invalidated next time you
    /// interact with the text box
    /// \prop
    //@{
    const char* GetSelectedText() const;
    void SetSelectedText(const char* text);
    //@}

    /// Gets or sets a value indicating the number of characters in the current selection in the
    /// text box
    /// \prop
    //@{
    int32_t GetSelectionLength() const;
    void SetSelectionLength(int32_t len);
    //@}

    /// Gets or sets a character index for the beginning of the current selection
    /// \prop
    //@{
    int32_t GetSelectionStart() const;
    void SetSelectionStart(int32_t pos);
    //@}

    /// Selects all text
    void SelectAll();

    /// Select the text in the given position and length
    void Select(int32_t start, int32_t length);

    /// Clear all the content in the TextBox control
    void Clear();

    /// Return the 0-based character index of the given point.  If there is no character
    /// at that point and snapToText is false, return -1. Point is specified in TextBox coordinates
    int32_t GetCharacterIndexFromPoint(Point point, bool snapToText) const;

    /// Return the 0-based character index of the first character of lineIndex, or -1 if no layout
    /// information is available
    int32_t GetCharacterIndexFromLineIndex(int32_t lineIndex) const;

    /// Return the 0-based index of the line containing the given character index, or -1 if no
    /// layout information is available
    int32_t GetLineIndexFromCharacterIndex(int32_t charIndex) const;

    /// Return the number of characters in the given line, or -1 if no
    /// layout information is available
    int32_t GetLineLength(int32_t lineIndex) const;

    /// Return the index of the first line that is currently visible in the TextBox, or -1 if no
    /// layout information is available
    int32_t GetFirstVisibleLineIndex() const;

    /// Return the index of the last line that is currently visible in the TextBox, or -1 if no
    /// layout information is available
    int32_t GetLastVisibleLineIndex() const;

    /// Scroll the minimal amount necessary to bring the given line into full view.
    void ScrollToLine(int32_t lineIndex);

    /// Gets or sets the horizontal alignment of the contents of the text box
    //@{
    TextAlignment GetTextAlignment() const;
    void SetTextAlignment(TextAlignment textAlign);
    //@}

    /// Gets or sets the text contents of the text box
    //@{
    const char* GetText() const;
    void SetText(const char* text);
    //@}

    /// Gets or sets how the text box should wrap text
    //@{
    TextWrapping GetTextWrapping() const;
    void SetTextWrapping(TextWrapping textWrap);
    //@}

    /// Gets the visual that renders the text of the TextBox
    Visual* GetTextView() const;

    /// Calculates the bounding box of a range of text
    Rect GetRangeBounds(uint32_t start, uint32_t end) const;

    /// Removes the caret until control gets focused again
    void HideCaret();

    /// Returns the number of IME composition underlines
    uint32_t GetNumCompositionUnderlines() const;

    /// Gets the specified IME composition underline
    const CompositionUnderline& GetCompositionUnderline(uint32_t index) const;

    /// Adds a new IME composition underline
    void AddCompositionUnderline(const CompositionUnderline& compositionUnderline);

    /// Removes all IME composition underlines
    void ClearCompositionUnderlines();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* MaxLengthProperty;
    static const DependencyProperty* MaxLinesProperty;
    static const DependencyProperty* MinLinesProperty;
    static const DependencyProperty* PlaceholderProperty;
    static const DependencyProperty* TextAlignmentProperty;
    static const DependencyProperty* TextProperty;
    static const DependencyProperty* TextWrappingProperty;
    //@}

protected:
    // From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    // From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnTextInput(const TextCompositionEventArgs& e) override;
    void OnGotKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnMouseRightButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseMove(const MouseEventArgs& e) override;
    void OnTouchDown(const TouchEventArgs& e) override;
    void OnTapped(const TappedEventArgs& e) override;
    void OnDoubleTapped(const DoubleTappedEventArgs& e) override;
    //@}

    // From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    //@}

    // From Control
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    void OnFontFamilyChanged(FontFamily* fontFamily) override;
    void OnFontSizeChanged(float fontSize) override;
    void OnFontStretchChanged(FontStretch fontStretch) override;
    void OnFontStyleChanged(FontStyle fontStyle) override;
    void OnFontWeightChanged(FontWeight fontWeight) override;
    void OnForegroundChanged(Brush* brush) override;
    void OnMouseDoubleClick(const MouseButtonEventArgs& e) override;
    void OnIsFocusEngagedChanged(bool engaged) override;
    //@}

    // From BaseTextBox
    //@{
    void OnCaretBrushChanged(Brush* oldBrush, Brush* newBrush) override;
    void OnSelectionBrushChanged(Brush* oldBrush, Brush* newBrush) override;
    void OnSelectionOpacityChanged(float oldOpacity, float newOpacity) override;
    void OnIsReadOnlyChanged(bool oldValue, bool newValue) override;
    void DoLineUp(ScrollViewer* scrollViewer) override;
    void DoLineDown(ScrollViewer* scrollViewer) override;
    ScrollViewer* GetScrollViewer() const override;
    //@}

private:
    friend class ComboBox;

    void UpdateContentHostHeight();
    void UpdateContentHostHeight(float padding);

    void EnableFocus();
    void DisableFocus();

private:
    Ptr<FrameworkElement> mContentHost;
    TextBoxTextContainer* mTextContainer;

    typedef Vector<CompositionUnderline> CompositionUnderlines;
    CompositionUnderlines mCompositionUnderlines;

    union
    {
        mutable struct
        {
            bool mouseDown : 1;
            bool updateContentHostMinMax : 1;
        } mFlags;

        // To quickly set all flags to 0
        mutable uint8_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(TextBox, BaseTextBox)
};

NS_WARNING_POP

}


#endif
