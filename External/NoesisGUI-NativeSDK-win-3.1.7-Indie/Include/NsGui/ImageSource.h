////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IMAGESOURCE_H__
#define __GUI_IMAGESOURCE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Animatable.h>
#include <NsGui/IRenderProxyCreator.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an image object that has width and height.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.imagesource.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ImageSource: public Animatable, public IRenderProxyCreator
{
public:
    ImageSource();
    virtual ~ImageSource() = 0;

    /// Gets the width of the image in measure units (96ths of an inch)
    /// \prop
    float GetWidth() const;

    /// Gets the height of the image in measure units (96ths of an inch)
    /// \prop
    float GetHeight() const;

    // Hides Freezable methods for convenience
    //@{
    Ptr<ImageSource> Clone() const;
    Ptr<ImageSource> CloneCurrentValue() const;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    /// Implemented by inheritors
    //@{
    virtual float GetWidthCore() const = 0;
    virtual float GetHeightCore() const = 0;
    //@}

    NS_DECLARE_REFLECTION(ImageSource, Animatable)
};

NS_WARNING_POP

}


#endif
