////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRIDLENGTH_H__
#define __GUI_GRIDLENGTH_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/StringFwd.h>


namespace Noesis
{

/// Describes the kind of value that a GridLength object is holding.
enum GridUnitType
{
    /// The size is determined by the size properties of the content object.
    GridUnitType_Auto,
    /// The value is expressed as a pixel.
    GridUnitType_Pixel,
    /// The value is expressed as a weighted proportion of available space.
    GridUnitType_Star
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the length of elements that explicitly support *Star* unit types. Elements such as
/// ColumnDefinition and RowDefinition use this type to describe width and height in order to
/// support variable distribution of available space.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.gridlength.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API GridLength
{
public:
    /// Initializes a new instance of GridLength with 1* default value
    GridLength();

    /// Initializes a new instance of GridLength using the specified absolute value in pixels
    GridLength(float value);

    /// Initializes a new instance of GridLength and specifies what kind of value it holds
    GridLength(float value, GridUnitType type);

    /// Associated GridUnitType for the GridLength
    GridUnitType GetGridUnitType() const;

    /// Indicates whether the GridLength holds a value that is expressed in pixels
    /// \prop
    bool IsAbsolute() const;

    /// Indicates whether the GridLength holds a value whose size is determined by the size
    /// properties of the content object
    /// \prop
    bool IsAuto() const;

    /// Indicates whether the GridLength holds a value that is expressed as a weighted proportion of 
    /// available space
    /// \prop
    bool IsStar() const;

    /// Gets a float that represents the value of the GridLength
    float GetValue() const;

    /// Copy constructor
    GridLength(const GridLength& gridLength);

    // Copy operator
    GridLength& operator=(const GridLength& gridLength);

    // Comparison operators
    //@{
    bool operator==(const GridLength& gridLength) const;
    bool operator!=(const GridLength& gridLength) const;
    //@}

    /// Creates a string representation of this structure
    String ToString() const;

    /// Tries to parse a GridLength from a string
    static bool TryParse(const char* str, GridLength& result);

private:
    GridUnitType mGridUnitType;
    float mValue;

    NS_DECLARE_REFLECTION(GridLength, NoParent)
};

}


#endif
