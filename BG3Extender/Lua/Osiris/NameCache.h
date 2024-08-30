#pragma once

#include <GameDefinitions/Osiris.h>
#include <Lua/Osiris/Function.h>

BEGIN_NS(esv::lua)

struct OsirisNameCache
{
	using NameHandle = uint64_t;

	NameHandle handle;
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

	inline bool IsInitialized() const
	{
		return !needsRebuild_;
	}

	void InvalidateCache();
	void RebuildCacheIfNecessary();
	std::optional<OsirisNameCache::NameHandle> GetNameIndex(FixedString const& name);
	std::optional<OsirisNameCache::NameHandle> GetLegacyNameIndex(FixedString const& name);
	FixedString GetName(OsirisNameCache::NameHandle handle);
	OsirisNameCache const* GetCache(OsirisNameCache::NameHandle handle);
	OsiFunction const* GetFunction(OsirisNameCache::NameHandle handle, uint32_t callerArity);

private:
	OsirisStaticGlobals const& globals_;
	OsirisBinding& binding_;
	Array<OsirisNameCache> caches_;
	HashMap<FixedString, uint32_t> nameToCache_;
	HashMap<FixedString, uint32_t> legacyNameToCache_;
	uint32_t nextCache_{ 0 };
	bool needsRebuild_{ true };

	void Register(Function& fun);
};

END_NS()
