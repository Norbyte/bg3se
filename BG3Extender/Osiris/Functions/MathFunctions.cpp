#include <stdafx.h>
#include "FunctionLibrary.h"
#include <Extender/ScriptExtender.h>

namespace bg3se::esv
{
	namespace func
	{
		bool Random(OsiArgumentDesc & args)
		{
			auto min = args[0].Int32;
			auto max = args[1].Int32;

			std::uniform_int_distribution<int> dist(min, max);
			args[2].Set(dist(ExtensionState::Get().OsiRng));
			return true;
		}

		bool RandomReal(OsiArgumentDesc & args)
		{
			auto min = args[0].Float;
			auto max = args[1].Float;

			std::uniform_real_distribution<float> dist(min, max);
			args[2].Set(dist(ExtensionState::Get().OsiRng));
			return true;
		}

		bool Sin(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(sin(x));
			return true;
		}

		bool Cos(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(cos(x));
			return true;
		}

		bool Tan(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(tan(x));
			return true;
		}

		bool Round(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(round(x));
			return true;
		}

		bool Ceil(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(ceil(x));
			return true;
		}

		bool Floor(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(floor(x));
			return true;
		}

		bool Abs(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(abs(x));
			return true;
		}

		bool Pow(OsiArgumentDesc & args)
		{
			auto base = args[0].Float;
			auto exp = args[1].Int32;
			args[2].Set((float)pow(base, exp));
			return true;
		}

		bool Sqrt(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(sqrt(x));
			return true;
		}

		bool Exp(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(exp(x));
			return true;
		}

		bool Factorial(OsiArgumentDesc & args)
		{
			auto n = args[0].Int32;
			int32_t fact = 0;
			for (int32_t i = 1; i <= n; i++) {
				fact *= i;
			}

			args[1].Set(fact);
			return true;
		}

		bool Log(OsiArgumentDesc & args)
		{
			auto x = args[0].Float;
			args[1].Set(log(x));
			return true;
		}
		
		bool IsDivisible(OsiArgumentDesc & args)
		{
			auto n = args[0].Int32;
			auto d = args[1].Int32;

			if (d != 0 && n%d == 0) {
				args[2].Set(1);
			} else {
				args[2].Set(0);
			}

			return true;
		}

		bool Min3(OsiArgumentDesc& args)
		{
			auto a = args[0].Int32;
			auto b = args[1].Int32;
			auto c = args[2].Int32;

			auto min = std::min({ a, b, c });
			args[3].Set(min);

			return true;
		}

		bool MinReal3(OsiArgumentDesc& args)
		{
			auto a = args[0].Float;
			auto b = args[1].Float;
			auto c = args[2].Float;

			auto min = std::min({ a, b, c });
			args[3].Set(min);

			return true;
		}

		bool Min4(OsiArgumentDesc& args)
		{
			auto a = args[0].Int32;
			auto b = args[1].Int32;
			auto c = args[2].Int32;
			auto d = args[3].Int32;

			auto min = std::min({ a, b, c, d });
			args[4].Set(min);

			return true;
		}

		bool MinReal4(OsiArgumentDesc& args)
		{
			auto a = args[0].Float;
			auto b = args[1].Float;
			auto c = args[2].Float;
			auto d = args[3].Float;

			auto min = std::min({ a, b, c, d });
			args[4].Set(min);

			return true;
		}

		bool Max3(OsiArgumentDesc& args)
		{
			auto a = args[0].Int32;
			auto b = args[1].Int32;
			auto c = args[2].Int32;

			auto max = std::max({ a, b, c });
			args[3].Set(max);

			return true;
		}

		bool MaxReal3(OsiArgumentDesc& args)
		{
			auto a = args[0].Float;
			auto b = args[1].Float;
			auto c = args[2].Float;

			auto max = std::max({ a, b, c });
			args[3].Set(max);

			return true;
		}

		bool Max4(OsiArgumentDesc& args)
		{
			auto a = args[0].Int32;
			auto b = args[1].Int32;
			auto c = args[2].Int32;
			auto d = args[3].Int32;

			auto max = std::max({ a, b, c, d });
			args[4].Set(max);

			return true;
		}

		bool MaxReal4(OsiArgumentDesc& args)
		{
			auto a = args[0].Float;
			auto b = args[1].Float;
			auto c = args[2].Float;
			auto d = args[3].Float;

			auto max = std::max({ a, b, c, d });
			args[4].Set(max);

			return true;
		}
	}

#define MATH_QUERY1(funcName) { \
	auto fn = std::make_unique<CustomQuery>( \
		"NRD_" #funcName, \
			std::vector<CustomFunctionParam>{ \
				{ "In", ValueType::Real, FunctionArgumentDirection::In }, \
				{ "Out", ValueType::Real, FunctionArgumentDirection::Out } \
		}, \
		&func::funcName \
	); \
	functionMgr.Register(std::move(fn)); \
}

	void CustomFunctionLibrary::RegisterMathFunctions()
	{
		auto & functionMgr = osiris_.GetCustomFunctionManager();

		auto random = std::make_unique<CustomQuery>(
			"NRD_Random",
			std::vector<CustomFunctionParam>{
				{ "Min", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Max", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Result", ValueType::Integer, FunctionArgumentDirection::Out },
			},
			&func::Random
		);
		functionMgr.Register(std::move(random));

		auto randomReal = std::make_unique<CustomQuery>(
			"NRD_RandomReal",
			std::vector<CustomFunctionParam>{
				{ "Min", ValueType::Real, FunctionArgumentDirection::In },
				{ "Max", ValueType::Real, FunctionArgumentDirection::In },
				{ "Result", ValueType::Real, FunctionArgumentDirection::Out },
			},
			&func::RandomReal
		);
		functionMgr.Register(std::move(randomReal));

		auto factorial = std::make_unique<CustomQuery>(
			"NRD_Factorial",
			std::vector<CustomFunctionParam>{
				{ "In", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Out", ValueType::Integer, FunctionArgumentDirection::Out },
			},
			&func::Factorial
		);
		functionMgr.Register(std::move(factorial));

		auto pow = std::make_unique<CustomQuery>(
			"NRD_Pow",
			std::vector<CustomFunctionParam>{
				{ "Base", ValueType::Real, FunctionArgumentDirection::In },
				{ "Exp", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Out", ValueType::Real, FunctionArgumentDirection::Out },
			},
			&func::Pow
		);
		functionMgr.Register(std::move(pow));

		MATH_QUERY1(Sin)
		MATH_QUERY1(Cos)
		MATH_QUERY1(Tan)
		MATH_QUERY1(Round)
		MATH_QUERY1(Ceil)
		MATH_QUERY1(Floor)
		MATH_QUERY1(Abs)
		MATH_QUERY1(Sqrt)
		MATH_QUERY1(Exp)
		MATH_QUERY1(Log)

		auto isDivisible = std::make_unique<CustomQuery>(
			"NRD_IsDivisible",
			std::vector<CustomFunctionParam>{
				{ "Numerator", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Denominator", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Divisible", ValueType::Integer, FunctionArgumentDirection::Out },
			},
			&func::IsDivisible
		);
		functionMgr.Register(std::move(isDivisible));

		auto min4 = std::make_unique<CustomQuery>(
			"NRD_Min",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Integer, FunctionArgumentDirection::In },
				{ "B", ValueType::Integer, FunctionArgumentDirection::In },
				{ "C", ValueType::Integer, FunctionArgumentDirection::In },
				{ "D", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Result", ValueType::Integer, FunctionArgumentDirection::Out },
			},
			&func::Min4
		);
		functionMgr.Register(std::move(min4));

		auto min3 = std::make_unique<CustomQuery>(
			"NRD_Min",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Integer, FunctionArgumentDirection::In },
				{ "B", ValueType::Integer, FunctionArgumentDirection::In },
				{ "C", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Result", ValueType::Integer, FunctionArgumentDirection::Out },
			},
			&func::Min3
		);
		functionMgr.Register(std::move(min3));

		auto minReal4 = std::make_unique<CustomQuery>(
			"NRD_MinReal",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Real, FunctionArgumentDirection::In },
				{ "B", ValueType::Real, FunctionArgumentDirection::In },
				{ "C", ValueType::Real, FunctionArgumentDirection::In },
				{ "D", ValueType::Real, FunctionArgumentDirection::In },
				{ "Result", ValueType::Real, FunctionArgumentDirection::Out },
			},
			&func::MinReal4
		);
		functionMgr.Register(std::move(minReal4));

		auto minReal3 = std::make_unique<CustomQuery>(
			"NRD_MinReal",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Real, FunctionArgumentDirection::In },
				{ "B", ValueType::Real, FunctionArgumentDirection::In },
				{ "C", ValueType::Real, FunctionArgumentDirection::In },
				{ "Result", ValueType::Real, FunctionArgumentDirection::Out },
			},
			&func::MinReal3
		);
		functionMgr.Register(std::move(minReal3));

		auto max4 = std::make_unique<CustomQuery>(
			"NRD_Max",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Integer, FunctionArgumentDirection::In },
				{ "B", ValueType::Integer, FunctionArgumentDirection::In },
				{ "C", ValueType::Integer, FunctionArgumentDirection::In },
				{ "D", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Result", ValueType::Integer, FunctionArgumentDirection::Out },
			},
			&func::Max4
		);
		functionMgr.Register(std::move(max4));

		auto max3 = std::make_unique<CustomQuery>(
			"NRD_Max",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Integer, FunctionArgumentDirection::In },
				{ "B", ValueType::Integer, FunctionArgumentDirection::In },
				{ "C", ValueType::Integer, FunctionArgumentDirection::In },
				{ "Result", ValueType::Integer, FunctionArgumentDirection::Out },
			},
			&func::Max3
		);
		functionMgr.Register(std::move(max3));

		auto maxReal4 = std::make_unique<CustomQuery>(
			"NRD_MaxReal",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Real, FunctionArgumentDirection::In },
				{ "B", ValueType::Real, FunctionArgumentDirection::In },
				{ "C", ValueType::Real, FunctionArgumentDirection::In },
				{ "D", ValueType::Real, FunctionArgumentDirection::In },
				{ "Result", ValueType::Real, FunctionArgumentDirection::Out },
			},
			&func::MaxReal4
		);
		functionMgr.Register(std::move(maxReal4));

		auto maxReal3 = std::make_unique<CustomQuery>(
			"NRD_MaxReal",
			std::vector<CustomFunctionParam>{
				{ "A", ValueType::Real, FunctionArgumentDirection::In },
				{ "B", ValueType::Real, FunctionArgumentDirection::In },
				{ "C", ValueType::Real, FunctionArgumentDirection::In },
				{ "Result", ValueType::Real, FunctionArgumentDirection::Out },
			},
			&func::MaxReal3
		);
		functionMgr.Register(std::move(maxReal3));
	}

}
