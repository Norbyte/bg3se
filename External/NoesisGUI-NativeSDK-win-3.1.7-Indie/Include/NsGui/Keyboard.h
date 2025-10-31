////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_KEYBOARD_H__
#define __GUI_KEYBOARD_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IView.h>
#include <NsGui/InputEnums.h>
#include <NsCore/Ptr.h>
#include <NsCore/Vector.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>


namespace Noesis
{

class View;
class UIElement;
class RoutedEvent;
class KeyboardNavigation;
struct DependencyPropertyChangedEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the keyboard device.
///
/// https://msdn.microsoft.com/en-us/library/system.windows.input.keyboard.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Keyboard: public BaseComponent
{
public:
    Keyboard(View* view);
    ~Keyboard();

    /// Resets keyboard state
    void ResetState();

    /// Gets the set of ModifierKeys that are currently pressed
    ModifierKeys GetModifiers() const;

    /// Gets the set of key states for the specified key
    KeyStates GetKeyStates(Key key) const;

    /// Determines whether the specified key is pressed
    bool IsKeyDown(Key key) const;

    /// Determines whether the specified key is released
    bool IsKeyUp(Key key) const;

    /// Determines whether the specified key is toggled
    bool IsKeyToggled(Key key) const;

    /// Indicates if a Keyboard input event is being processed
    bool IsProcessingKeyEvent() const;

    /// Gets the element that has keyboard focus
    UIElement* GetFocused() const;

    /// Sets keyboard focus on the specified element.
    /// Returns the element that has focus after calling the function; may be an element other than
    /// that specified, even a null element
    UIElement* Focus(UIElement* element);

    /// Clears focus
    void ClearFocus();

public:
    /// Attached routed events
    //@{
    static const RoutedEvent* GotKeyboardFocusEvent;
    static const RoutedEvent* KeyDownEvent;
    static const RoutedEvent* KeyUpEvent;
    static const RoutedEvent* LostKeyboardFocusEvent;
    static const RoutedEvent* PreviewGotKeyboardFocusEvent;
    static const RoutedEvent* PreviewKeyDownEvent;
    static const RoutedEvent* PreviewKeyUpEvent;
    static const RoutedEvent* PreviewLostKeyboardFocusEvent;
    //@}

private:
    friend class View;
    friend class ViewLayout;

    /// Indicates if keyboard requires to refresh focused element state
    bool NeedsUpdate() const;
    void Update();

    bool Char(uint32_t ch);
    bool KeyDown(Key key);
    bool KeyUp(Key key);

    void UpdateKey(Key key, bool isKeyDown);

    bool IsFocusable(UIElement* element) const;
    void Focus(UIElement* element, bool askOld, bool askNew, bool canBeNull);
    void ReevaluateFocus();

    void Activate();
    void Deactivate();

    void RegisterFocused();
    void UnregisterFocused();
    void OnFocusedAncestorChanged(FrameworkElement* element);
    void OnFocusedIsVisibleChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& e);
    void OnFocusedIsEnabledChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& e);
    void OnFocusedFocusableChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& e);

    friend class FrameworkElement;
    KeyboardNavigation* GetKeyboardNavigation() const;

private:
    // Last keyboard key states
    KeyStates mKeyStates[Key_Count];

    View* mView;

    Ptr<UIElement> mFocusedElement;
    Ptr<UIElement> mRestoreFocusedElement;

    typedef Vector<Ptr<UIElement>> Elements;
    Elements mFocusWithinElements;

    Ptr<KeyboardNavigation> mKeyboardNavigation;

    union
    {
        struct
        {
            bool inactive : 1;
            bool processingKey : 1;
            bool keyDownHandled : 1;
            bool reevaluateFocus : 1;
        } mFlags;

        // To quickly set all flags to 0
        uint32_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(Keyboard, BaseComponent)
};

NS_WARNING_POP

}


#endif
