////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ADORNER_H__
#define __GUI_ADORNER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class Adorner;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Abstract class that represents a FrameworkElement that decorates another element.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.documents.adorner
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Adorner: public FrameworkElement
{
public:
    Adorner(UIElement* adornedElement);
    ~Adorner();

    /// Gets the element that this adorner is bound to
    UIElement* GetAdornedElement() const;

    /// Gets or sets a value that indicates whether clipping of the adorner is enabled
    //@{
    bool GetIsClipEnabled() const;
    void SetIsClipEnabled(bool value);
    //@}

    /// Returns a transform for the adorner, based on the transform that is currently applied to the
    /// adorned element. This method is called by AdornerLayer to allow the adorner to filter out
    /// the transforms it doesn't want and return a new transform with just the transforms it wants
    /// applied. An adorner can also add an additional translation transform at this time, allowing
    /// it to be positioned somewhere other than the upper left corner of its adorned element
    virtual Matrix4 GetDesiredTransform(const Matrix4& transform) const;

protected:
    // For internal use only
    //@{
    Adorner();
    void SetAdornedElement(UIElement* adornedElement);
    //@}

    /// From UIElement
    //@{
    Size MeasureOverride(const Size& availableSize) override;
    Ptr<Geometry> GetLayoutClip(const Size& layoutSlotSize) const override;
    //@}

private:
    friend class AdornerLayer;

    UIElement* mAdornedElement;
    bool mIsClipEnabled;

    Size mElementSize;
    Matrix4 mDesiredTransform;

    NS_DECLARE_REFLECTION(Adorner, FrameworkElement);
};

NS_WARNING_POP

}

#endif
