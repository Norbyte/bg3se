////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_REFLECTIONIMPLEMENT_H__
#define __CORE_REFLECTIONIMPLEMENT_H__


#include <NsCore/Noesis.h> 
#include <NsCore/Symbol.h>
#include <NsCore/Reflection.h>
#include <NsCore/TypeClassCreator.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Shortcuts for defining reflection members
////////////////////////////////////////////////////////////////////////////////////////////////////

/// Adds metadatas
#define NsMeta helper.Meta

/// Indicates that type implements an interface
#define NsImpl Rebind_(helper).template Impl

/// Adds properties
#define NsProp helper.Prop

/// Adds events
#define NsEvent helper.Event


////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implements reflection for a class. This is the corresponding macro to NS_DECLARE_REFLECTION that
/// is used in the implementation file. Note that name, the second paremeter, is optional.
///
///  NS_IMPLEMENT_REFLECTION(ViewModel, "MyApp.ViewModel")
///  {
///     NsProp("Start", &ViewModel::GetStart);
///     NsProp("Settings", &ViewModel::GetSettings);
///     NsProp("Exit", &ViewModel::GetExit);
///  }
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_REFLECTION2(classType, name) \
const Noesis::TypeClass* classType::StaticGetClassType(Noesis::TypeTag<classType>*)\
{\
    static const Noesis::TypeClass* type;\
\
    if (NS_UNLIKELY(type == 0))\
    {\
        type = static_cast<const Noesis::TypeClass*>(Noesis::Reflection::RegisterType(name,\
            Noesis::TypeClassCreator::Create<SelfClass>,\
            Noesis::TypeClassCreator::Fill<SelfClass, ParentClass>));\
    }\
\
    return type;\
}\
\
const Noesis::TypeClass* classType::GetClassType() const\
{\
    return StaticGetClassType((Noesis::TypeTag<classType>*)nullptr);\
}\
\
struct classType::Rebind_ \
{ \
    Rebind_(const Rebind_&) = delete; \
    Rebind_& operator=(const Rebind_&) = delete; \
    NS_FORCE_INLINE Rebind_(Noesis::TypeClassCreator& helper_): helper(helper_) {} \
    template<class IFACE> NS_FORCE_INLINE void Impl() { helper.Impl<classType, IFACE>(); } \
    Noesis::TypeClassCreator& helper; \
}; \
\
NS_COLD_FUNC void classType::StaticFillClassType(Noesis::TypeClassCreator& helper)

#define NS_IMPLEMENT_REFLECTION1(classType) NS_IMPLEMENT_REFLECTION2(classType, #classType)

#define NS_IMPLEMENT_REFLECTION_EXPAND(x) x
#define NS_IMPLEMENT_REFLECTION_GET_OVERLOAD(_1, _2, MACRO, ...) MACRO
#define NS_IMPLEMENT_REFLECTION(...) NS_IMPLEMENT_REFLECTION_EXPAND( \
    NS_IMPLEMENT_REFLECTION_GET_OVERLOAD(__VA_ARGS__, NS_IMPLEMENT_REFLECTION2, \
    NS_IMPLEMENT_REFLECTION1)(__VA_ARGS__))

// Supress clang "-Winconsistent-missing-override" this way because push/pop is not working
#ifdef __clang__
#pragma clang system_header
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Instead of using the pair of macros NS_DECLARE_REFLECTION and NS_IMPLEMENT_REFLECTION, this
/// single macro, NS_IMPLEMENT_INLINE_REFLECTION, can be used for the same purpose. Its usage is not
/// recommended but in cases with no alternative like, for example, with templated classes this
/// macro can be used inlined in the class header. Note that name, the third paremeter, is optional.
///
///  NS_INTERFACE IList: public Interface
///  {
///      NS_IMPLEMENT_INLINE_REFLECTION_(IList, Interface, "IList")
///  };
///
////////////////////////////////////////////////////////////////////////////////////////////////////
#define NS_IMPLEMENT_INLINE_REFLECTION3(classType, parentType, name) \
public:\
    NS_NO_INLINE static const Noesis::TypeClass* StaticGetClassType(Noesis::TypeTag<classType>*)\
    {\
        static const Noesis::TypeClass* type;\
\
        if (NS_UNLIKELY(type == 0))\
        {\
            type = static_cast<const Noesis::TypeClass*>(Noesis::Reflection::RegisterType(name,\
                Noesis::TypeClassCreator::Create<SelfClass>,\
                Noesis::TypeClassCreator::Fill<SelfClass, ParentClass>));\
        }\
\
        return type;\
    }\
\
    const Noesis::TypeClass* GetClassType() const\
    {\
        return StaticGetClassType((Noesis::TypeTag<classType>*)nullptr);\
    }\
\
private:\
    typedef classType SelfClass;\
    typedef parentType ParentClass;\
\
    friend class Noesis::TypeClassCreator;\
\
    struct Rebind_ \
    { \
        Rebind_(const Rebind_&) = delete; \
        Rebind_& operator=(const Rebind_&) = delete; \
        NS_FORCE_INLINE Rebind_(Noesis::TypeClassCreator& helper_): helper(helper_) {} \
        template<class IFACE> NS_FORCE_INLINE void Impl() { helper.Impl<classType, IFACE>(); } \
        Noesis::TypeClassCreator& helper; \
    }; \
\
    NS_COLD_FUNC static void StaticFillClassType(Noesis::TypeClassCreator& helper)

#define NS_IMPLEMENT_INLINE_REFLECTION2(classType, parentType) \
    NS_IMPLEMENT_INLINE_REFLECTION3(classType, parentType, #classType)

#define NS_IMPLEMENT_INLINE_REFLECTION_EXPAND(x) x
#define NS_IMPLEMENT_INLINE_REFLECTION_GET_OVERLOAD(_1, _2, _3, MACRO, ...) MACRO
#define NS_IMPLEMENT_INLINE_REFLECTION(...) NS_IMPLEMENT_INLINE_REFLECTION_EXPAND( \
    NS_IMPLEMENT_INLINE_REFLECTION_GET_OVERLOAD(__VA_ARGS__, NS_IMPLEMENT_INLINE_REFLECTION3, \
    NS_IMPLEMENT_INLINE_REFLECTION2)(__VA_ARGS__))


#endif
