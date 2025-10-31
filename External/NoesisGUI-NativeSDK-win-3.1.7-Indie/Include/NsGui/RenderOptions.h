////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RENDEROPTIONS_H__
#define __GUI_RENDEROPTIONS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
enum BitmapScalingMode: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides options for controlling the rendering behavior of objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API RenderOptions
{
    /// Dependency properties
    //@{
    static const DependencyProperty* BitmapScalingModeProperty;
    //@}

    /// Gets the *BitmapScalingMode* for a given DependencyObject
    static BitmapScalingMode GetBitmapScalingMode(const DependencyObject* obj);
    /// Sets the *BitmapScalingMode* for a given DependencyObject
    static void SetBitmapScalingMode(DependencyObject* obj, BitmapScalingMode bitmapScalingMode);

    NS_DECLARE_REFLECTION(RenderOptions, NoParent)
};

}


#endif
