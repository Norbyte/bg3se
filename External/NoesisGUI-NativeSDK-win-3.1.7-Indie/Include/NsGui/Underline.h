////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_UNDERLINE_H__
#define __GUI_UNDERLINE_H__


#include <NsCore/Noesis.h>
#include <NsGui/Span.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An inline-level flow content element which causes content to render with an underlined text
/// decoration.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.documents.underline.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Underline: public Span
{
public:
    Underline();
    Underline(Inline* childInline);

    NS_DECLARE_REFLECTION(Underline, Span)
};

}


#endif
