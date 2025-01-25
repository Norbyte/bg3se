BEGIN_SE()

uint32_t FixedStringBase::CreateFixedString(StringView const& str)
{
    if (str.size() >= 2047) {
        ERR("Tried to create FixedString of length %d - this will crash! %s", str.size(), str.data());
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

END_SE()
