#include <Extender/Shared/UserVariables.h>
#include <GameDefinitions/Components/Components.h>
#include <Lua/Libs/Json.h>

//#define USER_VAR_DBG(msg, ...)
#define USER_VAR_DBG(msg, ...) DEBUG(msg, __VA_ARGS__)

BEGIN_NS(net)

void ExtenderProtocolBase::SyncUserVars(MsgUserVars const& msg)
{
	USER_VAR_DBG("Received sync message from peer");
	auto state = gExtender->GetCurrentExtensionState();
	for (auto const& var : msg.vars()) {
		if (var.type() == UserVarType::MODULE_VAR) {
			state->GetModVariables().NetworkSync(var);
		} else {
			state->GetUserVariables().NetworkSync(var);
		}
	}
}

END_NS()

BEGIN_SE()

void UserVariable::SavegameVisit(ObjectVisitor* visitor)
{
	if (visitor->IsReading()) {
		uint8_t type;
		visitor->VisitUInt8(GFS.strType, type, (uint8_t)UserVariableType::Null);
		Type = (UserVariableType)type;

		switch (Type) {
		case UserVariableType::Int64:
			visitor->VisitInt64(GFS.strValue, Int, 0ll);
			break;
		case UserVariableType::Double:
			visitor->VisitDouble(GFS.strValue, Dbl, 0.0);
			break;
		case UserVariableType::String:
			visitor->VisitFixedString(GFS.strValue, Str, GFS.strEmpty);
			break;
		case UserVariableType::Composite:
			visitor->VisitSTDString(GFS.strValue, CompositeStr, STDString{});
			break;
		}
	} else {
		auto type = (uint8_t)Type;
		visitor->VisitUInt8(GFS.strType, type, (uint8_t)UserVariableType::Null);
		switch (Type) {
		case UserVariableType::Int64:
			visitor->VisitInt64(GFS.strValue, Int, 0ll);
			break;
		case UserVariableType::Double:
			visitor->VisitDouble(GFS.strValue, Dbl, 0.0);
			break;
		case UserVariableType::String:
			visitor->VisitFixedString(GFS.strValue, Str, GFS.strEmpty);
			break;
		case UserVariableType::Composite:
			visitor->VisitSTDString(GFS.strValue, CompositeStr, STDString{});
			break;
		}
	}
}

void UserVariable::ToNetMessage(net::UserVar& var) const
{
	switch (Type) {
	case UserVariableType::Null:
		break;

	case UserVariableType::Int64:
		var.set_intval(Int);
		break;

	case UserVariableType::Double:
		var.set_dblval(Dbl);
		break;

	case UserVariableType::String:
		var.set_strval(Str.GetString());
		break;

	case UserVariableType::Composite:
		var.set_luaval(CompositeStr.c_str(), CompositeStr.size());
		break;
	}
}

void UserVariable::FromNetMessage(net::UserVar const& var)
{
	switch (var.val_case()) {
	case net::UserVar::kIntval:
		Type = UserVariableType::Int64;
		Int = var.intval();
		break;

	case net::UserVar::kDblval:
		Type = UserVariableType::Double;
		Dbl = var.dblval();
		break;

	case net::UserVar::kStrval:
		Type = UserVariableType::String;
		Str = FixedString(var.strval());
		break;

	case net::UserVar::kLuaval:
		Type = UserVariableType::Composite;
		CompositeStr = var.luaval();
		break;

	case net::UserVar::VAL_NOT_SET:
	default:
		Type = UserVariableType::Null;
		break;
	}
}


size_t UserVariable::Budget() const
{
	size_t budget = 12;
	switch (Type) {
	case UserVariableType::Null: budget += 0; break;
	case UserVariableType::Int64: budget += 8; break;
	case UserVariableType::Double: budget += 8; break;
	case UserVariableType::String: budget += Str.GetLength(); break;
	case UserVariableType::Composite: budget += CompositeStr.size(); break;
	}

	return budget;
}


bool UserVariablePrototype::NeedsRebroadcast(bool server) const
{
	if (
		// There is no server->client->server broadcast functionality, 
		// only client->server->client messages can be resynced
		server
		// Ensure that clients actually need to see this variable
		&& IsAvailableFor(false)
		// Ensure that we can sync to clients
		&& NeedsSyncFor(true)
	) {
		// If we only have a local client, no need to resync user vars
		auto server = gExtender->GetServer().GetNetworkManager().GetServer();
		if (server && server->ActivePeerIds.size() > 1) {
			return true;
		}
	}

	return false;
}


void UserVariableSyncWriter::Flush(bool force)
{
	if (isServer_) {
		auto state = GetStaticSymbols().GetServerState();
		if (!state 
			|| *state == esv::GameState::LoadSession
			|| *state == esv::GameState::LoadLevel
			|| *state == esv::GameState::Sync) {
			return;
		}
	} else {
		auto state = GetStaticSymbols().GetClientState();
		if (!state
			|| *state == ecl::GameState::LoadSession
			|| *state == ecl::GameState::LoadLevel
			|| *state == ecl::GameState::PrepareRunning) {
			return;
		}
	}


	if (!deferredSyncs_.empty()) {
		USER_VAR_DBG("Flushing deferred syncs");
		FlushSyncQueue(deferredSyncs_);
	}
	
	if (force && !nextTickSyncs_.empty()) {
		USER_VAR_DBG("Flushing next tick syncs");
		FlushSyncQueue(nextTickSyncs_);
	}

	SendSyncs();
}

void UserVariableSyncWriter::Sync(Guid const& entity, FixedString const& key, UserVariablePrototype const& proto, UserVariable const* value)
{
	if (proto.NeedsSyncFor(isServer_)) {
		if (value && proto.Has(UserVariableFlags::SyncOnWrite)) {
			USER_VAR_DBG("Immediate sync var %s/%s", entity.ToString().c_str(), key.GetString());
			if (MakeSyncMessage()) {
				AppendToSyncMessage(entity, key, *value);
				SendSyncs();
			}
		} else if (proto.Has(UserVariableFlags::SyncOnTick)) {
			USER_VAR_DBG("Request next tick sync for var %s/%s", entity.ToString().c_str(), key.GetString());
			nextTickSyncs_.push_back(SyncRequest{
				.Entity = entity,
				.Variable = key
			});
		} else {
			USER_VAR_DBG("Request deferred sync for var %s/%s", entity.ToString().c_str(), key.GetString());
			deferredSyncs_.push_back(SyncRequest{
				.Entity = entity,
				.Variable = key
			});
		}
	}
}

void UserVariableSyncWriter::DeferredSync(Guid const& entity, FixedString const& key)
{
	deferredSyncs_.push_back(SyncRequest{
		.Entity = entity,
		.Variable = key
	});
}

void UserVariableSyncWriter::AppendToSyncMessage(Guid const& entity, FixedString const& key, UserVariable const& value)
{
	if (syncMsgBudget_ > SyncMessageBudget) {
		SendSyncs();
		MakeSyncMessage();
	}

	auto var = syncMsg_->GetMessage().mutable_user_vars()->add_vars();
	var->set_uuid1(entity.Val[0]);
	var->set_uuid2(entity.Val[1]);
	var->set_key(key.GetString());
	value.ToNetMessage(*var);
	syncMsgBudget_ += value.Budget() + key.GetLength();
}

void UserVariableSyncWriter::FlushSyncQueue(Array<SyncRequest>& queue)
{
	if (!MakeSyncMessage()) return;

	for (auto const& req : queue) {
		auto value = vars_->Get(req.Entity, req.Variable);
		if (value && value->Dirty) {
			USER_VAR_DBG("Flush sync var %s/%s", req.Entity.ToString().c_str(), req.Variable.GetString());
			AppendToSyncMessage(req.Entity, req.Variable, *value);
			value->Dirty = false;
		}
	}

	queue.clear();
}

bool UserVariableSyncWriter::MakeSyncMessage()
{
	if (syncMsg_ == nullptr) {
		if (isServer_) {
			syncMsg_ = gExtender->GetServer().GetNetworkManager().GetFreeMessage();
		} else {
			syncMsg_ = gExtender->GetClient().GetNetworkManager().GetFreeMessage();
		}

		if (syncMsg_) {
			syncMsg_->GetMessage().mutable_user_vars();
		}
	}

	return syncMsg_ != nullptr;
}

void UserVariableSyncWriter::SendSyncs()
{
	if (syncMsg_ && syncMsg_->GetMessage().user_vars().vars_size() > 0) {
		if (isServer_) {
			USER_VAR_DBG("Syncing user vars to client(s)");
			gExtender->GetServer().GetNetworkManager().BroadcastToConnectedPeers(syncMsg_, ReservedUserId, false);
		} else {
			USER_VAR_DBG("Syncing user vars to server");
			gExtender->GetClient().GetNetworkManager().Send(syncMsg_);
		}

		syncMsg_ = nullptr;
		syncMsgBudget_ = 0;
	}
}


UserVariable* UserVariableManager::Get(Guid const& entity, FixedString const& key)
{
	auto it = vars_.Find(entity);
	if (it) {
		auto valueIt = (*it)->Vars.Find(key);
		if (valueIt) {
			return *valueIt;
		}
	}

	return nullptr;
}

MultiHashMap<FixedString, UserVariable>* UserVariableManager::GetAll(Guid const& entity)
{
	auto it = vars_.Find(entity);
	if (it) {
		return &(*it)->Vars;
	} else {
		return nullptr;
	}
}

MultiHashMap<Guid, UserVariableManager::EntityVariables>& UserVariableManager::GetAll()
{
	return vars_;
}

void UserVariableManager::MarkDirty(Guid const& entity, FixedString const& key, UserVariable& value)
{
	auto proto = GetPrototype(key);
	if (proto == nullptr) return;

	value.Dirty = true;
	sync_.Sync(entity, key, *proto, &value);
}

UserVariableManager::EntityVariables* UserVariableManager::Set(Guid const& entity, FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value)
{
	if (value.Dirty) {
		sync_.Sync(entity, key, proto, &value);
	}

	auto it = vars_.Find(entity);
	if (it) {
		auto valueIt = (*it)->Vars.Find(key);
		if (valueIt) {
			**valueIt = std::move(value);
		} else {
			(*it)->Vars.Set(key, std::move(value));
		}
		return *it;
	} else {
		auto gameObjectVars = vars_.Set(entity, EntityVariables{});
		gameObjectVars->Vars.Set(key, std::move(value));
		return gameObjectVars;
	}
}

void UserVariableManager::BindCache(lua::CachedUserVariableManager* cache)
{
	cache_ = cache;
}

void UserVariableManager::Update()
{
	Flush(true);
}

void UserVariableManager::Flush(bool force)
{
	sync_.Flush(force);
}

UserVariablePrototype const* UserVariableManager::GetPrototype(FixedString const& key) const
{
	auto it = prototypes_.Find(key);
	if (it) {
		return *it;
	} else {
		return nullptr;
	}
}

void UserVariableManager::RegisterPrototype(FixedString const& key, UserVariablePrototype const& proto)
{
	prototypes_.Set(key, proto);
}

void UserVariableManager::SavegameVisit(ObjectVisitor* visitor)
{
	if (visitor->IsReading()) {
		vars_.clear();
	}

	STDString nullStr;
	if (visitor->EnterNode(GFS.strUserVariables, GFS.strEmpty)) {
		if (visitor->IsReading()) {
			uint32_t numVars;
			visitor->VisitCount(GFS.strEntityVariables, &numVars);

			for (uint32_t i = 0; i < numVars; i++) {
				if (visitor->EnterNode(GFS.strEntityVariables, GFS.strEntity)) {
					Guid entity;
					visitor->VisitGuid(GFS.strEntity, entity, Guid::Null);
					auto entityVars = vars_.Set(entity, EntityVariables{});

					uint32_t numEntityVars;
					visitor->VisitCount(GFS.strVariable, &numEntityVars);

					for (uint32_t j = 0; j < numEntityVars; j++) {
						if (visitor->EnterNode(GFS.strVariable, GFS.strName)) {
							FixedString name;
							visitor->VisitFixedString(GFS.strName, name, GFS.strEmpty);
							USER_VAR_DBG("Savegame restore var %s/%s", entity.ToString().c_str(), name.GetString());
							
							auto var = entityVars->Vars.Set(name, UserVariable{});
							var->SavegameVisit(visitor);
							visitor->ExitNode(GFS.strVariable);

							auto proto = GetPrototype(name);
							if (proto && proto->NeedsSyncFor(isServer_)) {
								USER_VAR_DBG("Request deferred sync for var %s/%s", entity.ToString().c_str(), name.GetString());
								var->Dirty = true;
								sync_.DeferredSync(entity, name);
							}
						}
					}

					visitor->ExitNode(GFS.strEntityVariables);
				}
			}
		} else {
			if (cache_) {
				cache_->Flush();
			}

			for (auto& entity : vars_) {
				if (visitor->EnterNode(GFS.strEntityVariables, GFS.strEntity)) {
					visitor->VisitGuid(GFS.strEntity, entity.Key(), Guid::Null);

					for (auto& kv : entity.Value().Vars) {
						auto proto = GetPrototype(kv.Key());
						if (proto && proto->Has(UserVariableFlags::Persistent)) {
							if (visitor->EnterNode(GFS.strVariable, GFS.strName)) {
								visitor->VisitFixedString(GFS.strName, kv.Key(), GFS.strEmpty);
								USER_VAR_DBG("Savegame persist var %s/%s", entity.Key().ToString().c_str(), kv.Key().GetString());
								kv.Value().SavegameVisit(visitor);
								visitor->ExitNode(GFS.strVariable);
							}
						}
					}

					visitor->ExitNode(GFS.strEntityVariables);
				}
			}
		}

		visitor->ExitNode(GFS.strUserVariables);
	}
}

void UserVariableManager::NetworkSync(net::UserVar const& var)
{
	Guid entityGuid;
	entityGuid.Val[0] = var.uuid1();
	entityGuid.Val[1] = var.uuid2();

	USER_VAR_DBG("Received sync for %d/%s/%s", var.type(), entityGuid.ToString().c_str(), var.key().c_str());
	auto entity = GuidToEntity(entityGuid);
	if (!entity) return;

	FixedString key(var.key());
	auto proto = GetPrototype(key);
	if (!proto) {
		ERR("Tried to sync variable '%s' that has no prototype!", var.key().c_str());
		return;
	}
	
	if (!proto->NeedsSyncFor(!isServer_)) {
		ERR("Tried to sync variable '%s' in illegal direction!", var.key().c_str());
		return;
	}

	UserVariable value;
	value.FromNetMessage(var);
	value.Dirty = proto->NeedsRebroadcast(isServer_);

	Set(entityGuid, key, *proto, std::move(value));

	if (cache_ && !proto->Has(UserVariableFlags::DontCache)) {
		cache_->Invalidate(entity, key);
	}
}

Guid UserVariableManager::EntityToGuid(EntityHandle const& entity) const
{
	auto uuid = entityHelpers_.GetComponent<UuidComponent>(entity);
	if (uuid) {
		return uuid->EntityUuid;
	} else {
		return Guid{};
	}
}

EntityHandle UserVariableManager::GuidToEntity(Guid const& uuid) const
{
	auto entity = entityHelpers_.GetEntityHandle(uuid);

	if (!entity) {
		ERR("Tried to sync variables of unknown game object %s!", uuid.ToString().c_str());
	}

	return entity;
}



UserVariable* ModVariableMap::Get(FixedString const& key)
{
	auto it = vars_.Find(key);
	return it ? *it : nullptr;
}

ModVariableMap::VariableMap& ModVariableMap::GetAll()
{
	return vars_;
}

void ModVariableMap::ClearVars()
{
	vars_.clear();
}

UserVariable* ModVariableMap::Set(FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value)
{
	UserVariable* var;
	auto valueIt = vars_.Find(key);
	if (valueIt) {
		**valueIt = std::move(value);
		var = &**valueIt;
	} else {
		var = vars_.Set(key, std::move(value));
	}

	return var;
}

UserVariablePrototype const* ModVariableMap::GetPrototype(FixedString const& key) const
{
	auto it = prototypes_.Find(key);
	return it ? *it : nullptr;
}

void ModVariableMap::RegisterPrototype(FixedString const& key, UserVariablePrototype const& proto)
{
	prototypes_.Set(key, proto);
}

void ModVariableMap::SavegameVisit(ObjectVisitor* visitor)
{
	STDString nullStr;
	if (visitor->IsReading()) {
		uint32_t numVars;
		visitor->VisitCount(GFS.strVariable, &numVars);

		for (uint32_t j = 0; j < numVars; j++) {
			if (visitor->EnterNode(GFS.strVariable, GFS.strName)) {
				FixedString name;
				visitor->VisitFixedString(GFS.strName, name, GFS.strEmpty);
				USER_VAR_DBG("Savegame restore var %s/%s", moduleUuid_.ToString().c_str(), name.GetString());
							
				auto var = vars_.Set(name, UserVariable{});
				var->SavegameVisit(visitor);
				visitor->ExitNode(GFS.strVariable);

				auto proto = GetPrototype(name);
				if (proto && proto->NeedsSyncFor(isServer_)) {
					USER_VAR_DBG("Request deferred sync for var %s/%s", moduleUuid_.ToString().c_str(), name.GetString());
					var->Dirty = true;
				}
			}
		}
	} else {
		for (auto& kv : vars_) {
			auto proto = GetPrototype(kv.Key());
			if (proto && proto->Has(UserVariableFlags::Persistent)) {
				if (visitor->EnterNode(GFS.strVariable, GFS.strName)) {
					visitor->VisitFixedString(GFS.strName, kv.Key(), GFS.strEmpty);
					USER_VAR_DBG("Savegame persist var %s/%s", moduleUuid_.ToString().c_str(), kv.Key().GetString());
					kv.Value().SavegameVisit(visitor);
					visitor->ExitNode(GFS.strVariable);
				}
			}
		}
	}
}






std::optional<int32_t> ModVariableManager::GuidToModId(Guid const& uuid) const
{
	auto it = modIndices_.Find(uuid);
	if (it) {
		return **it;
	} else {
		return {};
	}
}

UserVariable* ModVariableManager::Get(Guid const& modUuid, FixedString const& key)
{
	auto it = vars_.Find(modUuid);
	if (it) {
		return (*it)->Get(key);
	}

	return nullptr;
}

ModVariableMap::VariableMap* ModVariableManager::GetAll(Guid const& modUuid)
{
	auto it = vars_.Find(modUuid);
	if (it) {
		return &(*it)->GetAll();
	} else {
		return nullptr;
	}
}

MultiHashMap<Guid, ModVariableMap>& ModVariableManager::GetAll()
{
	return vars_;
}

ModVariableMap* ModVariableManager::GetMod(Guid const& modUuid)
{
	auto it = vars_.Find(modUuid);
	return it ? *it : nullptr;
}

ModVariableMap* ModVariableManager::GetOrCreateMod(Guid const& modUuid)
{
	auto it = vars_.Find(modUuid);
	if (it) {
		return *it;
	} else {
		return vars_.Set(modUuid, ModVariableMap(modUuid, isServer_));
	}
}

UserVariablePrototype const* ModVariableManager::GetPrototype(Guid const& modUuid, FixedString const& key) const
{
	auto it = vars_.Find(modUuid);
	if (it) {
		return (*it)->GetPrototype(key);
	} else {
		return nullptr;
	}
}

void ModVariableManager::MarkDirty(Guid const& modUuid, FixedString const& key, UserVariable& value)
{
	auto proto = GetPrototype(modUuid, key);
	if (proto == nullptr) return;

	value.Dirty = true;
	sync_.Sync(modUuid, key, *proto, &value);
}

ModVariableMap* ModVariableManager::Set(Guid const& modUuid, FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value)
{
	auto mod = GetOrCreateMod(modUuid);
	Set(*mod, key, proto, std::move(value));
	return mod;
}

void ModVariableManager::Set(ModVariableMap& mod, FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value)
{
	if (value.Dirty) {
		sync_.Sync(mod.ModuleUuid(), key, proto, &value);
	}

	mod.Set(key, proto, std::move(value));
}

void ModVariableManager::Flush(bool force)
{
	sync_.Flush(force);
}

void ModVariableManager::BindCache(lua::CachedModVariableManager* cache)
{
	cache_ = cache;
}

void ModVariableManager::Update()
{
	Flush(true);
}

void ModVariableManager::OnModuleLoading()
{
	modIndices_.clear();
	auto& mod = (isServer_ ? GetStaticSymbols().GetModManagerServer()->BaseModule : GetStaticSymbols().GetModManagerClient()->BaseModule);
	uint32_t nextIndex{ 0 };
	for (auto const& mod : mod.LoadOrderedModules) {
		modIndices_.Set(mod.Info.ModuleUUID, nextIndex++);
	}
}

void ModVariableManager::RegisterPrototype(Guid const& modUuid, FixedString const& key, UserVariablePrototype const& proto)
{
	GetOrCreateMod(modUuid)->RegisterPrototype(key, proto);
}

void ModVariableManager::SavegameVisit(ObjectVisitor* visitor)
{
	if (visitor->IsReading()) {
		for (auto& mod : vars_) {
			mod.Value().ClearVars();
		}
	}

	STDString nullStr;
	if (visitor->EnterNode(GFS.strModVariables, GFS.strEmpty)) {
		if (visitor->IsReading()) {
			uint32_t numVars;
			visitor->VisitCount(GFS.strModVariables, &numVars);

			for (uint32_t i = 0; i < numVars; i++) {
				if (visitor->EnterNode(GFS.strModVariables, GFS.strModule)) {
					Guid modUuid;
					visitor->VisitGuid(GFS.strModule, modUuid, Guid::Null);
					auto mod = GetOrCreateMod(modUuid);
					mod->SavegameVisit(visitor);
					for (auto const& kv : mod->GetAll()) {
						if (kv.Value().Dirty) {
							sync_.DeferredSync(modUuid, kv.Key());
						}
					}
					visitor->ExitNode(GFS.strModVariables);
				}
			}
		} else {
			if (cache_) {
				cache_->Flush();
			}

			for (auto& mod : vars_) {
				if (visitor->EnterNode(GFS.strModVariables, GFS.strModule)) {
					visitor->VisitGuid(GFS.strModule, mod.Key(), Guid::Null);
					mod.Value().SavegameVisit(visitor);
					visitor->ExitNode(GFS.strModVariables);
				}
			}
		}

		visitor->ExitNode(GFS.strModVariables);
	}
}

void ModVariableManager::NetworkSync(net::UserVar const& var)
{
	Guid modUuid;
	modUuid.Val[0] = var.uuid1();
	modUuid.Val[1] = var.uuid2();

	USER_VAR_DBG("Received sync for %s/%s", modUuid.ToString().c_str(), var.key().c_str());

	auto map = GetMod(modUuid);
	if (!map) {
		ERR("Tried to sync variable for nonexistent mod '%s'!", modUuid.ToString().c_str());
		return;
	}

	FixedString key(var.key());
	auto proto = map->GetPrototype(key);
	if (!proto) {
		ERR("Tried to sync variable %s/%s that has no prototype!", modUuid.ToString().c_str(), var.key().c_str());
		return;
	}

	if (!proto->NeedsSyncFor(!isServer_)) {
		ERR("Tried to sync variable %s/%s in illegal direction!", modUuid.ToString().c_str(), var.key().c_str());
		return;
	}

	UserVariable value;
	value.FromNetMessage(var);
	value.Dirty = proto->NeedsRebroadcast(isServer_);

	Set(*map, key, *proto, std::move(value));

	if (proto && cache_ && !proto->Has(UserVariableFlags::DontCache)) {
		auto modIndex = modIndices_.Find(modUuid);
		if (modIndex) {
			cache_->Invalidate(**modIndex, key);
		}
	}
}

END_SE()

BEGIN_NS(lua)

CachedUserVariable::CachedUserVariable(lua_State* L, UserVariable const& v)
{
	switch (v.Type) {
	case UserVariableType::Int64:
		Type = CachedUserVariableType::Int64;
		Int = v.Int;
		break;

	case UserVariableType::Double:
		Type = CachedUserVariableType::Double;
		Dbl = v.Dbl;
		break;

	case UserVariableType::String:
		Type = CachedUserVariableType::String;
		Str = v.Str;
		break;

	case UserVariableType::Composite:
		ParseReference(L, v.CompositeStr);
		break;

	case UserVariableType::Null:
	default:
		Type = CachedUserVariableType::Null;
		break;
	}
}

CachedUserVariable::CachedUserVariable(lua_State* L, Ref const& v)
{
	if (v.Type() != RefType::Local) {
		ERR("Tried to make cached user variable from non-local ref!");
		Type = CachedUserVariableType::Null;
		return;
	}

	switch (lua_type(L, v.Index())) {
	case LUA_TNIL:
		Type = CachedUserVariableType::Null;
		break;
		
	case LUA_TNUMBER:
		if (lua_isinteger(L, v.Index())) {
			Type = CachedUserVariableType::Int64;
			Int = get<int64_t>(L, v.Index());
		} else {
			Type = CachedUserVariableType::Double;
			Dbl = get<double>(L, v.Index());
		}
		break;

	case LUA_TSTRING:
		Type = CachedUserVariableType::String;
		Str = get<FixedString>(L, v.Index());
		break;

	case LUA_TTABLE:
		Type = CachedUserVariableType::Reference;
		Reference = RegistryEntry(L, v);
		break;

	default:
		Type = CachedUserVariableType::Null;
		ERR("Tried to make cached user variable from unsupported type '%s'!", lua_typename(L, lua_type(L, v.Index())));
		break;
	}
}

CachedUserVariable::CachedUserVariable(CachedUserVariable&& o)
{
	Type = o.Type;
	switch (o.Type) {
	case CachedUserVariableType::Int64:
		Int = o.Int;
		break;

	case CachedUserVariableType::Double:
		Dbl = o.Dbl;
		break;

	case CachedUserVariableType::String:
		Str = std::move(o.Str);
		break;

	case CachedUserVariableType::Reference:
		Reference = std::move(o.Reference);
		break;
	}
}

CachedUserVariable& CachedUserVariable::operator = (CachedUserVariable&& o)
{
	Type = o.Type;
	switch (o.Type) {
	case CachedUserVariableType::Int64:
		Int = o.Int;
		break;

	case CachedUserVariableType::Double:
		Dbl = o.Dbl;
		break;

	case CachedUserVariableType::String:
		Str = std::move(o.Str);
		break;

	case CachedUserVariableType::Reference:
		Reference = std::move(o.Reference);
		break;
	}

	return *this;
}

void CachedUserVariable::ParseReference(lua_State* L, StringView json)
{
	if (json::Parse(L, json)) {
		Reference = RegistryEntry(L, -1);
		lua_pop(L, 1);
		Type = CachedUserVariableType::Reference;
	} else {
		ERR("Failed to parse user variable blob");
		Type = CachedUserVariableType::Null;
	}
}

void CachedUserVariable::Push(lua_State* L) const
{
	switch (Type) {
	case CachedUserVariableType::Int64: push(L, Int); break;
	case CachedUserVariableType::Double: push(L, Dbl); break;
	case CachedUserVariableType::String: push(L, Str); break;
	case CachedUserVariableType::Reference: push(L, Reference); break;
	case CachedUserVariableType::Null:
	default: 
		push(L, nullptr);
		break;
	}
}

bool CachedUserVariable::LikelyChanged(CachedUserVariable const& o) const
{
	if (Type != o.Type) return true;

	switch (Type) {
	case CachedUserVariableType::Int64:
		return Int != o.Int;
	case CachedUserVariableType::Double:
		return Dbl != o.Dbl;
	case CachedUserVariableType::String:
		return Str != o.Str;
	case CachedUserVariableType::Reference:
		// There is no fast way to compare values apart from serializing the values on both sides;
		// assume that a write to a composite value is an explicit way to indicate that the value changed.
		return true;

	case CachedUserVariableType::Null:
	default:
		return false;
	}
}

UserVariable CachedUserVariable::ToUserVariable(lua_State* L) const
{
	UserVariable var;
	switch (Type) {
	case CachedUserVariableType::Null:
		var.Type = UserVariableType::Null;
		break;
		
	case CachedUserVariableType::Int64:
		var.Type = UserVariableType::Int64;
		var.Int = Int;
		break;
		
	case CachedUserVariableType::Double:
		var.Type = UserVariableType::Double;
		var.Dbl = Dbl;
		break;
		
	case CachedUserVariableType::String:
		var.Type = UserVariableType::String;
		var.Str = Str;
		break;
		
	case CachedUserVariableType::Reference:
		var.CompositeStr = StringifyReference(L);
		if (!var.CompositeStr.empty()) {
			var.Type = UserVariableType::Composite;
		} else {
			var.Type = UserVariableType::Null;
		}
		
		break;
	}

	return var;
}

STDString CachedUserVariable::StringifyReference(lua_State* L) const
{
	Reference.Push();
	json::StringifyContext ctx;
	ctx.Beautify = false;

	STDString str;
	try {
		str = json::Stringify(L, ctx, lua_absindex(L, -1));
	} catch (std::runtime_error& e) {
		ERR("Error stringifying user variable: %s", e.what());
		str.clear();
	}

	lua_pop(L, 1);
	return str;
}


CachedUserVariableManager::CachedUserVariableManager(UserVariableManager& global, bool isServer)
	: global_(global), isServer_(isServer)
{
	global_.BindCache(this);
}

CachedUserVariableManager::~CachedUserVariableManager()
{
	global_.BindCache(nullptr);
}

CachedUserVariable* CachedUserVariableManager::GetFromCache(EntityHandle entity, FixedString const& key, Guid& entityGuid)
{
	auto it = vars_.Find(entity);
	if (it) {
		auto valueIt = (*it)->Vars.Find(key);
		if (valueIt) {
			entityGuid = (*it)->CachedGuid;
			return *valueIt;
		}
	}

	return nullptr;
}

CachedUserVariable* CachedUserVariableManager::GetFromCache(EntityHandle entity, FixedString const& key)
{
	auto it = vars_.Find(entity);
	if (it) {
		auto valueIt = (*it)->Vars.Find(key);
		if (valueIt) {
			return *valueIt;
		}
	}

	return nullptr;
}

CachedUserVariable* CachedUserVariableManager::PutCache(EntityHandle entity, FixedString const& key, Guid const& entityGuid, UserVariablePrototype const& proto, CachedUserVariable&& value, bool isWrite)
{
	CachedUserVariable* var;
	EntityVariables* vars;
	bool wasDirty{ false };

	auto it = vars_.Find(entity);
	if (it) {
		vars = *it;
		auto valueIt = vars->Vars.Find(key);
		if (valueIt) {
			wasDirty = (*valueIt)->Dirty;
			bool dirty = (*valueIt)->Dirty || (isWrite && (*valueIt)->LikelyChanged(value));
			**valueIt = std::move(value);
			var = *valueIt;
			var->Dirty = dirty;
		} else {
			var = vars->Vars.Set(key, std::move(value));
			var->Dirty = isWrite;
		}
	} else {
		vars = vars_.Set(entity, EntityVariables{});
		if (entityGuid) {
			vars->CachedGuid = entityGuid;
		} else {
			vars->CachedGuid = global_.EntityToGuid(entity);
		}

		var = vars->Vars.Set(key, std::move(value));
		var->Dirty = isWrite;
	}

	if (!wasDirty && var->Dirty) {
		USER_VAR_DBG("Mark cached var for flush %s/%s", vars->CachedGuid.ToString().c_str(), key.GetString());
		flushQueue_.push_back(FlushRequest{
			.Entity = entity,
			.Variable = key,
			.Proto = &proto
		});
	}

	return var;
}

CachedUserVariable* CachedUserVariableManager::PutCache(lua_State* L, EntityHandle entity, FixedString const& key, Guid const& entityGuid, UserVariablePrototype const& proto, UserVariable const& value)
{
	CachedUserVariable var(L, value);
	return PutCache(entity, key, entityGuid, proto, std::move(var), false);
}

void CachedUserVariableManager::Push(lua_State* L, EntityHandle entity, FixedString const& key)
{
	auto proto = global_.GetPrototype(key);
	if (!proto) {
		push(L, nullptr);
		return;
	}

	return Push(L, entity, key, *proto);
}

void CachedUserVariableManager::Push(lua_State* L, EntityHandle entity, FixedString const& key, UserVariablePrototype const& proto)
{
	Guid entityGuid;
	if (!proto.Has(UserVariableFlags::DontCache)) {
		auto cached = GetFromCache(entity, key, entityGuid);
		if (cached) {
			cached->Push(L);
			return;
		}
	}

	if (!entityGuid) {
		entityGuid = global_.EntityToGuid(entity);
		if (!entityGuid) {
			push(L, nullptr);
			return;
		}
	}

	auto value = global_.Get(entityGuid, key);
	if (!value) {
		push(L, nullptr);
		return;
	}

	if (!proto.Has(UserVariableFlags::DontCache)) {
		USER_VAR_DBG("Cache global var %016llx/%s", entity.Handle, key.GetString());
		auto cached = PutCache(L, entity, key, entityGuid, proto, *value);
		cached->Push(L);
	} else {
		CachedUserVariable cached(L, *value);
		cached.Push(L);
	}
}

void CachedUserVariableManager::Set(lua_State* L, EntityHandle entity, FixedString const& key, CachedUserVariable && var)
{
	auto proto = global_.GetPrototype(key);
	if (!proto) {
		OsiError("Tried to set user variable '" << key << "' that is not registered!");
		return;
	}

	Set(L, entity, key, *proto, std::move(var));
}

void CachedUserVariableManager::Set(lua_State* L, EntityHandle entity, FixedString const& key, UserVariablePrototype const& proto, CachedUserVariable && var)
{
	if (!proto.Has(UserVariableFlags::DontCache)) {
		USER_VAR_DBG("Set cached var %016llx/%s", entity.Handle, key.GetString());
		auto cachedVar = PutCache(entity, key, Guid{}, proto, std::move(var), true);

		if (cachedVar->Dirty && proto.NeedsSyncFor(isServer_) && proto.Has(UserVariableFlags::SyncOnWrite)) {
			USER_VAR_DBG("Set global var %016llx/%s", entity.Handle, key.GetString());
			auto userVar = cachedVar->ToUserVariable(L);
			cachedVar->Dirty = false;
			userVar.Dirty = true;
			global_.Set(global_.EntityToGuid(entity), key, proto, std::move(userVar));
		}
	} else {
		USER_VAR_DBG("Set global var %016llx/%s", entity.Handle, key.GetString());
		auto userVar = var.ToUserVariable(L);
		userVar.Dirty = true;
		global_.Set(global_.EntityToGuid(entity), key, proto, std::move(userVar));
	}
}

void CachedUserVariableManager::Invalidate()
{
	vars_.clear();
	flushQueue_.clear();
}

void CachedUserVariableManager::Invalidate(EntityHandle entity, FixedString const& key)
{
	auto it = vars_.Find(entity);
	if (it) {
		if ((*it)->Vars.remove(key)) {
			USER_VAR_DBG("Invalidate cached var %016llx/%s", entity.Handle, key.GetString());
		}
	}
}

void CachedUserVariableManager::Flush()
{
	LuaVirtualPin lua;

	if (lua) {
		for (auto const& req : flushQueue_) {
			Guid entityGuid;
			auto var = GetFromCache(req.Entity, req.Variable, entityGuid);
			if (var && var->Dirty) {
				USER_VAR_DBG("Flush cached var %016llx/%s", req.Entity.Handle, req.Variable.GetString());
				auto userVar = var->ToUserVariable(lua->GetState());
				userVar.Dirty = true;
				global_.Set(entityGuid, req.Variable, *req.Proto, std::move(userVar));
				var->Dirty = false;
			}
		}

		flushQueue_.clear();
	}
}



CachedModVariableManager::CachedModVariableManager(ModVariableManager& global, bool isServer)
	: global_(global), isServer_(isServer)
{
	global_.BindCache(this);
}

CachedModVariableManager::~CachedModVariableManager()
{
	global_.BindCache(nullptr);
}

Guid CachedModVariableManager::ModIndexToGuid(uint32_t modIndex)
{
	if (isServer_) {
		return GetStaticSymbols().GetModManagerServer()->BaseModule.LoadOrderedModules[modIndex].Info.ModuleUUID;
	} else {
		return GetStaticSymbols().GetModManagerClient()->BaseModule.LoadOrderedModules[modIndex].Info.ModuleUUID;
	}
}

CachedUserVariable* CachedModVariableManager::GetFromCache(uint32_t modIndex, FixedString const& key, Guid& modUuid)
{
	auto it = vars_.Find(modIndex);
	if (it) {
		auto valueIt = (*it)->Vars.Find(key);
		if (valueIt) {
			modUuid = (*it)->CachedGuid;
			return *valueIt;
		}
	}

	return nullptr;
}

CachedUserVariable* CachedModVariableManager::GetFromCache(uint32_t modIndex, FixedString const& key)
{
	auto it = vars_.Find(modIndex);
	if (it) {
		auto valueIt = (*it)->Vars.Find(key);
		if (valueIt) {
			return *valueIt;
		}
	}

	return nullptr;
}

CachedUserVariable* CachedModVariableManager::PutCache(uint32_t modIndex, FixedString const& key, Guid const& modUuid, UserVariablePrototype const& proto, CachedUserVariable&& value, bool isWrite)
{
	CachedUserVariable* var;
	ModVariables* vars;
	bool wasDirty{ false };

	auto it = vars_.Find(modIndex);
	if (it) {
		vars = *it;
		auto valueIt = vars->Vars.Find(key);
		if (valueIt) {
			wasDirty = (*valueIt)->Dirty;
			bool dirty = (*valueIt)->Dirty || (isWrite && (*valueIt)->LikelyChanged(value));
			**valueIt = std::move(value);
			var = *valueIt;
			var->Dirty = dirty;
		} else {
			var = vars->Vars.Set(key, std::move(value));
			var->Dirty = isWrite;
		}
	} else {
		vars = vars_.Set(modIndex, ModVariables{});
		if (modUuid) {
			vars->CachedGuid = modUuid;
		} else {
			vars->CachedGuid = ModIndexToGuid(modIndex);
		}

		var = vars->Vars.Set(key, std::move(value));
		var->Dirty = isWrite;
	}

	if (!wasDirty && var->Dirty) {
		USER_VAR_DBG("Mark cached mod var for flush %s/%s", vars->CachedGuid.ToString().c_str(), key.GetString());
		flushQueue_.push_back(FlushRequest{
			.ModIndex = modIndex,
			.Variable = key,
			.Proto = &proto
		});
	}

	return var;
}

CachedUserVariable* CachedModVariableManager::PutCache(lua_State* L, uint32_t modIndex, FixedString const& key, Guid const& modUuid, UserVariablePrototype const& proto, UserVariable const& value)
{
	CachedUserVariable var(L, value);
	return PutCache(modIndex, key, modUuid, proto, std::move(var), false);
}

void CachedModVariableManager::Push(lua_State* L, uint32_t modIndex, FixedString const& key)
{
	auto proto = global_.GetPrototype(ModIndexToGuid(modIndex), key);
	if (!proto) {
		push(L, nullptr);
		return;
	}

	return Push(L, modIndex, key, *proto);
}

void CachedModVariableManager::Push(lua_State* L, uint32_t modIndex, FixedString const& key, UserVariablePrototype const& proto)
{
	Guid modUuid;
	if (!proto.Has(UserVariableFlags::DontCache)) {
		auto cached = GetFromCache(modIndex, key, modUuid);
		if (cached) {
			cached->Push(L);
			return;
		}
	}

	if (!modUuid) {
		modUuid = ModIndexToGuid(modIndex);
		if (!modUuid) {
			push(L, nullptr);
			return;
		}
	}

	auto value = global_.Get(modUuid, key);
	if (!value) {
		push(L, nullptr);
		return;
	}

	if (!proto.Has(UserVariableFlags::DontCache)) {
		USER_VAR_DBG("Cache global mod var %d/%s", modIndex, key.GetString());
		auto cached = PutCache(L, modIndex, key, modUuid, proto, *value);
		cached->Push(L);
	} else {
		CachedUserVariable cached(L, *value);
		cached.Push(L);
	}
}

void CachedModVariableManager::Set(lua_State* L, uint32_t modIndex, FixedString const& key, CachedUserVariable && var)
{
	auto proto = global_.GetPrototype(ModIndexToGuid(modIndex), key);
	if (!proto) {
		OsiError("Tried to set user variable '" << key << "' that is not registered!");
		return;
	}

	Set(L, modIndex, key, *proto, std::move(var));
}

void CachedModVariableManager::Set(lua_State* L, uint32_t modIndex, FixedString const& key, UserVariablePrototype const& proto, CachedUserVariable && var)
{
	if (!proto.Has(UserVariableFlags::DontCache)) {
		USER_VAR_DBG("Set cached mod var %d/%s", modIndex, key.GetString());
		auto cachedVar = PutCache(modIndex, key, Guid{}, proto, std::move(var), true);

		if (cachedVar->Dirty && proto.NeedsSyncFor(isServer_) && proto.Has(UserVariableFlags::SyncOnWrite)) {
			USER_VAR_DBG("Set global mod var %d/%s", modIndex, key.GetString());
			auto userVar = cachedVar->ToUserVariable(L);
			cachedVar->Dirty = false;
			userVar.Dirty = true;
			global_.Set(ModIndexToGuid(modIndex), key, proto, std::move(userVar));
		}
	} else {
		USER_VAR_DBG("Set global var %d/%s", modIndex, key.GetString());
		auto userVar = var.ToUserVariable(L);
		userVar.Dirty = true;
		global_.Set(ModIndexToGuid(modIndex), key, proto, std::move(userVar));
	}
}

void CachedModVariableManager::Invalidate()
{
	vars_.clear();
	flushQueue_.clear();
}

void CachedModVariableManager::Invalidate(uint32_t modIndex, FixedString const& key)
{
	auto it = vars_.Find(modIndex);
	if (it) {
		if ((*it)->Vars.remove(key)) {
			USER_VAR_DBG("Invalidate cached mod var %d/%s", modIndex, key.GetString());
		}
	}
}

void CachedModVariableManager::Flush()
{
	LuaVirtualPin lua;

	if (lua) {
		for (auto const& req : flushQueue_) {
			Guid modUuid;
			auto var = GetFromCache(req.ModIndex, req.Variable, modUuid);
			if (var && var->Dirty) {
				USER_VAR_DBG("Flush cached mod var %d/%s", req.ModIndex, req.Variable.GetString());
				auto userVar = var->ToUserVariable(lua->GetState());
				userVar.Dirty = true;
				global_.Set(modUuid, req.Variable, *req.Proto, std::move(userVar));
				var->Dirty = false;
			}
		}

		flushQueue_.clear();
	}
}

END_NS()
