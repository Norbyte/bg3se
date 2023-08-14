#pragma once

#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Misc.h>

namespace bg3se
{
	struct StaticSymbols : Noncopyable<StaticSymbols>
	{
		FixedString::CreateFromStringProc* ls__FixedString__CreateFromString{ nullptr };
		FixedString::GetStringProc* ls__FixedString__GetString{ nullptr };
		FixedString::IncRefProc* ls__FixedString__IncRef{ nullptr };
		FixedString::DecRefProc* ls__FixedString__DecRef{ nullptr };
		GlobalStringTable** ls__gGlobalStringTable{ nullptr };

		ecl::EoCClient** ecl__EoCClient{ nullptr };

		ecl::EoCClient::HandleErrorProc* ecl__EoCClient__HandleError{ nullptr };

		TranslatedStringRepository** ls__gTranslatedStringRepository{ nullptr };

		ls__GlobalAllocator__AllocProc* ls__GlobalAllocator__Alloc{ nullptr };
		ls__GlobalAllocator__FreeProc* ls__GlobalAllocator__Free{ nullptr };

		inline StaticSymbols() {}
		StaticSymbols(StaticSymbols const &) = delete;
		StaticSymbols & operator = (StaticSymbols const &) = delete;

		inline ecl::EoCClient * GetEoCClient() const
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
				&& (*ecl__EoCClient)->GameStateMachine != nullptr) {
				return (*ecl__EoCClient)->GameStateMachine->State;
			} else {
				return {};
			}
		}
	};

	extern StaticSymbols* gStaticSymbols;

	void InitStaticSymbols();

	inline StaticSymbols& GetStaticSymbols()
	{
		return *gStaticSymbols;
	}
}
