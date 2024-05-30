#include <Extender/Shared/SavegameSerializer.h>
#include <Extender/Version.h>

BEGIN_SE()

void SavegameSerializer::SavegameVisit(ObjectVisitor* visitor)
{
	if (visitor->EnterRegion(GFS.strScriptExtenderSave)) {
		uint32_t version = SavegameVersion;
		visitor->VisitUInt32(GFS.strExtenderVersion, version, 0);
		if (visitor->IsReading()) {
			if (version > SavegameVersion) {
				ERR("Savegame version too new! Extender version %d, savegame version %d; savegame data will not be loaded!", SavegameVersion, version);
				std::stringstream ss;
				ss << "Could not load Script Extender save data - savegame is newer than the currently installed extender!<br>";
				ss << "Extender version v" << SavegameVersion << ", savegame version v" << version;
				gExtender->GetLibraryManager().ShowStartupError(ss.str().c_str(), true, false);
			} else {
				Serialize(visitor, version);
			}
		} else {
			Serialize(visitor, SavegameVersion);
		}

		visitor->ExitRegion(GFS.strScriptExtenderSave);
	}
}


void SavegameSerializer::Serialize(ObjectVisitor* visitor, uint32_t version)
{
	SerializePersistentVariables(visitor, version);
	// FIXME - persistent stat support disabled for now
	// SerializeStatObjects(visitor, version);

	auto& state = gExtender->GetServer().GetExtensionState();

	if (version >= SavegameVerAddedUserVars) {
		state.GetUserVariables().SavegameVisit(visitor);
		state.GetModVariables().SavegameVisit(visitor);
	}

	if (version >= SavegameVerAddedTimers) {
		auto lua = state.GetLua();
		if (lua) {
			lua->GetTimers().SavegameVisit(visitor);
		}
	}
}


void SavegameSerializer::SerializePersistentVariables(ObjectVisitor* visitor, uint32_t version)
{
	STDString nullStr;
	if (visitor->EnterNode(GFS.strLuaVariables, GFS.strEmpty)) {
		auto const& configs = gExtender->GetServer().GetExtensionState().GetConfigs();

		if (visitor->IsReading()) {
			std::unordered_map<FixedString, STDString> variables;
			uint32_t numMods{ 0 };
			visitor->VisitCount(GFS.strMod, &numMods);

			for (uint32_t i = 0; i < numMods; i++) {
				if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
					FixedString modId;
					visitor->VisitFixedString(GFS.strModId, modId, GFS.strEmpty);
					STDString modVars;
					visitor->VisitSTDString(GFS.strLuaVariables, modVars, nullStr);

					variables.insert(std::make_pair(modId, modVars));
					visitor->ExitNode(GFS.strMod);
				}
			}

			RestorePersistentVariables(variables);
		} else {
			auto& state = gExtender->GetServer().GetExtensionState();
			auto mods = state.GetPersistentVarMods();

			for (auto const& modId : mods) {
				auto vars = state.GetModPersistentVars(modId);
				if (vars) {
					if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
						FixedString modIdTemp = modId;
						visitor->VisitFixedString(GFS.strModId, modIdTemp, GFS.strEmpty);
						visitor->VisitSTDString(GFS.strLuaVariables, *vars, nullStr);
						visitor->ExitNode(GFS.strMod);
					}
				}
			}
		}

		visitor->ExitNode(GFS.strLuaVariables);
	}
}

void SavegameSerializer::RestorePersistentVariables(std::unordered_map<FixedString, STDString> const& variables)
{
	auto& state = gExtender->GetServer().GetExtensionState();
	for (auto const& var : variables) {
		state.RestoreModPersistentVars(var.first, var.second);
	}
}


void SavegameSerializer::SerializeStatObjects(ObjectVisitor* visitor, uint32_t version)
{
	STDString nullStr;
	if (visitor->EnterNode(GFS.strDynamicStats, GFS.strEmpty)) {
		if (visitor->IsReading()) {
			std::unordered_map<FixedString, STDString> variables;
			uint32_t numObjects{ 0 };
			visitor->VisitCount(GFS.strStatObject, &numObjects);

			for (uint32_t i = 0; i < numObjects; i++) {
				if (visitor->EnterNode(GFS.strStatObject, GFS.strStatId)) {
					FixedString statId, statType;
					ScratchBuffer blob;
					visitor->VisitFixedString(GFS.strStatId, statId, GFS.strEmpty);
					visitor->VisitFixedString(GFS.strStatType, statType, GFS.strEmpty);
					visitor->VisitBuffer(GFS.strBlob, blob);
					RestoreStatObject(statId, statType, blob);
					visitor->ExitNode(GFS.strStatObject);
				}
			}
		} else {
			auto const& statIds = gExtender->GetServer().GetExtensionState().GetPersistentStats();

			for (auto statId : statIds) {
				FixedString statType;
				ScratchBuffer blob;
				if (SerializeStatObject(statId, statType, blob)) {
					if (visitor->EnterNode(GFS.strStatObject, GFS.strStatId)) {
						visitor->VisitFixedString(GFS.strStatId, statId, GFS.strEmpty);
						visitor->VisitFixedString(GFS.strStatType, statType, GFS.strEmpty);
						visitor->VisitBuffer(GFS.strBlob, blob);
						visitor->ExitNode(GFS.strStatObject);
					}
				}
			}
		}

		visitor->ExitNode(GFS.strDynamicStats);
	}
}

void SavegameSerializer::RestoreStatObject(FixedString const& statId, FixedString const& statType, ScratchBuffer const& blob)
{
	auto stats = GetStaticSymbols().GetStats();

	auto object = stats->Objects.Find(statId);
	if (object) {
		auto modifier = stats->ModifierLists.Find(object->ModifierListIndex);
		if (modifier->Name != statType) {
			OsiError("Stat entry '" << statId << "' is a '" << statType << "' in the save, but a '" 
				<< modifier->Name << "' in the game. It will not be loaded from the savegame!");
			return;
		}
	} else {
		auto newObject = stats->CreateObject(statId, statType);
		if (!newObject) {
			OsiError("Couldn't construct stats entry '" << statId << "' of type '" << statType 
				<< "'! It will not be loaded from the savegame!");
			return;
		}
		object = *newObject;
	}
	/*
#if defined(NDEBUG)
	MsgS2CSyncStat msg;
#else
	// Workaround for different debug/release CRT runtimes between protobuf and the extender in debug mode
	MsgS2CSyncStat& msg = *GameAlloc<MsgS2CSyncStat>();
#endif
	if (!msg.ParseFromArray(blob.Buffer, blob.Size)) {
		OsiError("Unable to parse protobuf payload for stat '" << statId << "'! It will not be loaded from the savegame!");
		return;
	}

	object->FromProtobuf(msg);
	stats->SyncWithPrototypeManager(object);*/
	object->BroadcastSyncMessage(true);
	gExtender->GetServer().GetExtensionState().MarkDynamicStat(statId);
	gExtender->GetServer().GetExtensionState().MarkPersistentStat(statId);
}

bool SavegameSerializer::SerializeStatObject(FixedString const& statId, FixedString& statType, ScratchBuffer& blob)
{
	auto stats = GetStaticSymbols().GetStats();

	auto object = stats->Objects.Find(statId);
	if (!object) {
		OsiError("Stat entry '" << statId << "' is marked as modified but cannot be found! It will not be written to the savegame!");
		return false;
	}

	auto modifier = stats->ModifierLists.Find(object->ModifierListIndex);
	statType = modifier->Name;

	/*MsgS2CSyncStat msg;
	object->ToProtobuf(&msg);
	uint32_t size = (uint32_t)msg.ByteSizeLong();
	blob.Size = size;
	blob.Buffer = GameAllocRaw(size);
	msg.SerializeToArray(blob.Buffer, size);*/
	return true;
}

END_SE()
