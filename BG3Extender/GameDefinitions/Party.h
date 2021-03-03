#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Misc.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Stats/Functors.h>

namespace bg3se::esv
{
	struct Character;

	struct PartyGroup
	{
		ObjectSet<Character*> Characters;
		ObjectSet<void*> field_18;
		ObjectSet<ObjectHandle> UnknownHandles;
	};


	struct PartyNPCData
	{
		float IdentifyPriceModifier;
		float RepairPriceModifier;
		float ShopPriceModifier;
	};


	struct Party
	{
		void* VMT;
		FixedString GUID;
		NetId NetID;
		ObjectHandle Handle;
		ObjectSet<Character*> Players;
		ObjectSet<Character*> Characters;
		ObjectSet<PartyGroup*> Groups;
		ObjectSet<int> field_68;
		ObjectSet<int> field_80;
		ObjectSet<FixedString> field_98;
		RefMap<ObjectHandle, PartyNPCData> NPCData;
		ObjectSet<ObjectHandle> InventoryViews_M;
		ObjectSet<FixedString> OS_FS2;
		ObjectSet<ObjectHandle> OS_OH2;
		__int64 field_F8;
		__int64 field_100;
		__int64 field_108;
		STDString field_110;
		char field_130;
		char field_131;
		char field_132;
		__declspec(align(8)) bool field_138;
		char field_139;
		char field_13A;
		char field_13B;
		int field_13C;
	};


	struct PartyManager : public NetworkObjectFactoryBase
	{
		// FIXME - 2 fields before factory!
		void* VMT;
		__int64 field_8;
		// end FIXME
		__int64 field_130;
		__int64 field_138;
		__int64 field_140;
		__int64 field_148;
		Array<void*> field_150;
		__int64 field_168;
		__int64 field_170;
		__int64 field_178;
		RefMap<FixedString, ObjectHandle> PartiesByCharacterGuid;
		ObjectSet<Character*> CharactersInParties;
		ObjectSet<Character*> AllCharacters;
		EntityWorldHandle GameMasterPartyHandle_M;
		FixedString TeleportTargetID_M;
		STDString TeleportArriveEvent_M;
		int64_t TeleportNumRequest_M;
		int field_1F8;
		__int64 field_200;
		int field_208;
		char field_20C;
	};
}
