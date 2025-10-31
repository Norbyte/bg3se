////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IMAGE_H__
#define __GUI_IMAGE_H__


#include <NsCore/Noesis.h>
#include <NsGui/FrameworkElement.h>


namespace Noesis
{

class ImageSource;
enum Stretch: int32_t;
enum StretchDirection: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that displays an image.
///
/// Example:
///
/// .. code-block:: xml
///
///     <Image Source="Images/melon1.png"/>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.image.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Image: public FrameworkElement
{
public:
    Image();
    ~Image();

    /// Gets or sets image source
    //@{
    ImageSource* GetSource() const;
    void SetSource(ImageSource* source);
    //@}

    /// Gets or sets a value that describes how an Image should be stretched to fill the
    /// destination rectangle
    //@{
    Stretch GetStretch() const;
    void SetStretch(Stretch stretch);
    //@}

    /// Gets or sets a value that indicates how the image is scaled
    //@{
    StretchDirection GetStretchDirection() const;
    void SetStretchDirection(StretchDirection stretchDirection);
    //@}

    // Invalidates image for render (to allow texture updates)
    void InvalidateImage();

protected:
    /// From FrameworkElement
    //@{
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    void OnRender(DrawingContext* context);
    //@}

private:
    Size CalculateFinalSize(const Size& availableSize) const;

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* SourceProperty;
    static const DependencyProperty* StretchProperty;
    static const DependencyProperty* StretchDirectionProperty;
    //@}

    NS_DECLARE_REFLECTION(Image, FrameworkElement)
};

}


#endif
