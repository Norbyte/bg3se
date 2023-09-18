#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/EntitySystemHelpers.h>

namespace bg3se::lua
{
	class EntityProxy : public Userdata<EntityProxy>, public Indexable, public Stringifiable, public Pushable
	{
	public:
		static char const* const MetatableName;

		EntityProxy(EntityHandle const& handle);

		template <class T>
		bool HasComponent(lua_State* L)
		{
			return GetEntitySystem(L)->GetComponent<T>(handle_) != nullptr;
		}

		template <class T>
		T* GetComponent(lua_State* L)
		{
			return GetEntitySystem(L)->GetComponent<T>(handle_);
		}

		static int GetComponent(lua_State* L);
		static int GetAllComponents(lua_State* L);
		static int GetAllComponentNames(lua_State* L);
		static int GetEntityType(lua_State* L);
		static int GetSalt(lua_State* L);
		static int GetIndex(lua_State* L);
		static int IsAlive(lua_State* L);

		int Index(lua_State* L);
		int ToString(lua_State* L);

		inline EntityHandle const& Handle() const
		{
			return handle_;
		}

		static ecs::EntitySystemHelpersBase* GetEntitySystem(lua_State* L);

	private:
		EntityHandle handle_;
	};


	class ComponentHandleProxy : public Userdata<ComponentHandleProxy>, public Indexable, public Stringifiable, public Pushable
	{
	public:
		static char const* const MetatableName;

		ComponentHandleProxy(ComponentHandle const& handle, ecs::EntitySystemHelpersBase* entitySystem);

		static int GetType(lua_State* L);
		static int GetTypeName(lua_State* L);
		static int GetSalt(lua_State* L);
		static int GetIndex(lua_State* L);
		static int GetComponent(lua_State* L);
		static void PopulateMetatable(lua_State* L);

		int Index(lua_State* L);
		int ToString(lua_State* L);

		inline ComponentHandle const& Handle() const
		{
			return handle_;
		}

		inline ecs::EntitySystemHelpersBase* EntitySystem() const
		{
			return entitySystem_;
		}

	private:
		ComponentHandle handle_;
		ecs::EntitySystemHelpersBase* entitySystem_;
	};

}
