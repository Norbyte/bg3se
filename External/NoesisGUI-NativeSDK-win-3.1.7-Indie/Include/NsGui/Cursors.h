////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CURSORS_H__
#define __GUI_CURSORS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class Cursor;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a set of default cursors.
///
/// This static class defines a set of default cursors common to applications.
///
/// https://docs.microsoft.com/en-us/dotnet/api/system.windows.input.cursors
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API Cursors
{
    /// Gets the Cursor that appears when an application is starting
    /// \prop
    static Cursor* AppStarting();

    /// Gets the Arrow Cursor
    /// \prop
    static Cursor* Arrow();

    /// Gets the arrow with a compact disk Cursor
    /// \prop
    static Cursor* ArrowCD();

    /// Gets the crosshair Cursor
    /// \prop
    static Cursor* Cross();

    /// Gets a hand Cursor
    /// \prop
    static Cursor* Hand();

    /// Gets a help Cursor which is a combination of an arrow and a question mark
    /// \prop
    static Cursor* Help();

    /// Gets an I-beam Cursor, which is used to show where the text cursor appears when
    /// the mouse is clicked
    /// \prop
    static Cursor* IBeam();

    /// Gets a Cursor with which indicates that a particular region is invalid for a
    /// given operation
    /// \prop
    static Cursor* No();

    /// Gets a special cursor that is invisible
    /// \prop
    static Cursor* None();

    /// Gets a pen Cursor
    /// \prop
    static Cursor* Pen();

    /// Gets the scroll all Cursor
    /// \prop
    static Cursor* ScrollAll();

    /// Gets the scroll east Cursor
    /// \prop
    static Cursor* ScrollE();

    /// Gets the scroll north Cursor
    /// \prop
    static Cursor* ScrollN();

    /// Gets the scroll northeast Cursor
    /// \prop
    static Cursor* ScrollNE();

    /// Gets the scroll north/south Cursor
    /// \prop
    static Cursor* ScrollNS();

    /// Gets a scroll northwest Cursor
    /// \prop
    static Cursor* ScrollNW();

    /// Gets the scroll south Cursor
    /// \prop
    static Cursor* ScrollS();

    /// Gets a south/east scrolling Cursor
    /// \prop
    static Cursor* ScrollSE();

    /// Gets the scroll southwest Cursor
    /// \prop
    static Cursor* ScrollSW();

    /// Gets the scroll west Cursor
    /// \prop
    static Cursor* ScrollW();

    /// Gets a west/east scrolling Cursor
    /// \prop
    static Cursor* ScrollWE();

    /// Gets a four-headed sizing Cursor, which consists of four joined arrows
    /// that point north, south, east, and west
    /// \prop
    static Cursor* SizeAll();

    /// Gets a two-headed northeast/southwest sizing Cursor
    /// \prop
    static Cursor* SizeNESW();

    /// Gets a two-headed north/south sizing Cursor
    /// \prop
    static Cursor* SizeNS();

    /// Gets a two-headed northwest/southeast sizing Cursor
    /// \prop
    static Cursor* SizeNWSE();

    /// Gets a two-headed west/east sizing Cursor
    /// \prop
    static Cursor* SizeWE();

    /// Gets an up arrow Cursor, which is typically used to identify an insertion point
    /// \prop
    static Cursor* UpArrow();

    /// Gets a wait (or hourglass) Cursor
    /// \prop
    static Cursor* Wait();
};

}

#endif
