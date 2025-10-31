////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_TYPECLASSCREATOREMPTY_H__
#define __CORE_TYPECLASSCREATOREMPTY_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

class Type;
class TypeClassBuilder;
template<int N> struct Int2Type;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// TypeClassCreatorEmpty. Creates an empty TypeClass adding only information about base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class ClassT, class BaseT>
class TypeClassCreatorEmpty
{
public:
    TypeClassCreatorEmpty(const TypeClassCreatorEmpty&) = delete;
    TypeClassCreatorEmpty& operator=(const TypeClassCreatorEmpty&) = delete;

    /// Creates a TypeClass for the specified class
    static Type* Create(Symbol name);

    /// Fills TypeClass with members of the class (only the base type in this case)
    static void Fill(Type* type);

private:
    /// Constructor
    TypeClassCreatorEmpty(TypeClassBuilder* typeClass);

    /// Adds base parent to the type class (only if T is different of NoParent)
    //@{
    template<class T>
    void Base(Int2Type<0>);

    template<class T>
    void Base(Int2Type<1>);
    //@}

private:
    TypeClassBuilder* mTypeClass;
};

}

/// Inline include
#include <NsCore/TypeClassCreatorEmpty.inl>

#endif
