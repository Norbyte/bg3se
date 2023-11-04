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

		Array<ExtComponentType> GetAllComponentTypes(ecs::EntitySystemHelpersBase* ecs) const;
		void GetComponentByType(lua_State* L, ExtComponentType componentType) const;

		static int CreateComponent(lua_State* L);
		static int GetComponent(lua_State* L);
		static int GetAllComponents(lua_State* L);
		static int GetAllComponentNames(lua_State* L);
		static int GetEntityType(lua_State* L);
		static int GetSalt(lua_State* L);
		static int GetIndex(lua_State* L);
		static int IsAlive(lua_State* L);
		static int Replicate(lua_State* L);
		static int SetReplicationFlags(lua_State* L);
		static int GetReplicationFlags(lua_State* L);

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

}
