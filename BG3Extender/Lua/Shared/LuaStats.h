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

namespace bg3se::lua
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

	int GetStatEntries(lua_State* L);
	int GetStatEntriesLoadedBefore(lua_State* L);
	/*int GetSkillSet(lua_State* L);
	int UpdateSkillSet(lua_State* L);
	int GetEquipmentSet(lua_State* L);
	int UpdateEquipmentSet(lua_State* L);
	int GetTreasureTable(lua_State* L);
	int UpdateTreasureTable(lua_State* L);
	int GetTreasureCategory(lua_State* L);
	int UpdateTreasureCategory(lua_State* L);
	int GetItemCombo(lua_State* L);
	int UpdateItemCombo(lua_State* L);
	int GetItemComboPreviewData(lua_State* L);
	int UpdateItemComboPreviewData(lua_State* L);
	int GetItemComboProperty(lua_State* L);
	int UpdateItemComboProperty(lua_State* L);
	int GetItemGroup(lua_State* L);
	int GetNameGroup(lua_State* L);*/

	int StatGetAttribute(lua_State* L);
	int StatSetAttribute(lua_State* L);
	//int StatAddCustomDescriptionWrapper(lua_State* L);
	int StatSetLevelScaling(lua_State* L);
	int GetStat(lua_State* L);
	int CreateStat(lua_State* L);
	int SyncStatWrapper(lua_State* L);
	int StatSetPersistenceWrapper(lua_State* L);
	/*int GetDeltaMod(lua_State* L);
	int UpdateDeltaMod(lua_State* L);*/

	int EnumIndexToLabel(lua_State* L);
	int EnumLabelToIndex(lua_State* L);
	/*int NewDamageList(lua_State* L);
	int GetSurfaceTemplate(lua_State* L);*/

}
