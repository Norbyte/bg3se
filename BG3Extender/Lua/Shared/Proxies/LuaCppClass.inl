#include <Lua/Shared/Proxies/LuaCppClass.h>

BEGIN_NS(lua)

void CppPropertyMapManager::RegisterComponents(ecs::EntitySystemHelpersBase& helpers)
{
	for (auto pm : gStructRegistry.StructsById) {
		if (pm != nullptr && pm->ComponentType) {
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
