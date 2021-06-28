#include <Extender/Shared/ModuleHasher.h>

BEGIN_SE()

__declspec(thread) unsigned ModuleHasher::hashDepth_{ 0 };

void ModuleHasher::PostStartup()
{
	using namespace std::placeholders;
	/*gExtender->GetLibraryManager().Module__Hash.SetWrapper(
		std::bind(&ModuleHasher::OnModuleHash, this, _1, _2)
	);*/
}

void ModuleHasher::ClearCaches()
{
	std::lock_guard _(mutex_);
	hashCache_.clear();
}

bool ModuleHasher::FetchHashFromCache(Module& mod)
{
	auto it = hashCache_.find(mod.Info.ModuleUUIDString);
	if (it != hashCache_.end()) {
		mod.Info.Hash = it->second;
		mod.HasValidHash = true;
		UpdateDependencyHashes(mod);
		return true;
	}
	else {
		return false;
	}
}

void ModuleHasher::UpdateDependencyHashes(Module& mod)
{
	/*for (auto& dependency : mod.DependentModules) {
		if (!FetchHashFromCache(dependency)) {
			GetStaticSymbols().Module__Hash(&dependency);
		}
	}

	for (auto& addon : mod.AddonModules) {
		if (!FetchHashFromCache(addon)) {
			GetStaticSymbols().Module__Hash(&addon);
		}
	}*/
}

bool ModuleHasher::OnModuleHash(Module::HashProc* next, Module* self)
{
	std::lock_guard _(mutex_);

	if (FetchHashFromCache(*self)) {
		return true;
	}

	if (!hashStack_.empty()) {
		auto mod = *hashStack_.rbegin();
		if (!mod->Info.Hash.empty()) {
			hashCache_.insert(std::make_pair(mod->Info.ModuleUUIDString, mod->Info.Hash));
		}
	}

	hashStack_.push_back(self);

	hashDepth_++;
	auto ok = next(self);
	hashDepth_--;
	if (!self->Info.Hash.empty()) {
		hashCache_.insert(std::make_pair(self->Info.ModuleUUIDString, self->Info.Hash));
	}

	hashStack_.pop_back();
	return ok;
}

END_SE()
