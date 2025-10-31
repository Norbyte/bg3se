////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BRUSHES_H__
#define __GUI_BRUSHES_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

struct Color;
class SolidColorBrush;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Declares a set of predefined SolidColorBrush objects.
///
/// These colors and their names are based on the UNIX X11 color values. The following image shows
/// the color of each predefined brush, its name, and its hexadecimal value.
///
/// .. image:: BrushTable.png
///
/// https://msdn.microsoft.com/en-us/library/system.windows.media.brushes.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API Brushes
{
    /// Gets the solid fill color that has a hexadecimal value of #FFF0F8FF
    /// \prop
    static SolidColorBrush* AliceBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFFAEBD7
    /// \prop
    static SolidColorBrush* AntiqueWhite();

    /// Gets the solid fill color that has a hexadecimal value of #FF00FFFF
    /// \prop
    static SolidColorBrush* Aqua();

    /// Gets the solid fill color that has a hexadecimal value of #FF7FFFD4
    /// \prop
    static SolidColorBrush* Aquamarine();

    /// Gets the solid fill color that has a hexadecimal value of #FFF0FFFF
    /// \prop
    static SolidColorBrush* Azure();

    /// Gets the solid fill color that has a hexadecimal value of #FFF5F5DC
    /// \prop
    static SolidColorBrush* Beige();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFE4C4
    /// \prop
    static SolidColorBrush* Bisque();

    /// Gets the solid fill color that has a hexadecimal value of #FF000000
    /// \prop
    static SolidColorBrush* Black();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFEBCD
    /// \prop
    static SolidColorBrush* BlanchedAlmond();

    /// Gets the solid fill color that has a hexadecimal value of #FF0000FF
    /// \prop
    static SolidColorBrush* Blue();

    /// Gets the solid fill color that has a hexadecimal value of #FF8A2BE2
    /// \prop
    static SolidColorBrush* BlueViolet();

    /// Gets the solid fill color that has a hexadecimal value of #FFA52A2A
    /// \prop
    static SolidColorBrush* Brown();

    /// Gets the solid fill color that has a hexadecimal value of #FFDEB887
    /// \prop
    static SolidColorBrush* BurlyWood();

    /// Gets the solid fill color that has a hexadecimal value of #FF5F9EA0
    /// \prop
    static SolidColorBrush* CadetBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF7FFF00
    /// \prop
    static SolidColorBrush* Chartreuse();

    /// Gets the solid fill color that has a hexadecimal value of #FFD2691E
    /// \prop
    static SolidColorBrush* Chocolate();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF7F50
    /// \prop
    static SolidColorBrush* Coral();

    /// Gets the solid fill color that has a hexadecimal value of #FF6495ED
    /// \prop
    static SolidColorBrush* CornflowerBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFF8DC
    /// \prop
    static SolidColorBrush* Cornsilk();

    /// Gets the solid fill color that has a hexadecimal value of #FFDC143C
    /// \prop
    static SolidColorBrush* Crimson();

    /// Gets the solid fill color that has a hexadecimal value of #FF00FFFF
    /// \prop
    static SolidColorBrush* Cyan();

    /// Gets the solid fill color that has a hexadecimal value of #FF00008B
    /// \prop
    static SolidColorBrush* DarkBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF008B8B
    /// \prop
    static SolidColorBrush* DarkCyan();

    /// Gets the solid fill color that has a hexadecimal value of #FFB8860B
    /// \prop
    static SolidColorBrush* DarkGoldenrod();

    /// Gets the solid fill color that has a hexadecimal value of #FFA9A9A9
    /// \prop
    static SolidColorBrush* DarkGray();

    /// Gets the solid fill color that has a hexadecimal value of #FF006400
    /// \prop
    static SolidColorBrush* DarkGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFBDB76B
    /// \prop
    static SolidColorBrush* DarkKhaki();

    /// Gets the solid fill color that has a hexadecimal value of #FF8B008B
    /// \prop
    static SolidColorBrush* DarkMagenta();

    /// Gets the solid fill color that has a hexadecimal value of #FF556B2F
    /// \prop
    static SolidColorBrush* DarkOliveGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF8C00
    /// \prop
    static SolidColorBrush* DarkOrange();

    /// Gets the solid fill color that has a hexadecimal value of #FF9932CC
    /// \prop
    static SolidColorBrush* DarkOrchid();

    /// Gets the solid fill color that has a hexadecimal value of #FF8B0000
    /// \prop
    static SolidColorBrush* DarkRed();

    /// Gets the solid fill color that has a hexadecimal value of #FFE9967A
    /// \prop
    static SolidColorBrush* DarkSalmon();

    /// Gets the solid fill color that has a hexadecimal value of #FF8FBC8F
    /// \prop
    static SolidColorBrush* DarkSeaGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FF483D8B
    /// \prop
    static SolidColorBrush* DarkSlateBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF2F4F4F
    /// \prop
    static SolidColorBrush* DarkSlateGray();

    /// Gets the solid fill color that has a hexadecimal value of #FF00CED1
    /// \prop
    static SolidColorBrush* DarkTurquoise();

    /// Gets the solid fill color that has a hexadecimal value of #FF9400D3
    /// \prop
    static SolidColorBrush* DarkViolet();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF1493
    /// \prop
    static SolidColorBrush* DeepPink();

    /// Gets the solid fill color that has a hexadecimal value of #FF00BFFF
    /// \prop
    static SolidColorBrush* DeepSkyBlue();

    /// Gets the solid fill color that has a hexadecimal value of  #FF696969
    /// \prop
    static SolidColorBrush* DimGray();

    /// Gets the solid fill color that has a hexadecimal value of #FF1E90FF
    /// \prop
    static SolidColorBrush* DodgerBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFB22222
    /// \prop
    static SolidColorBrush* Firebrick();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFFAF0
    /// \prop
    static SolidColorBrush* FloralWhite(); 

    /// Gets the solid fill color that has a hexadecimal value of #FF228B22
    /// \prop
    static SolidColorBrush* ForestGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF00FF
    /// \prop
    static SolidColorBrush* Fuchsia();

    /// Gets the solid fill color that has a hexadecimal value of #FFDCDCDC
    /// \prop
    static SolidColorBrush* Gainsboro();

    /// Gets the solid fill color that has a hexadecimal value of #FFF8F8FF
    /// \prop
    static SolidColorBrush* GhostWhite();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFD700
    /// \prop
    static SolidColorBrush* Gold();

    /// Gets the solid fill color that has a hexadecimal value of #FFDAA520
    /// \prop
    static SolidColorBrush* Goldenrod();

    /// Gets the solid fill color that has a hexadecimal value of #FF808080
    /// \prop
    static SolidColorBrush* Gray();

    /// Gets the solid fill color that has a hexadecimal value of #FF008000
    /// \prop
    static SolidColorBrush* Green();

    /// Gets the solid fill color that has a hexadecimal value of #FFADFF2F
    /// \prop
    static SolidColorBrush* GreenYellow();

    /// Gets the solid fill color that has a hexadecimal value of #FFF0FFF0
    /// \prop
    static SolidColorBrush* Honeydew();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF69B4
    /// \prop
    static SolidColorBrush* HotPink();

    /// Gets the solid fill color that has a hexadecimal value of #FFCD5C5C
    /// \prop
    static SolidColorBrush* IndianRed();

    /// Gets the solid fill color that has a hexadecimal value of #FF4B0082
    /// \prop
    static SolidColorBrush* Indigo();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFFFF0
    /// \prop
    static SolidColorBrush* Ivory();

    /// Gets the solid fill color that has a hexadecimal value of #FFF0E68C
    /// \prop
    static SolidColorBrush* Khaki();

    /// Gets the solid fill color that has a hexadecimal value of #FFE6E6FA
    /// \prop
    static SolidColorBrush* Lavender();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFF0F5
    /// \prop
    static SolidColorBrush* LavenderBlush();

    /// Gets the solid fill color that has a hexadecimal value of #FF7CFC00
    /// \prop
    static SolidColorBrush* LawnGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFFACD
    /// \prop
    static SolidColorBrush* LemonChiffon();

    /// Gets the solid fill color that has a hexadecimal value of #FFADD8E6
    /// \prop
    static SolidColorBrush* LightBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFF08080
    /// \prop
    static SolidColorBrush* LightCoral();

    /// Gets the solid fill color that has a hexadecimal value of #FFE0FFFF
    /// \prop
    static SolidColorBrush* LightCyan();

    /// Gets the solid fill color that has a hexadecimal value of #FFFAFAD2
    /// \prop
    static SolidColorBrush* LightGoldenrodYellow();

    /// Gets the solid fill color that has a hexadecimal value of #FFD3D3D3
    /// \prop
    static SolidColorBrush* LightGray();

    /// Gets the solid fill color that has a hexadecimal value of #FF90EE90
    /// \prop
    static SolidColorBrush* LightGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFB6C1
    /// \prop
    static SolidColorBrush* LightPink();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFA07A
    /// \prop
    static SolidColorBrush* LightSalmon();

    /// Gets the solid fill color that has a hexadecimal value of #FF20B2AA
    /// \prop
    static SolidColorBrush* LightSeaGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FF87CEFA
    /// \prop
    static SolidColorBrush* LightSkyBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF778899
    /// \prop
    static SolidColorBrush* LightSlateGray();

    /// Gets the solid fill color that has a hexadecimal value of #FFB0C4DE
    /// \prop
    static SolidColorBrush* LightSteelBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFFFE0
    /// \prop
    static SolidColorBrush* LightYellow();

    /// Gets the solid fill color that has a hexadecimal value of #FF00FF00
    /// \prop
    static SolidColorBrush* Lime();

    /// Gets the solid fill color that has a hexadecimal value of #FF32CD32
    /// \prop
    static SolidColorBrush* LimeGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFFAF0E6
    /// \prop
    static SolidColorBrush* Linen();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF00FF
    /// \prop
    static SolidColorBrush* Magenta();

    /// Gets the solid fill color that has a hexadecimal value of #FF800000
    /// \prop
    static SolidColorBrush* Maroon();

    /// Gets the solid fill color that has a hexadecimal value of #FF66CDAA
    /// \prop
    static SolidColorBrush* MediumAquamarine();

    /// Gets the solid fill color that has a hexadecimal value of #FF0000CD
    /// \prop
    static SolidColorBrush* MediumBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFBA55D3
    /// \prop
    static SolidColorBrush* MediumOrchid();

    /// Gets the solid fill color that has a hexadecimal value of #FF9370DB
    /// \prop
    static SolidColorBrush* MediumPurple();

    /// Gets the solid fill color that has a hexadecimal value of #FF3CB371
    /// \prop
    static SolidColorBrush* MediumSeaGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FF7B68EE
    /// \prop
    static SolidColorBrush* MediumSlateBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF00FA9A
    /// \prop
    static SolidColorBrush* MediumSpringGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FF48D1CC
    /// \prop
    static SolidColorBrush* MediumTurquoise();

    /// Gets the solid fill color that has a hexadecimal value of #FFC71585
    /// \prop
    static SolidColorBrush* MediumVioletRed();

    /// Gets the solid fill color that has a hexadecimal value of #FF191970
    /// \prop
    static SolidColorBrush* MidnightBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFF5FFFA
    /// \prop
    static SolidColorBrush* MintCream();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFE4E1
    /// \prop
    static SolidColorBrush* MistyRose();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFE4B5
    /// \prop
    static SolidColorBrush* Moccasin();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFDEAD
    /// \prop
    static SolidColorBrush* NavajoWhite();

    /// Gets the solid fill color that has a hexadecimal value of #FF000080
    /// \prop
    static SolidColorBrush* Navy();

    /// Gets the solid fill color that has a hexadecimal value of #FFFDF5E6
    /// \prop
    static SolidColorBrush* OldLace();

    /// Gets the solid fill color that has a hexadecimal value of #FF808000
    /// \prop
    static SolidColorBrush* Olive();

    /// Gets the solid fill color that has a hexadecimal value of #FF6B8E23
    /// \prop
    static SolidColorBrush* OliveDrab();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFA500
    /// \prop
    static SolidColorBrush* Orange();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF4500
    /// \prop
    static SolidColorBrush* OrangeRed();

    /// Gets the solid fill color that has a hexadecimal value of #FFDA70D6
    /// \prop
    static SolidColorBrush* Orchid();

    /// Gets the solid fill color that has a hexadecimal value of #FFEEE8AA
    /// \prop
    static SolidColorBrush* PaleGoldenrod();

    /// Gets the solid fill color that has a hexadecimal value of #FF98FB98
    /// \prop
    static SolidColorBrush* PaleGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFAFEEEE
    /// \prop
    static SolidColorBrush* PaleTurquoise();

    /// Gets the solid fill color that has a hexadecimal value of #FFDB7093
    /// \prop
    static SolidColorBrush* PaleVioletRed();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFEFD5
    /// \prop
    static SolidColorBrush* PapayaWhip();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFDAB9
    /// \prop
    static SolidColorBrush* PeachPuff();

    /// Gets the solid fill color that has a hexadecimal value of #FFCD853F
    /// \prop
    static SolidColorBrush* Peru();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFC0CB
    /// \prop
    static SolidColorBrush* Pink();

    /// Gets the solid fill color that has a hexadecimal value of #FFDDA0DD
    /// \prop
    static SolidColorBrush* Plum();

    /// Gets the solid fill color that has a hexadecimal value of #FFB0E0E6
    /// \prop
    static SolidColorBrush* PowderBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF800080
    /// \prop
    static SolidColorBrush* Purple();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF0000
    /// \prop
    static SolidColorBrush* Red();

    /// Gets the solid fill color that has a hexadecimal value of #FFBC8F8F
    /// \prop
    static SolidColorBrush* RosyBrown();

    /// Gets the solid fill color that has a hexadecimal value of #FF4169E1
    /// \prop
    static SolidColorBrush* RoyalBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF8B4513
    /// \prop
    static SolidColorBrush* SaddleBrown();

    /// Gets the solid fill color that has a hexadecimal value of #FFFA8072
    /// \prop
    static SolidColorBrush* Salmon();

    /// Gets the solid fill color that has a hexadecimal value of #FFF4A460
    /// \prop
    static SolidColorBrush* SandyBrown();

    /// Gets the solid fill color that has a hexadecimal value of #FF2E8B57
    /// \prop
    static SolidColorBrush* SeaGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFF5EE
    /// \prop
    static SolidColorBrush* SeaShell();

    /// Gets the solid fill color that has a hexadecimal value of #FFA0522D
    /// \prop
    static SolidColorBrush* Sienna();

    /// Gets the solid fill color that has a hexadecimal value of #FFC0C0C0
    /// \prop
    static SolidColorBrush* Silver();

    /// Gets the solid fill color that has a hexadecimal value of #FF87CEEB
    /// \prop
    static SolidColorBrush* SkyBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF6A5ACD
    /// \prop
    static SolidColorBrush* SlateBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FF708090
    /// \prop
    static SolidColorBrush* SlateGray();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFFAFA
    /// \prop
    static SolidColorBrush* Snow();

    /// Gets the solid fill color that has a hexadecimal value of #FF00FF7F
    /// \prop
    static SolidColorBrush* SpringGreen();

    /// Gets the solid fill color that has a hexadecimal value of #FF4682B4
    /// \prop
    static SolidColorBrush* SteelBlue();

    /// Gets the solid fill color that has a hexadecimal value of #FFD2B48C
    /// \prop
    static SolidColorBrush* Tan();

    /// Gets the solid fill color that has a hexadecimal value of #FF008080
    /// \prop
    static SolidColorBrush* Teal();

    /// Gets the solid fill color that has a hexadecimal value of #FFD8BFD8
    /// \prop
    static SolidColorBrush* Thistle();

    /// Gets the solid fill color that has a hexadecimal value of #FFFF6347
    /// \prop
    static SolidColorBrush* Tomato();

    /// Gets the solid fill color that has a hexadecimal value of #00FFFFFF
    /// \prop
    static SolidColorBrush* Transparent();

    /// Gets the solid fill color that has a hexadecimal value of #FF40E0D0
    /// \prop
    static SolidColorBrush* Turquoise();

    /// Gets the solid fill color that has a hexadecimal value of #FFEE82EE
    /// \prop
    static SolidColorBrush* Violet();

    /// Gets the solid fill color that has a hexadecimal value of #FFF5DEB3
    /// \prop
    static SolidColorBrush* Wheat();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFFFFF
    /// \prop
    static SolidColorBrush* White();

    /// Gets the solid fill color that has a hexadecimal value of #FFF5F5F5
    /// \prop
    static SolidColorBrush* WhiteSmoke();

    /// Gets the solid fill color that has a hexadecimal value of #FFFFFF00
    /// \prop
    static SolidColorBrush* Yellow();

    /// Gets the solid fill color that has a hexadecimal value of #FF9ACD32
    /// \prop
    static SolidColorBrush* YellowGreen();

    /// Gets the solid fill color corresponding to the index of a predefined color
    static SolidColorBrush* FromIndex(int index);

    NS_DECLARE_REFLECTION(Brushes, NoParent)
};

}

#endif
