////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLORS_H__
#define __GUI_COLORS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsDrawing/Color.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements a set of predefined colors.
///
/// These colors and their names are based on the UNIX X11 color values. The following image shows
/// the color of each predefined brush, its name, and its hexadecimal value.
///
/// .. image:: BrushTable.png
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.colors.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API Colors
{
    /// Gets the system-defined color that has an ARGB value of #FFF0F8FF
    /// \prop
    static Color AliceBlue();

    /// Gets the system-defined color that has an ARGB value of #FFFAEBD7
    /// \prop
    static Color AntiqueWhite();

    /// Gets the system-defined color that has an ARGB value of #FF00FFFF
    /// \prop
    static Color Aqua();

    /// Gets the system-defined color that has an ARGB value of #FF7FFFD4
    /// \prop
    static Color Aquamarine();

    /// Gets the system-defined color that has an ARGB value of #FFF0FFFF
    /// \prop
    static Color Azure();

    /// Gets the system-defined color that has an ARGB value of #FFF5F5DC
    /// \prop
    static Color Beige();

    /// Gets the system-defined color that has an ARGB value of #FFFFE4C4
    /// \prop
    static Color Bisque();

    /// Gets the system-defined color that has an ARGB value of #FF000000
    /// \prop
    static Color Black();

    /// Gets the system-defined color that has an ARGB value of #FFFFEBCD
    /// \prop
    static Color BlanchedAlmond();

    /// Gets the system-defined color that has an ARGB value of #FF0000FF
    /// \prop
    static Color Blue();

    /// Gets the system-defined color that has an ARGB value of #FF8A2BE2
    /// \prop
    static Color BlueViolet();

    /// Gets the system-defined color that has an ARGB value of #FFA52A2A
    /// \prop
    static Color Brown();

    /// Gets the system-defined color that has an ARGB value of #FFDEB887
    /// \prop
    static Color BurlyWood();

    /// Gets the system-defined color that has an ARGB value of #FF5F9EA0
    /// \prop
    static Color CadetBlue();

    /// Gets the system-defined color that has an ARGB value of #FF7FFF00
    /// \prop
    static Color Chartreuse();

    /// Gets the system-defined color that has an ARGB value of #FFD2691E
    /// \prop
    static Color Chocolate();

    /// Gets the system-defined color that has an ARGB value of #FFFF7F50
    /// \prop
    static Color Coral();

    /// Gets the system-defined color that has an ARGB value of #FF6495ED
    /// \prop
    static Color CornflowerBlue();

    /// Gets the system-defined color that has an ARGB value of #FFFFF8DC
    /// \prop
    static Color Cornsilk();

    /// Gets the system-defined color that has an ARGB value of #FFDC143C
    /// \prop
    static Color Crimson();

    /// Gets the system-defined color that has an ARGB value of #FF00FFFF
    /// \prop
    static Color Cyan();

    /// Gets the system-defined color that has an ARGB value of #FF00008B
    /// \prop
    static Color DarkBlue();

    /// Gets the system-defined color that has an ARGB value of #FF008B8B
    /// \prop
    static Color DarkCyan();

    /// Gets the system-defined color that has an ARGB value of #FFB8860B
    /// \prop
    static Color DarkGoldenrod();

    /// Gets the system-defined color that has an ARGB value of #FFA9A9A9
    /// \prop
    static Color DarkGray();

    /// Gets the system-defined color that has an ARGB value of #FF006400
    /// \prop
    static Color DarkGreen();

    /// Gets the system-defined color that has an ARGB value of #FFBDB76B
    /// \prop
    static Color DarkKhaki();

    /// Gets the system-defined color that has an ARGB value of #FF8B008B
    /// \prop
    static Color DarkMagenta();

    /// Gets the system-defined color that has an ARGB value of #FF556B2F
    /// \prop
    static Color DarkOliveGreen();

    /// Gets the system-defined color that has an ARGB value of #FFFF8C00
    /// \prop
    static Color DarkOrange();

    /// Gets the system-defined color that has an ARGB value of #FF9932CC
    /// \prop
    static Color DarkOrchid();

    /// Gets the system-defined color that has an ARGB value of #FF8B0000
    /// \prop
    static Color DarkRed();

    /// Gets the system-defined color that has an ARGB value of #FFE9967A
    /// \prop
    static Color DarkSalmon();

    /// Gets the system-defined color that has an ARGB value of #FF8FBC8F
    /// \prop
    static Color DarkSeaGreen();

    /// Gets the system-defined color that has an ARGB value of #FF483D8B
    /// \prop
    static Color DarkSlateBlue();

    /// Gets the system-defined color that has an ARGB value of #FF2F4F4F
    /// \prop
    static Color DarkSlateGray();

    /// Gets the system-defined color that has an ARGB value of #FF00CED1
    /// \prop
    static Color DarkTurquoise();

    /// Gets the system-defined color that has an ARGB value of #FF9400D3
    /// \prop
    static Color DarkViolet();

    /// Gets the system-defined color that has an ARGB value of #FFFF1493
    /// \prop
    static Color DeepPink();

    /// Gets the system-defined color that has an ARGB value of #FF00BFFF
    /// \prop
    static Color DeepSkyBlue();

    /// Gets the system-defined color that has an ARGB value of #FF696969
    /// \prop
    static Color DimGray();

    /// Gets the system-defined color that has an ARGB value of #FF1E90FF
    /// \prop
    static Color DodgerBlue();

    /// Gets the system-defined color that has an ARGB value of #FFB22222
    /// \prop
    static Color Firebrick();

    /// Gets the system-defined color that has an ARGB value of #FFFFFAF0
    /// \prop
    static Color FloralWhite();

    /// Gets the system-defined color that has an ARGB value of #FF228B22
    /// \prop
    static Color ForestGreen();

    /// Gets the system-defined color that has an ARGB value of #FFFF00FF
    /// \prop
    static Color Fuchsia();

    /// Gets the system-defined color that has an ARGB value of #FFDCDCDC
    /// \prop
    static Color Gainsboro();

    /// Gets the system-defined color that has an ARGB value of #FFF8F8FF
    /// \prop
    static Color GhostWhite();

    /// Gets the system-defined color that has an ARGB value of #FFFFD700
    /// \prop
    static Color Gold();

    /// Gets the system-defined color that has an ARGB value of #FFDAA520
    /// \prop
    static Color Goldenrod();

    /// Gets the system-defined color that has an ARGB value of #FF808080
    /// \prop
    static Color Gray();

    /// Gets the system-defined color that has an ARGB value of #FF008000
    /// \prop
    static Color Green();

    /// Gets the system-defined color that has an ARGB value of #FFADFF2F
    /// \prop
    static Color GreenYellow(); 

    /// Gets the system-defined color that has an ARGB value of #FFF0FFF0
    /// \prop
    static Color Honeydew();

    /// Gets the system-defined color that has an ARGB value of #FFFF69B4
    /// \prop
    static Color HotPink();

    /// Gets the system-defined color that has an ARGB value of #FFCD5C5C
    /// \prop
    static Color IndianRed();

    /// Gets the system-defined color that has an ARGB value of #FF4B0082
    /// \prop
    static Color Indigo();

    /// Gets the system-defined color that has an ARGB value of #FFFFFFF0
    /// \prop
    static Color Ivory();

    /// Gets the system-defined color that has an ARGB value of #FFF0E68C
    /// \prop
    static Color Khaki();

    /// Gets the system-defined color that has an ARGB value of #FFE6E6FA
    /// \prop
    static Color Lavender();

    /// Gets the system-defined color that has an ARGB value of #FFFFF0F5
    /// \prop
    static Color LavenderBlush();

    /// Gets the system-defined color that has an ARGB value of #FF7CFC00
    /// \prop
    static Color LawnGreen();

    /// Gets the system-defined color that has an ARGB value of #FFFFFACD
    /// \prop
    static Color LemonChiffon();

    /// Gets the system-defined color that has an ARGB value of #FFADD8E6
    /// \prop
    static Color LightBlue();

    /// Gets the system-defined color that has an ARGB value of #FFF08080
    /// \prop
    static Color LightCoral();

    /// Gets the system-defined color that has an ARGB value of #FFE0FFFF
    /// \prop
    static Color LightCyan();

    /// Gets the system-defined color that has an ARGB value of #FFFAFAD2
    /// \prop
    static Color LightGoldenrodYellow();

    /// Gets the system-defined color that has an ARGB value of #FFD3D3D3
    /// \prop
    static Color LightGray();

    /// Gets the system-defined color that has an ARGB value of #FF90EE90
    /// \prop
    static Color LightGreen();

    /// Gets the system-defined color that has an ARGB value of #FFFFB6C1
    /// \prop
    static Color LightPink();

    /// Gets the system-defined color that has an ARGB value of #FFFFA07A
    /// \prop
    static Color LightSalmon();

    /// Gets the system-defined color that has an ARGB value of #FF20B2AA
    /// \prop
    static Color LightSeaGreen();

    /// Gets the system-defined color that has an ARGB value of #FF87CEFA
    /// \prop
    static Color LightSkyBlue();

    /// Gets the system-defined color that has an ARGB value of #FF778899
    /// \prop
    static Color LightSlateGray();

    /// Gets the system-defined color that has an ARGB value of #FFB0C4DE
    /// \prop
    static Color LightSteelBlue();

    /// Gets the system-defined color that has an ARGB value of #FFFFFFE0
    /// \prop
    static Color LightYellow();

    /// Gets the system-defined color that has an ARGB value of #FF00FF00
    /// \prop
    static Color Lime();

    /// Gets the system-defined color that has an ARGB value of #FF32CD32
    /// \prop
    static Color LimeGreen();

    /// Gets the system-defined color that has an ARGB value of #FFFAF0E6
    /// \prop
    static Color Linen();

    /// Gets the system-defined color that has an ARGB value of #FFFF00FF
    /// \prop
    static Color Magenta();

    /// Gets the system-defined color that has an ARGB value of #FF800000
    /// \prop
    static Color Maroon();

    /// Gets the system-defined color that has an ARGB value of #FF66CDAA
    /// \prop
    static Color MediumAquamarine();

    /// Gets the system-defined color that has an ARGB value of #FF0000CD
    /// \prop
    static Color MediumBlue();

    /// Gets the system-defined color that has an ARGB value of #FFBA55D3
    /// \prop
    static Color MediumOrchid();

    /// Gets the system-defined color that has an ARGB value of #FF9370DB
    /// \prop
    static Color MediumPurple();

    /// Gets the system-defined color that has an ARGB value of #FF3CB371
    /// \prop
    static Color MediumSeaGreen();

    /// Gets the system-defined color that has an ARGB value of #FF7B68EE
    /// \prop
    static Color MediumSlateBlue();

    /// Gets the system-defined color that has an ARGB value of #FF00FA9A
    /// \prop
    static Color MediumSpringGreen();

    /// Gets the system-defined color that has an ARGB value of #FF48D1CC
    /// \prop
    static Color MediumTurquoise();

    /// Gets the system-defined color that has an ARGB value of #FFC71585
    /// \prop
    static Color MediumVioletRed();

    /// Gets the system-defined color that has an ARGB value of #FF191970
    /// \prop
    static Color MidnightBlue();

    /// Gets the system-defined color that has an ARGB value of #FFF5FFFA
    /// \prop
    static Color MintCream();

    /// Gets the system-defined color that has an ARGB value of #FFFFE4E1
    /// \prop
    static Color MistyRose();

    /// Gets the system-defined color that has an ARGB value of #FFFFE4B5
    /// \prop
    static Color Moccasin();

    /// Gets the system-defined color that has an ARGB value of #FFFFDEAD
    /// \prop
    static Color NavajoWhite();

    /// Gets the system-defined color that has an ARGB value of #FF000080
    /// \prop
    static Color Navy();

    /// Gets the system-defined color that has an ARGB value of #FFFDF5E6
    /// \prop
    static Color OldLace();

    /// Gets the system-defined color that has an ARGB value of #FF808000
    /// \prop
    static Color Olive();

    /// Gets the system-defined color that has an ARGB value of #FF6B8E23
    /// \prop
    static Color OliveDrab();

    /// Gets the system-defined color that has an ARGB value of #FFFFA500
    /// \prop
    static Color Orange();

    /// Gets the system-defined color that has an ARGB value of #FFFF4500
    /// \prop
    static Color OrangeRed();

    /// Gets the system-defined color that has an ARGB value of #FFDA70D6
    /// \prop
    static Color Orchid();

    /// Gets the system-defined color that has an ARGB value of #FFEEE8AA
    /// \prop
    static Color PaleGoldenrod();

    /// Gets the system-defined color that has an ARGB value of #FF98FB98
    /// \prop
    static Color PaleGreen();

    /// Gets the system-defined color that has an ARGB value of #FFAFEEEE
    /// \prop
    static Color PaleTurquoise();

    /// Gets the system-defined color that has an ARGB value of #FFDB7093
    /// \prop
    static Color PaleVioletRed();

    /// Gets the system-defined color that has an ARGB value of #FFFFEFD5
    /// \prop
    static Color PapayaWhip();

    /// Gets the system-defined color that has an ARGB value of #FFFFDAB9
    /// \prop
    static Color PeachPuff();

    /// Gets the system-defined color that has an ARGB value of #FFCD853F
    /// \prop
    static Color Peru();

    /// Gets the system-defined color that has an ARGB value of #FFFFC0CB
    /// \prop
    static Color Pink();

    /// Gets the system-defined color that has an ARGB value of #FFDDA0DD
    /// \prop
    static Color Plum();

    /// Gets the system-defined color that has an ARGB value of #FFB0E0E6
    /// \prop
    static Color PowderBlue();

    /// Gets the system-defined color that has an ARGB value of #FF800080
    /// \prop
    static Color Purple();

    /// Gets the system-defined color that has an ARGB value of #FFFF0000
    /// \prop
    static Color Red();

    /// Gets the system-defined color that has an ARGB value of #FFBC8F8F
    /// \prop
    static Color RosyBrown();

    /// Gets the system-defined color that has an ARGB value of #FF4169E1
    /// \prop
    static Color RoyalBlue();

    /// Gets the system-defined color that has an ARGB value of #FF8B4513
    /// \prop
    static Color SaddleBrown();

    /// Gets the system-defined color that has an ARGB value of #FFFA8072
    /// \prop
    static Color Salmon();

    /// Gets the system-defined color that has an ARGB value of #FFF4A460
    /// \prop
    static Color SandyBrown();

    /// Gets the system-defined color that has an ARGB value of #FF2E8B57
    /// \prop
    static Color SeaGreen();

    /// Gets the system-defined color that has an ARGB value of #FFFFF5EE
    /// \prop
    static Color SeaShell();

    /// Gets the system-defined color that has an ARGB value of #FFA0522D
    /// \prop
    static Color Sienna();

    /// Gets the system-defined color that has an ARGB value of #FFC0C0C0
    /// \prop
    static Color Silver();

    /// Gets the system-defined color that has an ARGB value of #FF87CEEB
    /// \prop
    static Color SkyBlue();

    /// Gets the system-defined color that has an ARGB value of #FF6A5ACD
    /// \prop
    static Color SlateBlue();

    /// Gets the system-defined color that has an ARGB value of #FF708090
    /// \prop
    static Color SlateGray();

    /// Gets the system-defined color that has an ARGB value of #FFFFFAFA
    /// \prop
    static Color Snow();

    /// Gets the system-defined color that has an ARGB value of #FF00FF7F
    /// \prop
    static Color SpringGreen();

    /// Gets the system-defined color that has an ARGB value of #FF4682B4
    /// \prop
    static Color SteelBlue();

    /// Gets the system-defined color that has an ARGB value of #FFD2B48C
    /// \prop
    static Color Tan();

    /// Gets the system-defined color that has an ARGB value of #FF008080
    /// \prop
    static Color Teal();

    /// Gets the system-defined color that has an ARGB value of #FFD8BFD8
    /// \prop
    static Color Thistle();

    /// Gets the system-defined color that has an ARGB value of #FFFF6347
    /// \prop
    static Color Tomato();

    /// Gets the system-defined color that has an ARGB value of #00FFFFFF
    /// \prop
    static Color Transparent();

    /// Gets the system-defined color that has an ARGB value of #FF40E0D0
    /// \prop
    static Color Turquoise();

    /// Gets the system-defined color that has an ARGB value of #FFEE82EE
    /// \prop
    static Color Violet();

    /// Gets the system-defined color that has an ARGB value of #FFF5DEB3
    /// \prop
    static Color Wheat();

    /// Gets the system-defined color that has an ARGB value of #FFFFFFFF
    /// \prop
    static Color White();

    /// Gets the system-defined color that has an ARGB value of #FFF5F5F5
    /// \prop
    static Color WhiteSmoke();

    /// Gets the system-defined color that has an ARGB value of #FFFFFF00
    /// \prop
    static Color Yellow();

    /// Gets the system-defined color that has an ARGB value of #FF9ACD32
    /// \prop
    static Color YellowGreen();
};

}

#endif
