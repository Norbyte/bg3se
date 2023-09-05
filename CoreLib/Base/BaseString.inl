BEGIN_SE()

FixedString::FixedString(StringView str)
	: Index(NullIndex)
{
	auto create = gCoreLibPlatformInterface.ls__FixedString__CreateFromString;
	if (create) {
		Index = create(str);
	}
}

FixedString::FixedString(char const* str)
	: Index(NullIndex)
{
	auto create = gCoreLibPlatformInterface.ls__FixedString__CreateFromString;
	if (create) {
		Index = create(StringView(str, strlen(str)));
	}
}

char const* FixedString::GetString() const
{
	if (Index != NullIndex) {
		auto getter = gCoreLibPlatformInterface.ls__FixedString__GetString;
		if (getter) {
			StringView sv;
#if defined(_DEBUG)
			__try {
				getter(sv, Index);
			} __except (EXCEPTION_EXECUTE_HANDLER) {
				return "<<< EXCEPTION THROWN WHILE READING STRING >>>";
			}
#else
			getter(sv, Index);
#endif

			return sv.data();
		}
	}

	return "";
}

uint32_t FixedString::GetHash() const
{
	if (Index != NullIndex) {
		auto getter = gCoreLibPlatformInterface.ls__FixedString__GetString;
		if (getter) {
			StringView sv;
			getter(sv, Index);
			if (sv.data()) {
				auto entry = reinterpret_cast<GlobalStringTable::StringEntryHeader const*>(sv.data() - sizeof(GlobalStringTable::StringEntryHeader));
				return entry->Hash;
			}
		}
	}

	return 0;
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

	auto header = (GlobalStringTable::StringEntryHeader*)(subTable.Buckets[bucketIdx] + entryIdx * subTable.EntrySize);

	return header->Id == Index;
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
		auto decRef = gCoreLibPlatformInterface.ls__FixedString__DecRef;
		if (decRef) {
			decRef(Index);
		}
	}
}

END_SE()
