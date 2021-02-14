#pragma once

namespace bg3se
{
	struct esv::GameActionManager
	{
		ls::NetworkObjectFactory b;
		esv::GameActionManagerInner Inner;
	};

	struct esv::GameActionManagerInner
	{
		__int64 VMT;
		__int64 field_8;
		ObjectSet Actions;
		__int64 field_28;
	};

	struct __declspec(align(8)) esv::GameAction
	{
		esv::GameActionVMT* VMT;
		FixedString field_8;
		NetId NetID;
		__int64 field_18;
		int field_20;
		__declspec(align(8)) int field_28;
		__declspec(align(8)) int StoryActionID;
		__declspec(align(8)) ls::ActionOriginator Originator;
		ObjectHandle MyHandle_M;
		char field_58;
		char Active;
		char Dirty;
		float ActivateTimer;
	};
}