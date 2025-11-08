#pragma once

#include <cstdint>

BEGIN_SE()

unsigned int GetNearestMultiHashMapPrime(unsigned int num);
unsigned int GetNearestSmallMultiHashMapPrime(unsigned int num);

template <class T>
inline uint64_t SparseHashMapHash(T const& v);

// Special hashing needed for FixedStrings in the new hash table

template <class T>
inline uint64_t HashMapHash(T const& v)
{
    return Hash(v);
}

template <>
inline uint64_t HashMapHash<FixedString>(FixedString const& v)
{
    return v.GetHash();
}

template <>
inline uint64_t HashMapHash<FixedStringNoRef>(FixedStringNoRef const& v)
{
    return v.Index;
}

template <>
inline uint64_t HashMapHash<FixedStringUnhashed>(FixedStringUnhashed const& v)
{
    return v.Index;
}

template <>
inline uint64_t HashMapHash<UserId>(UserId const& v)
{
    return v.Id;
}

template <class T>
uint32_t GetHashSizeFor(std::span<T const> const& vals, uint32_t maxCollisions)
{
    uint32_t hsize = (uint32_t)vals.size() + 1;
    uint32_t collisions{ 0 };
    uint32_t numBuckets{ 0 };
    for (;;) {
        numBuckets = GetNearestSmallMultiHashMapPrime(hsize + 1);

        collisions = 0;

        BitSet<> buckets;
        buckets.EnsureSize(numBuckets);
        for (auto const& k : vals) {
            auto hash = (uint32_t)HashMapHash(k) % numBuckets;
            if (buckets[hash]) {
                collisions++;
            }

            buckets.Set(hash);
        }

        if (collisions <= maxCollisions) {
            return numBuckets;
        }

        hsize = numBuckets + 1;
    }
}


template <class T>
class HashSet
{
public:
    using value_type = T;
    using reference = T&;
    using const_reference = T const&;
    using iterator = ContiguousIterator<T>;
    using const_iterator = ContiguousConstIterator<T>;
    using difference_type = int32_t;
    using size_type = uint32_t;

    HashSet() noexcept
    {}

    HashSet(HashSet const& other)
        : HashKeys(other.HashKeys), NextIds(other.NextIds), Keys(other.Keys)
    {}

    HashSet(HashSet&& other) noexcept
        : HashKeys(std::move(other.HashKeys)), NextIds(std::move(other.NextIds)), Keys(std::move(other.Keys))
    {}

    ~HashSet()
    {}

    HashSet& operator =(HashSet const& other)
    {
        HashKeys = other.HashKeys;
        NextIds = other.NextIds;
        Keys = other.Keys;
        return *this;
    }

    HashSet& operator =(HashSet&& other) noexcept
    {
        if (this != &other) {
            HashKeys = std::move(other.HashKeys);
            NextIds = std::move(other.NextIds);
            Keys = std::move(other.Keys);
        }

        return *this;
    }

    inline uint32_t size() const
    {
        return Keys.size();
    }

    inline bool empty() const
    {
        return Keys.empty();
    }

    Array<T> const& keys() const
    {
        return Keys;
    }

    StaticArray<int32_t> const& hash_keys() const
    {
        return HashKeys;
    }

    Array<int32_t> const& next_ids() const
    {
        return NextIds;
    }

    void clear()
    {
        std::fill(HashKeys.begin(), HashKeys.end(), -1);
        NextIds.clear();
        Keys.clear();
    }

    int find_index(T const& key) const
    {
        if (HashKeys.size() == 0) return -1;

        auto keyIndex = HashKeys[HashMapHash(key) % HashKeys.size()];
        while (keyIndex >= 0) {
            if (Keys[keyIndex] == key) return keyIndex;
            keyIndex = NextIds[keyIndex];
        }

        return -1;
    }

    int insert(T const& key)
    {
        auto index = find_index(key);
        if (index != -1) {
            return index;
        }

        int keyIdx = (int)Keys.size();
        Keys.push_back(key);
        NextIds.push_back(-1);

        auto desiredSize = Keys.size() + (Keys.size() >> 1);
        if (HashKeys.size() >= desiredSize) {
            InsertToHashMap(key, keyIdx);
        } else {
            ResizeHashMap(desiredSize);
        }

        return keyIdx;
    }

    ContiguousIterator<T> begin()
    {
        return Keys.begin();
    }

    ContiguousConstIterator<T> begin() const
    {
        return Keys.begin();
    }

    ContiguousIterator<T> end()
    {
        return Keys.end();
    }

    ContiguousConstIterator<T> end() const
    {
        return Keys.end();
    }

    ContiguousIterator<T> find(T const& key) const
    {
        auto idx = find_index(key);
        return idx != -1 ? (Keys.begin() + idx) : Keys.end();
    }

    ContiguousIterator<T> find(T const& key)
    {
        auto idx = find_index(key);
        return idx != -1 ? (Keys.begin() + idx) : Keys.end();
    }

    bool remove(T const& key)
    {
        auto idx = find_index(key);
        if (idx != -1) {
            removeAt(idx);
            return true;
        } else {
            return false;
        }
    }

    bool contains(T const& key) const
    {
        return find_index(key) != -1;
    }

protected:
    StaticArray<int32_t> HashKeys;
    Array<int32_t> NextIds;
    Array<T> Keys;

    uint32_t bucketFromIndex(int32_t index)
    {
        while (index >= 0) {
            index = NextIds[index];
        }

        se_assert(index < -1);
        return (uint32_t)(-2 - index);
    }

    void swapKeyInHashTable(uint32_t bucket, int32_t indexToRemove, int32_t nextIndex)
    {
        auto index = HashKeys[bucket];
        if (indexToRemove == index) {
            HashKeys[bucket] = nextIndex;
        } else {
            int32_t lastIndex = -1;
            do {
                se_assert(index >= 0);
                lastIndex = index;
                index = NextIds[index];
            } while (index != indexToRemove);

            NextIds[lastIndex] = nextIndex;
        }
    }

    void removeAt(int32_t index)
    {
        auto bucketIndex = bucketFromIndex(index);
        auto nextIndex = NextIds[index];
        swapKeyInHashTable(bucketIndex, index, nextIndex);
        Keys.remove_at(index);
        auto followUpIndex = NextIds.pop_last();
        auto removedIndex = NextIds.size();

        if (index != removedIndex) {
            NextIds[index] = followUpIndex;
            auto removedBucketIndex = bucketFromIndex(followUpIndex);
            swapKeyInHashTable(removedBucketIndex, removedIndex, index);
        }
    }

private:
    void InsertToHashMap(T const& key, int keyIdx)
    {
        auto bucket = (uint32_t)(HashMapHash(key) % HashKeys.size());
        auto prevKeyIdx = HashKeys[bucket];
        if (prevKeyIdx < 0) {
            prevKeyIdx = -2 - (int)bucket;
        }

        NextIds[keyIdx] = prevKeyIdx;
        HashKeys[bucket] = keyIdx;
    }

    void ResizeHashMap(unsigned int newSize)
    {
        auto numBuckets = GetNearestMultiHashMapPrime(newSize);

        HashKeys = StaticArray<int32_t>(numBuckets, -1);
        for (unsigned k = 0; k < Keys.size(); k++) {
            InsertToHashMap(Keys[k], k);
        }
    }
};

template <class T>
struct VirtualMultiHashSet : public HashSet<T>
{
    virtual inline void Dummy() {}
};

template <class TKey, class TValue>
class HashMap : private HashSet<TKey>
{
public:
    class ConstIterator
    {
    public:
        ConstIterator(HashMap const* map)
            : Map(map), Index(0)
        {}
            
        ConstIterator(HashMap const* map, int index)
            : Map(map), Index(index)
        {}

        ConstIterator operator ++ ()
        {
            ConstIterator it(Map, Index);
            Index++;
            return it;
        }

        ConstIterator& operator ++ (int)
        {
            ++Index;
            return *this;
        }

        bool operator == (ConstIterator const& it)
        {
            return it.Map == Map && it.Index == Index;
        }

        bool operator != (ConstIterator const& it)
        {
            return it.Map != Map || it.Index != Index;
        }

        TKey const& Key() const
        {
            return Map->Keys[Index];
        }

        TValue const& Value() const
        {
            return Map->Values[Index];
        }

        ConstIterator& operator * ()
        {
            return *this;
        }

        ConstIterator* operator -> ()
        {
            return *this;
        }

        inline operator bool() const
        {
            return Index != Map->Keys.size();
        }

        inline bool operator !() const
        {
            return Index == Map->Keys.size();
        }

    private:
        HashMap const* Map;
        int32_t Index;
    };

    class Iterator
    {
    public:
        Iterator(HashMap * map)
            : Map(map), Index(0)
        {}
            
        Iterator(HashMap * map, int index)
            : Map(map), Index(index)
        {}

        Iterator operator ++ ()
        {
            Iterator it(Map, Index);
            Index++;
            return it;
        }

        Iterator& operator ++ (int)
        {
            ++Index;
            return *this;
        }

        bool operator == (Iterator const& it)
        {
            return it.Map == Map && it.Index == Index;
        }

        bool operator != (Iterator const& it)
        {
            return it.Map != Map || it.Index != Index;
        }

        TKey const & Key() const
        {
            return Map->Keys[Index];
        }

        TValue & Value() const
        {
            return Map->Values[Index];
        }

        Iterator& operator * ()
        {
            return *this;
        }

        Iterator* operator -> ()
        {
            return this;
        }

        inline operator bool() const
        {
            return Index != Map->Keys.size();
        }

        inline bool operator !() const
        {
            return Index == Map->Keys.size();
        }

    protected:
        friend HashMap;
        HashMap* Map;
        int32_t Index;
    };

    HashMap() noexcept
    {}

    HashMap(HashMap const& other)
        : HashSet<TKey>(other)
    {
        Values.copy_from(other.Values, 0, other.Keys.size());
    }

    HashMap(HashMap&& other) noexcept
        : HashSet<TKey>(std::move(other)),
        Values(std::move(other.Values))
    {}

    ~HashMap()
    {
        Values.clear(this->Keys.size());
    }

    HashMap& operator =(HashMap const& other)
    {
        auto oldSize = this->Keys.size();
        HashSet<TKey>::operator =(other);
        Values.copy_from(other.Values, oldSize, other.Keys.size());

        return *this;
    }

    HashMap& operator =(HashMap&& other) noexcept
    {
        HashSet<TKey>::operator =(std::move(other));
        Values = std::move(other.Values);

        return *this;
    }

    inline uint32_t size() const
    {
        return this->Keys.size();
    }

    inline bool empty() const
    {
        return this->Keys.empty();
    }

    Array<TKey> const& keys() const
    {
        return this->Keys;
    }

    std::span<TValue> values()
    {
        return std::span(Values.raw_buf(), Values.raw_buf() + this->Keys.size());
    }

    std::span<TValue const> values() const
    {
        return std::span(Values.raw_buf(), Values.raw_buf() + this->Keys.size());
    }

    UninitializedStaticArray<TValue>& raw_values()
    {
        return Values;
    }

    UninitializedStaticArray<TValue> const& raw_values() const
    {
        return Values;
    }

    void reserve(uint32_t size)
    {
        assert(size < this->Keys.size());
        if (size > this->Keys.capacity()) {
            this->Keys.reserve(size);
            Values.resize(size, this->Keys.size(), this->Keys.size());
        }
    }

    void clear()
    {
        Values.clear(this->Keys.size());
        HashSet<TKey>::clear();
    }

    TValue* set(TKey const& key, TValue&& value)
    {
        auto index = this->insert(key);
        if (Values.size() < this->Keys.capacity()) {
            Values.resize(this->Keys.capacity(), index, index);
        }

        return new (&Values[index]) TValue(std::move(value));
    }

    TValue* set(TKey const& key, TValue const& value)
    {
        auto index = this->insert(key);
        if (Values.size() < this->Keys.capacity()) {
            Values.resize(this->Keys.capacity(), index, index);
        }

        return new (&Values[index]) TValue(value);
    }

    TValue* add_key(TKey const& key)
    {
        auto index = this->insert(key);
        if (Values.size() < this->Keys.capacity()) {
            Values.resize(this->Keys.capacity(), index, index);
        }

        return new (&Values[index]) TValue();
    }

    bool remove(Iterator const& it)
    {
        if (it) {
            removeAt(it.Index);
            return true;
        } else {
            return false;
        }
    }

    bool remove(TKey const& key)
    {
        auto index = find_index(key);
        if (index >= 0) {
            removeAt(index);
            return true;
        } else {
            return false;
        }
    }

    Iterator begin()
    {
        return Iterator(this, 0);
    }

    ConstIterator begin() const
    {
        return ConstIterator(this, 0);
    }

    Iterator end()
    {
        return Iterator(this, this->Keys.size());
    }

    ConstIterator end() const
    {
        return ConstIterator(this, this->Keys.size());
    }

    Iterator find(TKey const& key)
    {
        auto idx = this->find_index(key);
        return Iterator(this, idx != -1 ? idx : this->Keys.size());
    }

    ConstIterator find(TKey const& key) const
    {
        auto idx = this->find_index(key);
        return ConstIterator(this, idx != -1 ? idx : this->Keys.size());
    }

    TValue const* try_get(TKey const& key) const
    {
        auto index = this->find_index(key);
        if (index == -1) {
            return nullptr;
        } else {
            return &Values[index];
        }
    }

    TValue* try_get(TKey const& key)
    {
        auto index = this->find_index(key);
        if (index == -1) {
            return nullptr;
        } else {
            return &Values[index];
        }
    }

    TValue* get_or_add(TKey const& key)
    {
        auto index = this->find_index(key);
        if (index == -1) {
            return add_key(key);
        } else {
            return &Values[index];
        }
    }

    TValue get_or_default(TKey const& key, TValue const& defaultv = TValue{}) const
    {
        auto index = this->find_index(key);
        if (index == -1) {
            return defaultv;
        } else {
            return Values[index];
        }
    }

    inline int find_index(TKey const& key) const
    {
        return HashSet<TKey>::find_index(key);
    }

protected:
    void removeAt(int32_t index)
    {
        HashSet<TKey>::removeAt(index);
        auto lastIndex = this->Keys.size();
        if (index < (int32_t)lastIndex) {
            Values[index] = std::move(Values[lastIndex]);
            Values[lastIndex].~TValue();
        }
    }

private:
    UninitializedStaticArray<TValue> Values;
};

template <class TKey, class TValue>
struct VirtualHashMap : public HashMap<TKey, TValue>
{
    virtual inline void Dummy() {}
};

template <class TK, class TV>
class StaticHashMap
{
public:
    struct Entry
    {
        TK Key{};
        int32_t Next{ -1 };
        TV Value{};
    };

    inline StaticHashMap()
    {}

    ~StaticHashMap()
    {
        if (Hash) {
            GameDeleteArray(Hash, TotalSize);
        }
    }

    void Build(Array<TK> const& keys, Array<TV> const& values)
    {
        auto hashSize = GetHashSizeFor(std::span<TK const>(keys.raw_buf(), keys.raw_buf() + keys.size()), 2);
        Build(hashSize, keys, values);
    }

    void Build(uint32_t hashSize, Array<TK> const& keys, Array<TV> const& values)
    {
        uint32_t collisions = 0;
        BitSet<> buckets;
        buckets.EnsureSize(hashSize);
        for (auto const& k : keys) {
            auto hash = (uint32_t)(HashMapHash(k) % hashSize);
            if (buckets[hash]) {
                collisions++;
            }

            buckets.Set(hash);
        }

        Hash = GameAllocArray<Entry>(hashSize + collisions);
        HashSize = hashSize;
        TotalSize = hashSize + collisions;

        uint32_t nextCollision = HashSize;
        for (uint32_t i = 0; i < keys.size(); i++) {
            int32_t hash = HashMapHash(keys[i]) % HashSize;
            int32_t prev = -1;
            while (Hash[hash].Key != TK{}) {
                prev = hash;
                if (Hash[hash].Next < 0) {
                    hash = nextCollision++;
                    break;
                } else {
                    hash = Hash[hash].Next;
                }
            }

            if (prev >= 0) {
                Hash[prev].Next = hash;
            }

            auto& bucket = Hash[hash];
            se_assert(bucket.Key == TK{});
            bucket.Key = keys[i];
            bucket.Value = values[i];
        }
    }

    TV const& get(TK const& key) const
    {
        if (HashSize == 0) return Tombstone;

        auto hash = (int32_t)(HashMapHash(key) % HashSize);
        for (;;) {
            auto& k = Hash[hash];
            if (k.Key == key) {
                return k.Value;
            }

            if (k.Next >= 0) {
                hash = k.Next;
            } else {
                return Tombstone;
            }
        }
    }

public:
    Entry* Hash{ nullptr };
    uint32_t HashSize{ 0 };
    uint32_t TotalSize{ 0 };
    TV Tombstone;
};

END_SE()
