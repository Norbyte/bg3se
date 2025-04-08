#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>

BEGIN_NS(input)

using InputPlayerId = uint8_t;
using InputRawType = uint16_t;
using InputEventId = uint32_t;

enum class InputDeviceId : int16_t
{
	C = -2,
	Unknown = -1,
	Key = 0,
	Mouse = 1,
	Controller1 = 3,
	Controller2 = 4,
	Controller3 = 5,
	Controller4 = 6,
	Controller5 = 7,
	Controller6 = 8,
	Controller7 = 9,
	Controller8 = 10,
	Touchbar = 11
};

FixedString InputDeviceIdToString(InputDeviceId id);
InputDeviceId StringToInputDeviceId(FixedString id);

struct InputValue
{
	float Value;
	float Value2;
	InputState State;
};


struct InputDevice
{
	int64_t InputPlayerIndex;
	int32_t field_8;
	int32_t ControllerMapping;
	int16_t DeviceId;
	std::array<float, 4> field_14;
	uint8_t field_24;
};


struct InputEventDesc
{
	int EventID;
	uint32_t Flags;
	InputType Type;
	FixedString CategoryName;
	void* EventListeners[4];
	StringView EventName;
	TranslatedString EventDesc;
	int field_E8;
};

struct InputRaw
{
	InputRawType InputId;
	InputDeviceId DeviceId;

	inline FixedString LuaGetDeviceId()
	{
		return InputDeviceIdToString(DeviceId);
	}

	inline void LuaSetDeviceId(FixedString id)
	{
		DeviceId = StringToInputDeviceId(id);
	}
};

struct InputRawChange
{
	InputRaw Input;
	InputValue Value;
	bool InjectRawInput;
	bool Debug;
};

struct InjectInputData : public InputRawChange
{
};

struct InjectTextData
{
	int16_t DeviceId;
	char Text[64];
};

struct InjectDeviceEvent
{
	uint32_t EventId;
	uint16_t DeviceId;
};

struct InputEventText
{
	InputPlayerId PlayerIndex;
	char const* Text;
	uint64_t TextLength;
};

struct InputEvent
{
	int EventId;
	InputDeviceId DeviceId;
	InputPlayerId InputPlayerIndex;
	InputType Type;
	InputValue OldValue;
	InputValue NewValue;
	bool WasPreferred;

	inline FixedString LuaGetDeviceId()
	{
		return InputDeviceIdToString(DeviceId);
	}

	inline void LuaSetDeviceId(FixedString id)
	{
		DeviceId = StringToInputDeviceId(id);
	}
};

struct FireEventDesc
{
	InputEventDesc* EventDesc;
	uint8_t PlayerIndex;
	InputEvent Event;
	int16_t DeviceId;
	int16_t InputId;
	int32_t field_34;
};


struct HoldRepeatEventDesc
{
	double ReleaseTime;
	double PressTime;
	double HoldTime;
	int Acceleration;
	FireEventDesc FireEvent;
};

struct InputEventListener
{
	virtual ~InputEventListener() = 0;
	virtual int GetInputListenerPriority() = 0;
	virtual uint64_t GetInputListenerFilter() = 0;
	virtual const char* GetInputListenerName() = 0;
	virtual bool IsModal() = 0;
	virtual bool InTextInput(int playerIndex) = 0;
	virtual uint16_t* OnInputEvent(uint16_t* eventRetVal, InputEvent* inputEvent) = 0;
	virtual void OnInputModifierChangedEvent(bool, bool, bool, bool) = 0;
	virtual uint16_t* OnInputEvent(uint16_t* eventRetVal, InputEventText* inputEvent) = 0;
	virtual uint16_t* OnUnlinkedInput(uint16_t* eventRetVal, uint16_t inputDeviceId) = 0;
};

struct InputListenerGroup
{
	int RefCount;
	Array<InputEventListener*> TraversingListeners;
	Array<InputEventListener*> Listeners;
	bool IsTraversing;
};

struct InputBinding : public InputRaw
{
	InputModifier Modifiers;
	uint8_t field_9;
	uint8_t InputPlayerIndex;
};

struct InputBindingDesc
{
	int32_t InputIndex;
	InputBinding Binding;
	int32_t BindingIndex;
};

struct PendingInputRemap
{
	void* InputRemapListener;
	InputBinding Binding;
};

struct RawBindingDataMapping
{
	Array<InputBindingDesc> Bindings;
	bool HasModifierKeys;
};

struct InputScheme
{
	struct BindingSet
	{
		Array<InputBindingDesc> Bindings;
		bool Initialized;
	};

	std::array<LegacyRefMap<uint16_t, uint16_t>, 4> ControllerRemaps;
	std::array<RawBindingDataMapping, 186>* RawToBinding;
	std::array<LegacyRefMap<InputEventId, LegacyArray<InputBinding>>, 4> InputBindings;
	Array<void*> DeviceIdToPlayerId; // FIXME! MAP Array<uint16_t, int32_t>
	std::array<Array<InputDeviceId>, 4> DeviceLists;
	std::array<Array<void*>, 4> field_D8; // FIXME! MAP 
	std::array<HashSet<InputEventId>, 4> DebugEvents;
};

struct InputValueSet
{
	std::array<InputValue, 186> Inputs;
	bool Initialized;
};

struct HoldRepeatEvents
{
	Array<FireEventDesc> PressEvents;
	Array<FireEventDesc> ReleaseEvents;
	Array<FireEventDesc> ValueChangeEvents;
	Array<FireEventDesc> HoldEvents;
	Array<FireEventDesc> RepeatEvents;
	Array<HoldRepeatEventDesc> HoldRepeatEvents;
};

struct InputManager
{
	using GetInstanceProc = InputManager * ();
	using InjectInputProc = bool (InputManager*, InjectInputData const&);

	CRITICAL_SECTION CS;
	UnknownSignal OnInputEvent;
	HashMap<uint32_t, HoldRepeatEvents> HoldRepeatEvents;
	HashMap<uint16_t, InputValueSet*> InputStates;
	LegacyRefMap<int32_t, InputEventDesc> InputDefinitions;
	InputScheme InputScheme;
	InputModifier PressedModifiers;
	double LastUpdateTime;
	bool ControllerAllowKeyboardMouseInput;
	bool AllowDeviceEvents;
	InputListenerGroup InputListeners;
	InputListenerGroup DeviceListeners;
	InputListenerGroup NotifyUpListeners;
	Array<InputRaw> RawInputs;
	std::array<int, 4> PlayerDevices;
	std::array<InputDeviceId, 4> PlayerDeviceIDs;
	PendingInputRemap CurrentRemap;
#if !defined(OSI_EOCAPP)
	std::array<Array<uint32_t>, 12> RawInputTypes;
#endif
	Array<FireEventDesc> Events;
	Array<InjectTextData> TextInjects;
	Array<InjectInputData> InputInjects;
	Array<InjectDeviceEvent> DeviceEventInjects;
	glm::vec4 PlayerColors[4];
	std::array<InputDevice, 12> PerDeviceData;
	void* KeyboardLayoutTranslator;
};

END_NS()
