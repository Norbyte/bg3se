#pragma once

#include <GameDefinitions/BaseUtilities.h>
#include <GameDefinitions/BaseMemory.h>
#include <GameDefinitions/BaseString.h>
#include <GameDefinitions/BaseContainers.h>

#include <glm/glm.hpp>

#define UUID_DEFINED
struct UUID
{
	uint64_t Val[2];

	inline bool operator ==(UUID const& o) const
	{
		return Val[0] == o.Val[0] && Val[1] == o.Val[1];
	}

	inline bool operator !=(UUID const& o) const
	{
		return Val[0] != o.Val[0] || Val[1] != o.Val[1];
	}
};

namespace dse
{
	template <>
	inline uint64_t Hash<UUID>(UUID const& h)
	{
		return h.Val[0] ^ h.Val[1];
	}

#if defined(OSI_EOCAPP)
	template <class T>
	using Vector = std::vector<T, GameAllocator<T>>;
#else
	template <class T>
	using Vector = std::vector<T, MSVCAllocator<T>>;
#endif

	using Vector3 = glm::vec3;

	struct NetId
	{
		static constexpr uint64_t Unassigned = 0xfffffffffffffffful;

		uint64_t Id;

		inline constexpr NetId() : Id(Unassigned) {}
		inline explicit constexpr NetId(uint64_t id) : Id(id) {}
		inline constexpr NetId(NetId const & id) : Id(id.Id) {}

		inline operator bool() const
		{
			return Id != Unassigned;
		}

		inline bool operator !() const
		{
			return Id == Unassigned;
		}

		inline constexpr uint64_t GetIndex() const
		{
			return (Id & 0xfffffffffffful);
		}

		inline constexpr uint16_t GetSalt() const
		{
			return (uint16_t)(Id >> 48);
		}
	};

	enum class PeerIdTag {};
	using PeerId = TypedIntegral<int32_t, PeerIdTag>;

	struct UserId
	{
		static constexpr int32_t Unassigned = (int32_t)0xFFFF0000;

		int32_t Id;

		inline constexpr UserId() : Id(Unassigned) {}
		inline explicit constexpr UserId(int32_t id) : Id(id) {}
		inline constexpr UserId(UserId const& id) : Id(id.Id) {}

		inline operator bool() const
		{
			return Id != Unassigned;
		}

		inline bool operator !() const
		{
			return Id == Unassigned;
		}

		inline bool operator ==(UserId const& o) const
		{
			return Id == o.Id;
		}

		inline bool operator !=(UserId const& o) const
		{
			return Id != o.Id;
		}

		inline constexpr PeerId GetPeerId() const
		{
			return PeerId(Id >> 16);
		}
	};

	static constexpr UserId ReservedUserId{ UserId::Unassigned };


	struct Path
	{
		STDString Name;
#if !defined(OSI_EOCAPP)
		uint32_t Unknown{ 0 };
#endif
	};

	struct RuntimeStringHandle
	{
		FixedString Handle;
		uint16_t Version{ 0 };
	};

	struct TranslatedString
	{
		RuntimeStringHandle Handle;
		RuntimeStringHandle ArgumentString;
	};


	struct ObjectHandle
	{
		static constexpr uint64_t NullHandle = 0xFFC0000000000000ull;

		uint64_t Handle;

		inline ObjectHandle()
			: Handle(NullHandle)
		{}

		explicit inline ObjectHandle(uint64_t handle)
			: Handle(handle)
		{}

		explicit inline ObjectHandle(int64_t handle)
			: Handle((uint64_t)handle)
		{}

		inline ObjectHandle(uint64_t type, uint64_t index, uint64_t salt)
		{
			assert(type < 0x400 && salt < 0x400000);
			Handle = index | (salt << 32) | (type << 54);
		}

		inline ObjectHandle(ObjectHandle const & oh)
			: Handle(oh.Handle)
		{}

		inline ObjectHandle & operator = (ObjectHandle const & oh)
		{
			Handle = oh.Handle;
			return *this;
		}

		inline bool operator == (ObjectHandle const & oh) const
		{
			return Handle == oh.Handle;
		}

		inline uint32_t GetType() const
		{
			return Handle >> 54;
		}

		inline uint32_t GetSalt() const
		{
			return (Handle >> 32) & 0x3fffff;
		}

		inline uint32_t GetIndex() const
		{
			return (uint32_t)(Handle & 0xffffffff);
		}

		explicit inline operator bool() const
		{
			return Handle != NullHandle;
		}

		inline bool operator !() const
		{
			return Handle == NullHandle;
		}

		explicit inline operator int64_t() const
		{
			return (int64_t)Handle;
		}
	};

	template <>
	inline uint64_t Hash<ObjectHandle>(ObjectHandle const& h)
	{
		return h.Handle;
	}

	struct ObjectVisitor : public ProtectedGameObject<ObjectVisitor>
	{
		virtual void Destroy() = 0;
		virtual void SetFileName(Path*) = 0;
		virtual void SetFileVersion(uint32_t*) = 0;
		virtual void SetCodeVersion(uint32_t*) = 0;
		virtual Path* GetFileName() = 0;
		virtual uint32_t GetCodeVersion() = 0;
		virtual uint32_t GetFileVersion() = 0;
		virtual void SetIO() = 0;
		virtual bool IsReading() = 0;
		virtual void Save(Path*) = 0;
		virtual void Load(Path*) = 0;
		virtual void Print(const char*) = 0;
		virtual bool TryEnterRegion(FixedString const&) = 0;
		virtual bool EnterRegion(FixedString const&) = 0;
		virtual void HasRegion(FixedString const&) = 0;
		virtual void ExitRegion(FixedString const&) = 0;
		virtual bool EnterNode(FixedString const&, FixedString const&) = 0;
		virtual void HasNode(FixedString const&) = 0;
		virtual void ExitNode(FixedString const&) = 0;
		virtual void VisitCount(FixedString const&, uint32_t*) = 0;
		virtual void FindDataType(FixedString const&) = 0;
		virtual void VisitRaw(FixedString const&, void*, uint64_t, uint32_t) = 0;
		virtual void VisitTransatedFSString() = 0;
		virtual void VisitGuid(FixedString const&, void*, void const*) = 0;
		virtual void VisitTranslatedString(FixedString const&, TranslatedString&, TranslatedString const&) = 0;
		virtual void VisitSTDWString(FixedString const&, STDWString&, STDWString const&) = 0;
		virtual void VisitSTDString(FixedString const&, STDString&, STDString const&) = 0;
		virtual void VisitFixedString(FixedString const&, FixedString&, FixedString const&) = 0;
		virtual void VisitPath(FixedString const&, Path*, Path*) = 0;
		virtual void VisitWCharT() = 0;
		virtual void VisitWCharT2() = 0;
		virtual void VisitCharArray() = 0;
		virtual void VisitCharArray2() = 0;
		virtual void VisitObjectHandle(FixedString const&, void*, void const*) = 0;
		virtual void VisitQuaternion(FixedString const&, float&, float const&) = 0;
		virtual void VisitMatrix4x3(FixedString const&, float&, float const&) = 0;
		virtual void VisitMatrix3x4(FixedString const&, float&, float const&) = 0;
		virtual void VisitMatrix4x4(FixedString const&, float&, float const&) = 0;
		virtual void VisitMatrix3x3(FixedString const&, float&, float const&) = 0;
		virtual void VisitMatrix2x2(FixedString const&, float&, float const&) = 0;
		virtual void VisitVector4Unaligned(FixedString const&, float&, float const&) = 0;
		virtual void VisitVector4(FixedString const&, float&, float const&) = 0;
		virtual void VisitVector3(FixedString const&, float&, float const&) = 0;
		virtual void VisitVector2(FixedString const&, float&, float const&) = 0;
		virtual void VisitVector3i(FixedString const&, int*, int*) = 0;
		virtual void VisitVector2i(FixedString const&, int*, int*) = 0;
		virtual void VisitUInt64(FixedString const&, uint64_t&, uint64_t) = 0;
		virtual void VisitInt64(FixedString const&, int64_t&, int64_t) = 0;
		virtual void VisitFloat(FixedString const&, float&, float) = 0;
		virtual void VisitDouble(FixedString const&, double&, double) = 0;
		virtual void VisitInt32(FixedString const&, int32_t&, int32_t) = 0;
		virtual void VisitUInt32(FixedString const&, uint32_t&, uint32_t) = 0;
		virtual void VisitUInt16(FixedString const&, uint16_t&, uint16_t) = 0;
		virtual void VisitInt16(FixedString const&, int16_t&, int16_t) = 0;
		virtual void VisitBool(FixedString const&, bool&, bool) = 0;
		virtual void VisitUInt8(FixedString const&, uint8_t&, uint8_t) = 0;
		virtual void VisitInt8(FixedString const&, int8_t&, int8_t) = 0;
		virtual void VisitBuffer(FixedString const&, ScratchBuffer&) = 0;
	};

	struct Pool
	{
		int PoolSize;
		int MaxSize;
#if !defined(OSI_EOCAPP)
		char* PoolName;
#endif
	};

	struct Transform
	{
		glm::mat4x4 Matrix[16];
		glm::mat3x3 Rotate[9];
		glm::vec3 Translate[3];
		glm::vec3 Scale[3];
	};
}


namespace std
{
	template<> struct hash<dse::ObjectHandle>
	{
		typedef dse::ObjectHandle argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& fn) const noexcept
		{
			return std::hash<uint64_t>{}(fn.Handle);
		}
	};

	/*template<> struct hash<dse::UserId>
	{
		typedef dse::UserId argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& fn) const noexcept
		{
			return std::hash<int32_t>{}(fn.Id);
		}
	};*/

	inline ostream& operator << (ostream& out, dse::ObjectHandle const& h)
	{
		if (h) {
			out << "(Handle 0x" << std::hex << h.Handle << std::dec << ")";
		} else {
			out << "(Invalid handle)";
		}
		return out;
	}
}
