#include <stdafx.h>
#include "FunctionLibrary.h"
#include <OsirisProxy.h>
#include <random>
#include <regex>

namespace dse::esv
{
	FunctionHandle ForLoopEventHandle;
	FunctionHandle ForLoopObjectEventHandle;

	namespace func
	{
		void DebugLog(OsiArgumentDesc const & args)
		{
			auto msg = args[0].ToString();
			OsiMsg(msg);
		}

		STDString StringFmtTemp;

		char const * StringFormatArgNames[10] = {
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

		char const * StringFormatReplacements[10] = {
			"[1]",
			"[2]",
			"[3]",
			"[4]",
			"[5]",
			"[6]",
			"[7]",
			"[8]",
			"[9]",
			"[10]",
		};

		bool StringFormat(OsiArgumentDesc & args)
		{
			auto fmtString = args[0].String;
			auto & output = args[1];

			StringFmtTemp = fmtString;
			auto arg = args.NextParam->NextParam;
			auto argIndex = 0;
			while (arg != nullptr && argIndex < 10) {
				auto replace = StringFormatReplacements[argIndex];

				for (;;) {
					std::size_t pos = StringFmtTemp.find(replace);
					if (pos != STDString::npos) {
						StringFmtTemp = StringFmtTemp.substr(0, pos) + STDString(arg->Value.ToString())
							+ StringFmtTemp.substr(pos + 3);
					} else {
						break;
					}
				}

				argIndex++;
				arg = arg->NextParam;
			}

			output.Set(StringFmtTemp.c_str());
			return true;
		}

		bool Substring(OsiArgumentDesc & args)
		{
			StringFmtTemp = args[0].String;
			auto from = args[1].Int32;
			auto length = args[2].Int32;
			auto & output = args[3];

			if (from < 0) {
				if (-from <= StringFmtTemp.size()) {
					from = (int)StringFmtTemp.size() + from;
				} else {
					from = 0;
				}
			} else if (from > StringFmtTemp.size()) {
				from = (int)StringFmtTemp.size();
			}

			auto availableLength = (int)StringFmtTemp.size() - from;
			if (length < 0) {
				int to;
				if (-length <= StringFmtTemp.size()) {
					to = (int)StringFmtTemp.size() + length;
				} else {
					to = 0;
				}

				if (to < from) {
					to = from;
				}

				length = to - from;
			}

			if (length > availableLength) {
				length = availableLength;
			}

			if (from >= 0 && from <= StringFmtTemp.size()
				&& length >= 0 && length + from <= StringFmtTemp.size()) {
				StringFmtTemp = StringFmtTemp.substr(from, length);
				output.Set(StringFmtTemp.c_str());
				return true;
			} else {
				OsiError("Invalid bounds in NRD_Substring()");
				return false;
			}
		}

		bool RegexMatch(OsiArgumentDesc & args)
		{
			auto input = args[0].String;
			std::string regex(args[1].String);
			auto fullMatch = args[2].Int32;
			auto & output = args[3];

			std::replace(regex.begin(), regex.end(), '^', '#');

			try {
				std::regex re(regex);

				bool match;
				if (fullMatch != 0) {
					match = std::regex_match(input, re);
				} else {
					match = std::regex_search(input, re);
				}

				output.Set(match ? 1 : 0);
				return true;
			} catch (std::regex_error & e) {
				OsiError("Regular expression \"" << input << "\" invalid: " << e.what());
				return false;
			}
		}

		bool RegexReplace(OsiArgumentDesc & args)
		{
			auto input = args[0].String;
			std::string regex(args[1].String);
			auto replacement = args[2].String;
			auto & output = args[3];

			std::replace(regex.begin(), regex.end(), '^', '#');

			try {
				std::regex re(regex);
				StringFmtTemp = std::regex_replace(input, re, replacement);
				output.Set(StringFmtTemp.c_str());
				return true;
			} catch (std::regex_error & e) {
				OsiError("Regular expression \"" << input << "\" invalid: " << e.what());
				return false;
			}
		}

		bool StringCompare(OsiArgumentDesc & args)
		{
			auto a = args[0].String;
			auto b = args[1].String;

			int rel = strcmp(a, b);
			if (rel < 0) {
				args[2].Set(-1);
			}
			else if (rel == 0) {
				args[2].Set(0);
			}
			else {
				args[2].Set(1);
			}

			return true;
		}

		bool StringLength(OsiArgumentDesc & args)
		{
			auto str = args[0].String;
			args[1].Set((int32_t)strlen(str));
			return true;
		}

		bool StringToInt(OsiArgumentDesc & args)
		{
			auto a = args[0].String;
			try {
				args[1].Set(std::stoi(a));
				return true;
			}
			catch (std::invalid_argument &) {
				return false;
			}
		}

		bool StringToReal(OsiArgumentDesc & args)
		{
			auto a = args[0].String;
			try {
				args[1].Set(std::stof(a));
				return true;
			}
			catch (std::invalid_argument &) {
				return false;
			}
		}

		bool StringToGuidString(OsiArgumentDesc & args)
		{
			if (IsValidGuidString(args[0].String)) {
				args[1].Set(args[0].String);
				return true;
			} else {
				return false;
			}
		}

		// We need to keep the returned string alive
		// until it reaches Osi
		STDString gRealToStringTemp;

		bool RealToString(OsiArgumentDesc & args)
		{
			gRealToStringTemp = std::to_string(args[0].Float);
			args[1].Set(gRealToStringTemp.c_str());
			return true;
		}

		bool IntegerToString(OsiArgumentDesc & args)
		{
			gRealToStringTemp = std::to_string(args[0].Int32);
			args[1].Set(gRealToStringTemp.c_str());
			return true;
		}

		void ForLoop(OsiArgumentDesc const & args)
		{
			auto eventName = args[0].String;
			auto count = args[1].Int32;

			for (int32_t index = 0; index < count; index++) {
				auto eventArgs = OsiArgumentDesc::Create(OsiArgumentValue{ ValueType::String, eventName });
				eventArgs->Add(OsiArgumentValue{ (int64_t)index });

				gOsirisProxy->GetCustomFunctionInjector().ThrowEvent(ForLoopEventHandle, eventArgs);

				delete eventArgs;
			}
		}

		void ForLoopObject(OsiArgumentDesc const & args)
		{
			auto objectGuid = args[0].String;
			auto eventName = args[1].String;
			auto count = args[2].Int32;

			for (int32_t index = 0; index < count; index++) {
				auto eventArgs = OsiArgumentDesc::Create(OsiArgumentValue{ ValueType::GuidString, objectGuid });
				eventArgs->Add(OsiArgumentValue{ ValueType::String, eventName });
				eventArgs->Add(OsiArgumentValue{ (int64_t)index });

				gOsirisProxy->GetCustomFunctionInjector().ThrowEvent(ForLoopObjectEventHandle, eventArgs);

				delete eventArgs;
			}
		}
	}

	void CustomFunctionLibrary::RegisterHelperFunctions()
	{
		auto & functionMgr = osiris_.GetCustomFunctionManager();

		auto debugLog = std::make_unique<CustomCall>(
			"NRD_DebugLog",
			std::vector<CustomFunctionParam>{ { "Message", ValueType::None, FunctionArgumentDirection::In } },
			&func::DebugLog
		);
		functionMgr.Register(std::move(debugLog));

		auto stringCompare = std::make_unique<CustomQuery>(
			"NRD_StringCompare",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::String, FunctionArgumentDirection::In },
				{ "B", ValueType::String, FunctionArgumentDirection::In },
				{ "Result", ValueType::Integer, FunctionArgumentDirection::Out }
			},
			&func::StringCompare
		);
		functionMgr.Register(std::move(stringCompare));

		auto stringLength = std::make_unique<CustomQuery>(
			"NRD_StringLength",
			std::vector<CustomFunctionParam>{
				{ "String", ValueType::String, FunctionArgumentDirection::In },
				{ "Length", ValueType::Integer, FunctionArgumentDirection::Out }
			},
			&func::StringLength
		);
		functionMgr.Register(std::move(stringLength));

		for (auto i = 0; i <= 10; i++) {
			std::vector<CustomFunctionParam> args{
				{ "Format", ValueType::String, FunctionArgumentDirection::In },
				{ "Result", ValueType::String, FunctionArgumentDirection::Out },
			};
			for (auto arg = 0; arg < i; arg++) {
				args.push_back({ func::StringFormatArgNames[arg], ValueType::None, FunctionArgumentDirection::In });
			}

			auto fmtCall = std::make_unique<CustomQuery>(
				"NRD_StringFormat",
				args,
				&func::StringFormat
			);
			functionMgr.Register(std::move(fmtCall));
		}

		auto substring = std::make_unique<CustomQuery>(
			"NRD_Substring",
			std::vector<CustomFunctionParam>{
				{ "String", ValueType::String, FunctionArgumentDirection::In },
				{ "From", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Length", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Result", ValueType::String, FunctionArgumentDirection::Out }
			},
			&func::Substring
		);
		functionMgr.Register(std::move(substring));

		auto regexMatch = std::make_unique<CustomQuery>(
			"NRD_RegexMatch",
			std::vector<CustomFunctionParam>{
				{ "String", ValueType::String, FunctionArgumentDirection::In },
				{ "Regex", ValueType::String, FunctionArgumentDirection::In },
				{ "FullMatch", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Matched", ValueType::Integer, FunctionArgumentDirection::Out }
			},
			&func::RegexMatch
		);
		functionMgr.Register(std::move(regexMatch));

		auto regexReplace = std::make_unique<CustomQuery>(
			"NRD_RegexReplace",
			std::vector<CustomFunctionParam>{
				{ "String", ValueType::String, FunctionArgumentDirection::In },
				{ "Regex", ValueType::String, FunctionArgumentDirection::In },
				{ "Replacement", ValueType::String, FunctionArgumentDirection::In },
				{ "Result", ValueType::String, FunctionArgumentDirection::Out }
			},
			&func::RegexReplace
		);
		functionMgr.Register(std::move(regexReplace));

		auto stringToInt = std::make_unique<CustomQuery>(
			"NRD_StringToInt",
			std::vector<CustomFunctionParam>{
				{ "String", ValueType::String, FunctionArgumentDirection::In },
				{ "Result", ValueType::Integer, FunctionArgumentDirection::Out }
			},
			&func::StringToInt
		);
		functionMgr.Register(std::move(stringToInt));

		auto stringToReal = std::make_unique<CustomQuery>(
			"NRD_StringToReal",
			std::vector<CustomFunctionParam>{
				{ "String", ValueType::String, FunctionArgumentDirection::In },
				{ "Result", ValueType::Real, FunctionArgumentDirection::Out }
			},
			&func::StringToReal
		);
		functionMgr.Register(std::move(stringToReal));

		auto stringToGuidString = std::make_unique<CustomQuery>(
			"NRD_GuidString",
			std::vector<CustomFunctionParam>{
				{ "String", ValueType::String, FunctionArgumentDirection::In },
				{ "Result", ValueType::GuidString, FunctionArgumentDirection::Out }
			},
			&func::StringToGuidString
		);
		functionMgr.Register(std::move(stringToGuidString));

		auto realToString = std::make_unique<CustomQuery>(
			"NRD_RealToString",
			std::vector<CustomFunctionParam>{
				{ "Real", ValueType::Real, FunctionArgumentDirection::In },
				{ "Result", ValueType::String, FunctionArgumentDirection::Out }
			},
			&func::RealToString
		);
		functionMgr.Register(std::move(realToString));

		auto integerToString = std::make_unique<CustomQuery>(
			"NRD_IntegerToString",
			std::vector<CustomFunctionParam>{
				{ "Integer", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Result", ValueType::String, FunctionArgumentDirection::Out }
			},
			&func::IntegerToString
		);
		functionMgr.Register(std::move(integerToString));

		auto startLoop = std::make_unique<CustomCall>(
			"NRD_ForLoop",
			std::vector<CustomFunctionParam>{ 
				{ "Event", ValueType::String, FunctionArgumentDirection::In },
				{ "Count", ValueType::Integer, FunctionArgumentDirection::In },
			},
			&func::ForLoop
		);
		functionMgr.Register(std::move(startLoop));

		auto startLoop2 = std::make_unique<CustomCall>(
			"NRD_ForLoop",
			std::vector<CustomFunctionParam>{
				{ "Object", ValueType::GuidString, FunctionArgumentDirection::In },
				{ "Event", ValueType::String, FunctionArgumentDirection::In },
				{ "Count", ValueType::Integer, FunctionArgumentDirection::In },
			},
			&func::ForLoopObject
		);
		functionMgr.Register(std::move(startLoop2));

		auto forLoopEvent = std::make_unique<CustomEvent>(
			"NRD_Loop",
			std::vector<CustomFunctionParam>{
				{ "Event", ValueType::String, FunctionArgumentDirection::In },
				{ "Num", ValueType::Integer, FunctionArgumentDirection::In },
			}
		);
		ForLoopEventHandle = functionMgr.Register(std::move(forLoopEvent));

		auto forLoopObjectEvent = std::make_unique<CustomEvent>(
			"NRD_Loop",
			std::vector<CustomFunctionParam>{
				{ "Object", ValueType::GuidString, FunctionArgumentDirection::In },
				{ "Event", ValueType::String, FunctionArgumentDirection::In },
				{ "Num", ValueType::Integer, FunctionArgumentDirection::In },
			}
		);
		ForLoopObjectEventHandle = functionMgr.Register(std::move(forLoopObjectEvent));
	}

}
