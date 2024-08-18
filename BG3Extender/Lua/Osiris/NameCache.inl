#include <Lua/Osiris/NameCache.h>

BEGIN_NS(esv::lua)

void OsirisNameCache::Register(Function& fun, OsirisBinding& binding)
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

void OsirisNameResolver::UpdateCache()
{
	caches_.clear();
	caches_.resize(nextCache_);

	if ((*globals_.Functions) == nullptr) {
		return;
	}

	auto const& db = *globals_.Functions;
	db->Iterate([&](OsiString const&, Function* fun) {
		Register(*fun);
	});
}


std::optional<uint32_t> OsirisNameResolver::GetNameIndex(FixedString const& name) const
{
	auto it = nameToCache_.find(name);
	if (it != nameToCache_.end()) {
		return it.Value();
	} else {
		return {};
	}
}


FixedString OsirisNameResolver::GetName(uint32_t index) const
{
	return caches_[index].name;
}


OsirisNameCache const* OsirisNameResolver::GetCache(uint32_t index) const
{
	return &caches_[index];
}


OsiFunction const* OsirisNameResolver::GetFunction(uint32_t index, uint32_t callerArity) const
{
	return caches_[index].GetFunction(callerArity);
}


void OsirisNameResolver::Register(Function& fun)
{
	FixedString name{ fun.Signature->Name };
	auto index = nameToCache_.try_get(name);
	if (index) {
		auto& cache = caches_[*index];
		cache.name = name;
		cache.Register(fun, binding_);
	} else {
		nameToCache_.set(name, caches_.size());
		nextCache_ = caches_.size();
		auto& cache = caches_.push_back(OsirisNameCache{});
		cache.name = name;
		cache.Register(fun, binding_);
	}
}

}
