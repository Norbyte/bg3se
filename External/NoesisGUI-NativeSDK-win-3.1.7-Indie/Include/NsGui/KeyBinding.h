////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYBINDING_H__
#define __GUI_KEYBINDING_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputBinding.h>


namespace Noesis
{

class KeyGesture;
enum Key: int32_t;
enum ModifierKeys: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Binds a KeyGesture to a Command.
///
/// When defining a KeyBinding in XAML there are two ways to specify the KeyGesture. The first way
/// to establish a KeyBinding in XAML is to define the Gesture attribute of the KeyBinding element,
/// which enables a syntax to specify keys and modifiers as a single string, for example "CTRL+P".
/// The second way is to define the Key attribute and the Modifiers attributes of the KeyBinding
/// element. Both ways of setting the KeyGesture are equivalent and modify the same underlying
/// object, but there will be a conflict if both are used.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
///      <Grid.InputBindings>
///        <KeyBinding Gesture="Ctrl+O" Command="ApplicationCommands.Open"/>
///      </Grid.InputBindings>
///    </Grid>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.keybinding.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API KeyBinding: public InputBinding
{
public:
    KeyBinding();
    KeyBinding(ICommand* command, KeyGesture* gesture);
    KeyBinding(ICommand* command, Key key, ModifierKeys modifiers);
    ~KeyBinding();

    /// Gets or sets the Key of the KeyGesture associated with this KeyBinding
    //@{
    Key GetKey() const;
    void SetKey(Key key);
    //@}

    /// Gets the modifier keys (one or more Alt, Ctrl, Shift) of the associated KeyGesture
    //@{
    ModifierKeys GetModifiers() const;
    void SetModifiers(ModifierKeys modifiers);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<KeyBinding> Clone() const;
    Ptr<KeyBinding> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* KeyProperty;
    static const DependencyProperty* ModifiersProperty;

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From InputBinding
    //@{
    void OnGestureChanged(InputGesture* oldGesture, InputGesture* newGesture) override;
    //@}

private:
    void UpdateKeyAndModifers(KeyGesture* keyGesture);
    void UpdateGesture(Key key, ModifierKeys modifiers);

    NS_DECLARE_REFLECTION(KeyBinding, InputBinding)
};

}


#endif
