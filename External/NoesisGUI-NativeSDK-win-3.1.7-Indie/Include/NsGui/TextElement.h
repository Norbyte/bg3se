////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTELEMENT_H__
#define __GUI_TEXTELEMENT_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/FrameworkElement.h>
#include <NsGui/CoreApi.h>
#include <NsCore/String.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class Brush;
class FontFamily;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines text attached properties.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.documents.textelement.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TextElement: public FrameworkElement
{
public:
    /// Gets or sets the Brush used to fill the background of the content area
    //@{
    Brush* GetBackground() const;
    void SetBackground(Brush* background);
    //@}

    /// Gets or sets the uniform spacing between characters, in units of 1/1000 of an em
    //@{
    int32_t GetCharacterSpacing() const;
    void SetCharacterSpacing(int32_t spacing);
    //@}

    /// Gets or sets the preferred top-level font family for the content of the element
    //@{
    FontFamily* GetFontFamily() const;
    void SetFontFamily(FontFamily* family);
    //@}

    /// Gets or sets the font size for the content of the element
    //@{
    float GetFontSize() const;
    void SetFontSize(float size);
    //@}

    /// Gets or sets the font stretct for the content of the element
    //@{
    FontStretch GetFontStretch() const;
    void SetFontStretch(FontStretch stretch);
    //@}

    /// Gets or sets the font style for the content of the element
    //@{
    FontStyle GetFontStyle() const;
    void SetFontStyle(FontStyle style);
    //@}

    /// Gets or sets the top-level font weight for the content of the element
    //@{
    FontWeight GetFontWeight() const;
    void SetFontWeight(FontWeight weight);
    //@}

    /// Gets or sets the Brush to apply to the content of the element
    //@{
    Brush* GetForeground() const;
    void SetForeground(Brush* foreground);
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

    // As ATTACHED PROPERTIES

    /// Gets or sets the preferred top-level font family for the content of the element
    //@{
    static FontFamily* GetFontFamily(const DependencyObject* element);
    static void SetFontFamily(DependencyObject* element, FontFamily* family);
    //@}

    /// Gets or sets the font size for the content of the element
    //@{
    static float GetFontSize(const DependencyObject* element);
    static void SetFontSize(DependencyObject* element, float size);
    //@}

    /// Gets or sets the font stretch for the content of the element
    //@{
    static FontStretch GetFontStretch(const DependencyObject* element);
    static void SetFontStretch(DependencyObject* element, FontStretch stretch);
    //@}

    /// Gets or sets the font style for the content of the element
    //@{
    static FontStyle GetFontStyle(const DependencyObject* element);
    static void SetFontStyle(DependencyObject* element, FontStyle style);
    //@}

    /// Gets or sets the top-level font weight for the content of the element
    //@{
    static FontWeight GetFontWeight(const DependencyObject* element);
    static void SetFontWeight(DependencyObject* element, FontWeight weight);
    //@}

    /// Gets or sets the Brush to apply to the content of the element
    //@{
    static Brush* GetForeground(const DependencyObject* element);
    static void SetForeground(DependencyObject* element, Brush* foreground);
    //@}

    /// Gets or sets the Brush used to stroke the text
    //@{
    static Brush* GetStroke(const DependencyObject* element);
    static void SetStroke(DependencyObject* element, Brush* stroke);
    //@}

    /// Gets or sets the width (in pixels) of the text stroke
    //@{
    static float GetStrokeThickness(const DependencyObject* element);
    static void SetStrokeThickness(DependencyObject* element, float strokeThickness);
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
    static const DependencyProperty* StrokeProperty;
    static const DependencyProperty* StrokeThicknessProperty;
    //@}

    NS_DECLARE_REFLECTION(TextElement, FrameworkElement)
};

}


#endif
