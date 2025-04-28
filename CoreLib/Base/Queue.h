#pragma once

#include <CoreLib/Base/BaseArray.h>

BEGIN_SE()

template <class T> class Queue;

template <class T>
class QueueIterator
{
public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = int32_t;
    using size_type = uint32_t;
    using iterator_category = std::random_access_iterator_tag;

    QueueIterator(Queue<T>& q, int32_t i) : queue_(q), index_(i) {}

    QueueIterator operator ++ ()
    {
        QueueIterator<T> it(queue_, index_);
        index_++;
        return it;
    }

    QueueIterator& operator ++ (int)
    {
        index_++;
        return *this;
    }

    bool operator == (QueueIterator const& it) const
    {
        return it.index_ == index_;
    }

    bool operator != (QueueIterator const& it) const
    {
        return it.index_ != index_;
    }

    QueueIterator operator + (difference_type n) const
    {
        return QueueIterator(queue_, index_ + n);
    }

    QueueIterator operator - (difference_type n) const
    {
        return QueueIterator(queue_, index_ - n);
    }

    difference_type operator - (QueueIterator const& o) const
    {
        return (difference_type)(index_ - o.index_);
    }

    T& operator * () const
    {
        return queue_[index_];
    }

    T* operator -> () const
    {
        return &queue_[index_];
    }

    T* get () const
    {
        return &queue_[index_];
    }

private:
    Queue<T>& queue_;
    int32_t index_;
};


template <class T>
class QueueConstIterator
{
public:
    using value_type = T;
    using reference = T const&;
    using pointer = T const*;
    using difference_type = int32_t;
    using size_type = uint32_t;
    using iterator_category = std::random_access_iterator_tag;

    QueueConstIterator(Queue<T> const& q, int32_t i) : queue_(q), index_(i) {}

    QueueConstIterator operator ++ ()
    {
        QueueConstIterator<T> it(queue_, index_);
        index_++;
        return it;
    }

    QueueConstIterator& operator ++ (int)
    {
        index_++;
        return *this;
    }

    bool operator == (QueueConstIterator const& it) const
    {
        return it.index_ == index_;
    }

    bool operator != (QueueConstIterator const& it) const
    {
        return it.index_ != index_;
    }

    QueueConstIterator operator + (difference_type n) const
    {
        return QueueConstIterator(queue_, index_ + n);
    }

    QueueConstIterator operator - (difference_type n) const
    {
        return QueueConstIterator(queue_, index_ - n);
    }

    difference_type operator - (QueueConstIterator const& o) const
    {
        return (difference_type)(index_ - o.index_);
    }

    T const& operator * () const
    {
        return queue_[index_];
    }

    T const* operator -> () const
    {
        return &queue_[index_];
    }

    T const* get() const
    {
        return &queue_[index_];
    }

private:
    Queue<T> const& queue_;
    uint32_t index_;
};

template <class T>
class Queue
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using iterator = QueueIterator<T>;
    using const_iterator = QueueConstIterator<T>;
    using difference_type = int32_t;
    using size_type = uint32_t;

    inline Queue() {}

    Queue(Queue const& o)
    {
        copy_from(o);
    }

    Queue(Queue&& o)
        : buf_(o.buf_),
        capacity_(o.capacity_),
        size_(o.size_),
        readIndex_(o.readIndex_),
        writeIndex_(o.writeIndex_)
    {
        o.buf_ = nullptr;
        o.capacity_ = 0;
        o.readIndex_ = -1;
        o.writeIndex_ = 0;
    }

    ~Queue()
    {
        if (buf_) {
            GameDeleteArray<T>(buf_, capacity_);
        }
    }

    Queue& operator =(Queue const& o)
    {
        copy_from(o);
        return *this;
    }

    Queue& operator =(Queue&& o)
    {
        buf_ = o.buf_;
        capacity_ = o.capacity_;
        size_ = o.size_;
        readIndex_ = o.readIndex_;
        writeIndex_ = o.writeIndex_;

        o.buf_ = nullptr;
        o.capacity_ = 0;
        o.size_ = 0;
        o.readIndex_ = -1;
        o.writeIndex_ = 0;
        return *this;
    }

    size_type size() const
    {
        return size_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    size_type capacity() const
    {
        return capacity_;
    }

    inline T const& operator [] (size_type index) const
    {
        return buf_[(readIndex_ + index) % capacity_];
    }

    inline T& operator [] (size_type index)
    {
        return buf_[(readIndex_ + index) % capacity_];
    }

    iterator begin()
    {
        return iterator(*this, 0);
    }

    const_iterator begin() const
    {
        return const_iterator(*this, 0);
    }

    iterator end()
    {
        return iterator(*this, size());
    }

    const_iterator end() const
    {
        return const_iterator(*this, size());
    }

    void push_back(T const& value)
    {
        if (capacity_ == 0 || (size_ > 0 && readIndex_ == writeIndex_)) {
            resize(capacity_increment());
        }

        if (size_ == 0) {
            buf_[0] = value;
            size_ = 1;
            writeIndex_ = 1;
            readIndex_ = 0;
        } else {
            buf_[writeIndex_] = value;
            size_++;
            writeIndex_++;
        }

        if (writeIndex_ == capacity_) {
            writeIndex_ = 0;
        }
    }

    T pop()
    {
        se_assert(size_ > 0);

        T val = buf_[readIndex_];
        size_--;
        if (++readIndex_ == capacity_) {
            readIndex_ = 0;
        }

        if (readIndex_ == writeIndex_) {
            readIndex_ = -1;
            writeIndex_ = 0;
        }

        return val;
    }

    void resize(size_type newCapacity)
    {
        auto newBuf = GameAllocArray<T>(newCapacity);

        if (readIndex_ < writeIndex_) {
            auto wr = 0;
            for (int32_t i = readIndex_ + 1; i < writeIndex_; i++, wr++) {
                newBuf[wr] = buf_[i];
            }
        } else {
            auto wr = 0;
            for (int32_t i = readIndex_ + 1; i < (int32_t)capacity_; i++, wr++) {
                newBuf[wr] = buf_[i];
            }

            for (int32_t i = 0; i < writeIndex_; i++, wr++) {
                newBuf[wr] = buf_[i];
            }
        }

        size_ = std::min(size_, capacity_);
        for (size_type i = 0; i < std::min(size_, newCapacity); i++) {
            newBuf[i] = buf_[i];
        }

        if (buf_ != nullptr) {
            GameDeleteArray<T>(buf_, capacity_);
        }

        buf_ = newBuf;
        capacity_ = newCapacity;
        readIndex_ = 0;
        writeIndex_ = size_;
    }

    void clear()
    {
        size_ = 0; 
        readIndex_ = 0;
        writeIndex_ = 0;
    }

    T const* raw_buf() const
    {
        return buf_;
    }

private:
    T* buf_{ nullptr };
    size_type capacity_{ 0 };
    size_type size_{ 0 };
    difference_type readIndex_{ -1 };
    difference_type writeIndex_{ 0 };

    void copy_from(Queue const& a)
    {
        readIndex_ = a.readIndex_;
        writeIndex_ = a.writeIndex_;

        reallocate(a.capacity_);
        size_ = a.size_;
        for (size_type i = 0; i < capacity_; i++) {
            buf_[i] = a.buf_[i];
        }
    }

    size_type capacity_increment() const
    {
        if (capacity_ > 0) {
            return 2 * capacity_;
        } else {
            return 1;
        }
    }
};

END_SE()
