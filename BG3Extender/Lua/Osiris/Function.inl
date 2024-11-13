#include <Lua/Osiris/Function.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

bool OsiFunction::Bind(OsiFunctionDef const * func, OsirisBinding& binding)
{
	if (func->Type == FunctionType::Query
		|| func->Type == FunctionType::SysQuery
		|| func->Type == FunctionType::UserQuery
		// User queries appear in the function table using the 'Database' type,
		// however the node is a UserQueryNode, not a DatabaseNode.
		// Catch this case by checking if the node is a descendant of a DataNode or not.
		|| (func->Type == FunctionType::Database && func->Node.Get() && !func->Node.Get()->IsDataNode())) {

		auto numArgs = func->Signature->Params->Params.Size;
		auto adapter = binding.GetIdentityAdapterMap().FindAdapter((uint8_t)numArgs);
		if (adapter == nullptr) {
			OsiError("Couldn't bind query '" << func->Signature->Name 
				<< "': No identity adapter found for arity " << numArgs);
			return false;
		}

		adapter_.Id = adapter->Id;
	}

	function_ = func;
	return true;
}

void OsiFunction::Unbind()
{
	function_ = nullptr;
}

int OsiFunction::LuaCall(lua_State * L) const
{
	if (function_ == nullptr) {
		return luaL_error(L, "Attempted to call an unbound Osiris function");
	}

	int numArgs = lua_gettop(L);
	if (numArgs < 1) {
		return luaL_error(L, "Called Osi function without 'self' argument?");
	}

	auto state = State::FromLua(L);
	if (state->RestrictionFlags & State::RestrictOsiris) {
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

int OsiFunction::LuaGet(lua_State * L) const
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

	auto state = State::FromLua(L);
	if (state->RestrictionFlags & State::RestrictOsiris) {
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
			lua_rawset(L, -3);
		}

		current = current->Next;
	}

	return 1;
}

int OsiFunction::LuaDelete(lua_State * L) const
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

	auto state = State::FromLua(L);
	if (state->RestrictionFlags & State::RestrictOsiris) {
		return luaL_error(L, "Attempted to delete from Osiris database in restricted context");
	}

	OsiInsert(L, true);
	return 0;
}

int OsiFunction::LuaDeferredNotification(lua_State * L) const
{
	if (function_ == nullptr) {
		return luaL_error(L, "Attempted to call an unbound Osiris function");
	}

	int numArgs = lua_gettop(L);
	if (numArgs < 1) {
		return luaL_error(L, "Called Osi function without 'self' argument?");
	}

	auto state = State::FromLua(L);
	if (state->RestrictionFlags & State::RestrictOsiris) {
		return luaL_error(L, "Attempted to call Osiris function in restricted context");
	}

	if (function_->Type == FunctionType::Event) {
		OsiDeferredNotification(L);
		return 0;
	} else {
		return luaL_error(L, "Cannot queue deferred events on function of type %d", function_->Type);
	}
}

bool OsiFunction::MatchTuple(lua_State * L, int firstIndex, TupleVec const & tuple) const
{
	for (auto i = 0; i < tuple.Size; i++) {
		if (!lua_isnil(L, firstIndex + i)) {
			auto const & v = tuple.Values[i];
			switch (GetBaseType((ValueType)v.TypeId)) {
			case ValueType::Integer:
				if (v.Value.Int32 != lua_tointeger(L, firstIndex + i)) {
					return false;
				}
				break;

			case ValueType::Integer64:
				if (v.Value.Int64 != lua_tointeger(L, firstIndex + i)) {
					return false;
				}
				break;

			case ValueType::Real:
				if (abs(v.Value.Float - lua_tonumber(L, firstIndex + i)) > 0.00001f) {
					return false;
				}
				break;

			case ValueType::String:
			{
				auto str = lua_tostring(L, firstIndex + i);
				if (!str || _stricmp(v.Value.String, str) != 0) {
					return false;
				}
				break;
			}

			case ValueType::GuidString:
			{
				auto str = lua_tostring(L, firstIndex + i);
				if (!str) return false;

				auto len = strlen(str);
				auto valueLen = strlen(v.Value.String);
				if (valueLen < 36 || len < 36 || _stricmp(&v.Value.String[valueLen - 36], &str[len - 36]) != 0) {
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

void OsiFunction::ConstructTuple(lua_State * L, TupleVec const & tuple) const
{
	lua_newtable(L);
	for (auto i = 0; i < tuple.Size; i++) {
		push(L, i + 1);
		OsiToLua(L, tuple.Values[i]);
		lua_rawset(L, -3);
	}
}

void OsiFunction::OsiCall(lua_State * L) const
{
	auto funcArgs = function_->Signature->Params->Params.Size;
	int numArgs = lua_gettop(L);
	if (numArgs - 1 != funcArgs) {
		luaL_error(L, "Incorrect number of arguments for '%s'; expected %d, got %d",
			function_->Signature->Name, funcArgs, numArgs - 1);
	}

	auto state = ServerState::FromLua(L);
	OsiArgumentListPin<OsiArgumentDesc> args(state->Osiris().GetArgumentDescPool(), (uint32_t)funcArgs);
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

void OsiFunction::OsiDeferredNotification(lua_State * L) const
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

void OsiFunction::OsiInsert(lua_State * L, bool deleteTuple) const
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

	auto state = ServerState::FromLua(L);
	OsiArgumentListPin<TypedValue> tvs(state->Osiris().GetTypedValuePool(), (uint32_t)funcArgs);
	OsiArgumentListPin<ListNode<TypedValue *>> nodes(state->Osiris().GetTypedValueNodePool(), (uint32_t)funcArgs + 1);

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

int OsiFunction::OsiQuery(lua_State * L) const
{
	auto outParams = function_->Signature->OutParamList.numOutParams();
	auto numParams = function_->Signature->Params->Params.Size;
	auto inParams = numParams - outParams;

	int numArgs = lua_gettop(L);
	if (numArgs - 1 != inParams) {
		return luaL_error(L, "Incorrect number of IN arguments for '%s'; expected %d, got %d",
			function_->Signature->Name, inParams, numArgs - 1);
	}

	auto state = ServerState::FromLua(L);
	OsiArgumentListPin<OsiArgumentDesc> args(state->Osiris().GetArgumentDescPool(), (uint32_t)numParams);
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

int OsiFunction::OsiUserQuery(lua_State * L) const
{
	auto outParams = function_->Signature->OutParamList.numOutParams();
	auto numParams = function_->Signature->Params->Params.Size;
	auto inParams = numParams - outParams;

	int numArgs = lua_gettop(L);
	if (numArgs - 1 != inParams) {
		return luaL_error(L, "Incorrect number of IN arguments for '%s'; expected %d, got %d",
			function_->Signature->Name, inParams, numArgs - 1);
	}

	auto state = ServerState::FromLua(L);
	OsiArgumentListPin<ListNode<TupleLL::Item>> nodes(state->Osiris().GetTupleNodePool(), (uint32_t)numParams + 1);

	VirtTupleLL tuple;
		
	auto & args = tuple.Data.Items;
	auto argType = function_->Signature->Params->Params.Head->Next;
	args.Init(nodes.Args());

	auto prev = args.Head;
	uint32_t inputArgIndex = 0;
	for (uint32_t i = 0; i < numParams; i++) {
		auto node = nodes.Args() + i + 1;
		args.Insert(node, prev);
		node->Item.Index = i;
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
	bool valid = node->IsValid(&tuple, adapter_.Id);
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

END_NS()
