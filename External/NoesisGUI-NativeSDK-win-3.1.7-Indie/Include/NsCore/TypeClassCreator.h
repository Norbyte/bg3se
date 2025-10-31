////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPECLASSCREATOR_H__
#define __CORE_TYPECLASSCREATOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/CompilerTools.h>


namespace Noesis
{

class Type;
class TypeClassBuilder;
class TypeProperty;
class TypeMetaData;
class Symbol;
template<class T> class Delegate;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeClassCreator implements helper functions for building types using Noesis reflection macros.
///
///  NS_IMPLEMENT_REFLECTION(Square)
///  {
///    NsMeta<Desc>("Defines a square shape that can be drawn");
///    NsMeta<Tag>("Visual");
///    NsImpl<IShape>();
///    NsProp("side", &Square::mSide)
///        .Meta<Desc>("Length of the square side")
///        .Meta<Tag>("Editable");
///  }
///
////////////////////////////////////////////////////////////////////////////////////////////////////
class TypeClassCreator
{
public:
    TypeClassCreator(TypeClassBuilder* typeClass);
    TypeClassCreator(const TypeClassCreator&) = delete;
    TypeClassCreator& operator=(const TypeClassCreator&) = delete;

    /// Creates a TypeClass for the specified class
    template<class ClassT> 
    static Type* Create(Symbol name);

    /// Fills TypeClass with members of the class
    template<class ClassT, class BaseT> 
    static void Fill(Type* type);

    /// Adds meta data to the type class
    template<class T, class ...Args>
    T* Meta(Args... args);

    /// Specifies that the class implements the interface T
    template<class ClassT, class T>
    void Impl();

    /// Helper returned to add metadata to properties
    struct TypePropertyCreator
    {
        NS_FORCE_INLINE TypePropertyCreator(TypeProperty* typeProperty);
        template<class T, class ...Args> NS_FORCE_INLINE TypePropertyCreator& Meta(Args... args);

        TypeProperty* mTypeProperty;
    };

    /// Adds a property to the type class
    //@{
    template<class ClassT, class T>
    TypePropertyCreator Prop(const char* name, T ClassT::* prop);

    template<class ClassT, class T, int N>
    TypePropertyCreator Prop(const char* name, T (ClassT::* prop)[N]);

    template<class ClassT, class T, int N>
    TypePropertyCreator Prop(const char* name, T (ClassT::* prop)[N], uint32_t index);

    template<class T>
    TypePropertyCreator Prop(const char* name, uint32_t offset);

    template<class ClassT, class T>
    TypePropertyCreator Prop(const char* name, T (ClassT::*getter)() const);

    template<class ClassT, class T>
    TypePropertyCreator Prop(const char* name, T (ClassT::*getter)() const,
        void (ClassT::*setter)(T));
    //@}

    /// Adds a event to the type class
    template<class ClassT, class T>
    TypePropertyCreator Event(const char* name, Delegate<T> ClassT::* event);

    template<class ClassT, class T>
    TypePropertyCreator Event(const char* name, Delegate<T>& (ClassT::*getter)());

private:
    /// Adds base parent to the type class (only if T is different of NoParent)
    //@{
    template<class ClassT, class T>
    void Base(Int2Type<0>);

    template<class ClassT, class T>
    void Base(Int2Type<1>);
    //@}

private:
    TypeClassBuilder* mTypeClass;
};

}

#include <NsCore/TypeClassCreator.inl>

#endif
