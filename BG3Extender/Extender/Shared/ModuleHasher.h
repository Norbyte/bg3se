#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Module.h>
#include <mutex>
#include <unordered_map>

BEGIN_SE()

class ModuleHasher
{
public:
	void PostStartup();
	void ClearCaches();

	bool isHashing() const
	{
		return hashDepth_ > 0;
	}

private:
	std::vector<Module*> hashStack_;
	std::unordered_map<FixedString, STDString> hashCache_;
	std::recursive_mutex mutex_;
	static __declspec(thread) unsigned hashDepth_;

	bool FetchHashFromCache(Module& mod);
	void UpdateDependencyHashes(Module& mod);
	bool OnModuleHash(Module::HashProc* next, Module* self);
};

END_SE()
