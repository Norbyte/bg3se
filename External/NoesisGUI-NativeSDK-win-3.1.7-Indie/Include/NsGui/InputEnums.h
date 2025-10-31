////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_INPUTENUMS_H__
#define __GUI_INPUTENUMS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
enum MouseButton: int32_t
{
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,
    MouseButton_XButton1,
    MouseButton_XButton2,

    MouseButton_Count
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum MouseButtonState: int32_t
{
    MouseButtonState_Released,
    MouseButtonState_Pressed
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum MouseAction: int32_t
{
    MouseAction_None,
    MouseAction_LeftClick,
    MouseAction_RightClick,
    MouseAction_MiddleClick,
    MouseAction_WheelClick,
    MouseAction_LeftDoubleClick,
    MouseAction_RightDoubleClick,
    MouseAction_MiddleDoubleClick
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum Key: int32_t
{
    Key_None,

    Key_Cancel,
    Key_Back,
    Key_Tab,
    Key_LineFeed,
    Key_Clear,
    Key_Return,
    Key_Enter = Key_Return,
    Key_Pause,
    Key_Capital,
    Key_CapsLock = Key_Capital,

    Key_KanaMode,
    Key_HangulMode = Key_KanaMode,
    Key_JunjaMode,
    Key_FinalMode,
    Key_HanjaMode,
    Key_KanjiMode = Key_HanjaMode,

    Key_Escape,

    Key_ImeConvert,
    Key_ImeNonConvert,
    Key_ImeAccept,
    Key_ImeModeChange,

    Key_Space,

    Key_Prior,
    Key_PageUp = Key_Prior,
    Key_Next,
    Key_PageDown = Key_Next,
    Key_End,
    Key_Home,

    Key_Left,
    Key_Up,
    Key_Right,
    Key_Down,

    Key_Select,
    Key_Print,
    Key_Execute,
    Key_Snapshot,
    Key_PrintScreen = Key_Snapshot,
    Key_Insert,
    Key_Delete,
    Key_Help,

    Key_D0,
    Key_D1,
    Key_D2,
    Key_D3,
    Key_D4,
    Key_D5,
    Key_D6,
    Key_D7,
    Key_D8,
    Key_D9,

    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    Key_LWin,
    Key_RWin,
    Key_Apps,
    Key_Sleep,

    Key_NumPad0,
    Key_NumPad1,
    Key_NumPad2,
    Key_NumPad3,
    Key_NumPad4,
    Key_NumPad5,
    Key_NumPad6,
    Key_NumPad7,
    Key_NumPad8,
    Key_NumPad9,

    Key_Multiply,
    Key_Add,
    Key_Separator,
    Key_Subtract,
    Key_Decimal,
    Key_Divide,

    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_F13,
    Key_F14,
    Key_F15,
    Key_F16,
    Key_F17,
    Key_F18,
    Key_F19,
    Key_F20,
    Key_F21,
    Key_F22,
    Key_F23,
    Key_F24,

    Key_NumLock,
    Key_Scroll,

    Key_LeftShift,
    Key_RightShift,
    Key_LeftCtrl,
    Key_RightCtrl,
    Key_LeftAlt,
    Key_RightAlt,

    Key_BrowserBack,
    Key_BrowserForward,
    Key_BrowserRefresh,
    Key_BrowserStop,
    Key_BrowserSearch,
    Key_BrowserFavorites,
    Key_BrowserHome,

    Key_VolumeMute,
    Key_VolumeDown,
    Key_VolumeUp,

    Key_MediaNextTrack,
    Key_MediaPreviousTrack,
    Key_MediaStop,
    Key_MediaPlayPause,

    Key_LaunchMail,
    Key_SelectMedia,
    Key_LaunchApplication1,
    Key_LaunchApplication2,

    Key_Oem1,
    Key_OemSemicolon = Key_Oem1,
    Key_OemPlus,
    Key_OemComma,
    Key_OemMinus,
    Key_OemPeriod,
    Key_Oem2,
    Key_OemQuestion = Key_Oem2,
    Key_Oem3,
    Key_OemTilde = Key_Oem3,
    Key_AbntC1,
    Key_AbntC2,
    Key_Oem4,
    Key_OemOpenBrackets = Key_Oem4,
    Key_Oem5,
    Key_OemPipe = Key_Oem5,
    Key_Oem6,
    Key_OemCloseBrackets = Key_Oem6,
    Key_Oem7,
    Key_OemQuotes = Key_Oem7,
    Key_Oem8,
    Key_Oem102,
    Key_OemBackslash = Key_Oem102,
    Key_ImeProcessed,
    Key_System,
    Key_OemAttn,
    Key_DbeAlphanumeric = Key_OemAttn,
    Key_OemFinish,
    Key_DbeKatakana = Key_OemFinish,
    Key_OemCopy,
    Key_DbeHiragana = Key_OemCopy,
    Key_OemAuto,
    Key_DbeSbcsChar = Key_OemAuto,
    Key_OemEnlw,
    Key_DbeDbcsChar = Key_OemEnlw,
    Key_OemBackTab,
    Key_DbeRoman = Key_OemBackTab,
    Key_Attn,
    Key_DbeNoRoman = Key_Attn,
    Key_CrSel,
    Key_DbeEnterWordRegisterMode = Key_CrSel,
    Key_ExSel,
    Key_DbeEnterImeConfigureMode = Key_ExSel,
    Key_EraseEof,
    Key_DbeFlushString = Key_EraseEof,
    Key_Play,
    Key_DbeCodeInput = Key_Play,
    Key_Zoom,
    Key_DbeNoCodeInput = Key_Zoom,
    Key_NoName,
    Key_DbeDetermineString = Key_NoName,
    Key_Pa1,
    Key_DbeEnterDialogConversionMode = Key_Pa1,
    Key_OemClear,
    Key_DeadCharProcessed,

    Key_PageLeft,
    Key_PageRight,

    Key_GamepadLeft,
    Key_GamepadUp,
    Key_GamepadRight,
    Key_GamepadDown,
    Key_GamepadAccept,
    Key_GamepadCancel,
    Key_GamepadMenu,
    Key_GamepadView,
    Key_GamepadPageUp,
    Key_GamepadPageDown,
    Key_GamepadPageLeft,
    Key_GamepadPageRight,
    Key_GamepadContext1,
    Key_GamepadContext2,
    Key_GamepadContext3,
    Key_GamepadContext4,

    Key_Count
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum KeyStates: int32_t
{
    /// The key is not pressed (same as up).
    KeyStates_None = 0,

    /// The key is pressed.
    KeyStates_Down = 1,

    /// The key is toggled on.
    KeyStates_Toggled = 2
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum ModifierKeys: int32_t
{
    ModifierKeys_None = 0,
    ModifierKeys_Alt = 1,
    ModifierKeys_Control = 2,
    ModifierKeys_Shift = 4,
    ModifierKeys_Windows = 8
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Specifies how manipulation events are interpreted
////////////////////////////////////////////////////////////////////////////////////////////////////
enum ManipulationModes: int32_t
{
    /// Manipulation events do not occur
    ManipulationModes_None = 0,
    /// A manipulation can translate an object horizontally
    ManipulationModes_TranslateX = 1,
    /// A manipulation can translate an object vertically
    ManipulationModes_TranslateY = 2,
    /// A manipulation can translate an object
    ManipulationModes_Translate = ManipulationModes_TranslateX | ManipulationModes_TranslateY,
    /// A manipulation can rotate an object
    ManipulationModes_Rotate = 4,
    /// A manipulation can scale an object
    ManipulationModes_Scale = 8,
    /// A manipulation can scale, translate, or rotate an object
    ManipulationModes_All = ManipulationModes_Translate | ManipulationModes_Rotate |
        ManipulationModes_Scale
};

}

NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::MouseAction)
NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::Key)
NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::ModifierKeys)
NS_DECLARE_REFLECTION_ENUM_EXPORT(NS_GUI_CORE_API, Noesis::ManipulationModes)

#endif
