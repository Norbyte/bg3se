
BEGIN_NS(lua::json)

enum class TypeTag : uint8_t
{
    Null = 0,
    Bool = 1,
    Int0 = 2,
    Int8 = 3,
    Int16 = 4,
    Int32 = 5,
    Int64 = 6,
    Float = 7,
    String = 8,
    StringRef8 = 9,
    StringRef16 = 10,
    StringRef32 = 11,
    Object = 12,
    Array = 13,
    End = 14
};

class BinaryWriter
{
public:
    struct PooledString
    {
        uint32_t TagOffset;
        uint32_t StringOffset;
        uint32_t Length;
    };

    inline uint64_t Hash(char const* v, uint32_t len)
    {
        uint64_t hash[2];
        MurmurHash3_x64_128(v, len, 0, hash);
        return hash[0];
    }

    void Reserve(uint32_t bytes)
    {
        if (pos_ + bytes > buf_.size()) {
            buf_.resize(pos_ + bytes + (pos_ >> 1));
        }
    }

    void Null()
    {
        Reserve(1);
        Tag(TypeTag::Null);
    }

    void Bool(bool b)
    {
        Reserve(1);
        Tag(TypeTag::Bool, (unsigned)b);
    }

    void Int64(int64_t v)
    {
        if (v >= -4 && v <= 3) {
            Reserve(1);
            auto vl = (uint8_t)(v + 4);
            Tag(TypeTag::Int0, (vl & 0x0f));
        } else if (v >= -0x400 && v <= 0x3ff) {
            Reserve(2);
            auto vl = (uint16_t)(v + 0x400);
            Tag(TypeTag::Int8, (vl >> 8) & 0x0f);
            buf_[pos_++] = (uint8_t)(vl & 0xff);
        } else if (v >= -0x40000 && v <= 0x3ffff) {
            Reserve(3);
            auto vl = (uint32_t)(v + 0x40000);
            Tag(TypeTag::Int16, (vl >> 16) & 0x0f);
            *reinterpret_cast<uint16_t*>(buf_.raw_buf() + pos_) = (uint16_t)(vl & 0xffff);
            pos_ += 2;
        } else if (v >= -0x400000000l && v <= 0x3ffffffffl) {
            Reserve(5);
            auto vl = (uint64_t)(v + 0x400000000l);
            Tag(TypeTag::Int32, (uint32_t)(vl >> 32) & 0x0f);
            *reinterpret_cast<uint32_t*>(buf_.raw_buf() + pos_) = (uint32_t)(vl & 0xffffffff);
            pos_ += 4;
        } else {
            Reserve(9);
            Tag(TypeTag::Int64);
            *reinterpret_cast<int64_t*>(buf_.raw_buf() + pos_) = v;
            pos_ += 8;
        }
    }

    void Double(double v)
    {
        Reserve(5);
        Tag(TypeTag::Float, 4);
        *reinterpret_cast<float*>(buf_.raw_buf() + pos_) = (float)v;
        pos_ += 4;
    }

    void StringRef(uint32_t offset)
    {
        if (offset < 0xfff) {
            Reserve(2);
            Tag(TypeTag::StringRef8, (offset >> 8) & 0x0f);
            buf_[pos_++] = (int8_t)(offset & 0xff);
        } else if (offset < 0xfffff) {
            Reserve(3);
            Tag(TypeTag::StringRef16, (offset >> 16) & 0x0f);
            *reinterpret_cast<uint16_t*>(buf_.raw_buf() + pos_) = (uint16_t)offset;
            pos_ += 2;
        } else {
            Reserve(5);
            Tag(TypeTag::StringRef32);
            *reinterpret_cast<uint32_t*>(buf_.raw_buf() + pos_) = offset;
            pos_ += 4;
        }
    }

    void String(char const* v, uint32_t len)
    {
        auto hash = Hash(v, len);
        auto ref = strings_.try_get(hash);
        if (ref) {
            if (len == ref->Length && memcmp(v, buf_.raw_buf() + ref->StringOffset, len) == 0) {
                auto off = pos_ - ref->TagOffset;
                StringRef(off);
                return;
            }
        }

        Reserve(1 + 4 /* max tag length */ + len);
        auto pos = pos_;
        LongTag(TypeTag::String, len);
        auto stringPos = pos_;
        memcpy(buf_.raw_buf() + pos_, v, len);
        pos_ += len;

        strings_.set(hash, PooledString{
            .TagOffset = pos,
            .StringOffset = stringPos,
            .Length = len
        });
    }

    void String(char const* v)
    {
        String(v, (uint32_t)strlen(v));
    }

    void Key(char const* v)
    {
        String(v);
    }

    void Key(char const* v, uint32_t len)
    {
        String(v, len);
    }

    void StartObject()
    {
        Reserve(1);
        Tag(TypeTag::Object);
    }

    void EndObject()
    {
        Reserve(1);
        Tag(TypeTag::End);
    }

    void StartArray()
    {
        Reserve(1);
        Tag(TypeTag::Array);
    }

    void EndArray()
    {
        Reserve(1);
        Tag(TypeTag::End);
    }

    STDString GetString()
    {
        return STDString((char const*)buf_.raw_buf(), pos_);
    }

private:
    Array<uint8_t> buf_;
    uint32_t pos_{ 0 };
    HashMap<uint64_t, PooledString> strings_;

    void Tag(TypeTag tag)
    {
        buf_[pos_++] = (uint8_t)tag;
    }

    void Tag(TypeTag tag, uint32_t length)
    {
        buf_[pos_++] = (uint8_t)tag | ((uint8_t)(length & 0xf) << 4);
    }

    void LongTag(TypeTag tag, uint32_t length)
    {
        uint8_t lenBytes = 0;
        uint8_t tagLen = length & 0x03;
        length >>= 2;
        if (length >= 0x10000) {
            lenBytes = 3;
        } else if (length > 0x100) {
            lenBytes = 2;
        } else if (length > 0) {
            lenBytes = 1;
        }

        buf_[pos_++] = (uint8_t)tag | (tagLen << 4) | (lenBytes << 6);
        while (length) {
            buf_[pos_++] = (uint8_t)length;
            length >>= 8;
        }
    }
};

class BinaryReader
{
public:
    BinaryReader(std::span<uint8_t const> buf)
        : buf_(buf)
    {}

    inline uint32_t Available()
    {
        return (uint32_t)buf_.size() - pos_;
    }

    inline bool Available(uint32_t sz)
    {
        return Available() >= sz;
    }

    inline void Consume(uint32_t sz)
    {
        pos_ += sz;
    }

    inline uint8_t const* Cur(uint32_t off = 0)
    {
        return buf_.data() + pos_ + off;
    }

    inline uint8_t TypeData()
    {
        return *Cur() >> 4;
    }

    inline uint8_t const* Content()
    {
        return Cur(1);
    }

    inline bool PeekNext(TypeTag& type)
    {
        if (Available(1)) {
            type = (TypeTag)(*Cur() & 0x0f);
            return true;
        } else {
            return false;
        }
    }

    bool ReadLongTag(uint32_t& len)
    {
        auto tag = *Cur();
        auto length = (uint32_t)(tag >> 4) & 3;
        auto bytes = (tag >> 6);
        if (!Available(1 + bytes)) return false;
        Consume(1);

        unsigned off{ 2 };
        while (bytes--) {
            length |= ((uint32_t)*Cur() << off);
            off += 8;
            Consume(1);
        }

        len = length;
        return true;
    }

    bool ReadString(std::string_view& s)
    {
        uint32_t len{ 0 };
        if (!ReadLongTag(len)) return false;
        if (!Available(len)) return false;
        s = std::string_view((char const*)Cur(), len);
        Consume(len);
        return true;
    }

    bool ReadStringAt(std::string_view& s, uint32_t delta)
    {
        // Hijack read pointer for the duration of the read
        if (delta > pos_) return false;
        auto oldPos = pos_;
        pos_ -= delta;

        bool ok = ReadString(s);

        pos_ = oldPos;
        return ok;
    }

    bool ParseArray(lua_State* L)
    {
        int32_t i{ 1 };
        for (;;) {
            TypeTag type;
            if (!PeekNext(type)) return false;

            if (type == TypeTag::End) {
                Consume(1);
                return true;
            }

            if (ParseNext(L)) {
                lua_rawseti(L, -2, i++);
            } else {
                return false;
            }
        }
    }

    bool ParseObject(lua_State* L)
    {
        int32_t i{ 1 };
        for (;;) {
            TypeTag type;
            if (!PeekNext(type)) return false;

            if (type == TypeTag::End) {
                Consume(1);
                return true;
            }

            if (!ParseNext(L)) return false; // Key
            if (!ParseNext(L)) return false; // Value

            lua_rawset(L, -3);
        }
    }

    bool ParseNext(lua_State* L)
    {
        TypeTag type;
        if (!PeekNext(type)) return false;

        switch (type) {
        case TypeTag::Null:
            push(L, nullptr);
            Consume(1);
            break;

        case TypeTag::Bool:
            push(L, TypeData() ? true : false);
            Consume(1);
            break;

        case TypeTag::Int0:
            push(L, (int64_t)TypeData() - 4);
            Consume(1);
            break;

        case TypeTag::Int8:
            if (!Available(2)) return false;
            push(L, (int64_t)(((uint64_t)TypeData() << 8) | *Content()) - 0x400);
            Consume(2);
            break;

        case TypeTag::Int16:
            if (!Available(3)) return false;
            push(L, (int64_t)(((uint64_t)TypeData() << 16) | *(uint16_t*)Content()) - 0x40000);
            Consume(3);
            break;

        case TypeTag::Int32:
            if (!Available(5)) return false;
            push(L, (int64_t)(((uint64_t)TypeData() << 32) | *(uint32_t*)Content()) - 0x400000000l);
            Consume(5);
            break;

        case TypeTag::Int64:
            if (!Available(9)) return false;
            push(L, *(int64_t*)Content());
            Consume(9);
            break;

        case TypeTag::Float:
            if (!Available(5)) return false;
            push(L, *(float*)Content());
            Consume(5);
            break;

        case TypeTag::String:
        {
            std::string_view sv;
            if (ReadString(sv)) {
                push(L, sv);
                return true;
            } else {
                return false;
            }
        }

        case TypeTag::StringRef8:
        {
            if (!Available(2)) return false;
            auto off = ((uint32_t)TypeData() << 8) | *Content();
            std::string_view s;
            if (ReadStringAt(s, off)) {
                Consume(2);
                push(L, s);
                return true;
            } else {
                return false;
            }
        }

        case TypeTag::StringRef16:
        {
            if (!Available(3)) return false;
            auto off = ((uint32_t)TypeData() << 16) | *(uint16_t*)Content();
            std::string_view s;
            if (ReadStringAt(s, off)) {
                Consume(3);
                push(L, s);
                return true;
            } else {
                return false;
            }
        }

        case TypeTag::StringRef32:
        {
            if (!Available(5)) return false;
            auto off = *(uint32_t*)Content();
            std::string_view s;
            if (ReadStringAt(s, off)) {
                Consume(5);
                push(L, s);
                return true;
            } else {
                return false;
            }
        }

        case TypeTag::Object:
            lua_newtable(L);
            Consume(1);
            return ParseObject(L);

        case TypeTag::Array:
            lua_newtable(L);
            Consume(1);
            return ParseArray(L);

        case TypeTag::End: // Should be handled by ParseArray/ParseObject
        default:
            return false;
        }

        return true;
    }

private:
    std::span<uint8_t const> buf_;
    uint32_t pos_{ 0 };
};

END_NS()
