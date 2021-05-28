#include <stdafx.h>
#include <ScriptExtender.h>
//#include <GameDefinitions/Ai.h>
#include <GameDefinitions/RootTemplates.h>
#include <Lua/LuaSerializers.h>
#include <Lua/LuaBinding.h>
#include "resource.h"
#include <fstream>
#include <lstate.h>

// Callback from the Lua runtime when a handled (i.e. pcall/xpcall'd) error was thrown.
// This is needed to capture errors for the Lua debugger, as there is no
// built-in mechanism to capture handled errors.
//
// Lua ldebug.c (line ~639) must be patched like this:
//
// void nse_lua_report_handled_error(lua_State* L);
// 
// l_noret luaG_errormsg(lua_State* L) {
// 	if (L->errfunc != 0) {  /* is there an error handling function? */
// 	  nse_lua_report_handled_error(L);
//    StkId errfunc = restorestack(L, L->errfunc);
//
void nse_lua_report_handled_error(lua_State* L)
{
#if !defined(OSI_NO_DEBUGGER)
	char const* err = "(Unknown)";
	if (lua_type(L, -1) == LUA_TSTRING) {
		err = lua_tostring(L, -1);
	}

	auto debugger = bg3se::gExtender->GetLuaDebugger();
	if (debugger) {
		debugger->OnLuaError(L, err);
	}
#endif
}

namespace bg3se::lua::stats
{
	void RestoreLevelMaps(std::unordered_set<int32_t> const&);
}

namespace bg3se::lua
{
	char const* const ObjectProxy::MetatableName = "bg3se::Object";

	RegistryEntry::RegistryEntry()
		: L_(nullptr), ref_(-1)
	{}

	RegistryEntry::RegistryEntry(lua_State * L, int index)
		: L_(L)
	{
		lua_pushvalue(L, index);
		ref_ = luaL_ref(L, LUA_REGISTRYINDEX);
	}

	RegistryEntry::~RegistryEntry()
	{
		if (ref_ != -1) {
			luaL_unref(L_, LUA_REGISTRYINDEX, ref_);
		}
	}

	RegistryEntry::RegistryEntry(RegistryEntry && other)
		: L_(other.L_), ref_(other.ref_)
	{
		other.ref_ = -1;
	}

	RegistryEntry & RegistryEntry::operator = (RegistryEntry && other)
	{
		L_ = other.L_;
		ref_ = other.ref_;
		other.ref_ = -1;
		return *this;
	}

	void RegistryEntry::Push() const
	{
		assert(ref_ != -1);
		lua_rawgeti(L_, LUA_REGISTRYINDEX, ref_);
	}


	int TracebackHandler(lua_State * L)
	{
		const char *msg = lua_tostring(L, 1);
		if (msg == NULL) {  /* is error object not a string? */
			if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
				lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
				return 1;  /* that is the message */
			else
				msg = lua_pushfstring(L, "(error object is a %s value)",
					luaL_typename(L, 1));
		}
		luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
		return 1;  /* return the traceback */
	}

	int CallWithTraceback(lua_State * L, int narg, int nres)
	{
		int base = lua_gettop(L) - narg;  /* function index */
		lua_pushcfunction(L, &TracebackHandler);  /* push message handler */
		lua_insert(L, base);  /* put it under function and args */
		int status = lua_pcall(L, narg, nres, base);
		lua_remove(L, base);  /* remove message handler from the stack */
		return status;
	}

	// Calls Lua function.
	// Function and arguments must be already pushed to the Lua stack.
	// Returns false if call failed, true tuple otherwise.
	// Function name only needed for error reporting purposes
	bool CheckedCall(lua_State* L, int numArgs, char const* functionName)
	{
		if (CallWithTraceback(L, numArgs, 0) != 0) { // stack: errmsg
			ERR("%s Lua call failed: %s", functionName, lua_tostring(L, -1));
			lua_pop(L, 1);
			return false;
		}

		return true;
	}

	void PushExtFunction(lua_State * L, char const * func)
	{
		lua_getglobal(L, "Ext"); // stack: Ext
		lua_getfield(L, -1, func); // stack: Ext, fn
		lua_remove(L, -2); // stack: fn
	}


	void PushInternalFunction(lua_State* L, char const* func)
	{
		lua_getglobal(L, "Ext"); // stack: Ext
		lua_getfield(L, -1, "_Internal"); // stack: Ext, _I
		lua_remove(L, -2); // stack: _I
		lua_getfield(L, -1, func); // stack: _I, fn
		lua_remove(L, -2); // stack: fn
	}


	void PushModFunction(lua_State* L, char const* mod, char const* func)
	{
		lua_getglobal(L, "Mods"); // stack: Mods
		lua_getfield(L, -1, mod); // stack: Mods, mod
		lua_remove(L, -2); // stack: mod
		lua_getfield(L, -1, func); // stack: mod, fn
		lua_remove(L, -2); // stack: fn
	}



/*


	char const* const ObjectProxy<eoc::AiGrid>::MetatableName = "eoc::AiGrid";

	eoc::AiGrid* ObjectProxy<eoc::AiGrid>::Get(lua_State* L)
	{
		return obj_;
	}

	int AiGridSearchForCell(lua_State* L)
	{
		auto grid = ObjectProxy<eoc::AiGrid>::CheckedGet(L, 1);
		auto x = checked_get<float>(L, 2);
		auto z = checked_get<float>(L, 3);
		auto radius = checked_get<float>(L, 4);
		auto flags = checked_get_flags<ESurfaceFlag>(L, 5);
		auto bias = checked_get<float>(L, 6);

		auto search = GetStaticSymbols().eoc__AiGrid__SearchForCell;
		if (!search) {
			OsiError("AiGrid::SearchForCell not mapped!");
			return 0;
		}

		auto result = search(grid, x, z, radius, flags, nullptr, bias);
		push(L, !result);
		return 1;
	}

	int AiGridGetCellInfo(lua_State* L)
	{
		auto grid = ObjectProxy<eoc::AiGrid>::CheckedGet(L, 1);
		auto x = checked_get<float>(L, 2);
		auto z = checked_get<float>(L, 3);

		auto cell = grid->GetCell(glm::vec2(x, z));
		if (!cell) {
			OsiError("Could not find AiGrid cell at " << x << ";" << z);
			return 0;
		}

		auto groundIdx = grid->GetSurfaceIndex(cell, 0);
		auto cloudIdx = grid->GetSurfaceIndex(cell, 1);
		auto meta = grid->GetAiMetaData(cell);

		auto height = cell->Height * 0.25f + grid->DataGrid.OffsetY;

		lua_newtable(L);
		settable(L, "Flags", cell->AiFlags);
		settable(L, "Height", height);

		lua_newtable(L);
		if (meta != nullptr) {
			int32_t aiIdx = 1;
			for (auto ai : meta->Ai) {
				ObjectHandle handle;
				ai->GameObject->GetObjectHandle(handle);
				settable(L, aiIdx++, handle);
			}
		}

		lua_setfield(L, -2, "Objects");

		if (gExtender->IsInServerThread()) {
			auto level = GetStaticSymbols().GetCurrentServerLevel();
			if (!level || level->AiGrid != grid || !level->SurfaceManager) {
				OsiError("Current level not available yet!");
			} else {
				if (groundIdx != -1) {
					auto surface = level->SurfaceManager->Surfaces[groundIdx];
					settable(L, "GroundSurface", surface->MyHandle);
				}

				if (cloudIdx != -1) {
					auto surface = level->SurfaceManager->Surfaces[cloudIdx];
					settable(L, "CloudSurface", surface->MyHandle);
				}
			}
		}

		return 1;
	}

	int AiGridGetAiFlags(lua_State* L)
	{
		auto grid = ObjectProxy<eoc::AiGrid>::CheckedGet(L, 1);
		auto x = checked_get<float>(L, 2);
		auto z = checked_get<float>(L, 3);

		auto cell = grid->GetCell(glm::vec2(x, z));
		if (!cell) {
			OsiError("Could not find AiGrid cell at " << x << ";" << z);
			return 0;
		}

		push(L, cell->AiFlags);
		return 1;
	}

	int AiGridSetAiFlags(lua_State* L)
	{
		// Only allow updating Walkable/Reachable flags for now
		constexpr uint64_t UpdateFlags = 5;

		auto grid = ObjectProxy<eoc::AiGrid>::CheckedGet(L, 1);
		auto x = checked_get<float>(L, 2);
		auto z = checked_get<float>(L, 3);
		auto flags = checked_get<uint64_t>(L, 4);

		auto cell = grid->GetCell(glm::vec2(x, z));
		if (!cell) {
			OsiError("Could not find AiGrid cell at " << x << ";" << z);
			return 0;
		}

		cell->AiFlags = (flags & UpdateFlags) | (cell->AiFlags & ~UpdateFlags);
		return 0;
	}

	int ObjectProxy<eoc::AiGrid>::Index(lua_State* L)
	{
		auto grid = Get(L);
		if (!grid) return 0;

		auto prop = checked_get<FixedString>(L, 2);

		if (prop == GFS.strSearchForCell) {
			lua_pushcfunction(L, &AiGridSearchForCell);
			return 1;
		}

		if (prop == GFS.strGetCellInfo) {
			lua_pushcfunction(L, &AiGridGetCellInfo);
			return 1;
		}

		if (prop == GFS.strGetAiFlags) {
			lua_pushcfunction(L, &AiGridGetAiFlags);
			return 1;
		}

		if (prop == GFS.strSetAiFlags) {
			lua_pushcfunction(L, &AiGridSetAiFlags);
			return 1;
		}

		if (prop == GFS.strOffsetX) {
			push(L, grid->DataGrid.OffsetX);
			return 1;
		}

		if (prop == GFS.strOffsetY) {
			push(L, grid->DataGrid.OffsetY);
			return 1;
		}

		if (prop == GFS.strOffsetZ) {
			push(L, grid->DataGrid.OffsetZ);
			return 1;
		}

		if (prop == GFS.strGridScale) {
			push(L, grid->DataGrid.GridScale);
			return 1;
		}

		if (prop == GFS.strWidth) {
			push(L, (double)grid->DataGrid.Width * grid->DataGrid.GridScale);
			return 1;
		}

		if (prop == GFS.strHeight) {
			push(L, (double)grid->DataGrid.Height * grid->DataGrid.GridScale);
			return 1;
		}

		OsiError("Unknown eoc::AiGrid property: " << prop);
		return 0;
	}

	int ObjectProxy<eoc::AiGrid>::NewIndex(lua_State* L)
	{
		return luaL_error(L, "__newindex not supported for eoc::AiGrid");
	}*/


	/*char const * const DamageList::MetatableName = "CDamageList";

	void DamageList::PopulateMetatable(lua_State * L)
	{
		lua_newtable(L);

		lua_pushcfunction(L, &GetByType);
		lua_setfield(L, -2, "GetByType");

		lua_pushcfunction(L, &Add);
		lua_setfield(L, -2, "Add");

		lua_pushcfunction(L, &Clear);
		lua_setfield(L, -2, "Clear");

		lua_pushcfunction(L, &Multiply);
		lua_setfield(L, -2, "Multiply");

		lua_pushcfunction(L, &Merge);
		lua_setfield(L, -2, "Merge");

		lua_pushcfunction(L, &ConvertDamageType);
		lua_setfield(L, -2, "ConvertDamageType");

		lua_pushcfunction(L, &AggregateSameTypeDamages);
		lua_setfield(L, -2, "AggregateSameTypeDamages");

		lua_pushcfunction(L, &ToTable);
		lua_setfield(L, -2, "ToTable");

		lua_setfield(L, -2, "__index");
	}

	int DamageList::GetByType(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);
		auto damageType = checked_get<DamageType>(L, 2);

		int32_t amount = 0;
		for (auto const& dmg : self->damages_) {
			if (dmg.DamageType == damageType) {
				amount += dmg.Amount;
			}
		}

		push(L, amount);
		return 1;
	}

	int DamageList::Add(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);
		auto damageType = checked_get<DamageType>(L, 2);
		auto amount = (int32_t)luaL_checkinteger(L, 3);

		self->damages_.AddDamage(damageType, amount);

		return 0;
	}

	int DamageList::Clear(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);
		if (lua_gettop(L) >= 2) {
			auto damageType = checked_get<DamageType>(L, 2);
			self->damages_.ClearDamage(damageType);
		} else {
			self->damages_.Clear();
		}

		return 0;
	}

	int DamageList::Multiply(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);
		auto multiplier = luaL_checknumber(L, 2);

		for (auto& dmg : self->damages_) {
			dmg.Amount = (int32_t)round(dmg.Amount * multiplier);
		}

		return 0;
	}

	int DamageList::Merge(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);
		auto other = DamageList::CheckUserData(L, 2);

		for (auto const& dmg : other->damages_) {
			self->damages_.AddDamage(dmg.DamageType, dmg.Amount);
		}

		return 0;
	}

	int DamageList::ConvertDamageType(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);
		auto damageType = checked_get<DamageType>(L, 2);

		int32_t totalDamage = 0;
		for (auto const& dmg : self->damages_) {
			totalDamage += dmg.Amount;
		}

		self->damages_.Clear();
		self->damages_.AddDamage(damageType, totalDamage);

		return 0;
	}

	int DamageList::AggregateSameTypeDamages(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);

		for (uint32_t i = self->damages_.Size; i > 0; i--) {
			auto & src = self->damages_[i - 1];
			for (uint32_t j = i - 1; j > 0; j--) {
				auto & dest = self->damages_[j - 1];
				if (src.DamageType == dest.DamageType) {
					dest.Amount += src.Amount;
					self->damages_.Remove(i - 1);
					break;
				}
			}
		}

		return 0;
	}

	int DamageList::ToTable(lua_State * L)
	{
		auto self = DamageList::CheckUserData(L, 1);

		lua_newtable(L); // Stack: tab

		for (uint32_t i = 0; i < self->damages_.Size; i++) {
			auto const & item = self->damages_[i];

			push(L, i + 1); // Stack: tab, index
			lua_newtable(L); // Stack: tab, index, dmgTab
			setfield(L, "DamageType", item.DamageType);
			setfield(L, "Amount", item.Amount);

			lua_settable(L, -3); // Stack: tab
		}

		return 1;
	}*/


	void ExtensionLibrary::Register(lua_State * L)
	{
		RegisterLib(L);
		/*ObjectProxy<CharacterTemplate>::RegisterMetatable(L);
		ObjectProxy<ItemTemplate>::RegisterMetatable(L);
		ObjectProxy<ProjectileTemplate>::RegisterMetatable(L);
		ObjectProxy<CombatComponentTemplate>::RegisterMetatable(L);
		ObjectProxy<SurfaceTemplate>::RegisterMetatable(L);
		ObjectProxy<eoc::AiGrid>::RegisterMetatable(L);
		DamageList::RegisterMetatable(L);*/
	}


#if LUA_VERSION_NUM > 501
	// Unsafe libs (IO, OS, package, debug) removed
	static const luaL_Reg loadedlibs[] = {
	  {"_G", luaopen_base},
	  {LUA_TABLIBNAME, luaopen_table},
	  {LUA_STRLIBNAME, luaopen_string},
	  {LUA_MATHLIBNAME, luaopen_math},
	  // debug table is stripped in the sandbox startup script
	  {LUA_DBLIBNAME, luaopen_debug},
	  {NULL, NULL}
	};

#else
	// Unsafe libs (IO, OS, package, debug) removed
	static const luaL_Reg loadedlibs[] = {
	  {"", luaopen_base},
	  {LUA_TABLIBNAME, luaopen_table},
	  {LUA_STRLIBNAME, luaopen_string},
	  {LUA_MATHLIBNAME, luaopen_math},
	  {LUA_BITLIBNAME, luaopen_bit},
	  // debug table is stripped in the sandbox startup script
	  {LUA_DBLIBNAME, luaopen_debug},
	  {NULL, NULL}
	};

	static const luaL_Reg lj_lib_preload[] = {
	#if LJ_HASFFI
	  { LUA_FFILIBNAME,	luaopen_ffi },
	#endif
	  { NULL,		NULL }
	};

#endif

	void RegisterLib(lua_State* L, char const* name, luaL_Reg const * lib)
	{
		lua_getglobal(L, "Ext"); // stack: Ext
		lua_createtable(L, 0, 0); // stack: ext, lib
		luaL_setfuncs(L, lib, 0);
		lua_setfield(L, -2, name);
		lua_pop(L, 1);
	}

	int LuaPanic(lua_State * L)
	{
		char const* err = "(Unknown)";
		if (lua_type(L, -1) == LUA_TSTRING) {
			err = lua_tostring(L, -1);
		}

#if !defined(OSI_NO_DEBUGGER)
		auto debugger = gExtender->GetLuaDebugger();
		if (debugger) {
			debugger->OnLuaError(L, err);
		}
#endif

		throw Exception(err);
	}

	void* LuaAlloc(void* ud, void* ptr, size_t osize, size_t nsize)
	{
		(void)ud; (void)osize;  /* not used */
		if (nsize == 0) {
			GameFree(ptr);
			return NULL;
		} else {
			auto newBuf = GameAllocRaw(nsize);
			if (ptr != nullptr) {
				memcpy(newBuf, ptr, std::min(nsize, osize));
				GameFree(ptr);
			}

			return newBuf;
		}
	}


	State::State()
		: lifetimeStack_(lifetimePool_)
	{
		L = lua_newstate(LuaAlloc, this);
#if LUA_VERSION_NUM <= 501
		luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
#endif
		lua_atpanic(L, &LuaPanic);
		OpenLibs();
	}

	State::~State()
	{
		stats::RestoreLevelMaps(OverriddenLevelMaps);
		lua_close(L);
	}

	LifetimeHolder State::GetCurrentLifetime()
	{
		if (lifetimeStack_.IsEmpty()) {
			OsiErrorS("Attempted to construct Lua object proxy while lifetime stack is empty");
			return LifetimeHolder(lifetimePool_, nullptr);
		} else {
			return lifetimeStack_.GetCurrent();
		}
	}

	void State::LoadBootstrap(STDString const& path, STDString const& modTable)
	{
		CallExt("_LoadBootstrap", RestrictAll, path, modTable);
	}

	void State::FinishStartup()
	{
		assert(!startupDone_);
		startupDone_ = true;

#if !defined(OSI_NO_DEBUGGER)
		auto debugger = gExtender->GetLuaDebugMessageHandler();
		if (debugger && debugger->IsDebuggerReady()) {
			debugger->SendModInfo();
		}
#endif
	}
		
	void State::OpenLibs()
	{
#if LUA_VERSION_NUM > 501
		const luaL_Reg *lib;
		/* "require" functions from 'loadedlibs' and set results to global table */
		for (lib = loadedlibs; lib->func; lib++) {
			luaL_requiref(L, lib->name, lib->func, 1);
			lua_pop(L, 1);  /* remove lib */
		}
#else
		const luaL_Reg* lib;
		for (lib = loadedlibs; lib->func; lib++) {
			lua_pushcfunction(L, lib->func);
			lua_pushstring(L, lib->name);
			lua_call(L, 1, 0);
		}
		luaL_findtable(L, LUA_REGISTRYINDEX, "_PRELOAD",
			sizeof(lj_lib_preload) / sizeof(lj_lib_preload[0]) - 1);
		for (lib = lj_lib_preload; lib->func; lib++) {
			lua_pushcfunction(L, lib->func);
			lua_setfield(L, -2, lib->name);
		}
		lua_pop(L, 1);
#endif
	}

	std::optional<int> State::LoadScript(STDString const & script, STDString const & name, int globalsIdx)
	{
		int top = lua_gettop(L);

		/* Load the file containing the script we are going to run */
		int status = luaL_loadbufferx(L, script.c_str(), script.size(), name.c_str(), "text");
		if (status != LUA_OK) {
			LuaError("Failed to parse script: " << lua_tostring(L, -1));
			lua_pop(L, 1);  /* pop error message from the stack */
			return {};
		}

#if LUA_VERSION_NUM <= 501
		if (globalsIdx != 0) {
			lua_pushvalue(L, globalsIdx);
			lua_setfenv(L, -2);
		}
#endif

		/* Ask Lua to run our little script */
		LifetimePin _(lifetimeStack_);
		status = CallWithTraceback(L, 0, LUA_MULTRET);
		if (status != LUA_OK) {
			LuaError("Failed to execute script: " << lua_tostring(L, -1));
			lua_pop(L, 1); // pop error message from the stack
			return {};
		}

		return lua_gettop(L) - top;
	}

	/*std::optional<int32_t> State::GetHitChance(CDivinityStats_Character * attacker, CDivinityStats_Character * target)
	{
		Restriction restriction(*this, RestrictAll);

		PushInternalFunction(L, "_GetHitChance"); // stack: fn
		auto _{ PushArguments(L,
			std::tuple{Push<ObjectProxy<CDivinityStats_Character>>(attacker),
			Push<ObjectProxy<CDivinityStats_Character>>(target)}) };

		auto result = CheckedLifetimeCall<std::optional<int32_t>>(2, "Ext.GetHitChance");
		if (result) {
			return std::get<0>(*result);
		} else {
			return {};
		}
	}

	bool State::GetSkillDamage(SkillPrototype * skill, DamagePairList * damageList,
		CRPGStats_ObjectInstance *attacker, bool isFromItem, bool stealthed, float * attackerPosition,
		float * targetPosition, DeathType * pDeathType, int level, bool noRandomization)
	{
		Restriction restriction(*this, RestrictAll);

		PushInternalFunction(L, "_GetSkillDamage"); // stack: fn

		auto luaSkill = SkillPrototypeProxy::New(L, skill, -1); // stack: fn, skill
		UnbindablePin _(luaSkill);
		ItemOrCharacterPushPin _a(L, attacker);

		push(L, isFromItem);
		push(L, stealthed);
		
		// Push attacker position
		lua_newtable(L);
		settable(L, 1, attackerPosition[0]);
		settable(L, 2, attackerPosition[1]);
		settable(L, 3, attackerPosition[2]);

		// Push target position
		lua_newtable(L);
		settable(L, 1, targetPosition[0]);
		settable(L, 2, targetPosition[1]);
		settable(L, 3, targetPosition[2]);

		push(L, level);
		push(L, noRandomization);

		auto result = CheckedLifetimeCall<std::optional<DeathType>, std::optional<DamageList *>>(8, "Ext.GetSkillDamage");
		if (result) {
			auto deathType = std::get<0>(*result);
			auto damages = std::get<1>(*result);

			if (deathType && damages) {
				if (pDeathType) {
					*pDeathType = *deathType;
				}

				for (auto const& dmg : (*damages)->Get()) {
					damageList->AddDamage(dmg.DamageType, dmg.Amount);
				}

				return true;
			}
		}

		return false;
	}

	std::optional<std::pair<int, bool>> State::GetSkillAPCost(SkillPrototype* skill, CDivinityStats_Character* character, eoc::AiGrid* aiGrid,
		glm::vec3* position, float* radius)
	{
		Restriction restriction(*this, RestrictAll);

		PushInternalFunction(L, "_GetSkillAPCost");

		auto luaSkill = SkillPrototypeProxy::New(L, skill, -1);
		UnbindablePin _(luaSkill);
		ItemOrCharacterPushPin _ch(L, character);

		if (aiGrid != nullptr) {
			ObjectProxy<eoc::AiGrid>::New(L, aiGrid);
		} else {
			push(L, nullptr);
		}

		if (position != nullptr) {
			push(L, *position);
		} else {
			push(L, nullptr);
		}

		if (radius != nullptr) {
			push(L, *radius);
		} else {
			push(L, nullptr);
		}

		auto result = CheckedLifetimeCall<std::optional<bool>, std::optional<int>>(5, "Ext.GetSkillAPCost");
		if (result) {
			auto ap = std::get<1>(*result);
			auto elementalAffinity = std::get<0>(*result);

			if (ap && elementalAffinity) {
				return std::make_pair(*ap, *elementalAffinity);
			}
		}

		return {};
	}*/

	/*void State::OnNetMessageReceived(STDString const & channel, STDString const & payload, UserId userId)
	{
		CallExt("_NetMessageReceived", 0, ReturnType<>{}, channel, payload, userId.Id);
	}*/

	void State::OnGameSessionLoading()
	{
		CallExt("_OnGameSessionLoading", RestrictAll | ScopeSessionLoad);
	}

	void State::OnGameSessionLoaded()
	{
		CallExt("_OnGameSessionLoaded", RestrictAll);
	}

	void State::OnModuleLoadStarted()
	{
		CallExt("_OnModuleLoadStarted", RestrictAll | ScopeModulePreLoad);
	}

	void State::OnModuleLoading()
	{
		CallExt("_OnModuleLoading", RestrictAll | ScopeModuleLoad);
	}

	void State::OnStatsLoaded()
	{
		CallExt("_OnStatsLoaded", RestrictAll | ScopeModuleLoad);
	}

	void State::OnModuleResume()
	{
		CallExt("_OnModuleResume", RestrictAll | ScopeModuleResume);
	}

	void State::OnResetCompleted()
	{
		CallExt("_OnResetCompleted", 0);
	}

	STDString State::GetBuiltinLibrary(int resourceId)
	{
		auto resource = GetResource(resourceId);
		if (resource) {
			return STDString(resource->c_str());
		} else {
			return STDString();
		}
	}

	State* State::FromLua(lua_State* L)
	{
		return reinterpret_cast<State*>(L->l_G->ud);
	}
}
