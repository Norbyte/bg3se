#pragma once

#include <GameDefinitions/Osiris.h>
#include <Lua/Osiris/Function.h>

BEGIN_NS(esv::lua)

struct OsirisNameCache
{
	FixedString name;
	Array<OsiFunction> namesByArity;
	Array<std::optional<uint32_t>> inputArgsToArity;

	void Register(Function& fun, OsirisBinding& binding);
	OsiFunction const* GetFunction(uint32_t callerArity) const;
};

class OsirisNameResolver : Noncopyable<OsirisNameResolver>
{
public:
	OsirisNameResolver(OsirisBinding& binding, OsirisStaticGlobals const&);

	void UpdateCache();
	std::optional<uint32_t> GetNameIndex(FixedString const& name) const;
	FixedString GetName(uint32_t index) const;
	OsirisNameCache const* GetCache(uint32_t index) const;
	OsiFunction const* GetFunction(uint32_t index, uint32_t callerArity) const;

private:
	OsirisStaticGlobals const& globals_;
	OsirisBinding& binding_;
	Array<OsirisNameCache> caches_;
	MultiHashMap<FixedString, uint32_t> nameToCache_;

	void Register(Function& fun);
};

END_NS()
