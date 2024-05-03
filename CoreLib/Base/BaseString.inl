BEGIN_SE()

FixedString::FixedString(StringView str)
	: Index(NullIndex)
{
	if (str.size() >= 2047) {
		ERR("Tried to create FixedString of length %d - this will crash! %s", str.size(), str.data());
		return;
	}

	auto createGlobal = gCoreLibPlatformInterface.ls__GlobalStringTable__MainTable__CreateFromString;
	if (createGlobal) {
		LSStringView sv(str.data(), (uint32_t)str.size());
		createGlobal(&(*gCoreLibPlatformInterface.ls__gGlobalStringTable)->Main, this, &sv);
	} else {
		auto create = gCoreLibPlatformInterface.ls__FixedString__CreateFromString;
		if (create) {
			Index = create(LSStringView(str.data(), (uint32_t)str.size()));
		}
	}
}

FixedString::FixedString(char const* str)
	: Index(NullIndex)
{
	LSStringView sv(str, (uint32_t)strlen(str));
	if (sv.size() >= 2047) {
		ERR("Tried to create FixedString of length %d - this will crash! %s", sv.size(), str);
		return;
	}

	auto createGlobal = gCoreLibPlatformInterface.ls__GlobalStringTable__MainTable__CreateFromString;
	if (createGlobal) {
		createGlobal(&(*gCoreLibPlatformInterface.ls__gGlobalStringTable)->Main, this, &sv);
	} else {
		auto create = gCoreLibPlatformInterface.ls__FixedString__CreateFromString;
		if (create) {
			Index = create(sv);
		}
	}
}

char const* FixedString::GetPooledStringPtr() const
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

FixedString::Header const* FixedString::GetMetadata() const
{
	if (Index != NullIndex) {
		auto str = GetPooledStringPtr();
		return reinterpret_cast<Header const*>(str - sizeof(Header));
	} else {
		return nullptr;
	}
}

char const* FixedString::GetString() const
{
	auto str = GetPooledStringPtr();
	return str ? str : "";
}

StringView FixedString::GetStringView() const
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

uint32_t FixedString::GetLength() const
{
	if (Index != NullIndex) {
		return GetMetadata()->Length;
	} else {
		return 0;
	}
}

uint32_t FixedString::GetHash() const
{
	if (Index != NullIndex) {
		return GetMetadata()->Hash;
	} else {
		return 0;
	}
}

bool FixedString::IsValid() const
{
	if (Index == NullIndex) return true;

	auto gst = gCoreLibPlatformInterface.ls__gGlobalStringTable;
	if (!gst || !*gst) return false;

	auto subTableIdx = (Index & 0x0F);
	if (subTableIdx >= std::size((*gst)->SubTables)) {
		return false;
	}

	auto& subTable = (*gst)->SubTables[subTableIdx];

	auto bucketIdx = (Index >> 4) & 0xffff;
	auto entryIdx = (Index >> 20);

	if (bucketIdx >= subTable.NumBuckets || entryIdx >= subTable.EntriesPerBucket) {
		return false;
	}

	auto header = (Header*)(subTable.Buckets[bucketIdx] + entryIdx * subTable.EntrySize);

	if (header->RefCount > 0x1000000) return false;
	if (header->Length > subTable.EntrySize - 0x18) return false;

	return true;
}

void FixedString::IncRef()
{
	if (Index != NullIndex) {
		auto incRef = gCoreLibPlatformInterface.ls__FixedString__IncRef;
		if (incRef) {
			incRef(Index);
		}
	}
}

void FixedString::DecRef()
{
	if (Index != NullIndex) {
		auto decRefGlobal = gCoreLibPlatformInterface.ls__GlobalStringTable__MainTable__DecRef;
		if (decRefGlobal) {
			decRefGlobal(&(*gCoreLibPlatformInterface.ls__gGlobalStringTable)->Main, this);
		} else {
			auto decRef = gCoreLibPlatformInterface.ls__FixedString__DecRef;
			if (decRef) {
				decRef(Index);
			}
		}
	}
}

END_SE()
