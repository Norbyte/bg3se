////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTIONIMPLEMENTENUM_H__
#define __CORE_REFLECTIONIMPLEMENTENUM_H__


#include <NsCore/Noesis.h> 
#include <NsCore/CompilerTools.h>
#include <NsCore/TypeEnumCreator.h>
#include <NsCore/TypeEnumHelper.h>
#include <NsCore/TypeEnumImpl.h>
#include <NsCore/ReflectionDeclareEnum.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Shortcuts for defining reflection members
////////////////////////////////////////////////////////////////////////////////////////////////////

/// Adds metadatas
#define NsMeta helper.Meta

/// Defines a value for an enum
#define NsVal helper.Val


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements reflection for enumerations. Having a declaration with NS_DECLARE_REFLECTION_ENUM in
/// a header file, NS_DECLARE_REFLECTION_ENUM must be used in the implementation file. Note that
/// name, the second paramenter, is optional.
///
///  NS_IMPLEMENT_REFLECTION_ENUM(CharacterCasing, "CharacterCasing")
///  {
///     NsVal("Normal", CharacterCasing_Normal);
///     NsVal("Lower", CharacterCasing_Lower);
///     NsVal("Upper", CharacterCasing_Upper);
///  }
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_REFLECTION_ENUM2(T, name) \
    const Noesis::TypeEnum* Noesis::TypeEnumFiller<T>::GetType() \
    { \
        return Noesis::TypeEnumHelper<T>::GetType(name); \
    } \
    void Noesis::TypeEnumFiller<T>::Fill(Noesis::TypeEnumCreator<T>& helper)

#define NS_IMPLEMENT_REFLECTION_ENUM1(T) NS_IMPLEMENT_REFLECTION_ENUM2(T, #T)

#define NS_IMPLEMENT_REFLECTION_ENUM_EXPAND(x) x
#define NS_IMPLEMENT_REFLECTION_ENUM_GET_OVERLOAD(_1, _2, MACRO, ...) MACRO
#define NS_IMPLEMENT_REFLECTION_ENUM(...) NS_IMPLEMENT_REFLECTION_ENUM_EXPAND( \
    NS_IMPLEMENT_REFLECTION_ENUM_GET_OVERLOAD(__VA_ARGS__, NS_IMPLEMENT_REFLECTION_ENUM2, \
    NS_IMPLEMENT_REFLECTION_ENUM1)(__VA_ARGS__))


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Instead of using the pair of macros NS_DECLARE_REFLECTION_ENUM and NS_IMPLEMENT_REFLECTION_ENUM,
/// NS_IMPLEMENT_INLINE_REFLECTION_ENUM can be used for the same purpose. Note that name, the second
/// paremeter is optional.
///
///  NS_IMPLEMENT_INLINE_REFLECTION_ENUM(Enum, "Enum")
///  {
///      NsVal("Ok", TestEnum_Ok);
///      NsVal("Cancel", TestEnum_Cancel);
///      NsVal("TryAgain", TestEnum_TryAgain);
///  }
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_INLINE_REFLECTION_ENUM2(T, name) \
    NS_DECLARE_REFLECTION_ENUM(T) \
    inline const Noesis::TypeEnum* Noesis::TypeEnumFiller<T>::GetType() \
    { \
        return Noesis::TypeEnumHelper<T>::GetType(name); \
    } \
    inline void Noesis::TypeEnumFiller<T>::Fill(Noesis::TypeEnumCreator<T>& helper)

#define NS_IMPLEMENT_INLINE_REFLECTION_ENUM1(T) NS_IMPLEMENT_INLINE_REFLECTION_ENUM2(T, #T)

#define NS_IMPLEMENT_INLINE_REFLECTION_ENUM_EXPAND(x) x
#define NS_IMPLEMENT_INLINE_REFLECTION_ENUM_GET_OVERLOAD(_1, _2, MACRO, ...) MACRO
#define NS_IMPLEMENT_INLINE_REFLECTION_ENUM(...) NS_IMPLEMENT_INLINE_REFLECTION_ENUM_EXPAND( \
    NS_IMPLEMENT_INLINE_REFLECTION_ENUM_GET_OVERLOAD(__VA_ARGS__, \
    NS_IMPLEMENT_INLINE_REFLECTION_ENUM2, NS_IMPLEMENT_INLINE_REFLECTION_ENUM1)(__VA_ARGS__))


#endif
