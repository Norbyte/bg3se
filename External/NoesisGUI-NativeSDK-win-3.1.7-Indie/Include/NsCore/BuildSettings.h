////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_BUILDSETTINGS_H__
#define __CORE_BUILDSETTINGS_H__


// Give the chance to customize default values
#ifdef NS_CUSTOM_BUILD_SETTINGS_HEADER
    #include NS_CUSTOM_BUILD_SETTINGS_HEADER
#endif

// The width of the texture used to store color ramps
#ifndef RAMP_RESOLUTION
    #define RAMP_RESOLUTION 512
#endif

// The size of the LRU cache for color ramps
#ifndef RAMP_CACHE_SIZE
    #define RAMP_CACHE_SIZE 512
#endif

// PPAA extrudes the contours of the path to approximate antialiasing by using alpha blending
// PPA_EXTRUDE_IN defines the contraction, in pixels, of each vertex along the normal
#ifndef PPAA_EXTRUDE_IN
    #define PPAA_EXTRUDE_IN 0.25f
#endif

// PPAA extrudes the contours of the path to approximate antialiasing by using alpha blending
// PPAA_EXTRUDE_OUT defines how many pixels to extrude a vertex out
#ifndef PPAA_EXTRUDE_OUT
    #define PPAA_EXTRUDE_OUT 0.50f
#endif

// Horizontal lines of text are subdivided into chunks. Each chunk is composed by a fixed number of
// glyphs that can be send to GPU or rejected using its bounding box
#ifndef HLINE_CHUNK_SIZE
    #define HLINE_CHUNK_SIZE 255U
#endif

// Activates vertical hinting. You only need this in low-dpi screens (code size ~ 50KB)
#ifndef FREETYPE_HINTING
    #define FREETYPE_HINTING 0
#endif

// Support for TrueType (.ttf) format
#ifndef FREETYPE_TT_FORMAT
    #define FREETYPE_TT_FORMAT 1
#endif

// Support for OpenType (.otf) format (code size ~ 100KB)
#ifndef FREETYPE_CFF_FORMAT
    #define FREETYPE_CFF_FORMAT 1
#endif

// Support for color fonts (code size ~ 5KB)
#ifndef FREETYPE_EMOJIS
    #define FREETYPE_EMOJIS 1
#endif

// Support for system fonts (code size ~ 3KB)
#ifndef USE_SYSTEM_FONTS
    #define USE_SYSTEM_FONTS 1
#endif

// Enables touches and manipulations (code size ~ 13KB)
#ifndef TOUCH_ENABLED
    #define TOUCH_ENABLED 1
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// The macros below control what classes are registered in the factory. If your XAMLs don't
// use <Button/> for example, you can disable registering it in the factory and save a few KB.
// Note that this optimization only makes sense when compiling static libraries because when
// generating dynamic libraries, these clases have dll_export and can't be optimized out
////////////////////////////////////////////////////////////////////////////////////////////////////

// Enables Double animations (code size ~ 20KB)
#ifndef USE_DOUBLE_ANIMATIONS
    #define USE_DOUBLE_ANIMATIONS 1
#endif

// Enables Int16 animations (code size ~ 20KB)
#ifndef USE_INT16_ANIMATIONS
    #define USE_INT16_ANIMATIONS 1
#endif

// Enables Int32 animations (code size ~ 20KB)
#ifndef USE_INT32_ANIMATIONS
    #define USE_INT32_ANIMATIONS 1
#endif

// Enables Int64 animations (code size ~ 20KB)
#ifndef USE_INT64_ANIMATIONS
    #define USE_INT64_ANIMATIONS 1
#endif

// Enables Color animations (code size ~ 20KB)
#ifndef USE_COLOR_ANIMATIONS
    #define USE_COLOR_ANIMATIONS 1
#endif

// Enables Point animations (code size ~ 20KB)
#ifndef USE_POINT_ANIMATIONS
    #define USE_POINT_ANIMATIONS 1
#endif

// Enables Point3D animations (code size ~ 20KB)
#ifndef USE_POINT3D_ANIMATIONS
    #define USE_POINT3D_ANIMATIONS 1
#endif

// Enables Rect animations (code size ~ 20KB)
#ifndef USE_RECT_ANIMATIONS
    #define USE_RECT_ANIMATIONS 1
#endif

// Enables Size animations (code size ~ 20KB)
#ifndef USE_SIZE_ANIMATIONS
    #define USE_SIZE_ANIMATIONS 1
#endif

// Enables Thickness animations (code size ~ 20KB)
#ifndef USE_THICKNESS_ANIMATIONS
    #define USE_THICKNESS_ANIMATIONS 1
#endif

// Enables Boolean animations (code size ~ 10KB)
#ifndef USE_BOOLEAN_ANIMATIONS
    #define USE_BOOLEAN_ANIMATIONS 1
#endif

// Enables Object animations (code size ~ 10KB)
#ifndef USE_OBJECT_ANIMATIONS
    #define USE_OBJECT_ANIMATIONS 1
#endif

// Enables String animations (code size ~ 10KB)
#ifndef USE_STRING_ANIMATIONS
    #define USE_STRING_ANIMATIONS 1
#endif

// Enables Matrix animations (code size ~ 10KB)
#ifndef USE_MATRIX_ANIMATIONS
    #define USE_MATRIX_ANIMATIONS 1
#endif

#endif
