#pragma once

#include <cstdint>
#include <string>

#include <GameDefinitions/BaseUtilities.h>
#include <GameDefinitions/BaseMemory.h>

namespace bg3se
{
	using STDString = std::basic_string<char, std::char_traits<char>, GameAllocator<char>>;
	using STDWString = std::basic_string<wchar_t, std::char_traits<wchar_t>, GameAllocator<wchar_t>>;
	using StringView = std::string_view;
	using WStringView = std::wstring_view;

	bg3se::STDString ToUTF8(WStringView s);
	bg3se::STDWString FromUTF8(StringView s);

	struct FixedString
	{
		using CreateProc = FixedString * (FixedString&, char const* str, int length);
		using GetStringProc = char const* (FixedString const&);
		using IncRefProc = void(uint32_t index);
		using DecRefProc = void(uint32_t index);

		static constexpr uint32_t NullIndex = 0xffffffffu;

		inline FixedString()
			: Index(NullIndex)
		{}

		explicit FixedString(std::string_view str);
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
			uint32_t FastLock;
			uint32_t Length;
			uint32_t Id;
			uint64_t SlotIndex;
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
			__int64 field_1000[17];
			__int64 EntrySize;
			int EntriesPerBucket;
			__int64 field_1098[21];
			StringEntry** PoolPtr;
			__int64 field_1148[15];
			CRITICAL_SECTION CriticalSection;
			__int64 field_11E8[3];
		};

		struct MainTable
		{
			SubTable::Element SomeTable[64];
			__int64 field_1000;
			__int64 field_1008;
			__int64 field_1010;
			__int64 field_1018;
			__int64 field_1020;
			__int64 field_1028;
			__int64 field_1030;
			__int64 field_1038;
			HANDLE EventHandle;
			__int64 field_1048;
			__int64 field_1050;
			__int64 field_1058;
			__int64 field_1060;
			__int64 field_1068;
			__int64 field_1070;
			__int64 field_1078;
			GlobalStringTable* StringTable;
			__int64 field_1088;
			__int64 field_1090;
			__int64 field_1098;
			__int64 field_10A0;
			__int64 field_10A8;
			__int64 field_10B0;
			__int64 field_10B8;
			uint32_t field_10C0[65535];
		};

		SubTable SubTables[11];
		MainTable Main;
	};

	struct ScratchBuffer : public Noncopyable<ScratchBuffer>
	{
		struct Guts
		{
			char field_0;
			char field_1;
			__int16 field_2;
			int field_4;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
		};


		void* Buffer{ nullptr };
		uint32_t ReadSize{ 0 };
		uint32_t Size{ 0 };
		uint32_t WritePosition{ 0 };
		uint32_t ReadPosition{ 0 };
		Guts Guts_;
		uint64_t Unkn[4]{ 0 };
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
