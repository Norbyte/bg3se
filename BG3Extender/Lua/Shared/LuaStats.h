#pragma once

#include <Lua/LuaHelpers.h>
#include <lua/LuaBinding.h>
#include <PropertyMap.h>

#include <mutex>
#include <unordered_set>
#include <optional>

namespace bg3se
{
	struct CRPGStats_Object;
	struct SpellPrototype;
	struct CDivinityStats_Character;
	struct CDivinityStats_Item;
}

namespace bg3se::lua::stats
{
	class StatsProxy : public Userdata<StatsProxy>, public Indexable, public NewIndexable, public Pushable<PushPolicy::Unbind>
	{
	public:
		static char const * const MetatableName;

		StatsProxy(CRPGStats_Object * obj, std::optional<int> level)
			: obj_(obj), level_(level)
		{}

		void Unbind()
		{
			obj_ = nullptr;
		}

		int Index(lua_State * L);
		int NewIndex(lua_State * L);

	private:
		CRPGStats_Object * obj_;
		std::optional<int> level_;
	};

	class SpellPrototypeProxy : public Userdata<SpellPrototypeProxy>, public Indexable, public Pushable<PushPolicy::Unbind>
	{
	public:
		static char const * const MetatableName;

		SpellPrototypeProxy(SpellPrototype* obj, std::optional<int> level);

		void Unbind()
		{
			obj_ = nullptr;
		}

		int Index(lua_State * L);

	private:
		SpellPrototype * obj_;
		CRPGStats_Object * stats_;
		std::optional<int> level_;
	};

	class ItemOrCharacterPushPin
	{
	public:
		ItemOrCharacterPushPin(lua_State * L, CRPGStats_Object * obj);
		~ItemOrCharacterPushPin();

	private:
		ObjectProxy<CDivinityStats_Character> * character_{ nullptr };
		ObjectProxy<CDivinityStats_Item> * item_{ nullptr };
		StatsProxy * object_{ nullptr };
	};


	class StatsExtraDataProxy : public Userdata<StatsExtraDataProxy>, public Indexable, 
		public NewIndexable, public Pushable<PushPolicy::None>
	{
	public:
		static char const * const MetatableName;

		int Index(lua_State * L);
		int NewIndex(lua_State * L);
	};

	/*
	class DamageList : public Userdata<DamageList>, public Pushable<PushPolicy::None>
	{
	public:
		static char const * const MetatableName;

		static void PopulateMetatable(lua_State * L);

		inline DamagePairList & Get()
		{
			return damages_;
		}

	private:
		DamagePairList damages_;

		static int GetByType(lua_State * L);
		static int Add(lua_State * L);
		static int Clear(lua_State * L);
		static int Multiply(lua_State * L);
		static int Merge(lua_State * L);
		static int ConvertDamageType(lua_State * L);
		static int AggregateSameTypeDamages(lua_State * L);
		static int ToTable(lua_State * L);
	};
	*/

	/*int NewDamageList(lua_State* L);
	int GetSurfaceTemplate(lua_State* L);*/

	void RegisterStatsLib(lua_State* L);
}

namespace bg3se::lua::utils
{
	void RegisterUtilsLib(lua_State* L);
	void RegisterStaticDataLib(lua_State* L);
	void RegisterLocalizationLib(lua_State* L);
	void RegisterJsonLib(lua_State* L);
	void RegisterIOLib(lua_State* L);
	void RegisterMathLib(lua_State* L);
}

namespace bg3se::lua
{
	void RegisterSharedLibraries(lua_State* L);
}
