////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_COLOR_H__
#define __DRAWING_COLOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/StringFwd.h>
#include <NsDrawing/TypesApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// RGBA color in the sRGB (http://en.wikipedia.org/wiki/SRGB) space
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Color
{
    float r;
    float g;
    float b;
    float a;

    /// Constructs a black color
    Color();

    /// Constructor from float components with a range from 0 to 1
    Color(float r, float g, float b, float a = 1.0f);

    /// Constructor from integer components with a range from 0 to 255
    Color(int r, int g, int b, int a = 255);

    /// Creates a color from an integer representation as obtained from GetPackedColorBGRA
    static Color FromPackedBGRA(uint32_t color);
    
    /// Creates a color from an integer representation as obtained from GetPackedColorRGBA
    static Color FromPackedRGBA(uint32_t color);

    /// Creates a color from components given in linear RGB space
    static Color FromLinearRGB(float r, float g, float b, float a = 1.0f);

    /// Returns the BGRA packed representation
    uint32_t GetPackedColorBGRA() const;

    /// Returns the RGBA packed representation
    uint32_t GetPackedColorRGBA() const;

    /// Logic operators
    bool operator==(const Color& color) const;
    bool operator!=(const Color& color) const;
    
    /// Generates a string representation in the form #AARRGGBB
    NS_DRAWING_TYPES_API String ToString() const;

    /// Creates a color from a string
    /// #aarrggbb | #rrggbb | #argb | #rgb | sc#r,g,b | sc#a,r,g,b | PredefinedColor
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Color& output);
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Color& output, int& index);

    /// Returns the color corresponding to the index of a predefined color
    NS_DRAWING_TYPES_API static Color FromIndex(int index);

    /// Constants
    static Color Black();
    static Color Blue();
    static Color Cyan();
    static Color DarkGray();
    static Color Gray();
    static Color Green();
    static Color LightGray();
    static Color Magenta();
    static Color Orange();
    static Color Pink();
    static Color Red();
    static Color White();
    static Color Yellow();

    NS_IMPLEMENT_INLINE_REFLECTION(Color, NoParent)
    {
        NsProp("r", &Color::r);
        NsProp("g", &Color::g);
        NsProp("b", &Color::b);
        NsProp("a", &Color::a);
    }
};

/// Converts the given value from sRGB to linear color space
float SRGBToLinear(float v);

/// Converts the given value from linear to sRGB color space
float LinearToSRGB(float v);

/// Returns the BGRA packed representation for the given color components
uint32_t GetPackedColorBGRA(float r, float g, float b, float a);

/// Returns the RGBA packed representation for the given color components
uint32_t GetPackedColorRGBA(float r, float g, float b, float a);

}

#include <NsDrawing/Color.inl>

#endif
