#pragma once

BEGIN_NS(lua)

class ArrayProxyImplBase;
class MapProxyImplBase;
class SetProxyImplBase;

class CppPropertyMapManager
{
public:
    void RegisterComponents(ecs::EntitySystemHelpersBase& helpers);
    
    int RegisterArrayProxy(ArrayProxyImplBase* mt);
    ArrayProxyImplBase* GetArrayProxy(int index);
    
    int RegisterMapProxy(MapProxyImplBase* mt);
    MapProxyImplBase* GetMapProxy(int index);
    
    int RegisterSetProxy(SetProxyImplBase* mt);
    SetProxyImplBase* GetSetProxy(int index);

private:
    Array<ArrayProxyImplBase*> arrayProxies_;
    Array<MapProxyImplBase*> mapProxies_;
    Array<SetProxyImplBase*> setProxies_;
};

END_NS()
