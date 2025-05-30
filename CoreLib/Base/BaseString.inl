BEGIN_SE()

uint32_t FixedStringBase::CreateFixedString(StringView const& str)
{
    if (str.size() >= 2047) {
        ERR("Tried to create FixedString of length %d - this will crash! %s", str.size(), str.data());
        return NullIndex;
    }
    
    if (str.empty()) {
        return NullIndex;
    }

    auto createGlobal = gCoreLibPlatformInterface.ls__GlobalStringTable__MainTable__CreateFromString;
    if (createGlobal) {
        LSStringView sv(str.data(), (uint32_t)str.size());
        uint32_t id;
        createGlobal(&(*gCoreLibPlatformInterface.ls__gGlobalStringTable)->Main, &id, &sv);
        return id;
    } else {
        auto create = gCoreLibPlatformInterface.ls__FixedString__CreateFromString;
        if (create) {
            return create(LSStringView(str.data(), (uint32_t)str.size()));
        }
    }

    return NullIndex;
}

char const* FixedStringBase::GetPooledStringPtr() const
{
    if (Index != NullIndex) {
        auto getter = gCoreLibPlatformInterface.ls__FixedString__GetString;
        if (getter) {
            LSStringView sv;
            getter(this, sv);
            return sv.data();
        }
    }

    return nullptr;
}

FixedStringBase::Header const* FixedStringBase::GetMetadata() const
{
    if (Index != NullIndex) {
        auto str = GetPooledStringPtr();
        return reinterpret_cast<Header const*>(str - sizeof(Header));
    } else {
        return nullptr;
    }
}

char const* FixedStringBase::GetString() const
{
    auto str = GetPooledStringPtr();
    return str ? str : "";
}

StringView FixedStringBase::GetStringView() const
{
    if (Index != NullIndex) {
        auto getter = gCoreLibPlatformInterface.ls__FixedString__GetString;
        if (getter) {
            LSStringView sv;
            getter(this, sv);
            return StringView(sv.data(), sv.size());
        }
    }

    return StringView();
}

uint32_t FixedStringBase::GetLength() const
{
    if (Index != NullIndex) {
        return GetMetadata()->Length;
    } else {
        return 0;
    }
}

uint32_t FixedStringBase::GetHash() const
{
    if (Index != NullIndex) {
        return GetMetadata()->Hash;
    } else {
        return 0;
    }
}

bool FixedStringBase::IsValid() const
{
    if (Index == NullIndex) return true;

    auto header = FindEntry(Index);
    if (!header) return false;

    auto gst = gCoreLibPlatformInterface.ls__gGlobalStringTable;
    auto subTableIdx = (Index & 0x0F);
    auto& subTable = (*gst)->SubTables[subTableIdx];

    if (header->RefCount > 0x1000000) return false;
    if (header->Length > subTable.EntrySize - 0x18) return false;

    return true;
}

FixedStringBase::Header const* FixedStringBase::FindEntry(uint32_t id)
{
    if (id == NullIndex) return nullptr;

    auto gst = gCoreLibPlatformInterface.ls__gGlobalStringTable;
    if (!gst || !*gst) [[unlikely]] return nullptr;

    auto subTableIdx = (id & 0x0F);
    if (subTableIdx >= std::size((*gst)->SubTables)) [[unlikely]] {
        return nullptr;
    }

    auto& subTable = (*gst)->SubTables[subTableIdx];

    auto bucketIdx = (id >> 4) & 0xffff;
    auto entryIdx = (id >> 20);

    if (bucketIdx >= subTable.NumBuckets || entryIdx >= subTable.EntriesPerBucket) [[unlikely]] {
        return nullptr;
    }

    return (Header*)(subTable.Buckets[bucketIdx] + entryIdx * subTable.EntrySize);
}

void FixedStringBase::StaticIncRef(uint32_t index)
{
    if (index != NullIndex) {
        auto incRef = gCoreLibPlatformInterface.ls__FixedString__IncRef;
        if (incRef) {
            incRef(index);
        }
    }
}

void FixedStringBase::StaticDecRef(uint32_t index)
{
    if (index != NullIndex) {
        auto decRefGlobal = gCoreLibPlatformInterface.ls__GlobalStringTable__MainTable__DecRef;
        if (decRefGlobal) {
            decRefGlobal(&(*gCoreLibPlatformInterface.ls__gGlobalStringTable)->Main, &index);
        } else {
            auto decRef = gCoreLibPlatformInterface.ls__FixedString__DecRef;
            if (decRef) {
                decRef(index);
            }
        }
    }
}

void FixedStringBase::IncRef()
{
    StaticIncRef(Index);
}

void FixedStringBase::DecRef()
{
    StaticDecRef(Index);
}


FixedString::FixedString(StringView str)
    : FixedStringBase(CreateFixedString(str))
{}

FixedString::FixedString(char const* str)
    : FixedStringBase(CreateFixedString(StringView(str, (uint32_t)strlen(str))))
{}


FixedStringUnhashed::FixedStringUnhashed(StringView str)
    : FixedStringBase(CreateFixedString(str))
{}

FixedStringUnhashed::FixedStringUnhashed(char const* str)
    : FixedStringBase(CreateFixedString(StringView(str, (uint32_t)strlen(str))))
{}


FixedStringNoRef::FixedStringNoRef(StringView str)
    : FixedStringId(FixedStringBase::CreateFixedString(str))
{
    FixedStringBase::StaticDecRef(Index);
}

FixedStringNoRef::FixedStringNoRef(char const* str)
    : FixedStringId(FixedStringBase::CreateFixedString(StringView(str, (uint32_t)strlen(str))))
{
    FixedStringBase::StaticDecRef(Index);
}


MemBuffer::MemBuffer(std::span<char const> s)
{
    Buffer = GameAllocRaw(s.size());
    Size = s.size();
    memcpy(Buffer, s.data(), s.size());
}

MemBuffer::MemBuffer(MemBuffer const& o)
    : Meta(o.Meta)
{
    if (o.Buffer) {
        Meta.Origin = MemoryOrigin::Owned;
        Buffer = GameAllocRaw(o.Size);
        Size = o.Size;
        memcpy(Buffer, o.Buffer, o.Size);
    } else {
        Buffer = nullptr;
        Size = 0;
    }
}

MemBuffer::MemBuffer(MemBuffer&& o)
{
    Meta = o.Meta;
    Buffer = o.Buffer;
    Size = o.Size;

    o.Meta.Origin = MemoryOrigin::None;
    o.Buffer = nullptr;
    o.Size = 0;
}

MemBuffer::~MemBuffer()
{
    if (Meta.Origin == MemoryOrigin::Owned) {
        GameFree(Buffer);
    }
}

MemBuffer& MemBuffer::operator = (MemBuffer const& o)
{
    if (Meta.Origin == MemoryOrigin::Owned) {
        GameFree(Buffer);
    }

    new (this) MemBuffer(o);
    return *this;
}

MemBuffer& MemBuffer::operator = (MemBuffer&& o)
{
    if (Meta.Origin == MemoryOrigin::Owned) {
        GameFree(Buffer);
    }

    new (this) MemBuffer(std::move(o));
    return *this;
}


ScratchBuffer::ScratchBuffer(std::span<char const> s)
    : Buffer(s),
    Size(s.size())
{
    RemapWriteStream();
    RemapReadStream();
}

ScratchBuffer::ScratchBuffer(ScratchBuffer const& o)
    : Buffer(o.Buffer),
    Size(o.Size)
{
    RemapWriteStream();
    RemapReadStream();
}

ScratchBuffer::ScratchBuffer(ScratchBuffer&& o)
    : Buffer(std::move(o.Buffer)),
    Write(std::move(o.Write)),
    Read(std::move(o.Read)),
    Size(o.Size)
{}

ScratchBuffer::~ScratchBuffer()
{}

ScratchBuffer& ScratchBuffer::operator = (ScratchBuffer const& o)
{
    Buffer = o.Buffer;
    Size = o.Size;
    RemapWriteStream();
    RemapReadStream();
    return *this;
}

ScratchBuffer& ScratchBuffer::operator = (ScratchBuffer&& o)
{
    Buffer = std::move(o.Buffer);
    Write = std::move(o.Write);
    Read = std::move(o.Read);
    Size = o.Size;
    return *this;
}

void ScratchBuffer::RemapWriteStream()
{
    Write.Meta.Origin = MemoryOrigin::External;
    Write.Meta.Flags = MemBufferFlags::Mutable;
    Write.Meta.AllocatorTag = 0;
    Write.Meta.Alignment = 0;
    Write.Buffer = Buffer.Buffer;
    Write.Size = Buffer.Size;
    Write.Offset = 0;
}

void ScratchBuffer::RemapReadStream()
{
    Read.Buffer = Buffer.Buffer;
    Read.Size = Buffer.Size;
    Read.Offset = 0;
}


ScratchString::ScratchString(std::span<char const> s)
{
    Buffer = GameAllocArray<char>(s.size());
    Capacity = (uint32_t)s.size();
    Size = (uint32_t)s.size();
    Managed = true;
    CanGrow = true;
    memcpy(Buffer, s.data(), s.size());
}

ScratchString::ScratchString(ScratchString const& o)
    : Buffer(o.Buffer),
    Position(o.Position),
    Capacity(o.Capacity),
    Size(o.Size),
    FloatPrecision(o.FloatPrecision),
    Managed(o.Managed),
    CanGrow(o.CanGrow)
{
    if (Buffer) {
        Buffer = GameAllocArray<char>(o.Capacity);
        Managed = true;
        CanGrow = true;
        memcpy(Buffer, o.Buffer, o.Size);
    }
}

ScratchString::ScratchString(ScratchString&& o)
    : Buffer(o.Buffer),
    Position(o.Position),
    Capacity(o.Capacity),
    Size(o.Size),
    FloatPrecision(o.FloatPrecision),
    Managed(o.Managed),
    CanGrow(o.CanGrow)
{
    o.Buffer = nullptr;
    o.Managed = false;
    o.CanGrow = false;
}

ScratchString::~ScratchString()
{
    if (Managed && Buffer) {
        GameFree(Buffer);
    }
}

ScratchString& ScratchString::operator = (ScratchString const& o)
{
    if (Managed && Buffer) {
        GameFree(Buffer);
    }

    new (this) ScratchString(o);
    return *this;
}

ScratchString& ScratchString::operator = (ScratchString&& o)
{
    if (Managed && Buffer) {
        GameFree(Buffer);
    }

    new (this) ScratchString(std::move(o));
    return *this;
}

END_SE()
