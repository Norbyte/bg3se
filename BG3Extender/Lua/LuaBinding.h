#pragma once

#include <Lua/LuaHelpers.h>

#include <mutex>
#include <unordered_set>
#include <optional>

namespace bg3se
{
	struct CRPGStats_Object;
}

namespace bg3se::lua
{
	void PushExtFunction(lua_State * L, char const * func);
	void PushInternalFunction(lua_State * L, char const * func);
	void PushModFunction(lua_State* L, char const* mod, char const* func);

	template <class T>
	class LuaPropertyMap
	{
	public:
		struct PropertyAccessors
		{
			using Getter = bool(lua_State* L, T* object);
			using Setter = bool (lua_State* L, T* object, int index);

			Getter* Get;
			Setter* Set;
		};

		bool GetProperty(lua_State* L, T* object, STDString const& prop) const
		{
			auto it = Properties.find(prop);
			if (it == Properties.end()) {
				return 0;
			}

			return it->second.Get(L, object);
		}

		bool SetProperty(lua_State* L, T* object, STDString const& prop, int index) const
		{
			auto it = Properties.find(prop);
			if (it == Properties.end()) {
				return 0;
			}

			return it->second.Set(L, object, index);
		}

		void AddProperty(STDString const& prop, typename PropertyAccessors::Getter* getter, typename PropertyAccessors::Setter* setter)
		{
			Properties.insert(std::make_pair(prop, PropertyAccessors{ getter, setter }));
		}

		std::unordered_map<STDString, PropertyAccessors> Properties;
	};

	template <class T>
	struct StaticLuaPropertyMap
	{
		using ObjectType = T;

		static LuaPropertyMap<T> PropertyMap;
	};

	template <class T>
	class ObjectProxy2 : public Userdata<ObjectProxy2<T>>, public Indexable, public NewIndexable, 
		public Iterable, public Pushable<PushPolicy::Unbind>
	{
	public:
		static_assert(!std::is_pointer_v<T>, "ObjectProxy template parameter should not be a pointer type!");

		static char const * const MetatableName;

		ObjectProxy2(T * obj) : obj_(obj)
		{}

		void Unbind()
		{
			obj_ = nullptr;
		}

		T* Get(lua_State* L) const
		{
			return obj_;
		}

		int Index(lua_State* L)
		{
			StackCheck _(L, 1);
			if (!obj_) {
				push(L, nullptr);
				return 1;
			}

			auto prop = luaL_checkstring(L, 2);
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto fetched = map.GetProperty(L, obj_, prop);
			if (!fetched) {
				luaL_error(L, "Object of type '%s' has no property named '%s'", MetatableName, prop);
				push(L, nullptr);
			}

			return 1;
		}

		int NewIndex(lua_State* L)
		{
			StackCheck _(L, 0);
			if (!obj_) return 0;

			auto prop = luaL_checkstring(L, 2);
			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			auto ok = map.SetProperty(L, obj_, prop, 3);
			if (!ok) {
				luaL_error(L, "Object of type '%s' has no property named '%s'", MetatableName, prop);
			}

			return 0;
		}

		static T* CheckedGet(lua_State* L, int index)
		{
			auto self = CheckUserData(L, index);
			auto obj = self->Get(L);
			if (obj != nullptr) {
				return obj;
			} else {
				luaL_error(L, "No object bound to object proxy of type '%s'", MetatableName);
				return nullptr;
			}
		}

	private:
		T * obj_;

	protected:
		friend Userdata<ObjectProxy2<T>>;

		int Next(lua_State* L)
		{
			if (!obj_) return 0;

			auto const& map = StaticLuaPropertyMap<T>::PropertyMap;
			if (lua_type(L, 2) == LUA_TNIL) {
				if (!map.Properties.empty()) {
					StackCheck _(L, 2);
					auto it = map.Properties.begin();
					push(L, it->first);
					if (!it->second.Get(L, obj_)) {
						push(L, nullptr);
					}

					return 2;
				}
			} else {
				auto key = checked_get<char const*>(L, 2);
				auto it = map.Properties.find(key);
				if (it != map.Properties.end()) {
					++it;
					if (it != map.Properties.end()) {
						StackCheck _(L, 2);
						push(L, it->first);
						if (!it->second.Get(L, obj_)) {
							push(L, nullptr);
						}

						return 2;
					}
				}
			}

			return 0;
		}
	};


	class ExtensionLibrary
	{
	public:
		virtual void Register(lua_State * L);
		virtual void RegisterLib(lua_State * L) = 0;
	};

	class Exception : public std::runtime_error
	{
	public:
		Exception(std::string const& msg)
			: std::runtime_error("Lua error thrown: " + msg)
		{}
	};

	class State : Noncopyable<State>
	{
	public:
		enum RestrictionFlag
		{
			// Disable Osiris calls
			RestrictOsiris = 1 << 0,
			// Disable handle/guid to object conversion functions (Lua only)
			RestrictHandleConversion = 1 << 1,

			// Permit calls only available during module load state
			ScopeModuleLoad = 1 << 16,
			// Permit calls only available during session load state
			ScopeSessionLoad = 1 << 17,
			// Permit calls only available during module resume state
			ScopeModuleResume = 1 << 18,
			// Permit calls only available during initial module load state (before loading any files)
			ScopeModulePreLoad = 1 << 19,

			RestrictAll = 0x0000ffff,
		};

		uint32_t RestrictionFlags{ 0 };
		std::unordered_set<int32_t> OverriddenLevelMaps;

		State();
		~State();

		State(State const &) = delete;
		State(State &&) = delete;
		State & operator = (State const &) = delete;
		State & operator = (State &&) = delete;

		inline lua_State * GetState()
		{
			return L;
		}

		inline bool StartupDone() const
		{
			return startupDone_;
		}

		void FinishStartup();
		void LoadBootstrap(STDString const& path, STDString const& modTable);
		virtual void OnGameSessionLoading();
		void OnGameSessionLoaded();
		void OnModuleLoadStarted();
		void OnModuleLoading();
		void OnStatsLoaded();
		void OnModuleResume();
		void OnResetCompleted();

		template <class... Ret, class... Args>
		auto CallExt(char const * func, uint32_t restrictions, ReturnType<Ret...>, Args... args)
		{
			Restriction restriction(*this, restrictions);
			PushInternalFunction(L, func);
			auto _{ PushArguments(L, std::tuple{args...}) };
			return CheckedCall<Ret...>(L, sizeof...(args), func);
		}

		std::optional<int> LoadScript(STDString const & script, STDString const & name = "", int globalsIdx = 0);

		/*std::optional<int32_t> GetHitChance(CDivinityStats_Character * attacker, CDivinityStats_Character * target);
		bool GetSkillDamage(SkillPrototype * self, DamagePairList * damageList,
			CRPGStats_ObjectInstance *attackerStats, bool isFromItem, bool stealthed, float * attackerPosition,
			float * targetPosition, DeathType * pDeathType, int level, bool noRandomization);
		std::optional<std::pair<int, bool>> GetSkillAPCost(SkillPrototype* skill, CDivinityStats_Character* character, eoc::AiGrid* aiGrid,
			glm::vec3* position, float* radius);
		void OnNetMessageReceived(STDString const & channel, STDString const & payload, UserId userId);*/

		static STDString GetBuiltinLibrary(int resourceId);

	protected:
		lua_State * L;
		bool startupDone_{ false };

		void OpenLibs();
	};

	class Restriction
	{
	public:
		inline Restriction(State & state, uint32_t flags)
			: state_(state), oldFlags_(state_.RestrictionFlags)
		{
			state_.RestrictionFlags |= flags;
		}

		inline ~Restriction()
		{
			state_.RestrictionFlags = oldFlags_;
		}

	private:
		State & state_;
		uint32_t oldFlags_;
	};

	int GetCellInfo(lua_State* L);

	/*int NewDamageList(lua_State* L);
	int GetSurfaceTemplate(lua_State* L);*/
}

namespace bg3se::lua::stats
{
	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, int valueIdx);
}