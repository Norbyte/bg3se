////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MOUSEGESTURE_H__
#define __GUI_MOUSEGESTURE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputGesture.h>
#include <NsGui/InputEnums.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/StringFwd.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a mouse input gesture that can be used to invoke a command.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.mousegesture.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MouseGesture: public InputGesture
{
public:
    MouseGesture();
    MouseGesture(MouseAction key, ModifierKeys modifiers = ModifierKeys_None);
    ~MouseGesture();

    /// Gets the MouseAction associated with this gesture
    MouseAction GetMouseAction() const;

    /// Gets the modifier keys associated with this gesture
    ModifierKeys GetModifiers() const;

    /// From InputGesture
    //@{
    bool Matches(BaseComponent* target, const RoutedEventArgs& args) override;
    //@}

private:
    MouseAction mAction;
    ModifierKeys mModifiers;

    NS_DECLARE_REFLECTION(MouseGesture, InputGesture)
};

}


#endif
