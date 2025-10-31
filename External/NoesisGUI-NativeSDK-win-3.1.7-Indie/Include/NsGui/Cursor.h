////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CURSOR_H__
#define __GUI_CURSOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/String.h>
#include <NsCore/BaseComponent.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

// Specifies the built in cursor types
enum CursorType: int32_t
{
    // A value indicating that no cursor should be displayed
    CursorType_None,

    // No cursor
    CursorType_No,

    // A standard arrow cursor
    CursorType_Arrow,

    // A standard arrow with small hourglass cursor
    CursorType_AppStarting,

    // A crosshair cursor
    CursorType_Cross,

    // A help cursor
    CursorType_Help,

    // A text I-Beam cursor
    CursorType_IBeam,

    // A cursor with arrows pointing north, south, east, and west
    CursorType_SizeAll,

    // A cursor with arrows pointing northeast and southwest
    CursorType_SizeNESW,

    // A cursor with arrows pointing north and south
    CursorType_SizeNS,

    // A cursor with arrows pointing northwest and southeast
    CursorType_SizeNWSE,

    // A cursor with arrows pointing west and east
    CursorType_SizeWE,

    // A vertical arrow cursor
    CursorType_UpArrow,

    // An hourglass cursor
    CursorType_Wait,

    // A hand cursor
    CursorType_Hand,

    // A pen cursor
    CursorType_Pen,

    // A scrolling cursor with arrows pointing north and south
    CursorType_ScrollNS,

    // A scrolling cursor with arrows pointing west and east
    CursorType_ScrollWE,

    // A scrolling cursor with arrows pointing north, south, east, and west
    CursorType_ScrollAll,

    // A scrolling cursor with an arrow pointing north
    CursorType_ScrollN,

    // A scrolling cursor with an arrow pointing south
    CursorType_ScrollS,

    // A scrolling cursor with an arrow pointing west
    CursorType_ScrollW,

    // A scrolling cursor with an arrow pointing east
    CursorType_ScrollE,

    // A scrolling cursor with arrows pointing north and west
    CursorType_ScrollNW,

    // A scrolling cursor with arrows pointing north and east
    CursorType_ScrollNE,

    // A scrolling cursor with arrows pointing south and west
    CursorType_ScrollSW,

    // A scrolling cursor with arrows pointing south and east
    CursorType_ScrollSE,

    // An arrow cd cursor
    CursorType_ArrowCD,

    // A custom cursor
    CursorType_Custom,

    CursorType_Count
};

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the image used for the mouse pointer.
///
/// The Cursors class contains a set of default cursors. Custom cursors can be created from
/// specified .ani or .cur files.
///
/// .. code-block:: xml
///
///    <StackPanel xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
///      <Rectangle Height="100" Fill="Red" Cursor="Hand"/>
///      <Rectangle Height="100" Fill="Blue" Cursor="Resources/Cursors/link.cur"/>
///    </StackPanel>
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.input.cursor
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Cursor: public BaseComponent
{
public:
    /// Creates a custom cursor from a file
    Cursor(const char* filename);

    /// Generates a string representation of the cursor.
    /// Returns the standard cursor name, or the path for the custom cursor file
    String ToString() const;

    /// Returns the cursor type
    /// \prop
    CursorType Type() const;

    /// For custom cursors this returns the filename or empty string for built in cursors
    /// \prop
    const char* Filename() const;

private:
    friend struct StandardCursors;
    Cursor(CursorType type);

    CursorType mType;
    String mFilename;

    NS_DECLARE_REFLECTION(Cursor, BaseComponent)
};

}

NS_WARNING_POP

#endif
