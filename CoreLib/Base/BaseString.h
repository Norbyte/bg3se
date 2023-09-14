#pragma once

#include <cstdint>
#include <string>

namespace bg3se
{
	using STDString = std::basic_string<char, std::char_traits<char>, GameAllocator<char>>;
	using STDWString = std::basic_string<wchar_t, std::char_traits<wchar_t>, GameAllocator<wchar_t>>;
	using StringView = std::string_view;
	using WStringView = std::wstring_view;

	std::string ToStdUTF8(std::wstring_view s);
	std::wstring FromStdUTF8(std::string_view s);
	STDString ToUTF8(WStringView s);
	STDWString FromUTF8(StringView s);

	struct FixedString
	{
		using CreateFromStringProc = uint32_t (StringView const&);
		using GetStringProc = StringView * (StringView&, uint32_t index);
		using IncRefProc = void(uint32_t index);
		using DecRefProc = void(uint32_t index);

		static constexpr uint32_t NullIndex = 0xffffffffu;

		inline FixedString()
			: Index(NullIndex)
		{}

		explicit FixedString(StringView str);
		explicit FixedString(char const* str);

		inline FixedString(FixedString const& fs)
			: Index(fs.Index)
		{
			IncRef();
		}

		inline FixedString(FixedString&& fs) noexcept
			: Index(fs.Index)
		{
			fs.Index = NullIndex;
		}

		inline ~FixedString()
		{
			DecRef();
		}

		inline FixedString& operator = (FixedString const& fs)
		{
			if (fs.Index != Index) {
				DecRef();
				Index = fs.Index;
				IncRef();
			}

			return *this;
		}

		inline FixedString& operator = (FixedString&& fs) noexcept
		{
			Index = fs.Index;
			if (this != &fs) {
				fs.Index = NullIndex;
			}

			return *this;
		}

		inline bool operator == (FixedString const& fs) const
		{
			return Index == fs.Index;
		}

		inline bool operator != (FixedString const& fs) const
		{
			return Index != fs.Index;
		}

		inline bool operator !() const
		{
			return Index == 0xffffffffu;
		}

		inline explicit operator bool() const
		{
			return Index != 0xffffffffu;
		}

		char const* GetString() const;
		uint32_t GetHash() const;
		bool IsValid() const;

		uint32_t Index;

	private:
		void IncRef();
		void DecRef();
	};


	inline uint64_t Hash(FixedString const& s)
	{
		return (uint64_t)s.Index;
	}


	struct GlobalStringTable : public ProtectedGameObject<GlobalStringTable>
	{
		struct StringEntryHeader
		{
			uint32_t Hash;
			uint32_t RefCount;
			uint32_t Length;
			uint32_t Id;
			uint64_t NextFreeIndex;
		};

		struct StringEntry : public StringEntryHeader
		{
			char Str[1];
		};

		struct SubTable
		{
			struct Element
			{
				uint64_t field_0[8];
			}; 

			Element field_0[64];
			uint64_t LockCounter;
			uint64_t _Pad1[7];
			uint64_t LockEvent;
			uint64_t _Pad2[7];
			int32_t TableIndex;
			uint64_t EntrySize;
			uint32_t EntriesPerBucket;
			uint64_t _Pad3[5];
			uint32_t NumBuckets;
			uint64_t _Pad4[7];
			uint32_t field_1100;
			uint64_t _Pad5[7];
			uint8_t** Buckets;
			uint64_t _Pad6[7];
			uint64_t field_1180;
			uint64_t _Pad7[7];
			CRITICAL_SECTION CriticalSection;
			uint64_t field_11E8[3];
		};

		struct MainTable
		{
			SubTable::Element SomeTable[64];
			uint64_t field_1000;
			uint64_t _Pad1[7];
			HANDLE EventHandle;
			uint64_t _Pad2[7];
			GlobalStringTable* StringTable;
			uint64_t _Pad3[7];
			uint32_t field_10C0[65535];
		};

		SubTable SubTables[11];
		MainTable Main;
	};

	struct ScratchBuffer : public Noncopyable<ScratchBuffer>
	{
		uint8_t State_M{ 0 };
		uint8_t field_1{ 0 };
		uint16_t MemoryAlignment{ 1 };
		uint32_t MultipleOf{ 1 };
		void* Buffer{ nullptr };
		uint64_t Size{ 0 };
		bool HasBuffer2{ false };
		bool field_19{ false };
		int field_1C{ 0 };
		uint8_t* Buffer2{ nullptr };
		void* field_28{ nullptr };
		void* ReadPointer{ nullptr };
		void* ReadBuffer{ nullptr };
		void* ReadEnd_M{ nullptr };
		void* field_48{ nullptr };
		uint64_t Size2{ 0 };
	};

	struct ScratchString : public Noncopyable<ScratchString>
	{
		void* Buffer{ nullptr };
		uint32_t ReadSize{ 0 };
		uint32_t Size{ 0 };
		uint32_t WritePosition{ 0 };
		uint32_t ReadPosition{ 0 };
		bool Unkn1{ false };
		bool Unkn2{ false };
	};

	struct Guid
	{
		uint64_t Val[2]{ 0 };

		inline constexpr operator bool() const
		{
			return Val[0] != 0 || Val[1] != 0;
		}

		inline constexpr bool operator !() const
		{
			return Val[0] == 0 && Val[1] == 0;
		}

		inline constexpr bool operator ==(Guid const& o) const
		{
			return Val[0] == o.Val[0] && Val[1] == o.Val[1];
		}

		inline constexpr bool operator !=(Guid const& o) const
		{
			return Val[0] != o.Val[0] || Val[1] != o.Val[1];
		}

		STDString Print() const;
		static std::optional<Guid> Parse(StringView s);
		static std::optional<Guid> ParseGuidString(StringView nameGuid);
	};

	inline constexpr uint64_t Hash(Guid const& h)
	{
		return h.Val[0] ^ h.Val[1];
	}
}

namespace std
{
	template<> struct hash<bg3se::FixedString>
	{
		typedef bg3se::FixedString argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& fn) const noexcept
		{
			return std::hash<std::uintptr_t>{}((std::uintptr_t)fn.Index);
		}
	};

	inline ostream& operator << (ostream& out, bg3se::FixedString const& str)
	{
		if (str) {
			out << str.GetString();
		}
		else {
			out << "(null)";
		}
		return out;
	}
}
