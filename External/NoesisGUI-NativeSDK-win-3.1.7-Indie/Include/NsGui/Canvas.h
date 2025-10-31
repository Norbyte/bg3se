////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CANVAS_H__
#define __GUI_CANVAS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Panel.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines an area within which you can explicitly position child elements by using
/// coordinates that are relative to the Canvas area.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.canvas.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Canvas: public Panel
{
public:
    Canvas();

    /// Gets a value that represents the distance between the left side of an element and
    /// the left side of its parent Canvas
    static float GetLeft(UIElement* element);
    /// Sets a value that represents the distance between the left side of an element and
    /// the left side of its parent Canvas
    static void SetLeft(UIElement* element, float left);

    /// Gets a value that represents the distance between the top of an element and the top
    /// of its parent Canvas
    static float GetTop(UIElement* element);
    /// Sets a value that represents the distance between the top of an element and the top
    /// of its parent Canvas
    static void SetTop(UIElement* element, float top);

    /// Gets a value that represents the distance between the right side of an element and
    /// the right side of its parent Canvas
    static float GetRight(UIElement* element);
    /// Sets a value that represents the distance between the right side of an element and
    /// the right side of its parent Canvas
    static void SetRight(UIElement* element, float right);

    /// Gets a value that represents the distance between the bottom of an element and the
    /// bottom of its parent Canvas
    static float GetBottom(UIElement* element);
    /// Sets a value that represents the distance between the bottom of an element and the
    /// bottom of its parent Canvas
    static void SetBottom(UIElement* element, float bottom);

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* BottomProperty; // attached property
    static const DependencyProperty* LeftProperty; // attached property
    static const DependencyProperty* RightProperty; // attached property
    static const DependencyProperty* TopProperty; // attached property
    //@}

protected:
    /// From UIElement
    //@{
    Ptr<Geometry> GetLayoutClip(const Size& layoutSlotSize) const override;
    //@}

    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

    NS_DECLARE_REFLECTION(Canvas, Panel)
};

}

#endif
