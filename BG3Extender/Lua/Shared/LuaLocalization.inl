#include <stdafx.h>
#include <OsirisProxy.h>
#include <ScriptHelpers.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua::utils
{
	void AddVoiceMetaData(lua_State * L, char const* speakerGuid, char const* translatedStringKey, char const* source, 
		float length, std::optional<int> priority)
	{
		/*auto speakerMgr = GetStaticSymbols().eoc__SpeakerManager;
		if (speakerMgr == nullptr || *speakerMgr == nullptr) {
			OsiError("Speaker manager not initialized!");
			return;
		}

		auto speaker = (*speakerMgr)->SpeakerMetaDataHashMap->Insert(MakeFixedString(speakerGuid));
		auto voiceMeta = speaker->Insert(MakeFixedString(translatedStringKey));
		voiceMeta->CodecID = 4;
		voiceMeta->IsRecorded = true;
		voiceMeta->Length = (float)length;
		voiceMeta->Priority = priority ? *priority : 0;

		auto path = GetStaticSymbols().ToPath(source, PathRootType::Data);
		voiceMeta->Source.Name = path;*/
	}

	WrapLuaFunction(AddVoiceMetaData)

	STDString GetTranslatedString(lua_State* L, char const* translatedStringKey, std::optional<char const*> fallbackText)
	{
		STDWString translated;
		if (script::GetTranslatedString(translatedStringKey, translated)) {
			return ToUTF8(translated);
		} else {
			return fallbackText ? *fallbackText : "";
		}
	}

	WrapLuaFunction(GetTranslatedString)

	std::tuple<STDString, FixedString> GetTranslatedStringFromKey(lua_State* L, FixedString key)
	{
		/*TranslatedString translated;
		if (script::GetTranslatedStringFromKey(key, translated)) {
			return { ToUTF8(translated.Handle.ReferenceString), translated.Handle.Handle };
		} else {*/
			return {};
		//}
	}

	WrapLuaFunction(GetTranslatedStringFromKey)

	unsigned NextDynamicStringHandleId{ 1 };

	std::optional<STDString> CreateTranslatedString(lua_State* L, char const* keyStr, char const* value)
	{
		auto key = FixedString(keyStr);

		STDString handleStr = "ExtStr_";
		handleStr += std::to_string(NextDynamicStringHandleId++);
		FixedString handle(handleStr.c_str());

		if (script::CreateTranslatedStringKey(key, handle)) {
			STDWString str(FromUTF8(value));
			if (script::CreateTranslatedString(handle, str)) {
				return handleStr;
			}
		}

		return {};
	}

	WrapLuaFunction(CreateTranslatedString)

	bool CreateTranslatedStringKey(lua_State* L, char const* keyStr, char const* handleStr)
	{
		FixedString key(keyStr);
		FixedString handle(handleStr);
		return script::CreateTranslatedStringKey(key, handle);
	}

	WrapLuaFunction(CreateTranslatedStringKey)

	bool CreateTranslatedStringHandle(lua_State* L, char const* handleStr, char const* value)
	{
		FixedString handle(handleStr);

		STDWString str(FromUTF8(value));
		return script::CreateTranslatedString(handle, str);
	}

	WrapLuaFunction(CreateTranslatedStringHandle)

	void RegisterLocalizationLib(lua_State * L)
	{
		static const luaL_Reg locaLib[] = {
			{"AddVoiceMetaData", AddVoiceMetaDataWrapper},
			{"GetTranslatedString", GetTranslatedStringWrapper},
			{"GetTranslatedStringFromKey", GetTranslatedStringFromKeyWrapper},
			{"CreateTranslatedString", CreateTranslatedStringWrapper},
			{"CreateTranslatedStringKey", CreateTranslatedStringKeyWrapper},
			{"CreateTranslatedStringHandle", CreateTranslatedStringHandleWrapper},
			{0,0}
		};

		lua_getglobal(L, "Ext"); // stack: Ext
		luaL_newlib(L, locaLib); // stack: ext, lib
		lua_setfield(L, -2, "Localization");
		lua_pop(L, 1);
	}
}
