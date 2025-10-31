////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString::BaseString(uint32_t capacity): mSize(0), mCapacity(capacity), mIsSmall(1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString::~BaseString()
{
    if (!mIsSmall)
    {
        Dealloc(Begin());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char* BaseString::Begin()
{
    struct Layout: public BaseString
    {
        union
        {
            char* mBegin;
            char mSmallString[1];
        };
    };

    return mIsSmall ? (char*)&((Layout*)this)->mSmallString : ((Layout*)this)->mBegin;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* BaseString::Begin() const
{
    return ((BaseString*)this)->Begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char* BaseString::End()
{
    return Begin() + mSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* BaseString::End() const
{
    return Begin() + mSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t BaseString::Size() const
{
    return mSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Resize(uint32_t n, char c)
{
    if (n > mSize)
    {
        if (NS_UNLIKELY(n > Capacity()))
        {
            Grow(n);
        }

        memset(Begin() + mSize, c, n - mSize);
    }

    Begin()[n] = 0;
    ForceSize(n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::ForceSize(uint32_t n)
{
    NS_ASSERT(n <= Capacity());
    NS_ASSERT(Begin()[n] == 0);
    mSize = n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline uint32_t BaseString::Capacity() const
{
    return mCapacity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Reserve(uint32_t n)
{
    if (n > Capacity())
    {
        Grow(n);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Clear()
{
    mSize = 0;
    Begin()[0] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool BaseString::Empty() const
{
    return mSize == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char& BaseString::operator[](uint32_t i)
{
    NS_ASSERT(i <= mSize);
    return Begin()[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char& BaseString::operator[](uint32_t i) const
{
    NS_ASSERT(i <= mSize);
    return Begin()[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char& BaseString::Back()
{
    NS_ASSERT(!Empty());
    return Begin()[mSize - 1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char& BaseString::Back() const
{
    NS_ASSERT(!Empty());
    return Begin()[mSize - 1];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline char& BaseString::Front()
{
    NS_ASSERT(!Empty());
    return Begin()[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char& BaseString::Front() const
{
    NS_ASSERT(!Empty());
    return Begin()[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString& BaseString::operator+=(const BaseString& str)
{
    Append(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString& BaseString::operator+=(const char* str)
{
    Append(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString& BaseString::operator+=(char c)
{
    Append(1, c);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Append(const char* str)
{
    NS_ASSERT(str != nullptr);
    Append(str, (uint32_t)strlen(str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Append(const char* str, uint32_t n)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(str + n <= Begin() || str >= Begin() + mCapacity);

    if (NS_UNLIKELY(mSize + n > Capacity()))
    {
        Grow(mSize + n);
    }

    memcpy(Begin() + mSize, str, n);
    Begin()[mSize + n] = 0;
    ForceSize(mSize + n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Append(const BaseString& str)
{
    Append(str.Begin(), str.mSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Append(uint32_t n, char c)
{
    if (NS_UNLIKELY(mSize + n > Capacity()))
    {
        Grow(mSize + n);
    }

    memset(Begin() + mSize, c, n);
    Begin()[mSize + n] = 0;
    ForceSize(mSize + n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Append(const char* first, const char* last)
{
    NS_ASSERT(first <= last);
    return Append(first, (uint32_t)(last - first));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::AppendFormat(NS_FORMAT_PRINTF const char* format, ...)
{
    va_list args;
    va_start(args, format);
    AppendFormatVA(format, &args);
    va_end(args);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::AppendFormatVA(const char* format, va_list* args)
{
    NS_ASSERT(format != nullptr);

    // Copy args in case we need to invoke vsnprintf twice
    va_list args_;
    va_copy(args_, *args);

    int n = vsnprintf(Begin() + mSize, Capacity() + 1 - mSize, format, *args);
    NS_ASSERT(n >= 0);

    if (NS_UNLIKELY((int)mSize + n > (int)Capacity()))
    {
        Grow(mSize + n);
        int n_ = vsnprintf(Begin() + mSize, Capacity() + 1 - mSize, format, args_);
        NS_ASSERT(n_ >= 0);
        NS_ASSERT((int)mSize + n_ <= (int)Capacity());
    }

    ForceSize(mSize + n);
    va_end(args_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::PushBack(char c)
{
    Append(1, c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::PopBack()
{
    NS_ASSERT(!Empty());
    Begin()[mSize - 1] = 0;
    ForceSize(mSize - 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString& BaseString::operator=(const BaseString& str)
{
    Assign(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString& BaseString::operator=(const char* str)
{
    Assign(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline BaseString& BaseString::operator=(char c)
{
    Assign(1, c);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Assign(const char* str)
{
    Clear();
    Append(str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Assign(const char* str, uint32_t n)
{
    Clear();
    Append(str, n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Assign(const BaseString& str)
{
    if (this != &str)
    {
        Clear();
        Append(str);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Assign(BaseString&& str)
{
    Assign(MoveArg(str), 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline void BaseString::Assign(FixedString<N>&& str)
{
    Assign(MoveArg(str), N);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Assign(uint32_t n, char c)
{
    Clear();
    Append(n, c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Assign(const char* first, const char* last)
{
    Clear();
    Append(first, last);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Format(NS_FORMAT_PRINTF const char* format, ...)
{
    Clear();

    va_list args;
    va_start(args, format);
    AppendFormatVA(format, &args);
    va_end(args);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Insert(uint32_t pos, const char* str)
{
    NS_ASSERT(str != nullptr);
    Insert(pos, str, (uint32_t)strlen(str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Insert(uint32_t pos, const char* str, uint32_t n)
{
    NS_ASSERT(str != nullptr);
    NS_ASSERT(pos <= mSize);

    if (n > 0)
    {
        if (NS_UNLIKELY(mSize + n > Capacity()))
        {
            Grow(mSize + n);
        }

        char* dest = Begin() + pos;
        memmove(dest + n, dest, mSize - pos + 1);
        memcpy(dest, str, n);

        ForceSize(mSize + n);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Insert(uint32_t pos, uint32_t n, char c)
{
    NS_ASSERT(pos <= mSize);

    if (n > 0)
    {
        if (NS_UNLIKELY(mSize + n > Capacity()))
        {
            Grow(mSize + n);
        }

        char* dest = Begin() + pos;
        memmove(dest + n, dest, mSize - pos + 1);
        memset(dest, c, n);

        ForceSize(mSize + n);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Erase(uint32_t pos, uint32_t n)
{
    NS_ASSERT(pos <= mSize);

    if (n > 0)
    {
        if (mSize - pos <= n)
        {
            Begin()[pos] = 0;
            ForceSize(pos);
        }
        else
        {
            uint32_t end = pos + n;
            memmove(Begin() + pos, Begin() + end, mSize - end + 1);
            ForceSize(mSize - n);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Replace(uint32_t pos, uint32_t n, const char* str)
{
    NS_ASSERT(pos <= mSize);
    NS_ASSERT(str != nullptr);

    uint32_t removes = Min(n, mSize - pos);
    uint32_t inserts = (uint32_t)strlen(str);
    uint32_t newSize = mSize + inserts - removes;

    if (NS_UNLIKELY(newSize > Capacity()))
    {
        Grow(newSize);
    }

    char* dest = Begin() + pos;

    if (removes != inserts)
    {
        memmove(dest + inserts, dest + removes, mSize - pos - removes + 1);
    }

    memcpy(dest, str, inserts);
    ForceSize(newSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* BaseString::Str() const
{
    return Begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool BaseString::StartsWith(const char* str) const
{
    NS_ASSERT(str != nullptr);
    return strncmp(Begin(), str, strlen(str)) == 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int BaseString::Find(const char* str, uint32_t pos, uint32_t n) const
{
    NS_ASSERT(str != nullptr);

    for (uint32_t i = pos; i + n <= mSize; i++)
    {
        if (strncmp(Begin() + i, str, n) == 0)
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int BaseString::Find(const char* str, uint32_t pos) const
{
    return Find(str, pos, (uint32_t)strlen(str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int BaseString::FindLast(const char* str, uint32_t pos, uint32_t n) const
{
    NS_ASSERT(str != nullptr);

    for (int i = Min(pos, mSize) - n; i >= 0; i--)
    {
        if (strncmp(Begin() + i, str, n) == 0)
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int BaseString::FindLast(const char* str, uint32_t pos) const
{
    return FindLast(str, pos, (uint32_t)strlen(str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Trim()
{
    RTrim();
    LTrim();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::RTrim()
{
    int pos = (int)mSize - 1;
    while (pos >= 0 && Begin()[pos] == ' ')
    {
        pos--;
    }

    Begin()[pos + 1] = 0;
    ForceSize(pos + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::LTrim()
{
    int pos = 0;
    while (pos < (int)mSize && Begin()[pos] == ' ')
    {
        pos++;
    }

    Replace(0, pos, "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::ResetToSmall(uint32_t capacity)
{
    mSize = 0;
    mCapacity = capacity;
    mIsSmall = 1;
    Begin()[0] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Assign(BaseString&& str, uint32_t capacity)
{
    if (this != &str)
    {
        // If str is not using inlined memmory steals its buffer
        if (!str.mIsSmall)
        {
            if (!mIsSmall)
            {
                Dealloc(Begin());
            }

            SetStorage(str.Begin());
            mSize = str.mSize;
            mCapacity = str.mCapacity;

            str.ResetToSmall(capacity);
        }
        else
        {
            Clear();
            Append(str);
            str.Clear();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::SetStorage(char* ptr)
{
    struct Layout: public BaseString
    {
        char* mBegin;
    };

    ((Layout*)this)->mBegin = ptr;
    mIsSmall = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline void BaseString::Grow(uint32_t minCapacity)
{
    NS_ASSERT(minCapacity > mCapacity);
    uint32_t capacity = Max(2 * (uint32_t)mCapacity, minCapacity);

    // Extra byte for the null terminator
    uint32_t bytesNeeded = capacity + 1;

    if (mIsSmall)
    {
        char* str = (char*)Alloc(bytesNeeded);
        memcpy(str, Begin(), mSize + 1);

        SetStorage(str);
    }
    else
    {
        // It is debatable is realloc() is here a good idea. In case a new pointer is given all
        // the memory is internally copied instead of copying only the size of the string
        SetStorage((char*)Realloc(Begin(), bytesNeeded));
    }

    mCapacity = capacity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(): BaseString(N - 1)
{
    mSmallString[0] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(const FixedString& str): BaseString(N - 1)
{
    Append(str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(const FixedString& str, uint32_t pos, uint32_t n): BaseString(N - 1)
{
    Append(str.Begin() + pos, Min(n, str.mSize - pos));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(FixedString&& str): BaseString(N - 1)
{
    Assign(MoveArg(str));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(const char* str): BaseString(N - 1)
{
    Append(str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(const char* str, uint32_t n): BaseString(N - 1)
{
    Append(str, n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(VarArgs, NS_FORMAT_PRINTF const char* format, ...): BaseString(N - 1)
{
    va_list args;
    va_start(args, format);
    AppendFormatVA(format, &args);
    va_end(args);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(uint32_t n, char c): BaseString(N - 1)
{
    Append(n, c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>::FixedString(const char* first, const char* last): BaseString(N - 1)
{
    Append(first, last);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>& FixedString<N>::operator=(const FixedString& str)
{
    Assign(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>& FixedString<N>::operator=(FixedString&& str)
{
    Assign(MoveArg(str));
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>& FixedString<N>::operator=(const char* str)
{
    Assign(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>& FixedString<N>::operator=(char c)
{
    Clear();
    Append(1, c);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>& FixedString<N>::operator+=(const FixedString& str)
{
    Append(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>& FixedString<N>::operator+=(const char* str)
{
    Append(str);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N>& FixedString<N>::operator+=(char c)
{
    Append(1, c);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N> FixedString<N>::Substr(uint32_t pos, uint32_t n) const
{
    return FixedString<N>(*this, pos, n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator==(const BaseString& a, const BaseString& b)
{
    return (a.Size() == b.Size()) && (memcmp(a.Str(), b.Str(), a.Size()) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator==(const char* str, const BaseString& b)
{
    uint32_t n = (uint32_t)strlen(str);
    return (n == b.Size()) && (memcmp(str, b.Str(), n) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator==(const BaseString& a, const char* str)
{
    uint32_t n = (uint32_t)strlen(str);
    return (a.Size() == n) && (memcmp(a.Str(), str, n) == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator!=(const BaseString& a, const BaseString& b)
{
    return !(a == b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator!=(const char* str, const BaseString& b)
{
    return !(str == b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator!=(const BaseString& a, const char* str)
{
    return !(a == str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator<(const BaseString& a, const BaseString& b)
{
    uint32_t minN = Min(a.Size(), b.Size());
    int cmp = memcmp(a.Str(), b.Str(), minN);
    return cmp < 0 || (cmp == 0 && a.Size() < b.Size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator<(const char* str, const BaseString& b)
{
    uint32_t n = (uint32_t)strlen(str);
    uint32_t minN = Min(n, b.Size());
    int cmp = memcmp(str, b.Str(), minN);
    return cmp < 0 || (cmp == 0 && n < b.Size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator<(const BaseString& a, const char* str)
{
    uint32_t n = (uint32_t)strlen(str);
    uint32_t minN = Min(a.Size(), n);
    int cmp = memcmp(a.Str(),str, minN);
    return cmp < 0 || (cmp == 0 && a.Size() < n);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator>(const BaseString& a, const BaseString& b)
{
    return b < a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator>(const char* str, const BaseString& b)
{
    return b < str;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator>(const BaseString& a, const char* str)
{
    return str < a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator<=(const BaseString& a, const BaseString& b)
{
    return !(b < a);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator<=(const char* str, const BaseString& b)
{
    return !(b < str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator<=(const BaseString& a, const char* str)
{
    return !(str < a);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator>=(const BaseString& a, const BaseString& b)
{
    return !(a < b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator>=(const char* str, const BaseString& b)
{
    return !(str < b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator>=(const BaseString& a, const char* str)
{
    return !(a < str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N> operator+(const FixedString<N>& a, const FixedString<N>& b)
{
    FixedString<N> r;
    r.Reserve(a.Size() + b.Size());
    r.Append(a);
    r.Append(b);
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N> operator+(const char* str, const FixedString<N>& b)
{
    FixedString<N> r;
    r.Reserve((uint32_t)strlen(str) + b.Size());
    r.Append(str);
    r.Append(b);
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<unsigned N>
inline FixedString<N> operator+(const FixedString<N>& a, const char* str)
{
    FixedString<N> r;
    r.Reserve(a.Size() + (uint32_t)strlen(str));
    r.Append(a);
    r.Append(str);
    return r;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Hash function
////////////////////////////////////////////////////////////////////////////////////////////////////
struct StringHashKeyInfo
{
    static bool IsEmpty(const String& key) { return key.Size() == 0xFFFFFFFF; }
    static void MarkEmpty(String& key) { *(uint32_t*)&key = 0xFFFFFFFF; }
    static uint32_t HashValue(const String& key) { return StrHash(key.Str()); }
    static uint32_t HashValue(const char* key) { return StrHash(key); }
    static bool IsEqual(const String& l, const String& r) { return !IsEmpty(l) && l == r; }
    static bool IsEqual(const String& l, const char* r) { return !IsEmpty(l) && l == r; }
};

struct CaseStringHashKeyInfo
{
    static bool IsEmpty(const String& key) { return key.Size() == 0xFFFFFFFF; }
    static void MarkEmpty(String& key) { *(uint32_t*)&key = 0xFFFFFFFF; }
    static uint32_t HashValue(const String& key) { return StrCaseHash(key.Str()); }
    static uint32_t HashValue(const char* key) { return StrCaseHash(key); }

    static bool IsEqual(const String& l, const String& r)
    {
        return !IsEmpty(l) && l.Size() == r.Size() && StrCaseEquals(l.Str(), r.Str());
    }

    static bool IsEqual(const String& l, const char* r)
    {
        return !IsEmpty(l) && StrCaseEquals(l.Str(), r);
    }
};

template<typename Key, typename Value, typename KeyInfo> class HashBucket_KHV;
template<typename Key, typename KeyInfo> class HashBucket_KH;
template<typename Key, typename Value> struct HashBucket;
template<typename Key> struct SetBucket;

template<typename Value>
struct HashBucket<String, Value>
{
    typedef HashBucket_KHV<String, Value, StringHashKeyInfo> Type;
};

template<>
struct SetBucket<String>
{
    typedef HashBucket_KH<String, StringHashKeyInfo> Type;
};



}
