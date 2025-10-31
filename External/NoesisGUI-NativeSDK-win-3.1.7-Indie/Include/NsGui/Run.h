
////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RUN_H__
#define __GUI_RUN_H__


#include <NsCore/Noesis.h>
#include <NsGui/Inline.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An inline-level flow content element intended to contain a run of formatted or unformatted text.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.documents.run.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Run : public Inline
{
public:
    Run();
    Run(const char* text);

    /// Gets or sets the unformatted text contents of this text Run
    //@{
    const char* GetText() const;
    void SetText(const char* text);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* TextProperty;
    //@}

    NS_DECLARE_REFLECTION(Run, Inline)
};

}


#endif
