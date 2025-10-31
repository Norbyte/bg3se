////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_SORT_H__
#define __CORE_SORT_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

/// Checks if the elements in range [first, last) are sorted in non-descending order
template<class T, class Compare> bool IsSorted(T* first, T* last, Compare comp);
template<class T> bool IsSorted(T* first, T* last);

/// Simple sorting algorithm . Very fast for few items (<16) or for collections almost sorted
template<class T, class Compare> void InsertionSort(T* first, T* last, Compare comp);
template<class T> void InsertionSort(T* first, T* last);

/// Sorts the elements in the range [first, last) in ascending order. The order of equal elements
/// is not guaranteed to be preserved. 
template<class T, class Compare> void Sort(T* first, T* last, Compare comp);
template<class T> void Sort(T* first, T* last);

/// Finds the first position into which value can be inserted without violating the ordering
template<class T, class Val, class Compare> T* LowerBound(T* first, T* last, const Val& value, Compare comp);
template<class T, class Val> T* LowerBound(T* first, T* last, const Val& value);

}

#include <NsCore/Sort.inl>

#endif
