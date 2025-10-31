////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MOUSEBINDING_H__
#define __GUI_MOUSEBINDING_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputBinding.h>


namespace Noesis
{

class MouseGesture;
enum MouseAction: int32_t;
enum ModifierKeys: int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Binds a MouseGesture to a Command.
///
/// When defining a MouseBinding in XAML, there are two ways to specify the MouseGesture. The first
/// way to establish a MouseBinding in XAML is to define the Gesture attribute of the MouseBinding
/// element, which enables a syntax to specify mouse actions and modifiers as a single string; for
/// example, "CTRL+LeftClick". The second way is to define the MouseAction attribute of the
/// MouseBinding element. Both ways of setting the MouseGesture are equivalent and modify the same
/// underlying object, but there will be a conflict if both are used.
///
/// .. code-block:: xml
///
///    <Grid
///      xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
///      <Grid.InputBindings>
///        <MouseBinding Gesture="Ctrl+MiddleClick" Command="{Binding MiddleClickCommand}"/>
///      </Grid.InputBindings>
///    </Grid>
///
/// http://msdn.microsoft.com/en-us/library/system.windows.input.mousebinding.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MouseBinding: public InputBinding
{
public:
    MouseBinding();
    MouseBinding(ICommand* command, MouseGesture* gesture);
    MouseBinding(ICommand* command, MouseAction action, ModifierKeys modifiers);
    ~MouseBinding();

    /// Gets or sets the MouseAction of the MouseGesture associated with this MouseBinding
    //@{
    MouseAction GetMouseAction() const;
    void SetMouseAction(MouseAction action);
    //@}

    /// Gets the modifier keys (one or more Alt, Ctrl, Shift) of the associated MouseGesture
    //@{
    ModifierKeys GetModifiers() const;
    void SetModifiers(ModifierKeys modifiers);
    //@}

    // Hides Freezable methods for convenience
    //@{
    Ptr<MouseBinding> Clone() const;
    Ptr<MouseBinding> CloneCurrentValue() const;
    //@}

public:
    static const DependencyProperty* MouseActionProperty;
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
    void UpdateActionAndModifers(MouseGesture* keyGesture);
    void UpdateGesture(MouseAction action, ModifierKeys modifiers);

    NS_DECLARE_REFLECTION(MouseBinding, InputBinding)
};

}


#endif
