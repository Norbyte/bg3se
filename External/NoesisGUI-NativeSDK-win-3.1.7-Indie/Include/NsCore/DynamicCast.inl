////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/CompilerTools.h>
#include <NsCore/TypeClass.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class BaseObject;
NS_INTERFACE Interface;

NS_CORE_KERNEL_API void* Cast(const TypeClass* type, BaseObject* obj);

struct DynamicCast_
{
    template<class T> static BaseObject* GetBaseObject(T* obj, TrueType)
    {
        return (BaseObject*)(obj ? obj->GetBaseObject() : nullptr); 
    }

    template<class T> static BaseObject* GetBaseObject(T* obj, FalseType)
    {
        return (BaseObject*)obj;
    }

    template<class To_, class From_> static To_* Apply(From_* from)
    {
        const TypeClass* type = To_::StaticGetClassType((TypeTag<RemoveConst<To_>>*)nullptr);
        BaseObject* obj = GetBaseObject(from, Int2Type<IsInterface<From_>::Result>());
        return reinterpret_cast<To_*>(Cast(type, obj));
    }
};

struct StaticCast_
{
    template<class To_, class From_> static To_* Apply(From_* from)
    {
        return static_cast<To_*>(from);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class To, class From> To DynamicCast(From from)
{
    typedef Noesis::RemovePointer<From> From_;
    typedef Noesis::RemovePointer<To> To_;
    static_assert(!IsConst<From_>::Result || IsConst<To_>::Result, "cannot cast from const to non-const pointer");
    return If<IsDerived<From_, To_>::Result, StaticCast_, DynamicCast_>::template Apply<To_, From_>(from);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class To, class From> Ptr<To> DynamicPtrCast(const Ptr<From>& from)
{
    return Ptr<To>(DynamicCast<To*>(from.GetPtr()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class To, class From> Ptr<To> DynamicPtrCast(Ptr<From>&& from)
{
    To* to = DynamicCast<To*>(from.GetPtr());
    if (to != nullptr)
    {
        from.GiveOwnership();
        return Ptr<To>(*to);
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class To, class From> Ptr<To> StaticPtrCast(const Ptr<From>& from)
{
    return Ptr<To>(static_cast<To*>(from.GetPtr()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class To, class From> Ptr<To> StaticPtrCast(Ptr<From>&& from)
{
    return Ptr<To>(*static_cast<To*>(from.GiveOwnership()));
}

}
