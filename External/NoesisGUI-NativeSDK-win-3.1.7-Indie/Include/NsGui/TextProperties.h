////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTPROPERTIES_H__
#define __GUI_TEXTPROPERTIES_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

/// Specifies whether the text is left-aligned, right-aligned, centered, or justified
enum TextAlignment: int32_t
{
    TextAlignment_Left,
    TextAlignment_Right,
    TextAlignment_Center,
    TextAlignment_Justify,
};

/// Describes how text is trimmed when it overflows the edge of its containing box
enum TextTrimming: int32_t
{
    /// Not trimmed
    TextTrimming_None,
    /// Trimmed at a character boundary. An ellipsis(...) is drawn in place of remaining text
    TextTrimming_CharacterEllipsis,
    /// Trimmed at a word boundary. An ellipsis(...) is drawn in place of remaining text
    TextTrimming_WordEllipsis
};

/// Specifies whether text wraps when it reaches the edge of the containing box
enum TextWrapping: int32_t
{
    /// No line wrapping is performed
    TextWrapping_NoWrap,
    /// Line-breaking occurs if the line overflows beyond the available block width, even if the
    /// standard line breaking algorithm cannot determine any line break opportunity, as in the
    /// case of a very long word constrained in a fixed-width container with no scrolling allowed
    TextWrapping_Wrap,
    /// Line-breaking occurs if the line overflows beyond the available block width. However, a line
    /// may overflow beyond the block width if the line breaking algorithm cannot determine a line
    /// break opportunity, as in the case of a very long word constrained in a fixed-width container
    /// with no scrolling allowed
    TextWrapping_WrapWithOverflow
};

/// Describes a mechanism by which a line box is determined for each line
enum LineStackingStrategy: int32_t
{
    /// The stack height is determined by the block element line-height property value.
    LineStackingStrategy_BlockLineHeight,
    /// The stack height is the smallest value that containing all the inline elements on that line
    /// when those elements are properly aligned
    LineStackingStrategy_MaxHeight
};

}

#endif
