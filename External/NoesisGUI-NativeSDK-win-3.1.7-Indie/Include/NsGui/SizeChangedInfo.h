////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SIZECHANGEDINFO_H__
#define __GUI_SIZECHANGEDINFO_H__


#include <NsCore/Noesis.h>
#include <NsDrawing/Size.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Reports a size change. It is used as a parameter in OnRenderSizeChanged overrides.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SizeChangedInfo
{
    /// Gets the new size of the object
    Size newSize;
    /// Gets the previous size of the object
    Size previousSize;
    /// Gets a value that indicates whether the *Width* component of the Size changed
    bool widthChanged;
    /// Gets a value that indicates whether the *Height* component of the Size changed
    bool heightChanged;
};

}

#endif
