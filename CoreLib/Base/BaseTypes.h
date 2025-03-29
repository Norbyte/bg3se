#pragma once

#define GLM_FORCE_ALIGNED_GENTYPES

#include <array>
#include <string>
#include <cassert>
#include <optional>
#include <atomic>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_aligned.hpp>

namespace bg3se
{
    template <class T>
    using Vector = std::vector<T, GameAllocator<T>>;

    using Vector3 = glm::vec3;

    struct NetId
    {
        static constexpr uint64_t Unassigned = 0xfffffffffffffffful;

        uint64_t Id;

        inline constexpr NetId() : Id(Unassigned) {}
        inline explicit constexpr NetId(uint64_t id) : Id(id) {}
        inline constexpr NetId(NetId const & id) : Id(id.Id) {}

        inline explicit operator bool() const
        {
            return Id != Unassigned;
        }

        inline bool operator !() const
        {
            return Id == Unassigned;
        }

        inline bool operator ==(NetId const& o) const
        {
            return Id == o.Id;
        }

        inline bool operator !=(NetId const& o) const
        {
            return Id != o.Id;
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

    inline constexpr uint64_t Hash(NetId const& id)
    {
        return id.Id;
    }

    using TPeerId = uint32_t;
    using TUserId = uint32_t;
    enum class PeerId : TPeerId {};
    static constexpr PeerId LocalPeerId = PeerId{ 1 };

    struct UserId
    {
        static constexpr auto Unassigned = (TUserId)0xFFFF0000;

        TUserId Id;

        inline constexpr UserId() : Id(Unassigned) {}
        inline explicit constexpr UserId(TUserId id) : Id(id) {}
        inline constexpr UserId(UserId const& id) : Id(id.Id) {}

        inline explicit operator bool() const
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
        inline Path() {}

        inline Path(STDString const& s) : Name(s) {}

        STDString Name;
#if !defined(OSI_EOCAPP)
        uint32_t Unknown{ 0 };
#endif
    };

    struct TranslatedString;

    template <class T>
    struct TypedHandle
    {
        static constexpr uint64_t NullHandle = 0xFFC0000000000000ull;

        uint64_t Handle;

        inline constexpr TypedHandle()
            : Handle(NullHandle)
        {}

        explicit inline constexpr TypedHandle(uint64_t handle)
            : Handle(handle)
        {}

        explicit inline constexpr TypedHandle(int64_t handle)
            : Handle((uint64_t)handle)
        {}

        inline constexpr TypedHandle(uint64_t type, uint64_t index, uint64_t salt)
        {
            assert(type < 0x400 && salt < 0x400000);
            Handle = index | (salt << 32) | (type << 54);
        }

        inline constexpr TypedHandle(uint64_t type, uint64_t indexAndSalt)
        {
            assert(type < 0x400 && (indexAndSalt >> 32) < 0x400000);
            Handle = indexAndSalt | (type << 54);
        }

        inline constexpr TypedHandle(TypedHandle const & oh)
            : Handle(oh.Handle)
        {}

        inline constexpr TypedHandle& operator = (TypedHandle const & oh)
        {
            Handle = oh.Handle;
            return *this;
        }

        inline constexpr bool operator == (TypedHandle const & oh) const
        {
            return Handle == oh.Handle;
        }

        inline constexpr uint32_t GetType() const
        {
            return Handle >> 54;
        }

        inline constexpr uint32_t GetThreadIndex() const
        {
            return Handle >> 54;
        }

        inline constexpr uint32_t GetSalt() const
        {
            return (Handle >> 32) & 0x3fffff;
        }

        inline constexpr uint32_t GetIndex() const
        {
            return (uint32_t)(Handle & 0xffffffff);
        }

        explicit inline constexpr operator bool() const
        {
            return Handle != NullHandle;
        }

        inline constexpr bool operator !() const
        {
            return Handle == NullHandle;
        }

        explicit inline constexpr operator int64_t() const
        {
            return (int64_t)Handle;
        }
    };

    enum EntityHandleTag {};
    enum GenericComponentHandleTag {};

    using EntityHandle = TypedHandle<EntityHandleTag>;
    using ComponentHandle = TypedHandle<GenericComponentHandleTag>;
    static constexpr auto NullEntityHandle = EntityHandle{ EntityHandle::NullHandle };

    template <class T>
    inline constexpr uint64_t Hash(TypedHandle<T> const& h)
    {
        return h.Handle;
    }

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
        glm::quat RotationQuat;
        glm::vec3 Translate;
        glm::vec3 Scale;
    };
}


namespace std
{
    template<> struct hash<bg3se::EntityHandle>
    {
        typedef bg3se::EntityHandle argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& fn) const noexcept
        {
            return std::hash<uint64_t>{}(fn.Handle);
        }
    };

    /*template<> struct hash<bg3se::UserId>
    {
        typedef bg3se::UserId argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& fn) const noexcept
        {
            return std::hash<int32_t>{}(fn.Id);
        }
    };*/

    template <class T>
    inline ostream& operator << (ostream& out, bg3se::TypedHandle<T> const& h)
    {
        if (h) {
            out << "(Object handle 0x" << std::hex << h.Handle << std::dec << ")";
        } else {
            out << "(Invalid object handle)";
        }
        return out;
    }

    inline ostream& operator << (ostream& out, bg3se::EntityHandle const& h)
    {
        if (h) {
            out << "(Entity handle 0x" << std::hex << h.Handle << std::dec << ")";
        } else {
            out << "(Invalid entity handle)";
        }
        return out;
    }
}
