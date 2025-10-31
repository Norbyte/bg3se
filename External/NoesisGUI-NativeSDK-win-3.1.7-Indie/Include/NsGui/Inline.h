////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INLINE_H__
#define __GUI_INLINE_H__


#include <NsCore/Noesis.h>
#include <NsGui/TextElement.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class Inline;
struct NotifyCollectionChangedEventArgs;

template<class T> class UICollection;
typedef UICollection<Inline> InlineCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An abstract class that provides a base for all inline flow content elements.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.documents.inline.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Inline: public TextElement
{
public:
    Inline();

    /// A collection of Inlines that are siblings to this element. May return null if an element
    /// is not inserted into any tree.
    InlineCollection* GetSiblingInlines() const;

    /// Returns an Inline immediately preceding this one on the same level of siblings
    Inline* GetPreviousInline() const;

    /// Returns an Inline immediately following this one on the same level of siblings
    Inline* GetNextInline() const;

    /// Gets or sets the text decoration to apply to the element
    //@{
    TextDecorations GetTextDecorations() const;
    void SetTextDecorations(TextDecorations decorations);
    //@}

public:
    static const DependencyProperty* TextDecorationsProperty;

protected:
    // From FrameworkElement
    //@{
    void OnLogicalParentChanged(FrameworkElement* oldParent) override;
    //@}

private:
    static void UpdateInlines(InlineCollection* inlines, const NotifyCollectionChangedEventArgs& e);

private:
    friend class TextBlock;
    friend class Span;

    InlineCollection* mSiblingInlines;
    Inline* mPreviousInline;
    Inline* mNextInline;

    NS_DECLARE_REFLECTION(Inline, TextElement)
};

}


#endif
