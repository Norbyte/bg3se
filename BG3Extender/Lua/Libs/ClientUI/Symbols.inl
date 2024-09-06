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
	TypeInfo Uri;
	TypeInfo Color;
	TypeInfo Vector2;
	TypeInfo Vector3;
	TypeInfo Point;
	TypeInfo Rect;
	TypeInfo LocaString;
	TypeInfo TypeConst;
	TypeInfo TypePointer;
	TypeInfo TypePtr;
	TypeInfo CStringPtr;
	TypeInfo CTypePtr;

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
		Uri.Setup("Uri");
		Color.Setup("Color");
		Vector2.Setup("Vector2");
		Vector3.Setup("Vector3");
		Point.Setup("Point");
		Rect.Setup("Rect");
		LocaString.Setup("ls.LocaString");
		TypeConst.Setup("TypeConst");
		TypePointer.Setup("TypePointer");
		TypePtr.Setup("TypePtr");
		CStringPtr.Setup("const Char*");
		CTypePtr.Setup("const Type*");
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
	TypeClassInfo BaseCollection;
	TypeClassInfo BaseCommand;
	TypeClassInfo DependencyObject;
	TypeClassInfo TypeClass;
	TypeClassInfo TypeEnum;
	TypeClassInfo BoxedValue;
	TypeClassInfo UIElementData;
	TypeClassInfo DependencyData;
	TypeClassInfo FrameworkElement;

	TypeClassInfo InitialSubstate;
	TypeClassInfo StateEvent;
	TypeClassInfo StateWidget;
	TypeClassInfo State;
	TypeClassInfo Widget;
	TypeClassInfo DCWidget;

	void Setup()
	{
		BaseObject.Setup("BaseObject");
		BaseRefCounted.Setup("BaseRefCounted");
		BaseComponent.Setup("BaseComponent");
		BaseCollection.Setup("BaseCollection");
		BaseCommand.Setup("BaseCommand");
		DependencyObject.Setup("DependencyObject");
		TypeClass.Setup("TypeClass");
		TypeEnum.Setup("TypeEnum");
		BoxedValue.Setup("BoxedValue");
		UIElementData.Setup("UIElementData");
		DependencyData.Setup("DependencyData");
		FrameworkElement.Setup("FrameworkElement");

		InitialSubstate.Setup("ls.InitialSubstate");
		StateEvent.Setup("ls.StateEvent");
		StateWidget.Setup("ls.StateWidget");
		State.Setup("ls.State");
		Widget.Setup("ls.UIWidget");
		DCWidget.Setup("ui::DCWidget");
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

struct DeferredNames
{
	struct DeferredName
	{
		char const* Name;
		Symbol Sym;

		DeferredName(char const* name)
			: Name(name)
		{}

		Symbol Get()
		{
			if (!Sym) Sym = Symbol(Name, Symbol::NullIfNotFound{});
			return Sym;
		}

		operator Symbol()
		{
			return Get();
		}

		bool operator == (Symbol const& sym)
		{
			return Get() == sym;
		}
	};

	#define DEFN_NAME(k, v) DeferredName k{v};
	#include <Lua/Libs/ClientUI/Names.inl>
	#undef DEFN_NAME
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

		// LS classes have a different naming scheme
		SymbolInfo<ui::UIInitialSubstate>::Name = MakeNoesisSymbol("ls.InitialSubstate");
		SymbolInfo<ui::UIStateEvent>::Name = MakeNoesisSymbol("ls.StateEvent");
		SymbolInfo<ui::UIState>::Name = MakeNoesisSymbol("ls.State");
		SymbolInfo<ui::UIWidget>::Name = MakeNoesisSymbol("ls.UIWidget");
		SymbolInfo<ui::DCWidget>::Name = MakeNoesisSymbol("ui::DCWidget");

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
	DeferredNames DeferredNames;
	bool Initialized{ false };
};

ExtStaticSymbols gStaticSymbols;

Symbol MakeNoesisSymbol(char const* name)
{
	return gStaticSymbols.MakeSymbol(name);
}

END_BARE_NS()
