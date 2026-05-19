#pragma once

#include <GameDefinitions/Effect.h>

BEGIN_SE()

template <class T>
struct [[bg3::hidden]] ObjectFactory : public ProtectedGameObject<ObjectFactory<T>>
{
    virtual void ReevaluateHandle(ComponentHandle&) const = 0;
    virtual ComponentHandle GetFreeHandle() = 0;
    virtual bool IsAlive(ComponentHandle const&)const = 0;
    virtual bool IsReserved(ComponentHandle const&)const = 0;
    virtual void Reserve(ComponentHandle const&) = 0;
    virtual void Unreserve(ComponentHandle const&) = 0;
    virtual ~ObjectFactory() = 0;
    virtual void OnDestroyObject(T*) = 0;

    struct ObjectEntry
    {
        T* Object;
        int32_t FreeHandleIndex;
    };

    Array<ObjectEntry> Entries;
    LegacyArray<uint32_t> Salts;
    Array<uint32_t> FreeHandles;
    Array<T*> AliveObjects;
    uint32_t GrowSize;
    uint32_t HandleType;

    T* Get(ComponentHandle const& handle) const
    {
        if (handle.GetType() == HandleType
            && handle.GetSalt() != 0
            && handle.GetIndex() < Entries.size()
            && handle.GetSalt() == Salts[handle.GetIndex()]) {
            return Entries[handle.GetIndex()].Object;
        } else {
            return nullptr;
        }
    }
};

template <class T>
struct [[bg3::hidden]] NetworkObjectFactory : public ObjectFactory<T>
{
    virtual void MoveUUID(Guid const&, Guid const&) = 0;

    HashMap<Guid, T*> ObjectsByGuid;
    Array<void*> NetIdHashMap;
    HashSet<uint64_t> NetIdFreeList;
    LegacyArray<uint16_t> NetIdSalts;
    uint64_t NextNetId;
    bool CanCreateNetIds;
};

END_SE()
