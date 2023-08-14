BEGIN_SE()

FixedString::FixedString(StringView str)
	: Index(NullIndex)
{
	auto create = GetStaticSymbols().ls__FixedString__CreateFromString;
	if (create) {
		Index = create(str);
	}
}

FixedString::FixedString(char const* str)
	: Index(NullIndex)
{
	auto create = GetStaticSymbols().ls__FixedString__CreateFromString;
	if (create) {
		Index = create(StringView(str, strlen(str)));
	}
}

char const* FixedString::GetString() const
{
	if (Index != NullIndex) {
		auto getter = GetStaticSymbols().ls__FixedString__GetString;
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
		auto getter = GetStaticSymbols().ls__FixedString__GetString;
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

void FixedString::IncRef()
{
	if (Index != NullIndex) {
		auto incRef = GetStaticSymbols().ls__FixedString__IncRef;
		if (incRef) {
			incRef(Index);
		}
	}
}

void FixedString::DecRef()
{
	if (Index != NullIndex) {
		auto decRef = GetStaticSymbols().ls__FixedString__DecRef;
		if (decRef) {
			decRef(Index);
		}
	}
}

END_SE()
