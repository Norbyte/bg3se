#pragma once

#include <cstdint>
#include <unordered_map>
#include <GameDefinitions/Osiris.h>

namespace bg3se
{
	struct OsirisGlobals;

	class IdentityAdapterMap
	{
	public:
		const unsigned MaxColumns = 16;

		IdentityAdapterMap(OsirisStaticGlobals const &);

		void UpdateAdapters();
		bool HasAnyAdapters() const;
		bool HasAllAdapters() const;
		Adapter * FindAdapter(uint8_t columns) const;

	private:
		void TryAddAdapter(Adapter * adapter);

		OsirisStaticGlobals const & globals_;
		// Mapping of a rule action to its call site (rule then part, goal init/exit)
		std::unordered_map<uint8_t, Adapter *> adapters_;
	};
}
