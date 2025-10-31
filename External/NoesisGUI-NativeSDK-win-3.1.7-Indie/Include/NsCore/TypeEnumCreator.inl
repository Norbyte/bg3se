////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/TypeEnum.h>
#include <NsCore/TypeEnumImpl.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

template<class T> class TypeEnumImpl;
template<class T> struct TypeEnumFiller;

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class EnumT>
TypeEnumCreator<EnumT>::TypeEnumCreator(TypeEnum* typeEnum): mTypeEnum(typeEnum)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class EnumT>
Type* TypeEnumCreator<EnumT>::Create(Symbol name)
{
    return new TypeEnumImpl<EnumT>(name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class EnumT>
void TypeEnumCreator<EnumT>::Fill(Type* type)
{
    TypeEnumCreator<EnumT> helper(static_cast<TypeEnum*>(type));
    TypeEnumFiller<EnumT>::Fill(helper);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class EnumT>
template<class T, class ...Args>
void TypeEnumCreator<EnumT>::Meta(Args... args)
{
    Ptr<TypeMetaData> metaData(*new T(args...));
    mTypeEnum->AddMeta(metaData.GetPtr());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class EnumT>
void TypeEnumCreator<EnumT>::Val(const char* name, EnumT value)
{
    mTypeEnum->AddValue(Symbol::Static(name), (int)value);
}

}
