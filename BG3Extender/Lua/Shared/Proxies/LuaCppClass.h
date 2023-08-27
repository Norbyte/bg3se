#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/Proxies/LuaPropertyMap.h>

BEGIN_NS(lua)

class ArrayProxyImplBase;
class MapProxyImplBase;

class CppPropertyMapManager
{
public:
	int RegisterPropertyMap(GenericPropertyMap* mt);
	GenericPropertyMap* GetPropertyMap(int index);
	
	int RegisterArrayProxy(ArrayProxyImplBase* mt);
	ArrayProxyImplBase* GetArrayProxy(int index);
	
	int RegisterMapProxy(MapProxyImplBase* mt);
	MapProxyImplBase* GetMapProxy(int index);

private:
	Array<GenericPropertyMap*> propertyMaps_;
	Array<ArrayProxyImplBase*> arrayProxies_;
	Array<MapProxyImplBase*> mapProxies_;
};

END_NS()
