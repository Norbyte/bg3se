#pragma once

#include <cstdint>
#include <string>

#include <GameDefinitions/BaseUtilities.h>
#include <GameDefinitions/BaseMemory.h>

namespace dse
{
	using STDString = std::basic_string<char, std::char_traits<char>, GameAllocator<char>>;
	using STDWString = std::basic_string<wchar_t, std::char_traits<wchar_t>, GameAllocator<wchar_t>>;
	using StringView = std::string_view;
	using WStringView = std::wstring_view;

	dse::STDString ToUTF8(WStringView s);
	dse::STDWString FromUTF8(StringView s);

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
			return Index > 0xfffffffdu;
		}

		inline explicit operator bool() const
		{
			return Index <= 0xfffffffdu;
		}

		char const* GetString() const;

		uint32_t Index;

	private:

		void IncRef();
		void DecRef();
	};


	template <>
	inline uint64_t Hash<FixedString>(FixedString const& s)
	{
		return (uint64_t)s.Index;
	}


	struct GlobalStringTable : public ProtectedGameObject<GlobalStringTable>
	{
		struct Entry
		{
			const char* StringPtrs[10];
			Entry* Next;
			uint32_t StringPtrItems;
			uint32_t Unused;

			uint32_t Count() const;
			char const* Get(uint32_t i) const;
		};

		Entry HashTable[65521];

		const char* Find(char const* s, uint64_t length) const;
		static uint32_t Hash(char const* s, uint64_t length);
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
	template<> struct hash<dse::FixedString>
	{
		typedef dse::FixedString argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& fn) const noexcept
		{
			return std::hash<std::uintptr_t>{}((std::uintptr_t)fn.Index);
		}
	};

	inline ostream& operator << (ostream& out, dse::FixedString const& str)
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
