#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()
class EntitySystemHelpersBase;
END_SE()

namespace bg3se::lua
{
	class EntityProxy : public Userdata<EntityProxy>, public Indexable, public Pushable
	{
	public:
		static char const* const MetatableName;

		EntityProxy(EntityHandle const& handle, EntitySystemHelpersBase* entitySystem);

		static int HasComponent(lua_State* L);
		static int GetEntityType(lua_State* L);
		static int GetSalt(lua_State* L);
		static int GetIndex(lua_State* L);
		static int IsAlive(lua_State* L);
		static void PopulateMetatable(lua_State* L);

		int Index(lua_State* L);

		inline EntityHandle const& Handle() const
		{
			return handle_;
		}

		inline EntitySystemHelpersBase* EntitySystem() const
		{
			return entitySystem_;
		}

	private:
		EntityHandle handle_;
		EntitySystemHelpersBase* entitySystem_;
	};
	

	class ObjectHandleProxy : public Userdata<ObjectHandleProxy>, public Indexable, public Pushable
	{
	public:
		static char const* const MetatableName;

		ObjectHandleProxy(ObjectHandle const& handle, EntitySystemHelpersBase* entitySystem);

		static int GetType(lua_State* L);
		static int GetTypeName(lua_State* L);
		static int GetSalt(lua_State* L);
		static int GetIndex(lua_State* L);
		static void PopulateMetatable(lua_State* L);

		int Index(lua_State* L);

		inline ObjectHandle const& Handle() const
		{
			return handle_;
		}

		inline EntitySystemHelpersBase* EntitySystem() const
		{
			return entitySystem_;
		}

	private:
		ObjectHandle handle_;
		EntitySystemHelpersBase* entitySystem_;
	};

}
