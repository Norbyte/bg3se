////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_DYNAMICCAST_H__
#define __CORE_DYNAMICCAST_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

template<class T> class Ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Safely converts pointers along the inheritance hierarchy
///
///  FrameworkElement* element = view->GetContent();
///  Button* button = DynamicCast<Button*>(element);
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class To, class From> To DynamicCast(From from);

/// Returns new Ptr whose stored pointer is obtained doing a DynamicCast
template<class To, class From> Ptr<To> DynamicPtrCast(const Ptr<From>& from);
template<class To, class From> Ptr<To> DynamicPtrCast(Ptr<From>&& from);

/// Returns new Ptr whose stored pointer is obtained doing a static_cast
template<class To, class From> Ptr<To> StaticPtrCast(const Ptr<From>& from);
template<class To, class From> Ptr<To> StaticPtrCast(Ptr<From>&& from);

}

#include <NsCore/DynamicCast.inl>

#endif
