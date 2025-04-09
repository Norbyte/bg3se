#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>

BEGIN_NS(input)

using InputPlayerId = uint8_t;
using InputRawType = uint16_t;
using InputDeviceId = uint16_t;
using InputEventId = uint32_t;

static constexpr unsigned NumRawInputs = 186;
static constexpr unsigned NumPlayers = 4;

enum class EInputDeviceId : int16_t
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
	InputDeviceId DeviceId;
	std::array<float, 4> field_14;
	uint8_t field_24;
};


struct InputEventDesc
{
	[[bg3::readonly]] int EventID;
	uint32_t Flags;
	InputType Type;
	FixedString CategoryName;
	[[bg3::hidden]] std::array<void*, 4> EventListeners;
	[[bg3::readonly]] LSStringView EventName;
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

struct [[bg3::hidden]] InjectTextData
{
	InputDeviceId DeviceId;
	char Text[64];
};

struct InjectDeviceEvent
{
	uint32_t EventId;
	InputDeviceId DeviceId;
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
	InputDeviceId DeviceId;
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

struct [[bg3::hidden]] InputEventListener
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
	virtual uint16_t* OnUnlinkedInput(uint16_t* eventRetVal, InputDeviceId inputDeviceId) = 0;
};

struct [[bg3::hidden]] InputListenerGroup
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
	[[bg3::hidden]] void* InputRemapListener;
	InputBinding Binding;
};

struct RawBindingDataMapping
{
	Array<InputBindingDesc> Bindings;
	bool HasModifierKeys;
};

struct InputScheme : public ProtectedGameObject<InputScheme>
{
	std::array<LegacyRefMap<InputDeviceId, InputDeviceId>, NumPlayers> ControllerRemaps;
	std::array<RawBindingDataMapping, NumRawInputs>* RawToBinding;
	std::array<LegacyRefMap<InputEventId, LegacyArray<InputBinding>>, NumPlayers> InputBindings;
	[[bg3::hidden]] Array<void*> DeviceIdToPlayerId; // FIXME! MAP Array<InputDeviceId, int32_t>
	std::array<Array<InputDeviceId>, NumPlayers> DeviceLists;
	[[bg3::hidden]] std::array<Array<void*>, NumPlayers> field_D8; // FIXME! MAP 
	std::array<HashSet<InputEventId>, NumPlayers> DebugEvents;
};

struct InputValueSet
{
	std::array<InputValue, NumRawInputs> Inputs;
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

struct InputManager : public ProtectedGameObject<InputManager>
{
	[[bg3::hidden]] CRITICAL_SECTION CS;
	[[bg3::hidden]] UnknownSignal OnInputEvent;
	HashMap<uint32_t, HoldRepeatEvents> HoldRepeatEvents;
	HashMap<InputDeviceId, InputValueSet*> InputStates;
	LegacyRefMap<int32_t, InputEventDesc> InputDefinitions;
	InputScheme InputScheme;
	InputModifier PressedModifiers;
	double LastUpdateTime;
	bool ControllerAllowKeyboardMouseInput;
	bool AllowDeviceEvents;
	[[bg3::hidden]] InputListenerGroup InputListeners;
	[[bg3::hidden]] InputListenerGroup DeviceListeners;
	[[bg3::hidden]] InputListenerGroup NotifyUpListeners;
	Array<InputRaw> RawInputs;
	std::array<int, NumPlayers> PlayerDevices;
	std::array<InputDeviceId, NumPlayers> PlayerDeviceIDs;
	PendingInputRemap CurrentRemap;
#if 0 // Editor only
	// std::array<Array<uint32_t>, 12> RawInputTypes;
#endif
	Array<FireEventDesc> Events;
	[[bg3::hidden]] Array<InjectTextData> TextInjects;
	Array<InjectInputData> InputInjects;
	Array<InjectDeviceEvent> DeviceEventInjects;
	std::array<glm::vec4, NumPlayers> PlayerColors;
	std::array<InputDevice, 12> PerDeviceData;
	[[bg3::hidden]] void* KeyboardLayoutTranslator;
};

END_NS()
