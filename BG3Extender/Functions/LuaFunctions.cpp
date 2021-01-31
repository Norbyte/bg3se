#include <stdafx.h>
#include <OsirisProxy.h>
#include "FunctionLibrary.h"
#include <Version.h>

namespace dse::esv
{
	namespace func
	{
		void OsiLuaReset(OsiArgumentDesc const & args)
		{
			// _BootstrapMods parameter not used anymore
			bool resetServer = true;
			bool resetClient = true;
			if (args.Count() == 3) {
				resetServer = args[1].Int32 == 1;
				resetClient = args[2].Int32 == 1;
			}

#if defined(OSI_EOCAPP)
			if (!gOsirisProxy->GetConfig().DeveloperMode) {
				OsiError("NRD_LuaReset is only callable in developer mode");
				return;
			}
#endif

			gOsirisProxy->ResetLuaState(resetServer, resetClient);
		}

		void OsiLuaLoad(OsiArgumentDesc const & args)
		{
			LuaServerPin lua(ExtensionState::Get());
			if (!lua) {
				OsiErrorS("Called when the Lua VM has not been initialized!");
				return;
			}

			auto mod = args[0].String;
			auto fileName = args[1].String;

			if (strstr(fileName, "..") != nullptr) {
				OsiErrorS("Illegal file name");
				return;
			}

			ExtensionState::Get().LuaLoadModScript(mod, fileName);
		}

		void OsiLuaCall(OsiArgumentDesc const & args)
		{
			LuaServerPin lua(ExtensionState::Get());
			if (!lua) {
				OsiErrorS("Called when the Lua VM has not been initialized!");
				return;
			}

			auto func = args[0].String;
			auto numArgs = args.Count() - 1;
			std::vector<OsiArgumentValue> luaArgs;
			luaArgs.resize(numArgs);

			for (uint32_t i = 0; i < numArgs; i++) {
				luaArgs[i] = args[i + 1];
			}

			lua->Call(nullptr, func, luaArgs);
		}

		void OsiLuaModCall(OsiArgumentDesc const& args)
		{
			LuaServerPin lua(ExtensionState::Get());
			if (!lua) {
				OsiErrorS("Called when the Lua VM has not been initialized!");
				return;
			}

			auto mod = args[0].String;
			auto func = args[1].String;
			auto numArgs = args.Count() - 2;
			std::vector<OsiArgumentValue> luaArgs;
			luaArgs.resize(numArgs);

			for (uint32_t i = 0; i < numArgs; i++) {
				luaArgs[i] = args[i + 2];
			}

			lua->Call(mod, func, luaArgs);
		}

		char const * QueryArgNames[10] = {
			"Arg1",
			"Arg2",
			"Arg3",
			"Arg4",
			"Arg5",
			"Arg6",
			"Arg7",
			"Arg8",
			"Arg9",
			"Arg10"
		};

		char const * QueryOutArgNames[5] = {
			"Out1",
			"Out2",
			"Out3",
			"Out4",
			"Out5"
		};

		template <uint32_t TInParams>
		bool OsiLuaQuery(OsiArgumentDesc & args)
		{
			LuaServerPin lua(ExtensionState::Get());
			if (!lua) {
				OsiErrorS("Called when the Lua VM has not been initialized!");
				return false;
			}

			auto func = args[0].String;
			auto numArgs = args.Count() - 1;

			std::vector<CustomFunctionParam> signature;
			signature.reserve(numArgs);
			for (uint32_t i = 0; i < TInParams; i++) {
				signature.push_back(CustomFunctionParam{
					QueryArgNames[i], args[i + 1].TypeId, FunctionArgumentDirection::In
				});
			}

			for (uint32_t i = 0; i < numArgs - TInParams; i++) {
				signature.push_back(CustomFunctionParam{
					QueryOutArgNames[i], args[i + TInParams + 1].TypeId, FunctionArgumentDirection::Out
				});
			}

			return lua->Query(nullptr, func, nullptr, signature, *args.NextParam);
		}

		template <uint32_t TInParams>
		bool OsiLuaModQuery(OsiArgumentDesc & args)
		{
			LuaServerPin lua(ExtensionState::Get());
			if (!lua) {
				OsiErrorS("Called when the Lua VM has not been initialized!");
				return false;
			}

			auto mod = args[0].String;
			auto func = args[1].String;
			auto numArgs = args.Count() - 2;

			std::vector<CustomFunctionParam> signature;
			signature.reserve(numArgs);
			for (uint32_t i = 0; i < TInParams; i++) {
				signature.push_back(CustomFunctionParam{
					QueryArgNames[i], args[i + 2].TypeId, FunctionArgumentDirection::In
				});
			}

			for (uint32_t i = 0; i < numArgs - TInParams; i++) {
				signature.push_back(CustomFunctionParam{
					QueryOutArgNames[i], args[i + TInParams + 2].TypeId, FunctionArgumentDirection::Out
				});
			}

			return lua->Query(mod, func, nullptr, signature, *args.NextParam->NextParam);
		}
	}

	template <unsigned TInParams>
	void RegisterLuaQueries(CustomFunctionManager & functionMgr)
	{
		STDString procName = "NRD_LuaQuery";
		procName += std::to_string(TInParams);

		for (uint32_t out = 0; out <= 5; out++) {
			std::vector<CustomFunctionParam> args{
				{ "Func", ValueType::String, FunctionArgumentDirection::In }
			};

			for (uint32_t arg = 0; arg < TInParams; arg++) {
				args.push_back({ func::QueryArgNames[arg], ValueType::None, FunctionArgumentDirection::In });
			}

			for (uint32_t arg = 0; arg < out; arg++) {
				args.push_back({ func::QueryOutArgNames[arg], ValueType::None, FunctionArgumentDirection::Out });
			}

			auto luaQuery = std::make_unique<CustomQuery>(
				procName,
				args,
				&func::OsiLuaQuery<TInParams>
			);
			functionMgr.Register(std::move(luaQuery));
		}
	}

	template <unsigned TInParams>
	void RegisterLuaModQueries(CustomFunctionManager & functionMgr)
	{
		STDString procName = "NRD_ModQuery";
		procName += std::to_string(TInParams);

		for (uint32_t out = 0; out <= 5; out++) {
			std::vector<CustomFunctionParam> args{
				{ "Mod", ValueType::String, FunctionArgumentDirection::In },
				{ "Func", ValueType::String, FunctionArgumentDirection::In },
			};

			for (uint32_t arg = 0; arg < TInParams; arg++) {
				args.push_back({ func::QueryArgNames[arg], ValueType::None, FunctionArgumentDirection::In });
			}

			for (uint32_t arg = 0; arg < out; arg++) {
				args.push_back({ func::QueryOutArgNames[arg], ValueType::None, FunctionArgumentDirection::Out });
			}

			auto luaQuery = std::make_unique<CustomQuery>(
				procName,
				args,
				&func::OsiLuaModQuery<TInParams>
			);
			functionMgr.Register(std::move(luaQuery));
		}
	}

	void CustomFunctionLibrary::RegisterLuaFunctions()
	{
		auto & functionMgr = osiris_.GetCustomFunctionManager();
		auto luaReset = std::make_unique<CustomCall>(
			"NRD_LuaReset",
			std::vector<CustomFunctionParam>{
				{ "BootstrapMods", ValueType::Integer, FunctionArgumentDirection::In }
			},
			&func::OsiLuaReset
		);
		functionMgr.Register(std::move(luaReset));
		
		auto luaReset3 = std::make_unique<CustomCall>(
			"NRD_LuaReset",
			std::vector<CustomFunctionParam>{
				{ "BootstrapMods", ValueType::Integer, FunctionArgumentDirection::In },
				{ "ResetServer", ValueType::Integer, FunctionArgumentDirection::In },
				{ "ResetClient", ValueType::Integer, FunctionArgumentDirection::In }
			},
			&func::OsiLuaReset
		);
		functionMgr.Register(std::move(luaReset3));

		auto luaLoad = std::make_unique<CustomCall>(
			"NRD_LuaLoad",
			std::vector<CustomFunctionParam>{
				{ "ModNameGuid", ValueType::GuidString, FunctionArgumentDirection::In },
				{ "FileName", ValueType::String, FunctionArgumentDirection::In }
			},
			&func::OsiLuaLoad
		);
		functionMgr.Register(std::move(luaLoad));

		for (auto i = 0; i <= 10; i++) {
			std::vector<CustomFunctionParam> args{
				{ "Func", ValueType::String, FunctionArgumentDirection::In }
			};
			for (auto arg = 0; arg < i; arg++) {
				args.push_back({ func::QueryArgNames[arg], ValueType::None, FunctionArgumentDirection::In });
			}

			auto luaCall = std::make_unique<CustomCall>(
				"NRD_LuaCall",
				args,
				&func::OsiLuaCall
			);
			functionMgr.Register(std::move(luaCall));
		}

		for (auto i = 0; i <= 10; i++) {
			std::vector<CustomFunctionParam> args{
				{ "Mod", ValueType::String, FunctionArgumentDirection::In },
				{ "Func", ValueType::String, FunctionArgumentDirection::In }
			};
			for (auto arg = 0; arg < i; arg++) {
				args.push_back({ func::QueryArgNames[arg], ValueType::None, FunctionArgumentDirection::In });
			}

			auto luaCall = std::make_unique<CustomCall>(
				"NRD_ModCall",
				args,
				&func::OsiLuaModCall
			);
			functionMgr.Register(std::move(luaCall));
		}

		RegisterLuaQueries<0>(functionMgr);
		RegisterLuaQueries<1>(functionMgr);
		RegisterLuaQueries<2>(functionMgr);
		RegisterLuaQueries<3>(functionMgr);
		RegisterLuaQueries<4>(functionMgr);
		RegisterLuaQueries<5>(functionMgr);
		RegisterLuaQueries<6>(functionMgr);
		RegisterLuaQueries<7>(functionMgr);
		RegisterLuaQueries<8>(functionMgr);
		RegisterLuaQueries<9>(functionMgr);
		RegisterLuaQueries<10>(functionMgr);

		RegisterLuaModQueries<0>(functionMgr);
		RegisterLuaModQueries<1>(functionMgr);
		RegisterLuaModQueries<2>(functionMgr);
		RegisterLuaModQueries<3>(functionMgr);
		RegisterLuaModQueries<4>(functionMgr);
		RegisterLuaModQueries<5>(functionMgr);
		RegisterLuaModQueries<6>(functionMgr);
		RegisterLuaModQueries<7>(functionMgr);
		RegisterLuaModQueries<8>(functionMgr);
		RegisterLuaModQueries<9>(functionMgr);
		RegisterLuaModQueries<10>(functionMgr);
	}
}
