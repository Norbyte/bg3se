////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_VECTOR_H__
#define __CORE_VECTOR_H__


#include <NsCore/CompilerTools.h>
#include <NsCore/VectorFwd.h>
#include <NsCore/Math.h>
#include <NsCore/Memory.h>

#include <new>
#include <string.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Non-templated base class for Vector
////////////////////////////////////////////////////////////////////////////////////////////////////
class BaseVector_
{
public:
    /// Returns the number of elements in the vector
    uint32_t Size() const;

    /// Returns the number of elements that can be held in currently allocated storage
    uint32_t Capacity() const;

    /// Checks whether the container is empty
    bool Empty() const;

    /// Set array size without constructing or destructing elements. More efficient than Resize()
    /// to avoid initialization of elements that will be overwritten
    void ForceSize(uint32_t n);

protected:
    void* mBegin;
    uint32_t mSize;
    uint32_t mCapacity: 24;
    uint32_t mIsSmall: 8;

    BaseVector_(void* first, uint32_t capacity);

    /// Grows capacity exponentially
    static uint32_t GetNewCapacity(uint32_t currentCapacity);

    /// Implementation of grow() method which only works on POD-like data types
    void GrowPod(void* first, uint32_t minCapacity, uint32_t elementSize);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Common code factored out of the Vector class to reduce duplication based on the 'N' parameter
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class BaseVector: public BaseVector_
{
public:
    BaseVector(const BaseVector&) = delete;
    BaseVector& operator=(const BaseVector&) = delete;

    typedef T* Iterator;
    typedef const T* ConstIterator;
    typedef T ValueType;

    /// Returns an iterator pointing to the first element in the vector
    T* Begin();
    const T* Begin() const;

    /// Returns an iterator referring to the past-the-end element in the vector container
    T* End();
    const T* End() const;

    /// Returns a direct pointer to the memory array used internally by the vector
    T* Data();
    const T* Data() const;

    /// Returns a reference to the element at position n in the vector container
    T& operator[](uint32_t i);
    const T& operator[](uint32_t i) const;

    /// Returns a reference to the first element in the vector
    T& Front();
    const T& Front() const;

    /// Returns a reference to the last element in the vector
    T& Back();
    const T& Back() const;

    /// Removes the last element in the vector, effectively reducing the container size by one
    void PopBack();

    /// Removes all elements from the vector, leaving the container with a size of 0
    void Clear();

    /// Extends the vector by inserting new elements
    T* Insert(T* pos, const T& v);
    T* Insert(T* pos, T&& v);
    T* Insert(T* pos, const T* first, const T* last);

    /// Assigns new contents to the vector replacing its current contents
    void Assign(uint32_t n, const T& v);
    void Assign(const T* first, const T* last);
    void Assign(BaseVector&& v);

    /// Extends the vector by appending new elements
    void Append(uint32_t n, const T& v);
    void Append(const T* first, const T* last);

    /// Requests that the vector capacity be at least enough to contain n elements
    void Reserve(uint32_t n);

    /// Changes the number of elements stored
    void Resize(uint32_t n);
    void Resize(uint32_t n, const T& v);

    /// Adds a new element at the end of the vector
    void PushBack(T&& v);
    void PushBack(const T& v);
    template<typename... Args> T& EmplaceBack(Args&&... args);

    /// Removes elements
    T* Erase(const T* pos);
    T* Erase(const T* first, const T* last);
    template<class Predicate> void EraseIf(Predicate predicate);

    /// Searches for elements
    template<class Value> T* Find(const Value& value);
    template<class Value> const T* Find(const Value& value) const;
    template<class Predicate> T* FindIf(Predicate predicate);
    template<class Predicate> const T* FindIf(Predicate predicate) const;

    /// Equality operators
    bool operator==(const BaseVector& v) const;
    bool operator!=(const BaseVector& v) const;

protected:
    BaseVector(uint32_t size);
    ~BaseVector();

    void* GetFirst() const;
    void ResetToSmall(uint32_t capacity);

    template<bool> struct IsPod {};

    void Grow(uint32_t minSize, IsPod<true>);
    void Grow(uint32_t minSize, IsPod<false>);

    static void DestroyRange(T* first, T* last, IsPod<true>);
    static void DestroyRange(T* first, T* last, IsPod<false>);

    static void UninitializedFill(T* first, T* last, const T& v, IsPod<true>);
    static void UninitializedFill(T* first, T* last, const T& v, IsPod<false>);

    static void Fill(T* first, T* last, const T& v, IsPod<true>);
    static void Fill(T* first, T* last, const T& v, IsPod<false>);

    static void UninitializedCopy(const T* first, const T* last, T* dest, IsPod<true>);
    static void UninitializedCopy(const T* first, const T* last, T* dest, IsPod<false>);

    static T* Copy(const T* first, const T* last, T* dest, IsPod<true>);
    static T* Copy(const T* first, const T* last, T* dest, IsPod<false>);

    static void UninitializedMove(T* first, const T* last, T* dest, IsPod<true>);
    static void UninitializedMove(T* first, const T* last, T* dest, IsPod<false>);

    static T* Move(T* first, const T* last, T* dest, IsPod<true>);
    static T* Move(T* first, const T* last, T* dest, IsPod<false>);

    static T* MoveOverlap(T* first, const T* last, T* dest, IsPod<true>);
    static T* MoveOverlap(T* first, const T* last, T* dest, IsPod<false>);

    static void MoveBackward(T* first, const T* last, T* destLast, IsPod<true>);
    static void MoveBackward(T* first, const T* last, T* destLast, IsPod<false>);
};

template<typename T, unsigned N> struct VectorStorage { alignas(T) char _elements[sizeof(T) * N]; };
template<typename T> struct VectorStorage<T, 0> {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// A vector implementation with N elements of preallocated memory in-place
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
class Vector: public BaseVector<T>, public VectorStorage<T, N>
{
public:
    /// Constructors
    Vector();
    explicit Vector(uint32_t n, const T& v = T());
    Vector(const T* first, const T* last);
    Vector(const Vector& v);
    Vector(Vector&& v);

    /// Assignment operators
    const Vector& operator=(const Vector& v);
    const Vector& operator=(Vector&& v);

    /// Requests the removal of unused capacity
    void ShrinkToFit();

    /// Returns true if inlined memory is being used
    bool IsSmall() const;
};

}

#include <NsCore/Vector.inl>

#endif
