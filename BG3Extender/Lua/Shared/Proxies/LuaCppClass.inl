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


END_NS()
