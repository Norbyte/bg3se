#pragma once

#include <cstdint>
#include <span>

BEGIN_SE()

template <class T>
class ContiguousIterator
{
public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = int32_t;
    using size_type = uint32_t;
    using iterator_category = std::contiguous_iterator_tag;

    ContiguousIterator(T* p) : ptr_(p) {}

    ContiguousIterator operator ++ ()
    {
        ContiguousIterator<T> it(ptr_);
        ptr_++;
        return it;
    }

    ContiguousIterator& operator ++ (int)
    {
        ptr_++;
        return *this;
    }

    bool operator == (ContiguousIterator const& it) const
    {
        return it.ptr_ == ptr_;
    }

    bool operator != (ContiguousIterator const& it) const
    {
        return it.ptr_ != ptr_;
    }

    ContiguousIterator operator + (difference_type n) const
    {
        return ContiguousIterator(ptr_ + n);
    }

    ContiguousIterator operator - (difference_type n) const
    {
        return ContiguousIterator(ptr_ - n);
    }

    difference_type operator - (ContiguousIterator const& o) const
    {
        return (difference_type)(ptr_ - o.ptr_);
    }

    T& operator * () const
    {
        return *ptr_;
    }

    T* operator -> () const
    {
        return ptr_;
    }

    T* get () const
    {
        return ptr_;
    }

private:
    T* ptr_;
};


template <class T>
class ContiguousConstIterator
{
public:
    using value_type = T;
    using reference = T const&;
    using pointer = T const*;
    using difference_type = int32_t;
    using size_type = uint32_t;
    using iterator_category = std::contiguous_iterator_tag;

    ContiguousConstIterator(T const* p) : ptr_(p) {}

    ContiguousConstIterator operator ++ ()
    {
        ContiguousConstIterator<T> it(ptr_);
        ptr_++;
        return it;
    }

    ContiguousConstIterator& operator ++ (int)
    {
        ptr_++;
        return *this;
    }

    bool operator == (ContiguousConstIterator const& it) const
    {
        return it.ptr_ == ptr_;
    }

    bool operator != (ContiguousConstIterator const& it) const
    {
        return it.ptr_ != ptr_;
    }

    ContiguousConstIterator operator + (difference_type n) const
    {
        return ContiguousConstIterator(ptr_ + n);
    }

    ContiguousConstIterator operator - (difference_type n) const
    {
        return ContiguousConstIterator(ptr_ - n);
    }

    difference_type operator - (ContiguousConstIterator const& o) const
    {
        return (difference_type)(ptr_ - o.ptr_);
    }

    T const& operator * () const
    {
        return *ptr_;
    }

    T const* operator -> () const
    {
        return ptr_;
    }

    T const* get() const
    {
        return ptr_;
    }

private:
    T const* ptr_;
};


template <class TWord, unsigned NumWords>
struct BitArray
{
    static constexpr uint32_t BitsPerWord = sizeof(TWord) * CHAR_BIT;
    static constexpr uint32_t IndexBitsPerWord = (sizeof(TWord) == 4) ? 5 : 6;
    static constexpr uint32_t NumBits = NumWords * BitsPerWord;

    TWord Bits[NumWords];

    inline bool operator [] (uint32_t index) const
    {
        if (index >= NumBits) {
            return false;
        }

        return (Bits[index >> IndexBitsPerWord] & (TWord(1) << (index & (BitsPerWord - 1)))) != 0;
    }

    inline bool Set(uint32_t index)
    {
        if (index >= NumBits) {
            return false;
        }

        Bits[index >> IndexBitsPerWord] |= (TWord(1) << (index & (BitsPerWord - 1)));
        return true;
    }

    inline bool Clear(uint32_t index)
    {
        if (index >= NumBits) {
            return false;
        }

        Bits[index >> IndexBitsPerWord] &= ~(TWord(1) << (index & (BitsPerWord - 1)));
        return true;
    }

    inline bool IsSet(uint32_t index) const
    {
        if (index >= NumBits) {
            return false;
        }

        return (Bits[index >> IndexBitsPerWord] & (TWord(1) << (index & (BitsPerWord - 1)))) != 0;
    }

    inline uint32_t size() const
    {
        return NumWords * sizeof(TWord) * CHAR_BIT;
    }
};

template <class T>
class StaticArray
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using iterator = ContiguousIterator<T>;
    using const_iterator = ContiguousConstIterator<T>;
    using difference_type = int32_t;
    using size_type = uint32_t;

    inline StaticArray() {}
    
    StaticArray(size_type size)
    {
        Resize(size);
    }
    
    StaticArray(size_type size, T const& initval)
    {
        Resize(size, initval);
    }

    StaticArray(StaticArray const& a)
    {
        CopyFrom(a);
    }

    StaticArray(StaticArray&& a)
    {
        if (this != &a) {
            buf_ = a.buf_;
            size_ = a.size_;
            a.buf_ = nullptr;
            a.size_ = 0;
        }
    }

    ~StaticArray()
    {
        if (buf_) {
            for (size_type i = 0; i < size_; i++) {
                buf_[i].~T();
            }

            GameFree(buf_);
        }
    }

    StaticArray& operator =(StaticArray const& a)
    {
        CopyFrom(a);
        return *this;
    }

    StaticArray& operator =(StaticArray&& a)
    {
        if (this != &a) {
            buf_ = a.buf_;
            size_ = a.size_;
            a.buf_ = nullptr;
            a.size_ = 0;
        }

        return *this;
    }

    void CopyFrom(StaticArray const& a)
    {
        clear();

        if (a.size_ != size_) {
            Resize(a.size_);
            for (size_type i = 0; i < size_; i++) {
                new (buf_ + i) T(a[i]);
            }
        }
    }

    inline T* raw_buf() const
    {
        return buf_;
    }

    inline unsigned int size() const
    {
        return size_;
    }

    inline unsigned int Size() const
    {
        return size_;
    }

    inline T const& operator [] (size_type index) const
    {
        assert(index < size_);
        return buf_[index];
    }

    inline T& operator [] (size_type index)
    {
        assert(index < size_);
        return buf_[index];
    }

    void clear()
    {
        Resize(0);
    }

    void Resize(size_type newSize, T const& initval)
    {
        if (size_ != newSize) {
            T* newBuf;
            if (newSize > 0) {
                newBuf = GameMemoryAllocator::NewRaw<T>(newSize);
            } else {
                newBuf = nullptr;
            }

            for (size_type i = 0; i < std::min(size_, newSize); i++) {
                new (newBuf + i) T(std::move(buf_[i]));
            }
            
            for (size_type i = std::min(size_, newSize); i < newSize; i++) {
                new (newBuf + i) T(initval);
            }

            if (buf_ != nullptr) {
                for (size_type i = 0; i < size_; i++) {
                    buf_[i].~T();
                }

                GameFree(buf_);
            }

            buf_ = newBuf;
            size_ = newSize;
        }
    }

    void Resize(size_type newSize)
    {
        if (size_ != newSize) {
            T* newBuf;
            if (newSize > 0) {
                newBuf = GameMemoryAllocator::NewRaw<T>(newSize);
            } else {
                newBuf = nullptr;
            }

            for (size_type i = 0; i < std::min(size_, newSize); i++) {
                new (newBuf + i) T(std::move(buf_[i]));
            }
            
            for (size_type i = std::min(size_, newSize); i < newSize; i++) {
                new (newBuf + i) T();
            }

            if (buf_ != nullptr) {
                for (size_type i = 0; i < size_; i++) {
                    buf_[i].~T();
                }

                GameFree(buf_);
            }

            buf_ = newBuf;
            size_ = newSize;
        }
    }

    iterator begin()
    {
        return iterator(buf_);
    }

    const_iterator begin() const
    {
        return const_iterator(buf_);
    }

    iterator end()
    {
        return iterator(buf_ + size_);
    }

    const_iterator end() const
    {
        return const_iterator(buf_ + size_);
    }

private:
    T* buf_{ nullptr };
    size_type size_{ 0 };
};

template <class T>
class UninitializedStaticArray
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using difference_type = int32_t;
    using size_type = uint32_t;

    inline UninitializedStaticArray() {}
    
    UninitializedStaticArray(size_type size)
    {
        resize(size);
    }

    UninitializedStaticArray(UninitializedStaticArray const& a) = delete;

    UninitializedStaticArray(UninitializedStaticArray&& a)
    {
        if (this != &a) {
            buf_ = a.buf_;
            size_ = a.size_;
            a.buf_ = nullptr;
            a.size_ = 0;
        }
    }

    ~UninitializedStaticArray()
    {
        GameFree(buf_);
    }

    UninitializedStaticArray& operator =(UninitializedStaticArray const& a) = delete;

    UninitializedStaticArray& operator =(UninitializedStaticArray&& a)
    {
        if (this != &a) {
            buf_ = a.buf_;
            size_ = a.size_;
            a.buf_ = nullptr;
            a.size_ = 0;
        }

        return *this;
    }

    inline T* raw_buf()
    {
        return buf_;
    }

    inline T const* raw_buf() const
    {
        return buf_;
    }

    inline unsigned int size() const
    {
        return size_;
    }

    inline T const& operator [] (size_type index) const
    {
        assert(index < size_);
        return buf_[index];
    }

    inline T& operator [] (size_type index)
    {
        assert(index < size_);
        return buf_[index];
    }

    void clear(size_type initializedCapacity)
    {
        resize(0, 0, initializedCapacity);
    }

    void resize(size_type newSize, size_type newInitializedCapacity, size_type initializedCapacity)
    {
        assert(initializedCapacity <= size_);
        assert(newInitializedCapacity <= newSize);

        if (size_ != newSize) {
            T* newBuf;
            if (newSize > 0) {
                newBuf = GameMemoryAllocator::NewRaw<T>(newSize);
            } else {
                newBuf = nullptr;
            }

            for (size_type i = 0; i < std::min(initializedCapacity, newInitializedCapacity); i++) {
                new (newBuf + i) T(std::move(buf_[i]));
            }
            
            for (size_type i = std::min(initializedCapacity, newInitializedCapacity); i < newInitializedCapacity; i++) {
                new (newBuf + i) T();
            }

            if (buf_ != nullptr) {
                for (size_type i = 0; i < initializedCapacity; i++) {
                    buf_[i].~T();
                }

                GameFree(buf_);
            }

            buf_ = newBuf;
            size_ = newSize;
        }
    }

    void copy_from(UninitializedStaticArray const& a, size_type curInitializedCapacity, size_type newInitializedCapacity)
    {
        assert(newInitializedCapacity <= a.size());
        resize(a.size(), newInitializedCapacity, curInitializedCapacity);
        for (size_type i = 0; i < newInitializedCapacity; i++) {
            buf_[i] = a.buf_[i];
        }
    }

    size_type grow_size() const
    {
        if (size_ > 0) {
            return 2 * size_;
        } else {
            return 1;
        }
    }

private:
    T* buf_{ nullptr };
    size_type size_{ 0 };
};

template <class T>
class Array
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using iterator = ContiguousIterator<T>;
    using const_iterator = ContiguousConstIterator<T>;
    using difference_type = int32_t;
    using size_type = uint32_t;

    inline constexpr Array() noexcept {}

    Array(Array const& a)
    {
        CopyFrom(a);
    }

    Array(Array&& a) noexcept
    {
        if (this != &a) {
            buf_ = a.buf_;
            capacity_ = a.capacity_;
            size_ = a.size_;
            a.buf_ = nullptr;
            a.capacity_ = 0;
            a.size_ = 0;
        }
    }

    ~Array()
    {
        if (buf_) {
            clear();
            GameFree(buf_);
        }
    }

    Array& operator =(Array const& a)
    {
        CopyFrom(a);
        return *this;
    }

    Array& operator =(Array&& a) noexcept
    {
        if (this != &a) {
            buf_ = a.buf_;
            capacity_ = a.capacity_;
            size_ = a.size_;
            a.buf_ = nullptr;
            a.capacity_ = 0;
            a.size_ = 0;
        }
        return *this;
    }

    void CopyFrom(Array const& a)
    {
        clear();

        if (a.size_ > 0) {
            Reallocate(a.size_);
            size_ = a.size_;
            for (size_type i = 0; i < size_; i++) {
                new (buf_ + i) T(a[i]);
            }
        }
    }

    inline constexpr T* raw_buf() const noexcept
    {
        return buf_;
    }

    inline constexpr bool empty() const noexcept
    {
        return size_ == 0;
    }

    inline constexpr size_type size() const noexcept
    {
        return size_;
    }

    inline constexpr size_type capacity() const noexcept
    {
        return capacity_;
    }

    inline constexpr size_type Size() const noexcept
    {
        return size_;
    }

    inline T const& operator [] (size_type index) const
    {
        assert(index < size_);
        return buf_[index];
    }

    inline T& operator [] (size_type index)
    {
        assert(index < size_);
        return buf_[index];
    }

    constexpr size_type CapacityIncrement() const noexcept
    {
        if (capacity_ > 0) {
            return 2 * capacity_;
        } else {
            return 1;
        }
    }

    void clear()
    {
        for (size_type i = 0; i < size_; i++) {
            buf_[i].~T();
        }

        size_ = 0;
    }

    void Reallocate(size_type newCapacity)
    {
        auto newBuf = GameMemoryAllocator::NewRaw<T>(newCapacity);
        for (size_type i = 0; i < std::min(size_, newCapacity); i++) {
            new (newBuf + i) T(std::move(buf_[i]));
        }

        if (buf_ != nullptr) {
            for (size_type i = 0; i < size_; i++) {
                buf_[i].~T();
            }

            GameFree(buf_);
        }

        buf_ = newBuf;
        capacity_ = newCapacity;
    }

    void resize(size_type newSize)
    {
        if (newSize > capacity_) {
            Reallocate(newSize);
        }

        if (size_ > newSize) {
            for (size_type i = newSize; i < size_; i++) {
                buf_[i].~T();
            }
        } else {
            for (size_type i = size_; i < newSize; i++) {
                new (buf_ + i) T();
            }
        }

        size_ = newSize;
    }

    void Add(T const& value)
    {
        if (capacity_ <= size_) {
            Reallocate(CapacityIncrement());
        }

        new (&buf_[size_++]) T(value);
    }

    T& push_back(T const& value)
    {
        if (capacity_ <= size_) {
            Reallocate(CapacityIncrement());
        }

        return *(new (&buf_[size_++]) T(value));
    }

    T& push_back(T&& value)
    {
        if (capacity_ <= size_) {
            Reallocate(CapacityIncrement());
        }

        return *(new (&buf_[size_++]) T(std::move(value)));
    }

    void ordered_insert_at(size_type index, T const& value)
    {
        assert(index <= size_);
        if (capacity_ <= size_) {
            Reallocate(CapacityIncrement());
        }

        new (&buf_[size_++]) T();

        for (size_type i = size_ - 1; i > index; i++) {
            buf_[i] = std::move(buf_[i - 1]);
        }

        buf_[index] = value;
    }

    void insert_at(size_type index, T const& value)
    {
        assert(index <= size_);
        if (capacity_ <= size_) {
            Reallocate(CapacityIncrement());
        }

        new (&buf_[size_]) T(buf_[index]);
        buf_[index] = value;
        size_++;
    }

    void remove_at(size_type index)
    {
        assert(index < size_);

        if (index + 1 < size_) {
            buf_[index] = std::move(buf_[size_ - 1]);
        }

        buf_[size_ - 1].~T();
        size_--;
    }

    void ordered_remove_at(size_type index)
    {
        assert(index < size_);

        for (size_type i = index; i < size_ - 1; i++) {
            buf_[i] = std::move(buf_[i + 1]);
        }

        buf_[size_ - 1].~T();
        size_--;
    }

    void erase(iterator const& it)
    {
        assert(it != end());
        ordered_remove_at((size_type)(it.get() - buf_));
    }

    void remove_last()
    {
        assert(size_ > 0);
        buf_[size_ - 1].~T();
        size_--;
    }

    T pop_last()
    {
        assert(size_ > 0);
        return std::move(buf_[--size_]);
    }

    iterator find(T const& v)
    {
        for (size_type i = 0; i < size_; i++) {
            if (buf_[i] == v) return iterator(buf_ + i);
        }

        return end();
    }

    const_iterator find(T const& v) const
    {
        for (size_type i = 0; i < size_; i++) {
            if (buf_[i] == v) return const_iterator(buf_ + i);
        }

        return end();
    }

    iterator begin()
    {
        return iterator(buf_);
    }

    const_iterator begin() const
    {
        return const_iterator(buf_);
    }

    iterator end()
    {
        return iterator(buf_ + size_);
    }

    const_iterator end() const
    {
        return const_iterator(buf_ + size_);
    }

private:
    T* buf_{ nullptr };
    size_type capacity_{ 0 };
    size_type size_{ 0 };
};


template <class T>
class LegacyArray : public Array<T>
{
public:
    virtual ~LegacyArray() {}

private:
    uint32_t Used{ 0 };
};

END_SE()
