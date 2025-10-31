////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTIONIMPLEMENTEMPTY_H__
#define __CORE_REFLECTIONIMPLEMENTEMPTY_H__


#include <NsCore/Noesis.h>
#include <NsCore/Reflection.h>
#include <NsCore/TypeClassCreatorEmpty.h>


namespace Noesis
{
typedef void NoParent;
template<class T> struct TypeTag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// NS_IMPLEMENT_REFLECTION_ is the lightweight version of NS_IMPLEMENT_REFLECTION. This header
/// brings less dependencies and can be used when the reflection block is empty.
///
///  NS_IMPLEMENT_REFLECTION_(BaseEnumConverter)
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_REFLECTION_2(classType, name) \
const Noesis::TypeClass* classType::StaticGetClassType(Noesis::TypeTag<classType>*)\
{\
    static const Noesis::TypeClass* type;\
\
    if (NS_UNLIKELY(type == 0))\
    {\
        type = (const Noesis::TypeClass*)(Noesis::Reflection::RegisterType(name,\
            Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Create,\
            Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Fill));\
    }\
\
    return type;\
}\
\
const Noesis::TypeClass* classType::GetClassType() const\
{\
    return StaticGetClassType((Noesis::TypeTag<classType>*)nullptr);\
}

#define NS_IMPLEMENT_REFLECTION_1(classType) NS_IMPLEMENT_REFLECTION_2(classType, #classType)

#define NS_IMPLEMENT_REFLECTION_EXPAND_(x) x
#define NS_IMPLEMENT_REFLECTION_GET_OVERLOAD_(_1, _2, MACRO, ...) MACRO
#define NS_IMPLEMENT_REFLECTION_(...) NS_IMPLEMENT_REFLECTION_EXPAND_( \
    NS_IMPLEMENT_REFLECTION_GET_OVERLOAD_(__VA_ARGS__, NS_IMPLEMENT_REFLECTION_2, \
    NS_IMPLEMENT_REFLECTION_1)(__VA_ARGS__))

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements static reflection functions for a class inside class definition
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_INLINE_STATIC_REFLECTION_(classType, parentType, name) \
public:\
    static const Noesis::TypeClass* StaticGetClassType(Noesis::TypeTag<classType>*)\
    {\
        static const Noesis::TypeClass* type;\
\
        if (NS_UNLIKELY(type == 0))\
        {\
            type = (const Noesis::TypeClass*)(Noesis::Reflection::RegisterType(name,\
                Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Create,\
                Noesis::TypeClassCreatorEmpty<SelfClass, ParentClass>::Fill));\
        }\
\
        return type;\
    }\
\
private:\
    typedef classType SelfClass;\
    typedef parentType ParentClass;

// Supress clang "-Winconsistent-missing-override" this way because push/pop is not working
#ifdef __clang__
#pragma clang system_header
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// NS_IMPLEMENT_INLINE_REFLECTION_ is the lightweight version of NS_IMPLEMENT_INLINE_REFLECTION.
/// This header brings less dependencies and can be used when the reflection block is empty.
///
///  NS_INTERFACE ICommand: public Interface
///  {
///      NS_IMPLEMENT_INLINE_REFLECTION_(ICommand, Interface)
///  };
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_INLINE_REFLECTION_3(classType, parentType, name) \
public:\
    const Noesis::TypeClass* GetClassType() const\
    {\
        return StaticGetClassType((Noesis::TypeTag<classType>*)nullptr);\
    } \
    NS_IMPLEMENT_INLINE_STATIC_REFLECTION_(classType, parentType, name)

#define NS_IMPLEMENT_INLINE_REFLECTION_2(classType, parentType) \
    NS_IMPLEMENT_INLINE_REFLECTION_3(classType, parentType, #classType)

#define NS_IMPLEMENT_INLINE_REFLECTION_EXPAND_(x) x
#define NS_IMPLEMENT_INLINE_REFLECTION_GET_OVERLOAD_(_1, _2, _3, MACRO, ...) MACRO
#define NS_IMPLEMENT_INLINE_REFLECTION_(...) NS_IMPLEMENT_INLINE_REFLECTION_EXPAND_( \
    NS_IMPLEMENT_INLINE_REFLECTION_GET_OVERLOAD_(__VA_ARGS__, NS_IMPLEMENT_INLINE_REFLECTION_3, \
    NS_IMPLEMENT_INLINE_REFLECTION_2)(__VA_ARGS__))


#endif
