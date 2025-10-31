////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FONTPROPERTIES_H__
#define __GUI_FONTPROPERTIES_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

// Represents the density of a typeface, in terms of the lightness or heaviness of the strokes
enum FontWeight: int32_t
{
    FontWeight_Thin = 100,
    FontWeight_ExtraLight = 200,
    FontWeight_UltraLight = 200,
    FontWeight_Light = 300,
    FontWeight_SemiLight = 350,
    FontWeight_Normal = 400,
    FontWeight_Regular = 400,
    FontWeight_Medium = 500,
    FontWeight_DemiBold = 600,
    FontWeight_SemiBold = 600,
    FontWeight_Bold = 700,
    FontWeight_ExtraBold = 800,
    FontWeight_UltraBold = 800,
    FontWeight_Black = 900,
    FontWeight_Heavy = 900,
    FontWeight_ExtraBlack = 950,
    FontWeight_UltraBlack = 950
};

// Represents the style of a font face as normal, italic, or oblique
enum FontStyle: int32_t
{
    FontStyle_Normal,
    FontStyle_Oblique,
    FontStyle_Italic
};

// Represents the degree to which a font has been stretched compared to a font's normal aspect ratio
enum FontStretch: int32_t
{
    FontStretch_UltraCondensed = 1,
    FontStretch_ExtraCondensed = 2,
    FontStretch_Condensed = 3,
    FontStretch_SemiCondensed = 4,
    FontStretch_Normal = 5,
    FontStretch_Medium = 5,
    FontStretch_SemiExpanded = 6,
    FontStretch_Expanded = 7,
    FontStretch_ExtraExpanded = 8,
    FontStretch_UltraExpanded = 9
};

}

#endif
