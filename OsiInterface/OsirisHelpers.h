#pragma once

#include <cstdint>
#include <unordered_map>
#include <GameDefinitions/Osiris.h>

namespace dse
{
	struct OsirisGlobals;

	class IdentityAdapterMap
	{
	public:
		const unsigned MaxColumns = 16;

		IdentityAdapterMap(OsirisStaticGlobals const &);

		void UpdateAdapters();
		bool HasAllAdapters();
		Adapter * FindAdapter(uint8_t columns);

	private:
		void TryAddAdapter(Adapter * adapter);

		OsirisStaticGlobals const & globals_;
		// Mapping of a rule action to its call site (rule then part, goal init/exit)
		std::unordered_map<uint8_t, Adapter *> adapters_;
	};
}
