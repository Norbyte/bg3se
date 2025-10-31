////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_IDOF_H__
#define __CORE_IDOF_H__


#include <NsCore/Noesis.h>
#include <NsCore/KernelApi.h>


namespace Noesis
{

class Symbol;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper functions to format reflection type names for templated classes.
///
///  template<class T> struct Point
///  {
///     T x, y;
///
///     NS_IMPLEMENT_INLINE_REFLECTION_(Point, NoParent, IdOf("Point<", IdOf<T>(), ">"));
///  };
///
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> const char* IdOf();
Symbol IdOf(const char* str0, const char* str1);
Symbol IdOf(const char* str0, const char* str1, const char* str2);

}

#include <NsCore/IdOf.inl>

#endif
