////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYGESTURE_H__
#define __GUI_KEYGESTURE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputGesture.h>
#include <NsGui/InputEnums.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/StringFwd.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a keyboard combination that can be used to invoke a command.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.keygesture.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API KeyGesture: public InputGesture
{
public:
    KeyGesture();
    KeyGesture(Key key, ModifierKeys modifiers = ModifierKeys_None);
    ~KeyGesture();

    /// Gets the key associated with this KeyGesture
    Key GetKey() const;

    /// Gets the modifier keys associated with this KeyGesture
    ModifierKeys GetModifiers() const;

    /// Gets a string to be displayed in UI
    /// \prop
    String GetDisplayString() const;

    /// From InputGesture
    //@{
    bool Matches(BaseComponent* target, const RoutedEventArgs& args) override;
    //@}

private:
    Key mKey;
    ModifierKeys mModifiers;

    NS_DECLARE_REFLECTION(KeyGesture, InputGesture)
};

}


#endif
