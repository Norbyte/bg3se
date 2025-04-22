#pragma once

#include <CoreLib/Base/BaseArray.h>

BEGIN_SE()

template <class T, class TId = uint32_t, class TSalt = uint16_t, unsigned IdBits = 22, unsigned SaltBits = 10>
class SaltedPool
{
public:
    static_assert(sizeof(TId) * 8 >= IdBits + SaltBits);
    static_assert(sizeof(TSalt) * 8 > SaltBits); // Need 1 bit for deleted flag
    static constexpr TSalt DeletedFlag = TSalt(1) << (sizeof(TSalt) * 8 - 1);

    T* Add(TId& id)
    {
        TId index;
        if (freeIndices_.empty()) {
            index = pool_.size();
            pool_.push_back(T{});
            salts_.push_back(DeletedFlag);
        } else {
            index = freeIndices_.pop_last();
        }

        auto salt = salts_[(uint32_t)index];
        salt &= ~DeletedFlag;
        salts_[(uint32_t)index] = salt;
        id = index | (TId(salt) << IdBits);
        return &pool_[(uint32_t)index];
    }

    bool Free(TId id)
    {
        auto index = id & ((TId(1) << IdBits) - 1);
        auto salt = TSalt(id >> IdBits) & ((TSalt(1) << SaltBits) - 1);

        if (index < salts_.size() && salts_[(uint32_t)index] == salt) {
            freeIndices_.push_back(index);
            salt = (salt + 1) & TSalt(((TId(1) << SaltBits) - 1));
            salts_[(uint32_t)index] = salt | DeletedFlag;
            return true;
        } else {
            return false;
        }
    }

    T* Find(TId id)
    {
        auto index = id & ((TId(1) << IdBits) - 1);
        auto salt = TSalt(id >> IdBits) & TSalt((TId(1) << SaltBits) - 1);

        if (index < salts_.size() && salts_[(uint32_t)index] == salt) {
            return &pool_[(uint32_t)index];
        } else {
            return nullptr;
        }
    }

    T* Next(uint32_t& index)
    {
        for (auto i = index; i < pool_.size(); i++) {
            if ((salts_[i] & DeletedFlag) == 0) {
                index = i + 1;
                return &pool_[i];
            }
        }

        return nullptr;
    }

    T* PopNext(uint32_t& index)
    {
        for (auto i = index; i < pool_.size(); i++) {
            if ((salts_[i] & DeletedFlag) == 0) {
                MarkFree(i);
                index = i + 1;
                return &pool_[i];
            }
        }

        return nullptr;
    }

private:
    Array<T> pool_;
    Array<TId> freeIndices_;
    Array<TId> salts_;

    void MarkFree(uint32_t index)
    {
        freeIndices_.push_back(index);
        auto salt = salts_[index];
        salt = (salt + 1) & TSalt(((TId(1) << SaltBits) - 1));
        salts_[index] = salt | DeletedFlag;
    }
};

END_SE()
