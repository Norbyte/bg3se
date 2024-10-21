#include <Extender/Shared/UserVariables.h>
#include <GameDefinitions/Components/Components.h>
#include <Lua/Libs/Json.h>

#define USER_VAR_DBG(msg, ...)
//#define USER_VAR_DBG(msg, ...) DEBUG(msg, __VA_ARGS__)

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
		case UserVariableType::Boolean:
		{
			bool value{ false };
			visitor->VisitBool(GFS.strValue, value, false);
			Value = value;
			break;
		}
		case UserVariableType::Int64:
		{
			int64_t value{ 0 };
			visitor->VisitInt64(GFS.strValue, value, 0ll);
			Value = value;
			break;
		}
		case UserVariableType::Double:
		{
			double value{ 0.0 };
			visitor->VisitDouble(GFS.strValue, value, 0.0);
			Value = value;
			break;
		}
		case UserVariableType::String:
		{
			FixedString value;
			visitor->VisitFixedString(GFS.strValue, value, GFS.strEmpty);
			Value = value;
			break;
		}
		case UserVariableType::Composite:
		{
			STDString value;
			visitor->VisitSTDString(GFS.strValue, value, STDString{});
			Value = value;
			break;
		}
		}
	} else {
		auto type = (uint8_t)Type;
		visitor->VisitUInt8(GFS.strType, type, (uint8_t)UserVariableType::Null);
		switch (Type) {
		case UserVariableType::Boolean:
		{
			auto value = std::get<bool>(Value);
			visitor->VisitBool(GFS.strValue, value, false);
			break;
		}
		case UserVariableType::Int64:
		{
			auto value = std::get<int64_t>(Value);
			visitor->VisitInt64(GFS.strValue, value, 0ll);
			break;
		}
		case UserVariableType::Double:
		{
			auto value = std::get<double>(Value);
			visitor->VisitDouble(GFS.strValue, value, 0.0);
			break;
		}
		case UserVariableType::String:
		{
			auto value = std::get<FixedString>(Value);
			visitor->VisitFixedString(GFS.strValue, value, GFS.strEmpty);
			break;
		}
		case UserVariableType::Composite:
		{
			auto value = std::get<STDString>(Value);
			visitor->VisitSTDString(GFS.strValue, value, STDString{});
			break;
		}
		}
	}
}

void UserVariable::ToNetMessage(net::UserVar& var) const
{
	switch (Type) {
	case UserVariableType::Null:
		break;

	case UserVariableType::Boolean:
		var.set_boolval(std::get<bool>(Value));
		break;

	case UserVariableType::Int64:
		var.set_intval(std::get<int64_t>(Value));
		break;

	case UserVariableType::Double:
		var.set_dblval(std::get<double>(Value));
		break;

	case UserVariableType::String:
		var.set_strval(std::get<FixedString>(Value).GetString());
		break;

	case UserVariableType::Composite:
	{
		auto const& s = std::get<STDString>(Value);
		var.set_luaval(s.c_str(), s.size());
		break;
	}
	}
}

void UserVariable::FromNetMessage(net::UserVar const& var)
{
	switch (var.val_case()) {
	case net::UserVar::kBoolval:
		Type = UserVariableType::Boolean;
		Value = var.boolval();
		break;
		
	case net::UserVar::kIntval:
		Type = UserVariableType::Int64;
		Value = var.intval();
		break;

	case net::UserVar::kDblval:
		Type = UserVariableType::Double;
		Value = var.dblval();
		break;

	case net::UserVar::kStrval:
		Type = UserVariableType::String;
		Value = FixedString(var.strval());
		break;

	case net::UserVar::kLuaval:
		Type = UserVariableType::Composite;
		Value = STDString(var.luaval());
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
	case UserVariableType::Boolean: budget += 1; break;
	case UserVariableType::Int64: budget += 8; break;
	case UserVariableType::Double: budget += 8; break;
	case UserVariableType::String: budget += std::get<FixedString>(Value).GetLength(); break;
	case UserVariableType::Composite: budget += std::get<STDString>(Value).size(); break;
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


void UserVariableSyncWriter::Clear()
{
	deferredSyncs_.clear();
	nextTickSyncs_.clear();
	syncMsg_ = nullptr;
	syncMsgBudget_ = 0;
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
	switch (varClass_) {
	case UserVarClass::EntityVar: var->set_type(net::UserVarType::ENTITY_VAR); break;
	case UserVarClass::ModuleVar: var->set_type(net::UserVarType::MODULE_VAR); break;
	}
	
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
	auto vars = vars_.try_get(entity);
	if (vars) {
		return vars->Vars.try_get(key);
	}

	return nullptr;
}

HashMap<FixedString, UserVariable>* UserVariableManager::GetAll(Guid const& entity)
{
	auto vars = vars_.try_get(entity);
	if (vars) {
		return &vars->Vars;
	} else {
		return nullptr;
	}
}

HashMap<Guid, UserVariableManager::EntityVariables>& UserVariableManager::GetAll()
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

	auto it = vars_.try_get(entity);
	if (it) {
		auto valueIt = it->Vars.try_get(key);
		if (valueIt) {
			*valueIt = std::move(value);
		} else {
			it->Vars.set(key, std::move(value));
		}
		return it;
	} else {
		auto gameObjectVars = vars_.set(entity, EntityVariables{});
		gameObjectVars->Vars.set(key, std::move(value));
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
	return prototypes_.try_get(key);
}

void UserVariableManager::RegisterPrototype(FixedString const& key, UserVariablePrototype const& proto)
{
	prototypes_.set(key, proto);
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
					auto entityVars = vars_.add_key(entity);

					uint32_t numEntityVars;
					visitor->VisitCount(GFS.strVariable, &numEntityVars);

					for (uint32_t j = 0; j < numEntityVars; j++) {
						if (visitor->EnterNode(GFS.strVariable, GFS.strName)) {
							FixedString name;
							visitor->VisitFixedString(GFS.strName, name, GFS.strEmpty);
							USER_VAR_DBG("Savegame restore var %s/%s", entity.ToString().c_str(), name.GetString());
							
							auto var = entityVars->Vars.add_key(name);
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
					visitor->VisitGuid(GFS.strEntity, const_cast<Guid&>(entity.Key()), Guid::Null);

					for (auto& kv : entity.Value().Vars) {
						auto proto = GetPrototype(kv.Key());
						if (proto && proto->Has(UserVariableFlags::Persistent)) {
							if (visitor->EnterNode(GFS.strVariable, GFS.strName)) {
								visitor->VisitFixedString(GFS.strName, const_cast<FixedString&>(kv.Key()), GFS.strEmpty);
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
		USER_VAR_DBG("Tried to sync variables of unknown game object %s!", uuid.ToString().c_str());
	}

	return entity;
}



UserVariable* ModVariableMap::Get(FixedString const& key)
{
	return vars_.try_get(key);
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
	auto valueIt = vars_.try_get(key);
	if (valueIt) {
		*valueIt = std::move(value);
		var = valueIt;
	} else {
		var = vars_.set(key, std::move(value));
	}

	return var;
}

UserVariablePrototype const* ModVariableMap::GetPrototype(FixedString const& key) const
{
	return prototypes_.try_get(key);
}

void ModVariableMap::RegisterPrototype(FixedString const& key, UserVariablePrototype const& proto)
{
	prototypes_.set(key, proto);
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
							
				auto var = vars_.add_key(name);
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
					visitor->VisitFixedString(GFS.strName, const_cast<FixedString&>(kv.Key()), GFS.strEmpty);
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
	auto it = modIndices_.try_get(uuid);
	if (it) {
		return *it;
	} else {
		return {};
	}
}

UserVariable* ModVariableManager::Get(Guid const& modUuid, FixedString const& key)
{
	auto it = vars_.try_get(modUuid);
	if (it) {
		return it->Get(key);
	} else {
		return nullptr;
	}
}

ModVariableMap::VariableMap* ModVariableManager::GetAll(Guid const& modUuid)
{
	auto it = vars_.try_get(modUuid);
	if (it) {
		return &it->GetAll();
	} else {
		return nullptr;
	}
}

HashMap<Guid, ModVariableMap>& ModVariableManager::GetAll()
{
	return vars_;
}

ModVariableMap* ModVariableManager::GetMod(Guid const& modUuid)
{
	return vars_.try_get(modUuid);
}

ModVariableMap* ModVariableManager::GetOrCreateMod(Guid const& modUuid)
{
	auto it = vars_.try_get(modUuid);
	if (it) {
		return it;
	} else {
		return vars_.set(modUuid, ModVariableMap{ modUuid, isServer_ });
	}
}

UserVariablePrototype const* ModVariableManager::GetPrototype(Guid const& modUuid, FixedString const& key) const
{
	auto it = vars_.try_get(modUuid);
	if (it) {
		return it->GetPrototype(key);
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

void ModVariableManager::OnSessionLoading()
{
	modIndices_.clear();

	auto modManager = isServer_ ? GetStaticSymbols().GetModManagerServer() : GetStaticSymbols().GetModManagerClient();
	if (modManager != nullptr) {
		uint32_t nextIndex{ 0 };
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			modIndices_.set(mod.Info.ModuleUUID, nextIndex++);
		}
	} else {
		ERR("ModManager not initialized on session load?");
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
					visitor->VisitGuid(GFS.strModule, const_cast<Guid&>(mod.Key()), Guid::Null);
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
		auto modIndex = modIndices_.try_get(modUuid);
		if (modIndex) {
			cache_->Invalidate(*modIndex, key);
		}
	}
}

END_SE()

BEGIN_NS(lua)

CachedUserVariable::CachedUserVariable(lua_State* L, UserVariable const& v)
{
	switch (v.Type) {
	case UserVariableType::Boolean:
		Type = CachedUserVariableType::Boolean;
		Value = std::get<bool>(v.Value);
		break;
		
	case UserVariableType::Int64:
		Type = CachedUserVariableType::Int64;
		Value = std::get<int64_t>(v.Value);
		break;

	case UserVariableType::Double:
		Type = CachedUserVariableType::Double;
		Value = std::get<double>(v.Value);
		break;

	case UserVariableType::String:
		Type = CachedUserVariableType::String;
		Value = std::get<FixedString>(v.Value);
		break;

	case UserVariableType::Composite:
		ParseReference(L, std::get<STDString>(v.Value));
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

	case LUA_TBOOLEAN:
		Type = CachedUserVariableType::Boolean;
		Value = get<bool>(L, v.Index());
		break;
		
	case LUA_TNUMBER:
		if (lua_isinteger(L, v.Index())) {
			Type = CachedUserVariableType::Int64;
			Value = get<int64_t>(L, v.Index());
		} else {
			Type = CachedUserVariableType::Double;
			Value = get<double>(L, v.Index());
		}
		break;

	case LUA_TSTRING:
		Type = CachedUserVariableType::String;
		Value = get<FixedString>(L, v.Index());
		break;

	case LUA_TTABLE:
		Type = CachedUserVariableType::Reference;
		Value = RegistryEntry(L, v);
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
	Dirty = o.Dirty;
	Value = std::move(o.Value);
}

CachedUserVariable& CachedUserVariable::operator = (CachedUserVariable&& o)
{
	Type = o.Type;
	Dirty = o.Dirty;
	Value = o.Value;

	return *this;
}

void CachedUserVariable::ParseReference(lua_State* L, StringView json)
{
	if (json::Parse(L, json)) {
		Value = RegistryEntry(L, -1);
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
	case CachedUserVariableType::Boolean: push(L, std::get<bool>(Value)); break;
	case CachedUserVariableType::Int64: push(L, std::get<int64_t>(Value)); break;
	case CachedUserVariableType::Double: push(L, std::get<double>(Value)); break;
	case CachedUserVariableType::String: push(L, std::get<FixedString>(Value)); break;
	case CachedUserVariableType::Reference: push(L, std::get<RegistryEntry>(Value)); break;
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
	case CachedUserVariableType::Boolean:
		return std::get<bool>(Value) != std::get<bool>(o.Value);
	case CachedUserVariableType::Int64:
		return std::get<int64_t>(Value) != std::get<int64_t>(o.Value);
	case CachedUserVariableType::Double:
		return std::get<double>(Value) != std::get<double>(o.Value);
	case CachedUserVariableType::String:
		return std::get<FixedString>(Value) != std::get<FixedString>(o.Value);
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
		
	case CachedUserVariableType::Boolean:
		var.Type = UserVariableType::Boolean;
		var.Value = std::get<bool>(Value);
		break;
		
	case CachedUserVariableType::Int64:
		var.Type = UserVariableType::Int64;
		var.Value = std::get<int64_t>(Value);
		break;
		
	case CachedUserVariableType::Double:
		var.Type = UserVariableType::Double;
		var.Value = std::get<double>(Value);
		break;
		
	case CachedUserVariableType::String:
		var.Type = UserVariableType::String;
		var.Value = std::get<FixedString>(Value);
		break;
		
	case CachedUserVariableType::Reference:
		var.Value = StringifyReference(L);
		if (!std::get<STDString>(var.Value).empty()) {
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
	std::get<RegistryEntry>(Value).Push();
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
	auto it = vars_.try_get(entity);
	if (it) {
		auto valueIt = it->Vars.try_get(key);
		if (valueIt) {
			entityGuid = it->CachedGuid;
			return valueIt;
		}
	}

	return nullptr;
}

CachedUserVariable* CachedUserVariableManager::GetFromCache(EntityHandle entity, FixedString const& key)
{
	auto it = vars_.try_get(entity);
	if (it) {
		auto valueIt = it->Vars.try_get(key);
		return valueIt;
	}

	return nullptr;
}

CachedUserVariable* CachedUserVariableManager::PutCache(EntityHandle entity, FixedString const& key, Guid const& entityGuid, UserVariablePrototype const& proto, CachedUserVariable&& value, bool isWrite)
{
	CachedUserVariable* var;
	EntityVariables* vars;
	bool wasDirty{ false };

	vars = vars_.try_get(entity);
	if (vars) {
		var = vars->Vars.try_get(key);
		if (var) {
			wasDirty = var->Dirty;
			bool dirty = var->Dirty || (isWrite && var->LikelyChanged(value));
			*var = std::move(value);
			var->Dirty = dirty;
		} else {
			var = vars->Vars.set(key, std::move(value));
			var->Dirty = isWrite;
		}
	} else {
		vars = vars_.add_key(entity);
		if (entityGuid) {
			vars->CachedGuid = entityGuid;
		} else {
			vars->CachedGuid = global_.EntityToGuid(entity);
		}

		var = vars->Vars.set(key, std::move(value));
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

void CachedUserVariableManager::MoveToGlobal(lua_State* L, EntityHandle entity, FixedString const& key, UserVariablePrototype const& proto, CachedUserVariable& var)
{
	auto uuid = global_.EntityToGuid(entity);
	if (!uuid) {
		OsiError("Setting user variable on entity '" << entity << "' that has no Guid! Changes will not be synced to the client and will be lost on save!");
		return;
	}

	auto userVar = var.ToUserVariable(L);
	userVar.Dirty = true;
	global_.Set(uuid, key, proto, std::move(userVar));
}

void CachedUserVariableManager::Set(lua_State* L, EntityHandle entity, FixedString const& key, UserVariablePrototype const& proto, CachedUserVariable && var)
{
	if (!proto.Has(UserVariableFlags::DontCache)) {
		USER_VAR_DBG("Set cached var %016llx/%s", entity.Handle, key.GetString());
		auto cachedVar = PutCache(entity, key, Guid{}, proto, std::move(var), true);

		if (cachedVar->Dirty && proto.NeedsSyncFor(isServer_) && proto.Has(UserVariableFlags::SyncOnWrite)) {
			USER_VAR_DBG("Set global var %016llx/%s", entity.Handle, key.GetString());
			cachedVar->Dirty = false;
			MoveToGlobal(L, entity, key, proto, *cachedVar);
		}
	} else {
		USER_VAR_DBG("Set global var %016llx/%s", entity.Handle, key.GetString());
		MoveToGlobal(L, entity, key, proto, var);
	}
}

void CachedUserVariableManager::Invalidate()
{
	vars_.clear();
	flushQueue_.clear();
}

void CachedUserVariableManager::Invalidate(EntityHandle entity, FixedString const& key)
{
	auto it = vars_.try_get(entity);
	if (it) {
		if (it->Vars.remove(key)) {
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
	auto it = vars_.try_get(modIndex);
	if (it) {
		auto valueIt = it->Vars.try_get(key);
		if (valueIt) {
			modUuid = it->CachedGuid;
			return valueIt;
		}
	}

	return nullptr;
}

CachedUserVariable* CachedModVariableManager::GetFromCache(uint32_t modIndex, FixedString const& key)
{
	auto it = vars_.try_get(modIndex);
	if (it) {
		return it->Vars.try_get(key);
	}

	return nullptr;
}

CachedUserVariable* CachedModVariableManager::PutCache(uint32_t modIndex, FixedString const& key, Guid const& modUuid, UserVariablePrototype const& proto, CachedUserVariable&& value, bool isWrite)
{
	CachedUserVariable* var;
	ModVariables* vars;
	bool wasDirty{ false };

	vars = vars_.try_get(modIndex);
	if (vars) {
		var = vars->Vars.try_get(key);
		if (var) {
			wasDirty = var->Dirty;
			bool dirty = var->Dirty || (isWrite && var->LikelyChanged(value));
			*var = std::move(value);
			var->Dirty = dirty;
		} else {
			var = vars->Vars.set(key, std::move(value));
			var->Dirty = isWrite;
		}
	} else {
		vars = vars_.add_key(modIndex);
		if (modUuid) {
			vars->CachedGuid = modUuid;
		} else {
			vars->CachedGuid = ModIndexToGuid(modIndex);
		}

		var = vars->Vars.set(key, std::move(value));
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
	auto it = vars_.try_get(modIndex);
	if (it) {
		if (it->Vars.remove(key)) {
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
