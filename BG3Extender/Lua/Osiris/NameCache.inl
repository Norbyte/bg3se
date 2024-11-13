#include <Lua/Osiris/NameCache.h>

BEGIN_NS(esv::lua)

void OsirisNameCache::Register(OsiFunctionDef& fun, OsirisBinding& binding)
{
	auto arity = (uint32_t)fun.Signature->Params->Params.Size;
	if (namesByArity.size() < arity + 1) {
		namesByArity.resize(arity + 1);
	}

	namesByArity[arity].Bind(&fun, binding);

	auto inputArgs = arity - fun.Signature->OutParamList.numOutParams();
	if (inputArgsToArity.size() < inputArgs + 1) {
		inputArgsToArity.resize(inputArgs + 1);
	}

	inputArgsToArity[inputArgs] = arity;
}


OsiFunction const* OsirisNameCache::GetFunction(uint32_t callerArity) const
{
	if (callerArity < inputArgsToArity.size() && inputArgsToArity[callerArity]) {
		return &namesByArity[*inputArgsToArity[callerArity]];
	} else {
		return nullptr;
	}
}


OsirisNameResolver::OsirisNameResolver(OsirisBinding& binding, OsirisStaticGlobals const& globals)
	: binding_(binding),
	globals_(globals)
{}

void OsirisNameResolver::InvalidateCache()
{
	needsRebuild_ = true;
	caches_.clear();
}

void OsirisNameResolver::RebuildCacheIfNecessary()
{
	if (!needsRebuild_) return;

	if ((*globals_.Functions) == nullptr) {
		return;
	}

	needsRebuild_ = false;
	caches_.clear();
	caches_.resize(nextCache_);

	if (!binding_.GetIdentityAdapterMap().HasAnyAdapters()) {
		OsiErrorS("Couldn't bind Osiris queries - no adapters available (maybe story compile or merge failed?)");
		return;
	}

	auto const& db = *globals_.Functions;
	db->Iterate([&](OsiString const&, OsiFunctionDef* fun) {
		Register(*fun);
	});
}


std::optional<OsirisNameCache::NameHandle> OsirisNameResolver::GetNameIndex(FixedString const& name)
{
	RebuildCacheIfNecessary();

	auto it = nameToCache_.find(name);
	if (it != nameToCache_.end()) {
		return caches_[it.Value()].handle;
	} else {
		return {};
	}
}


std::optional<OsirisNameCache::NameHandle> OsirisNameResolver::GetLegacyNameIndex(FixedString const& name)
{
	RebuildCacheIfNecessary();

	STDString legacyName(name.GetStringView());
	std::transform(legacyName.begin(), legacyName.end(), legacyName.begin(), [](unsigned char c) { return std::tolower(c); });

	auto it = legacyNameToCache_.find(FixedString(legacyName));
	if (it != legacyNameToCache_.end()) {
		return caches_[it.Value()].handle;
	} else {
		return {};
	}
}


FixedString OsirisNameResolver::GetName(OsirisNameCache::NameHandle handle)
{
	auto cache = GetCache(handle);
	return cache ? cache->name : FixedString{};
}


OsirisNameCache const* OsirisNameResolver::GetCache(OsirisNameCache::NameHandle handle)
{
	RebuildCacheIfNecessary();

	auto index = (uint32_t)(handle & 0xffffffffull);
	if (index < caches_.size() && caches_[index].handle == handle) {
		return &caches_[index];
	} else {
		return nullptr;
	}
}


OsiFunction const* OsirisNameResolver::GetFunction(OsirisNameCache::NameHandle handle, uint32_t callerArity)
{
	auto cache = GetCache(handle);
	return cache ? cache->GetFunction(callerArity) : nullptr;
}


void OsirisNameResolver::Register(OsiFunctionDef& fun)
{
	FixedString name{ fun.Signature->Name };
	auto index = nameToCache_.try_get(name);
	if (index) {
		auto& cache = caches_[*index];
		cache.handle = *index | ((uint64_t)name.Index << 32);
		cache.name = name;
		cache.Register(fun, binding_);
	} else {
		auto handle = caches_.size() | ((uint64_t)name.Index << 32);

		nameToCache_.set(name, caches_.size());
		STDString legacyName(fun.Signature->Name);
		std::transform(legacyName.begin(), legacyName.end(), legacyName.begin(), [](unsigned char c) { return std::tolower(c); });
		legacyNameToCache_.set(FixedString(legacyName), caches_.size());

		auto& cache = caches_.push_back(OsirisNameCache{});
		cache.handle = handle;
		cache.name = name;
		cache.Register(fun, binding_);
		nextCache_ = caches_.size();
	}
}

}
