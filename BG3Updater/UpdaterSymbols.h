#pragma once

#include <BG3Extender/GameDefinitions/TranslatedString.h>

BEGIN_SE()

struct SimplifiedGameStateMachine : public ProtectedGameObject<SimplifiedGameStateMachine>
{
	void* CurrentState;
	ecl::GameState State;
};

struct SimplifiedEoCClient : public ProtectedGameObject<SimplifiedEoCClient>
{
	using HandleErrorProc = void(void* self, TranslatedString const& message, bool exitGame, TranslatedString const& a4);

	uint8_t _Unmapped[0xA0];
	SimplifiedGameStateMachine* GameStateMachine;
};

struct UpdaterSymbols : Noncopyable<UpdaterSymbols>
{
	FixedString::CreateFromStringProc* ls__FixedString__CreateFromString{ nullptr };
	FixedString::GetStringProc* ls__FixedString__GetString{ nullptr };
	FixedString::IncRefProc* ls__FixedString__IncRef{ nullptr };
	FixedString::DecRefProc* ls__FixedString__DecRef{ nullptr };
	GlobalStringTable** ls__gGlobalStringTable{ nullptr };

	SimplifiedEoCClient** ecl__EoCClient{ nullptr };

	SimplifiedEoCClient::HandleErrorProc* ecl__EoCClient__HandleError{ nullptr };

	TranslatedStringRepository** ls__gTranslatedStringRepository{ nullptr };

	void* ls__gGlobalAllocator{ nullptr };
	ls__GlobalAllocator__AllocProc* ls__GlobalAllocator__Alloc{ nullptr };
	ls__GlobalAllocator__FreeProc* ls__GlobalAllocator__Free{ nullptr };

	inline UpdaterSymbols() {}
	UpdaterSymbols(UpdaterSymbols const &) = delete;
	UpdaterSymbols& operator = (UpdaterSymbols const &) = delete;

	inline SimplifiedEoCClient* GetEoCClient() const
	{
		if (ecl__EoCClient == nullptr || *ecl__EoCClient == nullptr) {
			return nullptr;
		} else {
			return *ecl__EoCClient;
		}
	}

	inline std::optional<ecl::GameState> GetClientState() const
	{
		if (ecl__EoCClient != nullptr
			&& *ecl__EoCClient != nullptr
			&& (*ecl__EoCClient)->GameStateMachine != nullptr
			&& !IsBadReadPtr((*ecl__EoCClient)->GameStateMachine, sizeof(SimplifiedGameStateMachine))) {
			return (*ecl__EoCClient)->GameStateMachine->State;
		} else {
			return {};
		}
	}
};

END_SE()
