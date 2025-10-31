////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseVector_::BaseVector_(void* first, uint32_t capacity): mBegin(first), mSize(0),
    mCapacity(capacity), mIsSmall(1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t BaseVector_::Size() const
{
    return mSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t BaseVector_::Capacity() const
{
    return mCapacity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool BaseVector_::Empty() const
{
    return mSize == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseVector_::ForceSize(uint32_t n)
{
    NS_ASSERT(n <= mCapacity);
    mSize = n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t BaseVector_::GetNewCapacity(uint32_t currentCapacity)
{
    return (currentCapacity > 0) ? 2 * currentCapacity : 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseVector_::GrowPod(void* first, uint32_t minCapacity, uint32_t elementSize)
{
    uint32_t capacity = Max(GetNewCapacity(mCapacity), minCapacity);

    // The inlined buffer cannot be reallocated
    if (mIsSmall)
    {
        mBegin = Alloc(capacity * elementSize);
        memcpy(mBegin, first, mSize * elementSize);
    }
    else
    {
        // It is debatable is realloc() is here a good idea. In case a new pointer is given all
        // the memory is internally copied instead of copying only the size of the vector
        mBegin = Realloc(mBegin, capacity * elementSize);
    }

    NS_ASSERT(capacity < (1 << 24));
    mCapacity = capacity;
    mIsSmall = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline BaseVector<T>::BaseVector(uint32_t size): BaseVector_(GetFirst(), size)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline BaseVector<T>::~BaseVector()
{
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    DestroyRange(Begin(), End(), IsPod());

    if (!mIsSmall)
    {
        Dealloc(Begin());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Begin()
{
    return (T*)mBegin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T* BaseVector<T>::Begin() const
{
    return (T*)mBegin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::End()
{
    return (T*)mBegin + mSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T* BaseVector<T>::End() const
{
    return (T*)mBegin + mSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Data()
{
    return (T*)mBegin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T* BaseVector<T>::Data() const
{
    return (T*)mBegin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T& BaseVector<T>::operator[](uint32_t i)
{
    NS_ASSERT(i < mSize);
    return ((T*)mBegin)[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T& BaseVector<T>::operator[](uint32_t i) const
{
    NS_ASSERT(i < mSize);
    return ((T*)mBegin)[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T& BaseVector<T>::Front()
{
    NS_ASSERT(!Empty());
    return ((T*)mBegin)[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T& BaseVector<T>::Front() const
{
    NS_ASSERT(!Empty());
    return ((T*)mBegin)[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T& BaseVector<T>::Back()
{
    NS_ASSERT(!Empty());
    return ((T*)mBegin + mSize)[-1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline const T& BaseVector<T>::Back() const
{
    NS_ASSERT(!Empty());
    return ((T*)mBegin + mSize)[-1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::PopBack()
{
    NS_ASSERT(!Empty());
    ForceSize(mSize - 1);
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    DestroyRange(End(), End() + 1, IsPod());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Clear()
{
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    DestroyRange(Begin(), End(), IsPod());
    mSize = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Insert(T* pos, const T &v)
{
    NS_ASSERT(&v < Begin() || &v >= End());
    NS_ASSERT(pos >= Begin());
    NS_ASSERT(pos <= End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (pos == End())
    {
        PushBack(v);
        return End() - 1;
    }

    if (mSize == mCapacity)
    {
        size_t distance = pos - Begin();
        Grow(0, IsPod());
        pos = Begin() + distance;
    }

    UninitializedMove(&Back(), &Back() + 1, End(), IsPod());
    MoveBackward(pos, End() - 1, End(), IsPod());
    ForceSize(mSize + 1);

    *pos = v;
    return pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Insert(T* pos, T &&v)
{
    NS_ASSERT(&v < Begin() || &v >= End());
    NS_ASSERT(pos >= Begin());
    NS_ASSERT(pos <= End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (pos == End())
    {
        PushBack(MoveArg(v));
        return End() - 1;
    }

    if (mSize == mCapacity)
    {
        size_t distance = pos - Begin();
        Grow(0, IsPod());
        pos = Begin() + distance;
    }

    UninitializedMove(&Back(), &Back() + 1, End(), IsPod());
    MoveBackward(pos, End() - 1, End(), IsPod());
    ForceSize(mSize + 1);

    *pos = MoveArg(v);
    return pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Insert(T* pos, const T* first, const T* last)
{
    NS_ASSERT(last <= Begin() || first >= End());
    NS_ASSERT(pos >= Begin());
    NS_ASSERT(pos <= End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    uint32_t d = uint32_t(pos - Begin());

    if (pos == End())
    {
        Append(first, last);
        return Begin() + d;
    }

    uint32_t n = uint32_t(last - first);
    if (mCapacity < mSize + n)
    {
        Grow(mSize + n, IsPod());
        pos = Begin() + d;
    }

    if (uint32_t(End() - pos) >= n)
    {
        UninitializedMove(End() - n, End(), End(), IsPod());
        MoveBackward(pos, End() - n, End(), IsPod());
        Copy(first, last, pos, IsPod());
    }
    else
    {
        size_t numOverwritten = End() - pos;
        UninitializedMove(pos, End(), End() + n - numOverwritten, IsPod());
        Copy(first, first + numOverwritten, pos, IsPod());
        UninitializedCopy(first + numOverwritten, last, End(), IsPod());
    }

    ForceSize(mSize + n);
    return pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Assign(uint32_t n, const T& v)
{
    NS_ASSERT(&v < Begin() || &v >= End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    uint32_t size = mSize;

    // If there is enough space, copy elements and shrink to new size
    if (size >= n)
    {
        T* end_ = Begin() + n;
        Fill(Begin(), end_, v, IsPod());
        DestroyRange(end_, End(), IsPod());
        ForceSize(n);
        return;
    }

    // If we have to grow to have enough elements, destroy the current elements
    if (mCapacity < n)
    {
        DestroyRange(Begin(), End(), IsPod());
        ForceSize(0);
        size = 0;
        Grow(n, IsPod());
    }

    T* end = Begin() + size;
    Fill(Begin(), end, v, IsPod());
    UninitializedFill(end, Begin() + n, v, IsPod());
    ForceSize(n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Assign(const T* first, const T* last)
{
    NS_ASSERT(last <= Begin() || first >= End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    uint32_t n = uint32_t(last - first);
    uint32_t size = mSize;

    // If there is enough space, copy elements and shrink to new size
    if (size >= n)
    {
        DestroyRange(Copy(first, last, Begin(), IsPod()), End(), IsPod());
        ForceSize(n);
        return;
    }

    // If we have to grow to have enough elements, destroy the current elements
    if (mCapacity < n)
    {
        DestroyRange(Begin(), End(), IsPod());
        ForceSize(0);
        size = 0;
        Grow(n, IsPod());
    }

    const T* end = first + size;
    UninitializedCopy(end, last, Copy(first, end, Begin(), IsPod()), IsPod());
    ForceSize(n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Assign(BaseVector&& v)
{
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    // If v is not using inlined memmory steals its buffer
    if (!v.mIsSmall)
    {
        DestroyRange(Begin(), End(), IsPod());

        if (!mIsSmall)
        {
            Dealloc(Begin());
        }

        mBegin = v.mBegin;
        mSize = v.mSize;
        mCapacity = v.mCapacity;
        mIsSmall = 0;

        v.ResetToSmall(0);
    }
    else
    {
        uint32_t n = v.Size();
        uint32_t size = mSize;

        // If there is enough space, move elements and shrink to new size
        if (size >= n)
        {
            DestroyRange(Move(v.Begin(), v.End(), Begin(), IsPod()), End(), IsPod());
        }
        else
        {
            // If v is small can't have more capacity than us
            NS_ASSERT(mCapacity >= n);
            T* end = v.Begin() + size;
            UninitializedMove(end, v.End(), Move(v.Begin(), end, Begin(), IsPod()), IsPod());
        }

        ForceSize(n);
        v.Clear();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Append(uint32_t n, const T& v)
{
    NS_ASSERT(&v < Begin() || &v >= End());
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (NS_UNLIKELY(n > mCapacity - mSize))
    {
        Grow(mSize + n, IsPod());
    }

    UninitializedFill(End(), End() + n, v, IsPod());
    ForceSize(mSize + n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Append(const T* first, const T* last)
{
    NS_ASSERT(last <= Begin() || first >= End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    uint32_t n = uint32_t(last - first);

    if (NS_UNLIKELY(n > mCapacity - mSize))
    {
        Grow(mSize + n, IsPod());
    }

    UninitializedCopy(first, last, End(), IsPod());
    ForceSize(mSize + n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Reserve(uint32_t n)
{
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (mCapacity < n)
    {
        Grow(n, IsPod());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Resize(uint32_t n)
{
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (n < mSize)
    {
        DestroyRange(Begin() + n, End(), IsPod());
    }
    else if (n > mSize)
    {
        if (mCapacity < n)
        {
            Grow(n, IsPod());
        }

        UninitializedFill(End(), Begin() + n, T(), IsPod());
    }

    ForceSize(n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Resize(uint32_t n, const T& v)
{
    NS_ASSERT(&v < Begin() || &v >= End());
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (n < mSize)
    {
        DestroyRange(Begin() + n, End(), IsPod());
    }
    else if (n > mSize)
    {
        if (mCapacity < n)
        {
            Grow(n, IsPod());
        }

        UninitializedFill(End(), Begin() + n, v, IsPod());
    }

    ForceSize(n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::PushBack(T&& v)
{
    NS_ASSERT(&v < Begin() || &v >= End());
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (NS_UNLIKELY(mSize >= mCapacity))
    {
        Grow(0, IsPod());
    }

    UninitializedMove(&v, &v + 1, End(), IsPod());
    ForceSize(mSize + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::PushBack(const T& v)
{
    NS_ASSERT(&v < Begin() || &v >= End());
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (NS_UNLIKELY(mSize >= mCapacity))
    {
        Grow(0, IsPod());
    }

    UninitializedCopy(&v, &v + 1, End(), IsPod());
    ForceSize(mSize + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
template<typename... Args>
inline T& BaseVector<T>::EmplaceBack(Args&&... args)
{
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    if (NS_UNLIKELY(mSize >= mCapacity))
    {
        Grow(0, IsPod());
    }

    new (End()) T(ForwardArg<Args>(args)...);
    ForceSize(mSize + 1);
    return Back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Erase(const T* pos)
{
    NS_ASSERT(pos >= Begin());
    NS_ASSERT(pos < End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    MoveOverlap((T*)pos + 1, End(), (T*)pos, IsPod());
    PopBack();

    return (T*)pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Erase(const T* first, const T* last)
{
    NS_ASSERT(first >= Begin());
    NS_ASSERT(first <= last);
    NS_ASSERT(last <= End());

    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;

    T* it = MoveOverlap((T*)last, End(), (T*)first, IsPod());
    DestroyRange(it, End(), IsPod());
    ForceSize((uint32_t)(it - Begin()));

    return (T*)first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> template<class Predicate>
inline void BaseVector<T>::EraseIf(Predicate predicate)
{
    const T* first = Begin();
    const T* last = End();

    while (first != last)
    {
        if (predicate(*first))
        {
            Erase(first);
            last--;
        }
        else
        {
            first++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> template<class Value>
inline T* BaseVector<T>::Find(const Value& value)
{
    T* first = Begin();
    T* last = End();

    while (first != last && !(*first == value))
    {
        first++;
    }

    return first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> template<class Value>
inline const T* BaseVector<T>::Find(const Value& value) const
{
    const T* first = Begin();
    const T* last = End();

    while (first != last && !(*first == value))
    {
        first++;
    }

    return first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> template<class Predicate>
inline T* BaseVector<T>::FindIf(Predicate predicate)
{
    T* first = Begin();
    T* last = End();

    while (first != last && !predicate(*first))
    {
        first++;
    }

    return first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> template<class Predicate>
inline const T* BaseVector<T>::FindIf(Predicate predicate) const
{
    const T* first = Begin();
    const T* last = End();

    while (first != last && !predicate(*first))
    {
        first++;
    }

    return first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool BaseVector<T>::operator==(const BaseVector& v) const
{
    if (mSize == v.mSize)
    {
        const T* first1 = Begin();
        const T* last1 = End();
        const T* first2 = v.Begin();

        for (; first1 != last1; ++first1, ++first2)
        {
            if (!(*first1 == *first2))
            {
                return false;
            }
        }

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline bool BaseVector<T>::operator!=(const BaseVector& v) const
{
    return !(*this == v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void* BaseVector<T>::GetFirst() const
{
    return ((Vector<T, 1>*)this)->_elements;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::ResetToSmall(uint32_t capacity)
{
    mBegin = GetFirst();
    mSize = 0;
    mCapacity = capacity;
    mIsSmall = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Grow(uint32_t minSize, IsPod<true>)
{
    GrowPod(GetFirst(), minSize, sizeof(T));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Grow(uint32_t minSize, IsPod<false>)
{
    uint32_t capacity = Max(GetNewCapacity(mCapacity), minSize);
    NS_ASSERT(capacity < (1 << 24));
    T* elements = (T*)Alloc(capacity * sizeof(T));

    // Move the elements over
    typedef IsPod<IsTriviallyCopiable<T>::Result> IsPod;
    UninitializedMove(Begin(), End(), elements, IsPod());

    // Destroy the original elements.
    DestroyRange(Begin(), End(), IsPod());

    // If this wasn't grown from the inline copy, deallocate the old space.
    if (!mIsSmall)
    {
        Dealloc(Begin());
    }

    mBegin = elements;
    mCapacity = capacity;
    mIsSmall = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::DestroyRange(T*, T*, IsPod<true>)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::DestroyRange(T* first, T* last, IsPod<false>)
{
    while (first != last)
    {
        --last;
        last->~T();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::UninitializedFill(T* first, T* last, const T& value, IsPod<true>)
{
    for (; first != last; ++first)
    {
        *first = value;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::UninitializedFill(T* first, T* last, const T& value, IsPod<false>)
{
    for (; first != last; ++first)
    {
        new (first) T(value);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Fill(T* first, T* last, const T& value, IsPod<true>)
{
    for (; first != last; ++first)
    {
        *first = value;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::Fill(T* first, T* last, const T& value, IsPod<false>)
{
    for (; first != last; ++first)
    {
        *first = value;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::UninitializedCopy(const T* first, const T* last, T* dest, IsPod<true>)
{
    NS_ASSERT(first <= last);
    NS_ASSERT(dest >= last || dest + (last - first) <= first);

    memcpy(dest, first, (last - first) * sizeof(T));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::UninitializedCopy(const T* first, const T* last, T* dest, IsPod<false>)
{
    for (; first != last; ++first, ++dest)
    {
        new (dest) T(*first);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Copy(const T* first, const T* last, T* dest, IsPod<true>)
{
    NS_ASSERT(first <= last);
    NS_ASSERT(dest >= last || dest + (last - first) <= first);

    size_t n = last - first;
    memcpy(dest, first, n * sizeof(T));
    return dest + n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Copy(const T* first, const T* last, T* dest, IsPod<false>)
{
    while (first != last)
    {
        *dest++ = *first++;
    }

    return dest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::UninitializedMove(T* first, const T* last, T* dest, IsPod<true>)
{
    NS_ASSERT(first <= last);
    NS_ASSERT(dest >= last || dest + (last - first) <= first);

    memcpy(dest, first, (last - first) * sizeof(T));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::UninitializedMove(T* first, const T* last, T* dest, IsPod<false>)
{
    for (; first != last; ++first, ++dest)
    {
        new (dest) T(MoveArg(*first));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Move(T* first, const T* last, T* dest, IsPod<true>)
{
    NS_ASSERT(first <= last);
    NS_ASSERT(dest >= last || dest + (last - first) <= first);

    size_t n = last - first;
    memcpy(dest, first, n * sizeof(T));
    return dest + n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::Move(T* first, const T* last, T* dest, IsPod<false>)
{
    while (first != last)
    {
        *dest++ = MoveArg(*first++);
    }

    return dest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::MoveOverlap(T* first, const T* last, T* dest, IsPod<true>)
{
    size_t n = last - first;
    memmove(dest, first, n * sizeof(T));
    return dest + n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T* BaseVector<T>::MoveOverlap(T* first, const T* last, T* dest, IsPod<false>)
{
    while (first != last)
    {
        *dest++ = MoveArg(*first++);
    }

    return dest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::MoveBackward(T* first, const T* last, T* destLast, IsPod<true>)
{
    size_t n = last - first;
    memmove(destLast - n, first, n * sizeof(T));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
inline void BaseVector<T>::MoveBackward(T* first, const T* last, T* destLast, IsPod<false>)
{
    while (first != last)
    {
        *(--destLast) = MoveArg(*(--last));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline Vector<T, N>::Vector(): BaseVector<T>(N)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline Vector<T, N>::Vector(uint32_t n, const T& v): BaseVector<T>(N)
{
    this->Append(n, v);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline Vector<T, N>::Vector(const T* first, const T* last): BaseVector<T>(N)
{
    this->Append(first, last);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline Vector<T, N>::Vector(const Vector& v): BaseVector<T>(N)
{
    if (!v.Empty())
    {
        this->Assign(v.Begin(), v.End());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline Vector<T, N>::Vector(Vector&& v): BaseVector<T>(N)
{
    if (!v.Empty())
    {
        this->Assign(MoveArg(v));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline const Vector<T, N>& Vector<T, N>::operator=(const Vector& v)
{
    if (this != &v)
    {
        this->Assign(v.Begin(), v.End());
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline const Vector<T, N>& Vector<T, N>::operator=(Vector&& v)
{
    if (this != &v)
    {
        this->Assign(MoveArg(v));
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline void Vector<T, N>::ShrinkToFit()
{
    if (!this->mIsSmall && this->mSize != this->mCapacity)
    {
        if (this->mSize == 0)
        {
            Dealloc(this->Begin());
            this->ResetToSmall(N);
        }
        else
        {
            typedef typename BaseVector<T>::template IsPod<IsTriviallyCopiable<T>::Result> IsPod;
            void* buffer = this->mSize <= N ? this->GetFirst() : Alloc(this->mSize * sizeof(T));
            this->UninitializedMove(this->Begin(), this->End(), (T*)buffer, IsPod());
            this->DestroyRange(this->Begin(), this->End(), IsPod());
            Dealloc(this->Begin());

            this->mBegin = buffer;
            this->mCapacity = Max(this->mSize, N);
            this->mIsSmall = this->mSize <= N ? 1 : 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
inline bool Vector<T, N>::IsSmall() const
{
    return this->mIsSmall > 0;
}

/// Range-based loop helpers
template<typename T> T* begin(BaseVector<T>& v) { return v.Begin(); }
template<typename T> const T* begin(const BaseVector<T>& v) { return v.Begin(); }
template<typename T> T* end(BaseVector<T>& v) { return v.End(); }
template<typename T> const T* end(const BaseVector<T>& v) { return v.End(); }

}
