////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef NS_COMPILER_MSVC
#include <malloc.h>
#endif


namespace Noesis
{

template<typename T> struct HashKeyInfo {};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct HashKeyInfo<uint32_t>
{
    static bool IsEmpty(uint32_t key) { return key == 0xffffffff; }
    static void MarkEmpty(uint32_t& key) { key = 0xffffffff; } 
    static uint32_t HashValue(uint32_t key) { return key; }
    static bool IsEqual(uint32_t lhs, uint32_t rhs) { return lhs == rhs; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct HashKeyInfo<int32_t>
{
    static bool IsEmpty(int32_t key) { return key == 0x7fffffff; }
    static void MarkEmpty(int32_t& key) { key = 0x7fffffff; } 
    static uint32_t HashValue(int32_t key) { return (uint32_t)key; }
    static bool IsEqual(int32_t lhs, int32_t rhs) { return lhs == rhs; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct HashKeyInfo<uint16_t>
{
    static bool IsEmpty(uint16_t key) { return key == 0xffff; }
    static void MarkEmpty(uint16_t& key) { key = 0xffff; } 
    static uint32_t HashValue(uint16_t key) { return key; }
    static bool IsEqual(uint16_t lhs, uint16_t rhs) { return lhs == rhs; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<> struct HashKeyInfo<uint8_t>
{
    static bool IsEmpty(uint8_t key) { return key == 0xff; }
    static void MarkEmpty(uint8_t& key) { key = 0xff; } 
    static uint32_t HashValue(uint8_t key) { return key; }
    static bool IsEqual(uint8_t lhs, uint8_t rhs) { return lhs == rhs; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> struct HashKeyInfo<T*>
{
    static bool IsEmpty(T* key) { return key == (T*)0x01; }
    static void MarkEmpty(T*& key) { key = (T*)0x01; }
    static uint32_t HashValue(T* key) { return Hash(key); }
    static bool IsEqual(T* lhs, T* rhs) { return lhs == rhs; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct CharArrayHashKeyInfo
{
    static bool IsEmpty(const char* key) { return key == (const char*)0x01; }
    static void MarkEmpty(const char*& key) { key = (const char*)0x01; }
    static uint32_t HashValue(const char* key) { return StrHash(key); }

    static bool IsEqual(const char* lhs, const char* rhs)
    {
        return !IsEmpty(lhs) && StrEquals(lhs, rhs);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct CaseCharArrayHashKeyInfo
{
    static bool IsEmpty(const char* key) { return key == (const char*)0x01; }
    static void MarkEmpty(const char*& key) { key = (const char*)0x01; }
    static uint32_t HashValue(const char* key) { return StrCaseHash(key); }

    static bool IsEqual(const char* lhs, const char* rhs)
    {
        return !IsEmpty(lhs) && StrCaseEquals(lhs, rhs);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Bucket storing key and value
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename KeyT, typename ValueT, typename KeyInfoT>
class HashBucket_KV
{
public:
    typedef KeyInfoT KeyInfo;
    typedef KeyT Key;

    const KeyT key;
    ValueT value;

    uint32_t Hash() const { return KeyInfo::HashValue(key); };

private:
    template<typename Bucket> friend class HashMapImpl;

    bool IsEmpty() const { return KeyInfo::IsEmpty(key); }
    void MarkEmpty() { KeyInfo::MarkEmpty((KeyT&)key); }

    typedef HashBucket_KV Content;
    Content* GetContent() { return this; }

    typedef const HashBucket_KV ConstContent;
    ConstContent* GetContent() const { return this; }

    template<typename Key_, typename... ValueArgs>
    void Create(uint32_t, Key_&& key_, ValueArgs&&... values)
    {
        new ((KeyT*)&key) KeyT(ForwardArg<Key_>(key_));
        new (&value) ValueT(ForwardArg<ValueArgs>(values)...);
    }

    template<class Key_>
    bool IsEqual(uint32_t, const Key_& key_) const { return KeyInfo::IsEqual(key, key_); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Bucket storing key, value and hash for efficient comparisons and rehashes
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename KeyT, typename ValueT, typename KeyInfoT>
class HashBucket_KHV
{
public:
    typedef KeyInfoT KeyInfo;
    typedef KeyT Key;

    const KeyT key;
    uint32_t hash;
    ValueT value;

    uint32_t Hash() const { return hash; }

private:
    template<typename Bucket> friend class HashMapImpl;

    bool IsEmpty() const { return KeyInfo::IsEmpty(key); }
    void MarkEmpty() { KeyInfo::MarkEmpty((KeyT&)key); }

    typedef HashBucket_KHV Content;
    Content* GetContent() { return this; }

    typedef const HashBucket_KHV ConstContent;
    ConstContent* GetContent() const { return this; }

    template<typename Key_, typename... ValueArgs>
    void Create(uint32_t hash_, Key_&& key_, ValueArgs&&... values)
    {
        hash = hash_;
        new ((KeyT*)&key) KeyT(ForwardArg<Key_>(key_));
        new (&value) ValueT(ForwardArg<ValueArgs>(values)...);
    }

    template<class Key_>
    bool IsEqual(uint32_t hash_, const Key_& key_) const
    {
        return hash == hash_ && KeyInfo::IsEqual(key, key_);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Bucket storing key
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename KeyT, typename KeyInfoT>
class HashBucket_K
{
public:
    typedef KeyInfoT KeyInfo;
    typedef KeyT Key;

    const KeyT key;

    uint32_t Hash() const { return KeyInfo::HashValue(key); };

private:
    template<typename Bucket> friend class HashMapImpl;

    bool IsEmpty() const { return KeyInfo::IsEmpty(key); }
    void MarkEmpty() { KeyInfo::MarkEmpty((KeyT&)key); }

    typedef const KeyT Content;
    Content* GetContent() { return &key; }

    typedef const KeyT ConstContent;
    ConstContent* GetContent() const { return &key; }

    template<typename Key_, typename... ValueArgs>
    void Create(uint32_t, Key_&& key_, ValueArgs&&...)
    {
        new ((KeyT*)&key) KeyT(ForwardArg<Key_>(key_));
    }

    template<class Key_>
    bool IsEqual(uint32_t, const Key_& key_) const { return KeyInfo::IsEqual(key, key_); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Bucket storing key and hash for efficient comparisons and rehashes
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename KeyT, typename KeyInfoT>
class HashBucket_KH
{
public:
    typedef KeyInfoT KeyInfo;
    typedef KeyT Key;

    const KeyT key;
    uint32_t hash;

    uint32_t Hash() const { return hash; };

private:
    template<typename Bucket> friend class HashMapImpl;

    bool IsEmpty() const { return KeyInfo::IsEmpty(key); }
    void MarkEmpty() { KeyInfo::MarkEmpty((KeyT&)key); }

    typedef const KeyT Content;
    Content* GetContent() { return &key; }

    typedef const KeyT ConstContent;
    ConstContent* GetContent() const { return &key; }

    template<typename Key_, typename... ValueArgs>
    void Create(uint32_t hash_, Key_&& key_, ValueArgs&&...)
    {
        hash = hash_;
        new ((KeyT*)&key) KeyT(ForwardArg<Key_>(key_));
    }

    template<class Key_>
    bool IsEqual(uint32_t hash_, const Key_& key_) const
    {
        return hash == hash_ && KeyInfo::IsEqual(key, key_);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename Value>
struct HashBucket
{
    // By default the hash is not cached
    typedef HashBucket_KV<Key, Value, HashKeyInfo<Key>> Type;
};

template<typename Value>
struct HashBucket<const char*, Value>
{
    // For strings the hash is cached to have faster comparisons and rehashes
    typedef HashBucket_KHV<const char*, Value, CharArrayHashKeyInfo> Type;
};

template<typename Key>
struct SetBucket
{
    // By default the hash is not cached
    typedef HashBucket_K<Key, HashKeyInfo<Key>> Type;
};

template<>
struct SetBucket<const char*>
{
    // For strings the hash is cached to have faster comparisons and rehashes
    typedef HashBucket_KH<const char*, CharArrayHashKeyInfo> Type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
class HashMapImpl<Bucket>::Iterator
{
public:
    Iterator(): mPtr(nullptr), mEnd(nullptr) {}

    typedef typename Bucket::Content Content;
    Content& operator*() const { NS_ASSERT(mPtr != mEnd); return *mPtr->GetContent(); }
    Content* operator->() const { NS_ASSERT(mPtr != mEnd); return mPtr->GetContent(); }

    bool operator==(const Iterator& it) const { return mPtr == it.mPtr; }
    bool operator!=(const Iterator& it) const { return mPtr != it.mPtr; }
    Iterator& operator++() { NS_ASSERT(mPtr != mEnd); ++mPtr; SkipEmpty(); return *this; }

private:
    Iterator(Bucket* ptr): mPtr(ptr), mEnd(ptr) {}
    Iterator(Bucket* ptr, Bucket* end): mPtr(ptr), mEnd(end) {}
    Iterator(Bucket* ptr, Bucket* end, bool): mPtr(ptr), mEnd(end)
    {
        NS_ASSERT(mPtr != mEnd);
        SkipEmpty();
    }

    void SkipEmpty() { while (mPtr != mEnd && mPtr->IsEmpty()) { ++mPtr; } }

    friend class HashMapImpl<Bucket>;
    friend class HashMapImpl<Bucket>::ConstIterator;

private:
    Bucket* mPtr;
    Bucket* mEnd;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
class HashMapImpl<Bucket>::ConstIterator
{
public:
    ConstIterator(): mPtr(nullptr), mEnd(nullptr) {}
    ConstIterator(const typename HashMapImpl<Bucket>::Iterator& it): mPtr(it.mPtr), mEnd(it.mEnd) {}

    typedef typename Bucket::ConstContent Content;
    Content& operator*() const { NS_ASSERT(mPtr != mEnd); return *mPtr->GetContent(); }
    Content* operator->() const { NS_ASSERT(mPtr != mEnd); return mPtr->GetContent(); }

    bool operator==(const ConstIterator& it) const { return mPtr == it.mPtr; }
    bool operator!=(const ConstIterator& it) const { return mPtr != it.mPtr; }
    ConstIterator& operator++() { NS_ASSERT(mPtr != mEnd); ++mPtr; SkipEmpty(); return *this; }

private:
    ConstIterator(const Bucket* ptr): mPtr(ptr), mEnd(ptr) {}
    ConstIterator(const Bucket* ptr, const Bucket* end): mPtr(ptr), mEnd(end) {}
    ConstIterator(const Bucket* ptr, const Bucket* end, bool): mPtr(ptr), mEnd(end)
    {
        NS_ASSERT(mPtr != mEnd);
        SkipEmpty();
    }

    void SkipEmpty() { while (mPtr != mEnd && mPtr->IsEmpty()) { ++mPtr; } }

    friend class HashMapImpl<Bucket>;

private:
    const Bucket* mPtr;
    const Bucket* mEnd;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline typename HashMapImpl<Bucket>::Iterator HashMapImpl<Bucket>::Begin()
{
    return Empty() ? End() : Iterator(mBuckets, mBuckets + mNumBuckets, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline typename HashMapImpl<Bucket>::ConstIterator HashMapImpl<Bucket>::Begin() const
{
    return Empty() ? End() : ConstIterator(mBuckets, mBuckets + mNumBuckets, true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline typename HashMapImpl<Bucket>::Iterator HashMapImpl<Bucket>::End()
{
    return Iterator(mBuckets + mNumBuckets);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline typename HashMapImpl<Bucket>::ConstIterator HashMapImpl<Bucket>::End() const
{
    return ConstIterator(mBuckets + mNumBuckets);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline bool HashMapImpl<Bucket>::Empty() const
{
    return mNumEntries == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline uint32_t HashMapImpl<Bucket>::Size() const
{
    return mNumEntries;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::Reserve(uint32_t numElements)
{
    uint32_t numBuckets = NumBucketsForSize(numElements);

    if (numBuckets > mNumBuckets)
    {
        Grow(numBuckets);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::Clear()
{
    if (mNumEntries != 0)
    {
        Bucket* end = mBuckets + mNumBuckets;
        for (Bucket* it = mBuckets; it != end; ++it)
        {
            if (!it->IsEmpty())
            {
                Delete(it);
            }
        }

        mNumEntries = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<class Key_>
inline typename HashMapImpl<Bucket>::Iterator HashMapImpl<Bucket>::Find(const Key_& key)
{
    uint32_t hash = Bucket::KeyInfo::HashValue(key);
    Bucket* bucket = FindBucket(hash, key);
    return bucket ? Iterator(bucket, mBuckets + mNumBuckets) : End();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<class Key_>
inline typename HashMapImpl<Bucket>::ConstIterator HashMapImpl<Bucket>::Find(const Key_& key) const
{
    uint32_t hash = Bucket::KeyInfo::HashValue(key);
    const Bucket* bucket = FindBucket(hash, key);
    return bucket ? ConstIterator(bucket, mBuckets + mNumBuckets) : End();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<class Key_, typename... Ts>
inline typename HashMapImpl<Bucket>::InsertResult HashMapImpl<Bucket>::Insert(Key_&& key,
    Ts&&... args)
{
    Bucket* bucket;
    uint32_t hash = Bucket::KeyInfo::HashValue(key);
    if (FindInsertBucket(hash, key, bucket))
    {
        return InsertResult(Iterator(bucket, mBuckets + mNumBuckets), false);
    }

    bucket = InsertIntoBucket(bucket, hash, ForwardArg<Key_>(key), ForwardArg<Ts>(args)...);
    return InsertResult(Iterator(bucket, mBuckets + mNumBuckets), true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<class Key_>
inline bool HashMapImpl<Bucket>::Erase(const Key_& key)
{
    uint32_t hash = Bucket::KeyInfo::HashValue(key);
    Bucket* bucket = FindBucket(hash, key);

    if (bucket)
    {
        Erase(bucket);
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline typename HashMapImpl<Bucket>::Iterator HashMapImpl<Bucket>::Erase(Iterator pos)
{
    NS_ASSERT(pos.mPtr >= mBuckets && pos.mPtr < mBuckets + mNumBuckets);
    NS_ASSERT(pos.mEnd == mBuckets + mNumBuckets);
    Bucket* bucket = pos.mPtr;
    Erase(bucket);
    return bucket->IsEmpty() ? ++pos : pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::Erase(Bucket* bucket)
{
    NS_ASSERT(!bucket->IsEmpty());
    mNumEntries--;

    const uint32_t bucketsMask = mNumBuckets - 1;
    uint32_t i = uint32_t(bucket - mBuckets);
    uint32_t j = i;

    // Shift backward all the entries in the same chain following the entry to delete
    // https://en.wikipedia.org/wiki/Open_addressing
    // This algorithm is the main reason we are doing linear probing instead of quadratic probing
    // With quadratic probing we need to do lazy deletions using tombstones. When the number of
    // tombstones is high we need to rehash which is slow.

    for (;;)
    {
        Delete(bucket);

        Bucket* bucket_;
        uint32_t k;

        do
        {
            j = (j + 1) & bucketsMask;
            bucket_ = mBuckets + j;

            if (bucket_->IsEmpty())
            {
                return;
            }

            k = Index(bucket_->Hash());
        }
        while ((i <= j) ? ((i < k) && (k <= j)) : ((i < k) || (k <= j)));

        new (bucket) Bucket(MoveArg(*bucket_));
        i = j;
        bucket = bucket_;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline HashMapImpl<Bucket>::HashMapImpl(Bucket* buckets, uint32_t numBuckets): mBuckets(buckets),
    mNumInlinedBuckets(numBuckets), mNumBuckets(0), mNumEntries(0), mHashShift(32)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline HashMapImpl<Bucket>::HashMapImpl(HashMapImpl&& m, Bucket* buckets, uint32_t numBuckets)
{
    NS_ASSERT(m.mNumInlinedBuckets == numBuckets);

    mNumInlinedBuckets = m.mNumInlinedBuckets;
    mNumBuckets = m.mNumBuckets;
    mHashShift = m.mHashShift;
    mNumEntries = m.mNumEntries;

    if (m.mNumBuckets > m.mNumInlinedBuckets || m.mBuckets == nullptr)
    {
        // If 'm' is not small, we just steal its heap pointer
        mBuckets = m.mBuckets;
    }
    else
    {
        // If 'm' is small we need to copy the buckets
        mBuckets = buckets;
        MoveBuckets(mBuckets, m.mBuckets, mNumBuckets);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline HashMapImpl<Bucket>::~HashMapImpl()
{
    DestroyBuckets();

    if (mNumBuckets > mNumInlinedBuckets)
    {
        Dealloc(mBuckets);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::InitBuckets()
{
    mNumEntries = 0;

    if (mNumBuckets > 0)
    {
        Bucket* end = mBuckets + mNumBuckets;
        for (Bucket* it = mBuckets; it != end; ++it)
        {
            it->MarkEmpty();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::DestroyBuckets()
{
    if (mNumBuckets > 0 && mNumEntries > 0)
    {
        Bucket* end = mBuckets + mNumBuckets;
        for (Bucket* it = mBuckets; it != end; ++it)
        {
            if (!it->IsEmpty())
            {
                it->~Bucket();
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::Delete(Bucket* bucket)
{
    bucket->~Bucket();
    bucket->MarkEmpty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline uint32_t HashMapImpl<Bucket>::Index(uint32_t hash) const
{
    // Fibonacci hashing
    return (2654435769U * hash) >> mHashShift;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
uint32_t HashMapImpl<Bucket>::NumBucketsForSize(uint32_t numElements)
{
    // For linear probing the recommened load factor is ~50%
    // This factor could be increased by implementing Robin Hood Hashing
    return numElements * 2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::AllocateBuckets(uint32_t numBuckets)
{
    NS_ASSERT(numBuckets > 0);
    NS_ASSERT(IsPow2(numBuckets));

    mNumBuckets = numBuckets;
    mHashShift = 32 - Log2(numBuckets);
    mBuckets = (Bucket*)Alloc(sizeof(Bucket) * mNumBuckets);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::Grow(uint32_t numBuckets)
{
    Bucket* oldBuckets = mBuckets;
    uint32_t oldNumBuckets = mNumBuckets;
    uint32_t newBucketCount = Max(4U, NextPow2(numBuckets));

    // If there is enough space in the inlined bucket we don't need to allocate memory
    if (newBucketCount <= mNumInlinedBuckets)
    {
        Bucket* buckets = oldNumBuckets ? (Bucket*)alloca(sizeof(Bucket) * oldNumBuckets) : nullptr;
        MoveBuckets(buckets, oldBuckets, oldNumBuckets);

        mNumBuckets = newBucketCount;
        mHashShift = 32 - Log2(mNumBuckets);
        InitBuckets();

        InsertBucketsFrom(buckets, oldNumBuckets);
    }
    else
    {
        AllocateBuckets(newBucketCount);
        InitBuckets();

        if (oldBuckets != nullptr)
        {
            InsertBucketsFrom(oldBuckets, oldNumBuckets);

            if (oldNumBuckets > mNumInlinedBuckets)
            {
                Dealloc(oldBuckets);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::MoveBuckets(Bucket* dst, Bucket* src, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        if (!src->IsEmpty())
        {
            new (dst) Bucket(MoveArg(*src));
            src->~Bucket();
        }
        else
        {
            dst->MarkEmpty();
        }

        ++src;
        ++dst;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
inline void HashMapImpl<Bucket>::InsertBucketsFrom(Bucket* src, uint32_t count)
{
    for (Bucket* it = src; it != src + count; ++it)
    {
        if (!it->IsEmpty())
        {
            // Insert the key/value into the new table.
            Bucket* bucket;
            bool found = FindInsertBucket(it->Hash(), it->key, bucket);
            NS_ASSERT(!found);

            new (bucket) Bucket(MoveArg(*it));
            it->~Bucket();

            mNumEntries++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<typename Key_, typename... ValueArgs>
inline Bucket* HashMapImpl<Bucket>::InsertIntoBucket(Bucket* bucket, uint32_t hash, Key_&& key,
    ValueArgs&&... values)
{
    // Grow the table if we reached max load factor
    if (NS_UNLIKELY(NumBucketsForSize(mNumEntries) >= mNumBuckets))
    {
        Grow(mNumBuckets * 2);
        bool found = FindInsertBucket(hash, key, bucket);
        NS_ASSERT(!found);
    }

    NS_ASSERT(bucket != nullptr && bucket->IsEmpty());
    mNumEntries++;

    bucket->Create(hash, ForwardArg<Key_>(key), ForwardArg<ValueArgs>(values)...);
    NS_ASSERT(!bucket->IsEmpty());

    return bucket;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<typename Key_>
inline bool HashMapImpl<Bucket>::FindInsertBucket(uint32_t hash, const Key_& key, Bucket*& foundBucket)
{
    if (mNumBuckets > 0)
    {
        Bucket* buckets = mBuckets;
        const uint32_t bucketsMask = mNumBuckets - 1;

        uint32_t index = Index(hash);
        NS_ASSERT(index < mNumBuckets);

        while (true)
        {
            Bucket* bucket = buckets + index;

            if (NS_LIKELY(bucket->IsEqual(hash, key)))
            {
                NS_ASSERT(!bucket->IsEmpty());
                foundBucket = bucket;
                return true;
            }

            if (NS_LIKELY(bucket->IsEmpty()))
            {
                foundBucket = bucket;
                return false;
            }

            // Continue linear probing
            index = (index + 1) & bucketsMask;
        }
    }

    foundBucket = nullptr;
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<typename Key_>
inline const Bucket* HashMapImpl<Bucket>::FindBucket(uint32_t hash, const Key_& key) const
{
    if (mNumBuckets > 0)
    {
        const Bucket* buckets = mBuckets;
        const uint32_t bucketsMask = mNumBuckets - 1;

        uint32_t index = Index(hash);
        NS_ASSERT(index < mNumBuckets);

        while (true)
        {
            const Bucket* bucket = buckets + index;

            if (NS_LIKELY(bucket->IsEqual(hash, key)))
            {
                NS_ASSERT(!bucket->IsEmpty());
                return bucket;
            }

            if (bucket->IsEmpty())
            {
                return nullptr;
            }

            // Continue linear probing
            index = (index + 1) & bucketsMask;
        }
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket> template<typename Key_>
inline Bucket* HashMapImpl<Bucket>::FindBucket(uint32_t hash, const Key_& key)
{
    return (Bucket*)((const HashMapImpl*)this)->FindBucket(hash, key);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename T, unsigned N, typename Bucket>
inline HashMap<Key, T, N, Bucket>::HashMap(): HashMapImpl<Bucket>(
    HashMapStorage<Bucket, N>::GetFirst(), HashMapStorage<Bucket, N>::GetCapacity()) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename T, unsigned N, typename Bucket>
inline HashMap<Key, T, N, Bucket>::HashMap(HashMap&& m): HashMapImpl<Bucket>(MoveArg(m),
    HashMapStorage<Bucket, N>::GetFirst(), HashMapStorage<Bucket, N>::GetCapacity())
{
    m.mBuckets = m.GetFirst();
    m.mNumBuckets = 0;
    m.mNumEntries = 0;
    m.mHashShift = 32;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename T, unsigned N, typename Bucket> template<class Key_>
inline T& HashMap<Key, T, N, Bucket>::operator[](const Key_& key)
{
    return this->Insert(key).first->value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename T, unsigned N, typename Bucket>
inline T& HashMap<Key, T, N, Bucket>::operator[](Key&& key)
{
    return this->Insert(MoveArg(key)).first->value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename Value, unsigned N, typename Bucket>
inline void HashMap<Key, Value, N, Bucket>::ShrinkToFit()
{
    if (this->Empty() && !this->IsSmall())
    {
        Dealloc(this->mBuckets);

        this->mBuckets = this->GetFirst();
        this->mNumBuckets = 0;
        this->mNumEntries = 0;
        this->mHashShift = 32;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, typename Value, unsigned N, typename Bucket>
inline bool HashMap<Key, Value, N, Bucket>::IsSmall() const
{
    return this->GetFirst() == this->mBuckets;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, unsigned N, typename Bucket>
inline HashSet<Key, N, Bucket>::HashSet(): HashMapImpl<Bucket>(
    HashMapStorage<Bucket, N>::GetFirst(), HashMapStorage<Bucket, N>::GetCapacity()) {}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, unsigned N, typename Bucket>
inline HashSet<Key, N, Bucket>::HashSet(HashSet&& m): HashMapImpl<Bucket>(MoveArg(m),
    HashMapStorage<Bucket, N>::GetFirst(), HashMapStorage<Bucket, N>::GetCapacity())
{
    m.mBuckets = m.GetFirst();
    m.mNumBuckets = 0;
    m.mNumEntries = 0;
    m.mHashShift = 32;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, unsigned N, typename Bucket>
inline void HashSet<Key, N, Bucket>::ShrinkToFit()
{
    if (this->Empty() && !this->IsSmall())
    {
        Dealloc(this->mBuckets);

        this->mBuckets = this->GetFirst();
        this->mNumBuckets = 0;
        this->mNumEntries = 0;
        this->mHashShift = 32;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Key, unsigned N, typename Bucket>
inline bool HashSet<Key, N, Bucket>::IsSmall() const
{
    return this->GetFirst() == this->mBuckets;
}

/// Range-based loop helpers
template<typename Bucket>
typename HashMapImpl<Bucket>::Iterator begin(HashMapImpl<Bucket>& map)
{
    return map.Begin();
}

template<typename Bucket>
typename HashMapImpl<Bucket>::ConstIterator begin(const HashMapImpl<Bucket>& map)
{
    return map.Begin();
}

template<typename Bucket>
typename HashMapImpl<Bucket>::Iterator end(HashMapImpl<Bucket>& map)
{
    return map.End();
}

template<typename Bucket>
typename HashMapImpl<Bucket>::ConstIterator end(const HashMapImpl<Bucket>& map)
{
    return map.End();
}

}
