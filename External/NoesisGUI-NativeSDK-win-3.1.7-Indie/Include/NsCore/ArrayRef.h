////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_ARRAYREF_H__
#define __CORE_ARRAYREF_H__


#include <NsCore/Vector.h>


namespace Noesis
{

typedef decltype(nullptr) NullPtrT;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A constant reference to an array. Note that it does not own the underlying data
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class ArrayRef
{
public:
    ArrayRef() = default;
    ArrayRef(const T& value);
    ArrayRef(NullPtrT);
    ArrayRef(const T* data, uint32_t size);
    template<unsigned N> ArrayRef(const T (&data)[N]);
    template<unsigned N> ArrayRef(const Vector<T, N>& data);

    /// Returns an iterator pointing to the first element in the array
    const T* Begin() const;

    /// Returns an iterator referring to the past-the-end element in the array
    const T* End() const;

    /// Checks whether the array is empty
    bool Empty() const;

    /// Returns the number of elements in the array
    uint32_t Size() const;

    /// Returns a direct pointer to the memory array
    const T* Data() const;

    /// Returns a reference to the element at position n in the array
    const T& operator[](uint32_t i) const;

private:
    const T* mBegin = nullptr;
    uint32_t mSize = 0;
};

}

#include <NsCore/ArrayRef.inl>

#endif
