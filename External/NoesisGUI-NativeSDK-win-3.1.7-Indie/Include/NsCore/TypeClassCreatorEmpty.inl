////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/CompilerTools.h>
#include <NsCore/TypeClassBuilder.h>
#include <NsCore/Error.h>


namespace Noesis
{

typedef void NoParent;
template<class T> struct TypeTag;

#ifndef __CORE_TYPECLASSFOR__
#define __CORE_TYPECLASSFOR__

template<class T> struct TypeClassFor
{
    static Type* Create(Symbol name)
    {
        return new TypeClass(name, IsInterface<T>::Result);
    }
};

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
TypeClassCreatorEmpty<ClassT, BaseT>::TypeClassCreatorEmpty(TypeClassBuilder* typeClass):
    mTypeClass(typeClass)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
Type* TypeClassCreatorEmpty<ClassT, BaseT>::Create(Symbol name)
{
    return TypeClassFor<ClassT>::Create(name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
void TypeClassCreatorEmpty<ClassT, BaseT>::Fill(Type* type)
{
    TypeClassCreatorEmpty<ClassT, BaseT> helper((TypeClassBuilder*)(type));

    helper.Base<BaseT>(Int2Type<IsSame<BaseT, NoParent>::Result>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
template<class T>
void TypeClassCreatorEmpty<ClassT, BaseT>::Base(Int2Type<0>)
{
    static_assert(IsDerived<ClassT, T>::Result, "class does not derive from specified base");

    mTypeClass->AddBase(T::StaticGetClassType((TypeTag<T>*)nullptr));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
template<class T>
void TypeClassCreatorEmpty<ClassT, BaseT>::Base(Int2Type<1>)
{
}

}
