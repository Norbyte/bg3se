#pragma once

#include <CoreLib/Base/PagedArray.h>

BEGIN_SE()

template <class TKey, class TAllocator = DefaultPagedAllocator>
struct PagedHashSet : protected TAllocator
{
    int32_t** hash_;
    int32_t** nextIds_;
    TKey** keys_;
    PageLayout hashLayout_;
    PageLayout keyLayout_;
    uint32_t hashSize_;
    uint32_t keysSize_;

    int find_index(TKey const& key) const
    {
        if (hashSize_ == 0) return -1;

        auto hash = (uint32_t)(HashMapHash(key) % hashSize_);
        auto keyIndex = *hashLayout_.at(hash_, hash);
        while (keyIndex >= 0) {
            auto keyBucket = keyIndex >> keyLayout_.BitsPerPage;
            auto keySlot = keyIndex & (keyLayout_.bucket_size() - 1);
            if (keys_[keyBucket][keySlot] == key) return keyIndex;
            keyIndex = nextIds_[keyBucket][keySlot];
        }

        return -1;
    }

    int insert(TKey const& key)
    {
        auto index = find_index(key);
        if (index != -1) {
            return index;
        }

        auto keyIdx = keysSize_++;

        if (keyIdx >= keyLayout_.capacity()) {
            auto layout = keyLayout_;
            PagedOps<int32_t, TAllocator>::Resize(keyIdx + 1, nextIds_, layout, *this);
            PagedOps<TKey, TAllocator>::Resize(keyIdx + 1, keys_, keyLayout_, *this);
        }
        
        *keyLayout_.at(keys_, keyIdx) = key;
        *keyLayout_.at(nextIds_, keyIdx) = -1;

        if (hashSize_ >= keysSize_ + (keysSize_ >> 1)) {
            hash_insert(key, keyIdx);
        } else {
            rehash(keysSize_ + (keysSize_ >> 1));
        }

        return (int)keyIdx;
    }

private:
    void hash_insert(TKey const& key, int keyIdx)
    {
        auto hash = (uint32_t)(HashMapHash(key) % hashSize_);
        auto prevKeyIdx = *hashLayout_.at(hash_, hash);
        if (prevKeyIdx < 0) {
            prevKeyIdx = -2 - (int)hash;
        }

        *keyLayout_.at(nextIds_, keyIdx) = prevKeyIdx;
        *hashLayout_.at(hash_, hash) = keyIdx;
    }

    void rehash(uint32_t size)
    {
        auto hashSize = GetNearestMultiHashMapPrime(size);
        PagedOps<int32_t, TAllocator>::Resize(hashSize, hash_, hashLayout_, *this);
        hashSize_ = hashSize;
        for (uint32_t i = 0; i < hashSize_; i++) {
            *hashLayout_.at(hash_, i) = -1;
        }

        for (uint32_t i = 0; i < keysSize_; i++) {
            hash_insert(*keyLayout_.at(keys_, i), i);
        }
    }
};


template <class TKey, class TValue, class TAllocator = DefaultPagedAllocator>
struct PagedHashMap : public PagedHashSet<TKey, TAllocator>
{
    PagedArray<TValue, TAllocator> Values;

    inline TAllocator const& allocator() const
    {
        return static_cast<TAllocator const&>(*this);
    }

    inline TKey const& key_at(uint32_t index) const
    {
        return *this->keyLayout_.at(this->keys_, index);
    }

    inline uint32_t size() const
    {
        return Values.size();
    }

    TValue* find(TKey const& key)
    {
        auto index = this->find_index(key);
        if (index < 0) return nullptr;

        return &Values[index];
    }

    TValue const* find(TKey const& key) const
    {
        auto index = this->find_index(key);
        if (index < 0) return nullptr;

        return &Values[index];
    }

    TValue* add_uninitialized(TKey const& key)
    {
        auto index = this->insert(key);
        return Values.add_uninitialized();
    }
};

END_SE()
