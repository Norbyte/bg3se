#include <stdafx.h>
#include <Extender/Shared/Hooks.h>
#include <Extender/ScriptExtender.h>
#include <GameDefinitions/Net.h>

BEGIN_SE()

decltype(Hooks::eocnet__ClientConnectMessage__Serialize)* decltype(Hooks::eocnet__ClientConnectMessage__Serialize)::gHook;

void Hooks::Startup()
{
	if (loaded_) {
		return;
	}

	auto& lib = gExtender->GetEngineHooks();
	lib.RPGStats__PreParseDataFolder.SetWrapper(&Hooks::OnParseDataFolder, this);
	eocnet__ClientConnectMessage__Serialize.SetWrapper(&Hooks::OnClientConnectMessage, this);
	
	loaded_ = true;
}

void Hooks::HookNetworkMessages(net::MessageFactory* factory)
{
	if (networkingInitialized_) {
		return;
	}

	if (factory->MessagePools.size() <= (unsigned)NetMessage::NETMSG_CLIENT_CONNECT) {
		ERR("MessageFactory not initialized yet");
		return;
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	auto clientConnect = factory->MessagePools[(unsigned)NetMessage::NETMSG_CLIENT_CONNECT]->Template;
	eocnet__ClientConnectMessage__Serialize.Wrap((*(net::Message::VMT**)clientConnect)->Serialize);

	DetourTransactionCommit();

	networkingInitialized_ = true;
}

void Hooks::OnParseDataFolder(stats::RPGStats::ParseStructureFolderProc* next, stats::RPGStats* self, Array<STDString>* paths)
{
	gExtender->GetStatLoadOrderHelper().OnLoadFinished();
	next(self, paths);

	LuaVirtualPin lua(gExtender->GetCurrentExtensionState());
	if (lua) {
		lua->OnStatsStructureLoaded();
	}
}

void Hooks::OnClientConnectMessage(net::Message::SerializeProc* wrapped, net::Message* msg, net::BitstreamSerializer* serializer)
{
	auto m = (net::ClientConnectMessage*)msg;
	if (serializer->IsWriting) {
		gExtender->GetClient().GetNetworkManager().OnClientConnectMessage(m);
	}

	wrapped(msg, serializer);
}

END_SE()
