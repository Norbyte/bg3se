BEGIN_BARE_NS(Noesis)

bool SymbolsInitialized{ false };

template <class T>
struct SymbolInfo
{
	static Symbol Name;
};

#define FOR_NOESIS_TYPE(T) Symbol SymbolInfo<T>::Name;
FOR_EACH_NOESIS_TYPE()
#undef FOR_NOESIS_TYPE

Symbol MakeNoesisSymbol(char const* name);

struct StaticTypes
{
	struct TypeInfo
	{
		Symbol Name;
		Type const* Type;

		void Setup(char const* name)
		{
			Name = MakeNoesisSymbol(name);
			Type = Reflection::GetType(Name);
		}
	};

	TypeInfo Int8;
	TypeInfo Int16;
	TypeInfo Int32;
	TypeInfo Int64;
	TypeInfo UInt8;
	TypeInfo UInt16;
	TypeInfo UInt32;
	TypeInfo UInt64;
	TypeInfo Single;
	TypeInfo Double;
	TypeInfo Bool;
	TypeInfo String;

	void Setup()
	{
		Int8.Setup("Int8");
		Int16.Setup("Int16");
		Int32.Setup("Int32");
		Int64.Setup("Int64");
		UInt8.Setup("UInt8");
		UInt16.Setup("UInt16");
		UInt32.Setup("UInt32");
		UInt64.Setup("UInt64");
		Single.Setup("Single");
		Double.Setup("Double");
		Bool.Setup("Bool");
		String.Setup("String");
	}
};

struct StaticTypeClasses
{
	struct TypeClassInfo
	{
		Symbol Name;
		TypeClass const* Type;

		void Setup(char const* name)
		{
			Name = MakeNoesisSymbol(name);
			auto type = Reflection::GetType(Name);
			assert(type->GetClassType()->GetTypeId() == SymbolInfo<Noesis::TypeClass>::Name);
			Type = static_cast<Noesis::TypeClass const*>(type);
		}
	};

	TypeClassInfo BaseObject;
	TypeClassInfo BaseRefCounted;
	TypeClassInfo BaseComponent;
	TypeClassInfo TypeClass;
	TypeClassInfo TypeEnum;
	TypeClassInfo BoxedValue;
	TypeClassInfo UIElementData;
	TypeClassInfo DependencyData;
	TypeClassInfo FrameworkElement;

	void Setup()
	{
		BaseObject.Setup("BaseObject");
		BaseRefCounted.Setup("BaseRefCounted");
		BaseComponent.Setup("BaseComponent");
		TypeClass.Setup("TypeClass");
		TypeEnum.Setup("TypeEnum");
		BoxedValue.Setup("BoxedValue");
		UIElementData.Setup("UIElementData");
		DependencyData.Setup("DependencyData");
		FrameworkElement.Setup("FrameworkElement");
	}
};

struct EventNames
{
	#define DEFN_EVENT(e, args) Symbol e;
	#include <Lua/Libs/ClientUI/Events.inl>
	#undef DEFN_EVENT

	void Setup()
	{
		#define DEFN_EVENT(e, args) e = Symbol(#e, Symbol::NullIfNotFound{});
		#include <Lua/Libs/ClientUI/Events.inl>
		#undef DEFN_EVENT
	}
};

struct ExtStaticSymbols
{
	void Initialize()
	{
		if (Initialized) return;

		auto& strings = Noesis::GetSymbolManager()->Strings;
		for (uint32_t i = 0; i < strings.Size(); i++) {
			Symbols.insert(std::make_pair(strings[i], i));
		}

		#define FOR_NOESIS_TYPE(T) SymbolInfo<T>::Name = MakeNoesisSymbol(#T);
		FOR_EACH_NOESIS_TYPE()
		#undef FOR_NOESIS_TYPE

		Types.Setup();
		TypeClasses.Setup();
		Events.Setup();

		Initialized = true;
	}

	Symbol MakeSymbol(char const* name)
	{
		bg3se::STDString n(name);
		if (n.starts_with("Noesis::")) {
			n = n.substr(8);
		}

		auto it = Symbols.find(n);
		if (it == Symbols.end()) {
			return Symbol{};
		} else {
			return it->second;
		}
	}

	std::unordered_map<bg3se::STDString, Symbol> Symbols;
	StaticTypes Types;
	StaticTypeClasses TypeClasses;
	EventNames Events;
	bool Initialized{ false };
};

ExtStaticSymbols gStaticSymbols;

Symbol MakeNoesisSymbol(char const* name)
{
	return gStaticSymbols.MakeSymbol(name);
}

END_BARE_NS()
