#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Lua/LuaBinding.h>
#include <fstream>


namespace bg3se::esv::lua
{
	using namespace bg3se::lua;

	ValueType GetBaseType(ValueType type)
	{
		return (*gExtender->GetServer().Osiris().GetGlobals().Types)->ResolveAlias((uint16_t)type);
	}

	int64_t LuaToInt(lua_State* L, int i, int type)
	{
		if (type == LUA_TNUMBER) {
#if LUA_VERSION_NUM > 501
			if (lua_isinteger(L, i)) {
				return (int64_t)lua_tointeger(L, i);
			} else {
				return (int64_t)lua_tonumber(L, i);
			}
#else
			return (int64_t)lua_tonumber(L, i);
#endif
		} else if (type == LUA_TLIGHTUSERDATA) {
			auto handle = get<ComponentHandle>(L, i);
			return (int64_t)handle.Handle;
		} else {
			luaL_error(L, "Number expected for argument %d, got %s", i, lua_typename(L, type));
		}

		return 0;
	}

	char * LuaToString(lua_State* L, int i, int type, char* reuseString)
	{
		if (type == LUA_TSTRING) {
			if (reuseString != nullptr) {
				size_t len;
				auto s = lua_tolstring(L, i, &len);
				strncpy_s(reuseString, 0x100, s, len);
				return reuseString;
			} else {
				// TODO - not sure if we're the owners of the string or the TypedValue is
				return _strdup(lua_tostring(L, i));
			}
		} else {
			luaL_error(L, "String expected for argument %d, got %s", i, lua_typename(L, type));
		}

		return nullptr;
	}

	void LuaToOsi(lua_State * L, int i, TypedValue & tv, ValueType osiType, bool allowNil)
	{
		tv.VMT = gExtender->GetServer().Osiris().GetGlobals().TypedValueVMT;
		tv.TypeId = (uint32_t)osiType;

		auto type = lua_type(L, i);
		if (allowNil && type == LUA_TNIL) {
			tv.TypeId = (uint32_t)ValueType::None;
			return;
		}

		switch (GetBaseType(osiType)) {
		case ValueType::Integer:
			tv.Value.Val.Int32 = (int32_t)LuaToInt(L, i, type);
			break;

		case ValueType::Integer64:
			tv.Value.Val.Int64 = (int64_t)LuaToInt(L, i, type);
			break;

		case ValueType::Real:
			if (type != LUA_TNUMBER) {
				luaL_error(L, "Number expected for argument %d, got %s", i, lua_typename(L, type));
			}

#if LUA_VERSION_NUM > 501
			if (lua_isinteger(L, i)) {
				tv.Value.Val.Float = (float)lua_tointeger(L, i);
			} else {
				tv.Value.Val.Float = (float)lua_tonumber(L, i);
			}
#else
			tv.Value.Val.Float = (float)lua_tonumber(L, i);
#endif
			break;

		case ValueType::String:
		case ValueType::GuidString:
			tv.Value.Val.String = LuaToString(L, i, type, nullptr);
			break;

		default:
			luaL_error(L, "Unhandled Osi argument type %d", osiType);
			break;
		}
	}

	TypedValue * LuaToOsi(lua_State * L, int i, ValueType osiType, bool allowNil)
	{
		auto tv = new TypedValue();
		LuaToOsi(L, i, *tv, osiType, allowNil);
		return tv;
	}

	void LuaToOsi(lua_State * L, int i, OsiArgumentValue & arg, ValueType osiType, bool allowNil, bool reuseStrings)
	{
		arg.TypeId = osiType;
		auto type = lua_type(L, i);
		if (allowNil && type == LUA_TNIL) {
			arg.TypeId = ValueType::None;
			return;
		}

		switch (GetBaseType(osiType)) {
		case ValueType::Integer:
			arg.Int32 = (int32_t)LuaToInt(L, i, type);
			break;

		case ValueType::Integer64:
			arg.Int64 = (int64_t)LuaToInt(L, i, type);
			break;

		case ValueType::Real:
			if (type != LUA_TNUMBER) {
				luaL_error(L, "Number expected for argument %d, got %s", i, lua_typename(L, type));
			}

#if LUA_VERSION_NUM > 501
			if (lua_isinteger(L, i)) {
				arg.Float = (float)lua_tointeger(L, i);
			} else {
				arg.Float = (float)lua_tonumber(L, i);
			}
#else
			arg.Float = (float)lua_tonumber(L, i);
#endif
			break;

		case ValueType::String:
		case ValueType::GuidString:
			if (reuseStrings) {
				arg.String = LuaToString(L, i, type, const_cast<char*>(arg.String));
			} else {
				arg.String = LuaToString(L, i, type, nullptr);
			}
			break;

		default:
			luaL_error(L, "Unhandled Osi argument type %d", osiType);
			break;
		}
	}

	void OsiToLua(lua_State * L, OsiArgumentValue const & arg)
	{
		switch (GetBaseType(arg.TypeId)) {
		case ValueType::None:
			lua_pushnil(L);
			break;

		case ValueType::Integer:
			push(L, arg.Int32);
			break;

		case ValueType::Integer64:
			push(L, arg.Int64);
			break;

		case ValueType::Real:
			push(L, arg.Float);
			break;

		case ValueType::String:
		case ValueType::GuidString:
			push(L, arg.String);
			break;

		default:
			luaL_error(L, "Unhandled Osi argument type %d", arg.TypeId);
			break;
		}
	}

	void OsiToLua(lua_State * L, TypedValue const & tv)
	{
		switch (GetBaseType((ValueType)tv.TypeId)) {
		case ValueType::None:
			lua_pushnil(L);
			break;

		case ValueType::Integer:
			push(L, tv.Value.Val.Int32);
			break;

		case ValueType::Integer64:
			push(L, tv.Value.Val.Int64);
			break;

		case ValueType::Real:
			push(L, tv.Value.Val.Float);
			break;

		case ValueType::String:
		case ValueType::GuidString:
			push(L, tv.Value.Val.String);
			break;

		default:
			luaL_error(L, "Unhandled Osi TypedValue type %d", tv.TypeId);
			break;
		}
	}

	uint32_t FunctionNameHash(char const * str)
	{
		uint32_t hash{ 0 };
		while (*str) {
			hash = (*str++ | 0x20) + 129 * (hash % 4294967);
		}

		return hash;
	}

	Function const* LookupOsiFunction(STDString const& name, uint32_t arity)
	{
		auto functions = gExtender->GetServer().Osiris().GetGlobals().Functions;
		if (!functions) {
			return nullptr;
		}

		OsiString sig(name);
		sig += "/";
		sig += std::to_string(arity);

		auto hash = FunctionNameHash(name.c_str()) + arity;
		auto func = (*functions)->Find(hash, sig);
		if (func == nullptr
			|| ((*func)->Node.Id == 0
				&& (*func)->Type != FunctionType::Call
				&& (*func)->Type != FunctionType::Query
				&& (*func)->Type != FunctionType::Event)) {
			return nullptr;
		}

		return *func;
	};


	bool OsiFunction::Bind(Function const * func, ServerState & state)
	{
		if (func->Type == FunctionType::Query
			|| func->Type == FunctionType::SysQuery
			|| func->Type == FunctionType::UserQuery
			// User queries appear in the function table using the 'Database' type,
			// however the node is a UserQueryNode, not a DatabaseNode.
			// Catch this case by checking if the node is a descendant of a DataNode or not.
			|| (func->Type == FunctionType::Database && func->Node.Get() && !func->Node.Get()->IsDataNode())) {

			auto numArgs = func->Signature->Params->Params.Size;
			auto adapter = state.Osiris().GetIdentityAdapterMap().FindAdapter((uint8_t)numArgs);
			if (adapter == nullptr) {
				OsiError("Couldn't bind query '" << func->Signature->Name 
					<< "': No identity adapter found for arity " << numArgs);
				return false;
			}

			adapter_.Manager = *gExtender->GetServer().Osiris().GetGlobals().Adapters;
			adapter_.Id = adapter->Id;
		}

		function_ = func;
		state_ = &state;
		return true;
	}

	void OsiFunction::Unbind()
	{
		function_ = nullptr;
	}

	int OsiFunction::LuaCall(lua_State * L)
	{
		if (function_ == nullptr) {
			return luaL_error(L, "Attempted to call an unbound Osiris function");
		}

		int numArgs = lua_gettop(L);
		if (numArgs < 1) {
			return luaL_error(L, "Called Osi function without 'self' argument?");
		}

		if (state_->RestrictionFlags & State::RestrictOsiris) {
			return luaL_error(L, "Attempted to call Osiris function in restricted context");
		}

		switch (function_->Type) {
		case FunctionType::Call:
			OsiCall(L);
			return 0;

		case FunctionType::Event:
		case FunctionType::Proc:
			OsiInsert(L, false);
			return 0;

		case FunctionType::Database:
		{
			// User queries appear in the function table using the 'Database' type,
			// however the node is a UserQueryNode, not a DatabaseNode.
			// Catch this case by checking if the node is a descendant of a DataNode or not.
			auto node = function_->Node.Get();
			if (node) {
				if (node->IsDataNode()) {
					OsiInsert(L, false);
					return 0;
				} else {
					return OsiUserQuery(L);
				}
			} else {
				return luaL_error(L, "Function has no node!");
			}
		}

		case FunctionType::Query:
			return OsiQuery(L);

		case FunctionType::SysQuery:
		case FunctionType::UserQuery:
			return OsiUserQuery(L);

		case FunctionType::SysCall:
		default:
			return luaL_error(L, "Cannot call function of type %d", function_->Type);
		}
	}

	int OsiFunction::LuaGet(lua_State * L)
	{
		if (!IsBound()) {
			return luaL_error(L, "Attempted to read an unbound Osiris database");
		}

		if (!IsDB()) {
			return luaL_error(L, "Attempted to read function that's not a database");
		}

		int numArgs = lua_gettop(L);
		if (numArgs < 1) {
			return luaL_error(L, "Read Osi database without 'self' argument?");
		}

		if (state_->RestrictionFlags & State::RestrictOsiris) {
			return luaL_error(L, "Attempted to read Osiris database in restricted context");
		}

		auto db = function_->Node.Get()->Database.Get();
		

		auto head = db->Facts.Head;
		auto current = head->Next;

		lua_newtable(L);
		auto index = 1;
		while (current != head) {
			if (MatchTuple(L, 2, current->Item)) {
				push(L, index++);
				ConstructTuple(L, current->Item);
				lua_settable(L, -3);
			}

			current = current->Next;
		}

		return 1;
	}

	int OsiFunction::LuaDelete(lua_State * L)
	{
		if (!IsBound()) {
			return luaL_error(L, "Attempted to delete from an unbound Osiris database");
		}

		if (!IsDB()) {
			return luaL_error(L, "Attempted to delete from function that's not a database");
		}

		int numArgs = lua_gettop(L);
		if (numArgs < 1) {
			return luaL_error(L, "Delete from Osi database without 'self' argument?");
		}

		if (state_->RestrictionFlags & State::RestrictOsiris) {
			return luaL_error(L, "Attempted to delete from Osiris database in restricted context");
		}

		OsiInsert(L, true);
		return 0;
	}

	int OsiFunction::LuaDeferredNotification(lua_State * L)
	{
		if (function_ == nullptr) {
			return luaL_error(L, "Attempted to call an unbound Osiris function");
		}

		int numArgs = lua_gettop(L);
		if (numArgs < 1) {
			return luaL_error(L, "Called Osi function without 'self' argument?");
		}

		if (state_->RestrictionFlags & State::RestrictOsiris) {
			return luaL_error(L, "Attempted to call Osiris function in restricted context");
		}

		if (function_->Type == FunctionType::Event) {
			OsiDeferredNotification(L);
			return 0;
		} else {
			return luaL_error(L, "Cannot queue deferred events on function of type %d", function_->Type);
		}
	}

	bool OsiFunction::MatchTuple(lua_State * L, int firstIndex, TupleVec const & tuple)
	{
		for (auto i = 0; i < tuple.Size; i++) {
			if (!lua_isnil(L, firstIndex + i)) {
				auto const & v = tuple.Values[i];
				switch ((ValueType)v.TypeId) {
				case ValueType::Integer:
					if (v.Value.Val.Int32 != lua_tointeger(L, firstIndex + i)) {
						return false;
					}
					break;

				case ValueType::Integer64:
					if (v.Value.Val.Int64 != lua_tointeger(L, firstIndex + i)) {
						return false;
					}
					break;

				case ValueType::Real:
					if (abs(v.Value.Val.Float - lua_tonumber(L, firstIndex + i)) > 0.00001f) {
						return false;
					}
					break;

				case ValueType::String:
				{
					auto str = lua_tostring(L, firstIndex + i);
					if (!str || _stricmp(v.Value.Val.String, str) != 0) {
						return false;
					}
					break;
				}

				case ValueType::GuidString:
				{
					auto str = lua_tostring(L, firstIndex + i);
					if (!str) return false;

					auto len = strlen(str);
					auto valueLen = strlen(v.Value.Val.String);
					if (valueLen < 36 || len < 36 || _stricmp(&v.Value.Val.String[valueLen - 36], &str[len - 36]) != 0) {
						return false;
					}
					break;
				}

				default:
					OsiError("Unsupported ValueType for comparison: " << v.TypeId);
					return false;
				}
			}
		}

		return true;
	}

	void OsiFunction::ConstructTuple(lua_State * L, TupleVec const & tuple)
	{
		lua_newtable(L);
		for (auto i = 0; i < tuple.Size; i++) {
			push(L, i + 1);
			OsiToLua(L, tuple.Values[i]);
			lua_settable(L, -3);
		}
	}

	void OsiFunction::OsiCall(lua_State * L)
	{
		auto funcArgs = function_->Signature->Params->Params.Size;
		int numArgs = lua_gettop(L);
		if (numArgs - 1 != funcArgs) {
			luaL_error(L, "Incorrect number of arguments for '%s'; expected %d, got %d",
				function_->Signature->Name, funcArgs, numArgs - 1);
		}

		OsiArgumentListPin<OsiArgumentDesc> args(state_->Osiris().GetArgumentDescPool(), (uint32_t)funcArgs);
		auto argType = function_->Signature->Params->Params.Head->Next;
		for (uint32_t i = 0; i < funcArgs; i++) {
			auto arg = args.Args() + i;
			if (i > 0) {
				args.Args()[i - 1].NextParam = arg;
			}
			LuaToOsi(L, i + 2, arg->Value, (ValueType)argType->Item.Type);
			argType = argType->Next;
		}

		gExtender->GetServer().Osiris().GetWrappers().Call.CallWithHooks(function_->GetHandle(), funcArgs == 0 ? nullptr : args.Args());
	}

	void OsiFunction::OsiDeferredNotification(lua_State * L)
	{
		/*auto funcArgs = function_->Signature->Params->Params.Size;
		int numArgs = lua_gettop(L);
		if (numArgs - 1 != funcArgs) {
			luaL_error(L, "Incorrect number of arguments for '%s'; expected %d, got %d",
				function_->Signature->Name, funcArgs, numArgs - 1);
			return;
		}

		auto story = GetStaticSymbols().GetStoryImplementation();
		if (story == nullptr || story->Manager == nullptr) {
			luaL_error(L, "Called when Osiris is not yet initialized");
			return;
		}

		auto key = function_->Key;
		if (key[0] != 3 || key[1] != 0 || key[2] >= story->Manager->Functions.size() || key[3] != 1) {
			luaL_error(L, "No engine function found with this Osiris function ID");
		}

		auto func = story->Manager->Functions[key[2]];
		if (func->FunctionType != 3) {
			luaL_error(L, "Attempted to defer notification on something that is not an event");
		}

		auto notification = static_cast<osi::OsirisNotification*>(func);

		auto arg = notification->ArgumentDescs;
		for (uint32_t i = 0; i < funcArgs; i++) {
			LuaToOsi(L, i + 2, arg->Value, (ValueType)arg->Value.TypeId, false, true);
			arg = arg->NextParam;
		}

		story->Manager->QueueNotification(notification);*/
		OsiError("FIXME: OsiDeferredNotification not implemented yet!");
	}

	void OsiFunction::OsiInsert(lua_State * L, bool deleteTuple)
	{
		auto funcArgs = function_->Signature->Params->Params.Size;
		int numArgs = lua_gettop(L);
		if (numArgs - 1 != funcArgs) {
			luaL_error(L, "Incorrect number of arguments for '%s'; expected %d, got %d",
				function_->Signature->Name, funcArgs, numArgs - 1);
		}

		if (function_->Node.Id == 0) {
			luaL_error(L, "Function has no node");
		}

		OsiArgumentListPin<TypedValue> tvs(state_->Osiris().GetTypedValuePool(), (uint32_t)funcArgs);
		OsiArgumentListPin<ListNode<TypedValue *>> nodes(state_->Osiris().GetTypedValueNodePool(), (uint32_t)funcArgs + 1);

		TuplePtrLL tuple;
		auto & args = tuple.Items;
		auto argType = function_->Signature->Params->Params.Head->Next;
		args.Init(nodes.Args());

		auto prev = args.Head;
		for (uint32_t i = 0; i < funcArgs; i++) {
			auto tv = tvs.Args() + i;
			LuaToOsi(L, i + 2, *tv, (ValueType)argType->Item.Type, deleteTuple);
			auto node = nodes.Args() + i + 1;
			args.Insert(tv, node, prev);
			prev = node;
			argType = argType->Next;
		}

		auto node = function_->Node.Get();
		if (deleteTuple) {
			node->DeleteTuple(&tuple);
		} else {
			node->InsertTuple(&tuple);
		}
	}

	int OsiFunction::OsiQuery(lua_State * L)
	{
		auto outParams = function_->Signature->OutParamList.numOutParams();
		auto numParams = function_->Signature->Params->Params.Size;
		auto inParams = numParams - outParams;

		int numArgs = lua_gettop(L);
		if (numArgs - 1 != inParams) {
			return luaL_error(L, "Incorrect number of IN arguments for '%s'; expected %d, got %d",
				function_->Signature->Name, inParams, numArgs - 1);
		}

		OsiArgumentListPin<OsiArgumentDesc> args(state_->Osiris().GetArgumentDescPool(), (uint32_t)numParams);
		auto argType = function_->Signature->Params->Params.Head->Next;
		uint32_t inputArg = 2;
		for (uint32_t i = 0; i < numParams; i++) {
			auto arg = args.Args() + i;
			if (i > 0) {
				args.Args()[i - 1].NextParam = arg;
			}

			if (function_->Signature->OutParamList.isOutParam(i)) {
				arg->Value.TypeId = (ValueType)argType->Item.Type;
			} else {
				LuaToOsi(L, inputArg++, arg->Value, (ValueType)argType->Item.Type);
			}

			argType = argType->Next;
		}

		bool handled = gExtender->GetServer().Osiris().GetWrappers().Query.CallWithHooks(function_->GetHandle(), numParams == 0 ? nullptr : args.Args());
		if (outParams == 0) {
			push(L, handled);
			return 1;
		} else {
			if (handled) {
				for (uint32_t i = 0; i < numParams; i++) {
					if (function_->Signature->OutParamList.isOutParam(i)) {
						OsiToLua(L, args.Args()[i].Value);
					}
				}
			} else {
				for (uint32_t i = 0; i < outParams; i++) {
					lua_pushnil(L);
				}
			}

			return outParams;
		}
	}

	int OsiFunction::OsiUserQuery(lua_State * L)
	{
		auto outParams = function_->Signature->OutParamList.numOutParams();
		auto numParams = function_->Signature->Params->Params.Size;
		auto inParams = numParams - outParams;

		int numArgs = lua_gettop(L);
		if (numArgs - 1 != inParams) {
			return luaL_error(L, "Incorrect number of IN arguments for '%s'; expected %d, got %d",
				function_->Signature->Name, inParams, numArgs - 1);
		}

		OsiArgumentListPin<ListNode<TupleLL::Item>> nodes(state_->Osiris().GetTupleNodePool(), (uint32_t)numParams + 1);

		VirtTupleLL tuple;
		
		auto & args = tuple.Data.Items;
		auto argType = function_->Signature->Params->Params.Head->Next;
		args.Init(nodes.Args());

		auto prev = args.Head;
		uint32_t inputArgIndex = 0;
		for (uint32_t i = 0; i < numParams; i++) {
			auto node = nodes.Args() + i + 1;
			args.Insert(node, prev);
			if (!function_->Signature->OutParamList.isOutParam(i)) {
				LuaToOsi(L, inputArgIndex + 2, node->Item.Value, (ValueType)argType->Item.Type);
				inputArgIndex++;
			} else {
				node->Item.Value.VMT = gExtender->GetServer().Osiris().GetGlobals().TypedValueVMT;
				node->Item.Value.TypeId = (uint32_t)ValueType::None;
			}

			prev = node;
			argType = argType->Next;
		}

		auto node = (*gExtender->GetServer().Osiris().GetGlobals().Nodes)->Db.Elements[function_->Node.Id - 1];
		bool valid = node->IsValid(&tuple, &adapter_);
		if (valid) {
			if (outParams > 0) {
				auto retType = function_->Signature->Params->Params.Head->Next;
				auto ret = args.Head->Next;
				for (uint32_t i = 0; i < numParams; i++) {
					if (function_->Signature->OutParamList.isOutParam(i)) {
						OsiToLua(L, ret->Item.Value);
					}

					ret = ret->Next;
					retType = retType->Next;
				}

				return outParams;
			} else {
				push(L, true);
				return 1;
			}
		} else {
			if (outParams > 0) {
				for (uint32_t i = 0; i < outParams; i++) {
					lua_pushnil(L);
				}

				return outParams;
			} else {
				push(L, false);
				return 1;
			}
		}
	}



	char const * const OsiFunctionNameProxy::MetatableName = "OsiFunctionNameProxy";

	void OsiFunctionNameProxy::PopulateMetatable(lua_State * L)
	{
		lua_newtable(L);

		lua_pushcfunction(L, &LuaGet);
		lua_setfield(L, -2, "Get");

		lua_pushcfunction(L, &LuaDelete);
		lua_setfield(L, -2, "Delete");

		lua_pushcfunction(L, &LuaDeferredNotification);
		lua_setfield(L, -2, "Defer");

		lua_setfield(L, -2, "__index");
	}

	OsiFunctionNameProxy::OsiFunctionNameProxy(STDString const & name, ServerState & state)
		: name_(name), state_(state), generationId_(state_.Osiris().GenerationId())
	{}

	void OsiFunctionNameProxy::UnbindAll()
	{
		functions_.clear();
	}

	bool OsiFunctionNameProxy::BeforeCall(lua_State * L)
	{
		if (state_.RestrictionFlags & State::RestrictOsiris) {
			luaL_error(L, "Attempted to access Osiris function in restricted context");
			return false;
		}

		if (generationId_ != state_.Osiris().GenerationId()) {
			// Clear cached functions if story was reloaded
			UnbindAll();
			generationId_ = state_.Osiris().GenerationId();
		}

		return true;
	}

	int OsiFunctionNameProxy::LuaCall(lua_State * L)
	{
		if (!BeforeCall(L)) return 1;

		auto arity = (uint32_t)lua_gettop(L) - 1;

		auto func = TryGetFunction(arity);
		if (func == nullptr) {
			return luaL_error(L, "No function named '%s' exists that can be called with %d parameters.",
				name_.c_str(), arity);
		}

		return func->LuaCall(L);
	}

	int OsiFunctionNameProxy::LuaGet(lua_State * L)
	{
		auto self = OsiFunctionNameProxy::CheckUserData(L, 1);
		if (!self->BeforeCall(L)) return 1;

		auto arity = (uint32_t)lua_gettop(L) - 1;

		auto func = self->TryGetFunction(arity);
		if (func == nullptr) {
			return luaL_error(L, "No database named '%s(%d)' exists", self->name_.c_str(), arity);
		}

		if (!func->IsDB()) {
			return luaL_error(L, "Function '%s(%d)' is not a database", self->name_.c_str(), arity);
		}

		return func->LuaGet(L);
	}

	int OsiFunctionNameProxy::LuaDelete(lua_State * L)
	{
		auto self = OsiFunctionNameProxy::CheckUserData(L, 1);
		if (!self->BeforeCall(L)) return 1;

		auto arity = (uint32_t)lua_gettop(L) - 1;

		auto func = self->TryGetFunction(arity);
		if (func == nullptr) {
			return luaL_error(L, "No database named '%s(%d)' exists", self->name_.c_str(), arity);
		}

		if (!func->IsDB()) {
			return luaL_error(L, "Function '%s(%d)' is not a database", self->name_.c_str(), arity);
		}

		return func->LuaDelete(L);
	}

	int OsiFunctionNameProxy::LuaDeferredNotification(lua_State * L)
	{
		auto self = OsiFunctionNameProxy::CheckUserData(L, 1);
		if (!self->BeforeCall(L)) return 1;

		auto arity = (uint32_t)lua_gettop(L) - 1;

		auto func = self->TryGetFunction(arity);
		if (func == nullptr) {
			return luaL_error(L, "No function named '%s' exists that can be called with %d parameters.",
				self->name_.c_str(), arity);
		}

		return func->LuaDeferredNotification(L);
	}

	OsiFunction * OsiFunctionNameProxy::TryGetFunction(uint32_t arity)
	{
		if (functions_.size() > arity
			&& functions_[arity].IsBound()) {
			return &functions_[arity];
		}

		// Look for Call/Proc/Event/Query (number of OUT args == 0)
		auto func = LookupOsiFunction(name_, arity);
		if (func != nullptr && func->Signature->OutParamList.numOutParams() == 0) {
			return CreateFunctionMapping(arity, func);
		}

		for (uint32_t args = arity + 1; args < arity + MaxQueryOutParams; args++) {
			// Look for Query/UserQuery (number of OUT args > 0)
			auto func = LookupOsiFunction(name_, args);
			if (func != nullptr) {
				auto outParams = func->Signature->OutParamList.numOutParams();
				auto params = func->Signature->Params->Params.Size - outParams;
				if (params == arity) {
					return CreateFunctionMapping(arity, func);
				}
			}
		}

		return nullptr;
	}

	OsiFunction * OsiFunctionNameProxy::CreateFunctionMapping(uint32_t arity, Function const * func)
	{
		if (functions_.size() <= arity) {
			functions_.resize(arity + 1);
		}

		if (functions_[arity].Bind(func, state_)) {
			return &functions_[arity];
		} else {
			return nullptr;
		}
	}


	bool CustomLuaCall::Call(OsiArgumentDesc const & params)
	{
		if (!ValidateArgs(params)) {
			return false;
		}

		LuaServerPin lua(ExtensionState::Get());
		if (!lua) {
			OsiErrorS("Call failed: Lua state not initialized");
			return false;
		}

		auto L = lua->GetState();
		lua_checkstack(L, params.Count() + 1);
		LifetimeStackPin _(lua->GetStack());
		handler_.Push();

		auto param = &params;
		int numParams{ 0 };
		while (param != nullptr) {
			OsiToLua(L, param->Value);
			numParams++;
			param = param->NextParam;
		}

		if (CallWithTraceback(lua->GetState(), numParams, 0) != 0) {
			LuaError("Handler for Osiris call '" << Name() << "' failed: " << lua_tostring(L, -1));
			lua_pop(L, 1);
			return false;
		}

		return true;
	}


	bool ServerState::Query(char const* mod, char const* name, RegistryEntry * func,
		std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params)
	{
		auto L = GetState();
		auto stackSize = lua_gettop(L);

		try {
			return QueryInternal(mod, name, func, signature, params);
		} catch (Exception &) {
			auto stackRemaining = lua_gettop(L) - stackSize;
			if (stackRemaining > 0) {
				if (mod != nullptr) {
					LuaError("Call to mod query '" << mod << "'.'" << func << "' failed: " << lua_tostring(L, -1));
				} else {
					LuaError("Call to mod query '" << func << "' failed: " << lua_tostring(L, -1));
				}
				lua_pop(L, stackRemaining);
			} else {
				if (mod != nullptr) {
					LuaError("Internal error during call to mod query '" << mod << "'.'" << func << "'");
				} else {
					LuaError("Internal error during call to mod query '" << func << "'");
				}
			}

			return false;
		}
	}


	bool ServerState::QueryInternal(char const* mod, char const* name, RegistryEntry * func,
		std::vector<CustomFunctionParam> const & signature, OsiArgumentDesc & params)
	{
		auto L = GetState();
		lua_checkstack(L, params.Count() + 1);
		LifetimeStackPin _(GetStack());

		auto stackSize = lua_gettop(L);
		if (func) {
			func->Push();
		} else if (mod != nullptr) {
			PushModFunction(L, mod, name);
		} else {
			lua_getglobal(L, name);
		}

		int numParams{ 0 };
		int numOutParams{ 0 };

		auto param = &params;
		int paramIndex{ 0 };
		while (param != nullptr) {
			if (signature[paramIndex].Dir == FunctionArgumentDirection::In) {
				OsiToLua(L, param->Value);
				numParams++;
			} else {
				numOutParams++;
			}

			param = param->NextParam;
			paramIndex++;
		}

		if (CallWithTraceback(L, numParams, LUA_MULTRET) != 0) {
			LuaError("Handler for '" << name << "' failed: " << lua_tostring(L, -1));
			lua_pop(L, 1);
			return false;
		}

		auto numReturnValues = lua_gettop(L) - stackSize;
		if (numReturnValues == 0) {
			// No values returned, assume that the query failed
			return false;
		} else if (numOutParams == 0) {
			// Query has no OUT parameters; it should return "true" or "false".
			if (numReturnValues == 1) {
				auto retType = lua_type(L, -1);
				if (retType != LUA_TBOOLEAN) {
					OsiError("Handler for '" << name << "' returned type " << retType << "; expected boolean");
					return false;
				}

				auto ok = lua_toboolean(L, -1);
				lua_pop(L, numReturnValues);
				return ok ? true : false;
			} else {
				lua_pop(L, numReturnValues);
				OsiError("Handler for '" << name << "' returned " << numReturnValues << " values; expected 1 bool");
				return false;
			}
		} else if (numOutParams != numReturnValues) {
			lua_pop(L, numReturnValues);
			// Query has OUT parameters
			// Number of returned values should match the number of OUT params in the query
			OsiError("Handler for '" << name << "' returned " << numReturnValues << " values; expected " << numOutParams);
			return false;
		} else {
			// Lua call returned correct number of OUT parameters

			param = &params;
			paramIndex = 0;
			int stackIndex{ -numOutParams };
			int numNulls{ 0 };
			while (param != nullptr) {
				if (signature[paramIndex].Dir == FunctionArgumentDirection::Out) {
					if (lua_isnil(L, stackIndex)) {
						numNulls++;
					} else {
						LuaToOsi(L, stackIndex, param->Value, signature[paramIndex].Type);
					}

					numParams++;
					stackIndex++;
				}

				param = param->NextParam;
				paramIndex++;
			}

			lua_pop(L, numReturnValues);

			if (numNulls > 0 && numNulls != numOutParams) {
				OsiError("Handler for '" << name << "' returned " << numNulls <<
					" nil values; every return value must be nil or non-nil");
				return false;
			} else if (numNulls == numOutParams) {
				return false;
			} else {
				return true;
			}
		}

		return true;
	}


	bool CustomLuaQuery::Query(OsiArgumentDesc & params)
	{
		if (!ValidateArgs(params)) {
			return false;
		}

		LuaServerPin lua(ExtensionState::Get());
		if (!lua) {
			OsiErrorS("Call failed: Lua state not initialized");
			return false;
		}

		return lua->Query(nullptr, Name().c_str(), &handler_, Params(), params);
	}



	char const * const ExtensionLibraryServer::NameResolverMetatableName = "OsiProxyNameResolver";


	void ExtensionLibraryServer::RegisterNameResolverMetatable(lua_State * L)
	{
		lua_register(L, NameResolverMetatableName, nullptr);
		luaL_newmetatable(L, NameResolverMetatableName); // stack: mt
		lua_pushcfunction(L, &LuaIndexResolverTable); // stack: mt, &LuaIndexResolverTable
		lua_setfield(L, -2, "__index"); // mt.__index = &LuaIndexResolverTable; stack: mt
		lua_pop(L, 1); // stack: mt
	}

	void ExtensionLibraryServer::CreateNameResolver(lua_State * L)
	{
		lua_newtable(L); // stack: osi
		luaL_setmetatable(L, NameResolverMetatableName); // stack: osi
		lua_setglobal(L, "Osi"); // stack: -
	}

	int ExtensionLibraryServer::LuaIndexResolverTable(lua_State * L)
	{
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = luaL_checkstring(L, 2);

		LuaServerPin lua(ExtensionState::Get());
		OsiFunctionNameProxy::New(L, name, std::ref(lua.Get())); // stack: tab, name, proxy

		lua_pushvalue(L, 1); // stack: fun, tab
		push(L, name); // stack: fun, tab, name
		lua_pushvalue(L, -3); // stack: fun, tab, name, fun
		lua_settable(L, -3); // stack: fun
		lua_pop(L, 1);
		return 1;
	}

	STDString ExtensionLibraryServer::GenerateOsiHelpers()
	{
		std::stringstream ss;

		auto const& sigs = gExtender->GetServer().Osiris().GetCustomFunctionInjector().OsiSymbols();
		for (auto const & sig : sigs) {
			if (sig.EoCFunctionId != 0 && sig.params.size() <= 16) {
				ss << sig.name << " = Osi." << sig.name << "\r\n";
			}
		}

		return STDString(ss.str());
	}
}
