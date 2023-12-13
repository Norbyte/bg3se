#include <Lua/Shared/Proxies/LuaCppClass.h>

BEGIN_NS(lua)

int CppPropertyMapManager::RegisterPropertyMap(GenericPropertyMap* mt)
{
	propertyMaps_.push_back(mt);
	return (int)(propertyMaps_.Size() - 1);
}

GenericPropertyMap* CppPropertyMapManager::GetPropertyMap(int index)
{
	assert(index >= 0 && (uint32_t)index < propertyMaps_.Size());
	return propertyMaps_[index];
}

void CppPropertyMapManager::UpdateInheritance()
{
	auto pendingUpdates = propertyMaps_;
	Array<GenericPropertyMap*> nextBatchUpdates;

	do {
		bool progressed{ false };
		for (auto pm : pendingUpdates) {
			assert(!pm->InheritanceUpdated);
			if (pm->Parent == nullptr) {
				pm->InheritanceUpdated = true;
				progressed = true;
			} else if (pm->Parent->InheritanceUpdated) {
				InheritProperties(*pm->Parent, *pm);
				progressed = true;
			} else {
				nextBatchUpdates.push_back(pm);
			}
		}

		pendingUpdates = nextBatchUpdates;
		nextBatchUpdates.clear();
		assert(progressed && "Recursion in property map inheritance tree?");
	} while (!pendingUpdates.empty());
}

void CppPropertyMapManager::RegisterComponents(ecs::EntitySystemHelpersBase& helpers)
{
	for (auto pm : propertyMaps_) {
		if (pm->ComponentType) {
			helpers.BindPropertyMap(*pm->ComponentType, pm);
		}
	}
}

int CppPropertyMapManager::RegisterArrayProxy(ArrayProxyImplBase* mt)
{
	arrayProxies_.push_back(mt);
	return (int)(arrayProxies_.Size() - 1);
}

ArrayProxyImplBase* CppPropertyMapManager::GetArrayProxy(int index)
{
	assert(index >= 0 && (uint32_t)index < arrayProxies_.Size());
	return arrayProxies_[index];
}

int CppPropertyMapManager::RegisterMapProxy(MapProxyImplBase* mt)
{
	mapProxies_.push_back(mt);
	return (int)(mapProxies_.Size() - 1);
}

MapProxyImplBase* CppPropertyMapManager::GetMapProxy(int index)
{
	assert(index >= 0 && (uint32_t)index < mapProxies_.Size());
	return mapProxies_[index];
}

int CppPropertyMapManager::RegisterSetProxy(SetProxyImplBase* mt)
{
	setProxies_.push_back(mt);
	return (int)(setProxies_.Size() - 1);
}

SetProxyImplBase* CppPropertyMapManager::GetSetProxy(int index)
{
	assert(index >= 0 && (uint32_t)index < setProxies_.Size());
	return setProxies_[index];
}


END_NS()
