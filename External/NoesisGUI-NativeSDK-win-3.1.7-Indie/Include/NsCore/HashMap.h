////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_HASHMAP_H__
#define __CORE_HASHMAP_H__


#include <NsCore/Noesis.h>
#include <NsCore/Pair.h>
#include <NsCore/Math.h>
#include <NsCore/Hash.h>
#include <NsCore/StringUtils.h>
#include <NsCore/CompilerTools.h>

#include <new>
#include <stdlib.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Open addressing hash map implementation
/// Linear probing, resizes at ~50% load factor
////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Bucket>
class HashMapImpl
{
public:
    class Iterator;
    class ConstIterator;
    typedef typename Bucket::Key Key;
    typedef Pair<Iterator, bool> InsertResult;

    /// Returns an iterator to the first element of the container
    Iterator Begin();
    ConstIterator Begin() const;

    /// Returns an iterator to the element following the last element of the container
    Iterator End();
    ConstIterator End() const;

    /// Checks if the container has no elements
    bool Empty() const;

    /// The number of elements in the container
    uint32_t Size() const;

    /// Reserves space for at least the specified number of elements
    void Reserve(uint32_t numElements);

    /// Erases all elements from the container
    void Clear();

    /// Finds element with specific key
    template<class Key_> Iterator Find(const Key_& key);
    template<class Key_> ConstIterator Find(const Key_& key) const;

    /// Inserts element into the container if the key isn't already in the map
    template<class Key_, typename... Ts> InsertResult Insert(Key_&& key, Ts&&... args);

    /// Removes the element with the specified key
    template<class Key_> bool Erase(const Key_& key);
    Iterator Erase(Iterator pos);
    void Erase(Bucket* bucket);

protected:
    HashMapImpl(Bucket* buckets, uint32_t numBuckets);
    HashMapImpl(HashMapImpl&& m, Bucket* buckets, uint32_t numBuckets);
    ~HashMapImpl();

    void InitBuckets();
    void DestroyBuckets();
    static void Delete(Bucket* bucket);
    uint32_t Index(uint32_t hash) const;

    static uint32_t NumBucketsForSize(uint32_t numElements);
    void AllocateBuckets(uint32_t numBuckets);
    void Grow(uint32_t numBuckets);

    static void MoveBuckets(Bucket* dst, Bucket* src, uint32_t count);
    void InsertBucketsFrom(Bucket* src, uint32_t count);

    template<typename Key_, typename... ValueArgs>
    Bucket* InsertIntoBucket(Bucket* bucket, uint32_t hash, Key_&& key, ValueArgs&&... values);
    template<typename Key_> bool FindInsertBucket(uint32_t hash, const Key_& key, Bucket*& foundBucket);

    template<typename Key_> const Bucket* FindBucket(uint32_t hash, const Key_& key) const;
    template<typename Key_> Bucket* FindBucket(uint32_t hash, const Key_& key);

protected:
    Bucket* mBuckets;
    uint32_t mNumInlinedBuckets;
    uint32_t mNumBuckets;
    uint32_t mNumEntries;
    uint32_t mHashShift;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned N>
struct HashMapStorage
{
    alignas(T) char _elements[sizeof(T) * Max(4U, NextPow2(2 * N))];

    T* GetFirst() const { return (T*)_elements; }
    uint32_t GetCapacity() const { return sizeof(_elements) / sizeof(T); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct HashMapStorage<T, 0>
{
    T* GetFirst() const { return nullptr; }
    uint32_t GetCapacity() const { return 0; }
};

template<typename Key, typename Value> struct HashBucket;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Associative container that contains key-value pairs with unique keys
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Key, class T, unsigned N = 0, class BucketT = typename HashBucket<Key, T>::Type>
class HashMap: public HashMapImpl<BucketT>, public HashMapStorage<BucketT, N>
{
public:
    typedef T Value;
    typedef BucketT Bucket;

    HashMap();
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;

    /// Note that only move constructor is implemented. It is cheap and useful for maps of maps
    HashMap(HashMap&& m);

    /// Access or insert specified element 
    template<class Key_> T& operator[](const Key_& key);
    T& operator[](Key&& key);

    /// Requests the removal of unused memory
    void ShrinkToFit();

    /// Returns true if inlined buckets are being used
    bool IsSmall() const;
};

template<typename Key> struct SetBucket;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Associative container that contains a set of unique objects of type Key
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Key, unsigned N = 0, class BucketT = typename SetBucket<Key>::Type>
class HashSet: public HashMapImpl<BucketT>, public HashMapStorage<BucketT, N>
{
public:
    typedef BucketT Bucket;

    HashSet();
    HashSet(const HashSet&) = delete;
    HashSet& operator=(const HashSet&) = delete;

    /// Note that only move constructor is implemented. It is cheap and useful for sets of sets
    HashSet(HashSet&& m);

    /// Requests the removal of unused memory
    void ShrinkToFit();

    /// Returns true if inlined buckets are being used
    bool IsSmall() const;
};

}

#include <NsCore/HashMap.inl>

#endif
