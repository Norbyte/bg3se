#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "Utils.h"
#include "GameDefinitions/Osiris.h"

namespace bg3se
{
	enum EoCFunctionType
	{
		Call = 1,
		Query = 2,
		Event = 3
	};

	struct FunctionHandle
	{
		inline constexpr FunctionHandle()
			: handle(0) {}

		inline constexpr FunctionHandle(uint32_t id)
			: handle(id) {}

		inline constexpr FunctionHandle(EoCFunctionType type, uint32_t classIndex, uint32_t functionIndex)
			: handle(((uint32_t)type & 7) | ((classIndex & 3) << 29) | ((functionIndex & 0x1ffffff) << 3) | (1 << 31))
		{}

		inline constexpr EoCFunctionType type() const
		{
			return (EoCFunctionType)(handle & 7);
		}

		inline constexpr uint32_t classIndex() const
		{
			return (handle >> 29) & 3;
		}

		inline constexpr uint32_t functionIndex() const
		{
			return (handle >> 3) & 0x1ffffff;
		}

		inline constexpr operator uint32_t() const
		{
			return handle;
		}

		inline constexpr bool operator == (FunctionHandle const & f) const
		{
			return handle == f.handle;
		}

		uint32_t handle;
	};

	struct FunctionNameAndArity
	{
		STDString Name;
		uint32_t Arity;

		inline bool operator == (FunctionNameAndArity const & fn) const
		{
			return Name == fn.Name && Arity == fn.Arity;
		}
	};
}

namespace std
{
	template<> struct hash<bg3se::FunctionNameAndArity>
	{
		typedef bg3se::FunctionNameAndArity argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& fn) const noexcept
		{
			result_type const h1(std::hash<bg3se::STDString>{}(fn.Name));
			result_type const h2(std::hash<uint32_t>{}(fn.Arity));
			return h1 ^ (h2 << 1);
		}
	};

	template<> struct hash<bg3se::FunctionHandle>
	{
		typedef bg3se::FunctionHandle argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& fn) const noexcept
		{
			return std::hash<uint32_t>{}((unsigned int)fn);
		}
	};
}

namespace bg3se
{
	struct CustomFunctionParam
	{
		STDString Name;
		ValueType Type;
		FunctionArgumentDirection Dir;
	};

	class CustomFunction
	{
	public:
		inline CustomFunction(STDString const & name, std::vector<CustomFunctionParam> params)
			: name_(name), params_(params)
		{}

		virtual ~CustomFunction();

		virtual FunctionType GetType() = 0;

		inline STDString const & Name() const
		{
			return name_;
		}

		inline std::vector<CustomFunctionParam> const & Params() const
		{
			return params_;
		}

		inline FunctionHandle Handle() const
		{
			return handle_;
		}

		inline FunctionNameAndArity NameAndArity() const
		{
			return { name_, (unsigned)params_.size() };
		}

		inline void AssignHandle(FunctionHandle handle)
		{
			handle_ = handle;
		}

		bool ValidateArgs(OsiArgumentDesc const & params) const;
		void GenerateHeader(std::stringstream & ss) const;

	private:
		STDString name_;
		std::vector<CustomFunctionParam> params_;
		FunctionHandle handle_;

		bool ValidateParam(CustomFunctionParam const & param, OsiArgumentValue const & value) const;
	};

	class CustomCallBase : public CustomFunction
	{
	public:
		inline CustomCallBase(STDString const & name, std::vector<CustomFunctionParam> params)
			: CustomFunction(name, std::move(params))
		{}

		virtual bool Call(OsiArgumentDesc const & params) = 0;

		inline virtual FunctionType GetType() override
		{
			return FunctionType::Call;
		}
	};

	class CustomCall : public CustomCallBase
	{
	public:
		inline CustomCall(STDString const & name, std::vector<CustomFunctionParam> params,
			std::function<void (OsiArgumentDesc const &)> handler)
			: CustomCallBase(name, std::move(params)), handler_(handler)
		{}

		virtual bool Call(OsiArgumentDesc const & params) override;

	private:
		std::function<void(OsiArgumentDesc const &)> handler_;
	};

	class CustomQueryBase : public CustomFunction
	{
	public:
		inline CustomQueryBase(STDString const & name, std::vector<CustomFunctionParam> params)
			: CustomFunction(name, std::move(params))
		{}

		virtual bool Query(OsiArgumentDesc & params) = 0;

		inline virtual FunctionType GetType() override
		{
			return FunctionType::Query;
		}
	};

	class CustomQuery : public CustomQueryBase
	{
	public:
		inline CustomQuery(STDString const & name, std::vector<CustomFunctionParam> params,
			std::function<bool(OsiArgumentDesc &)> handler)
			: CustomQueryBase(name, std::move(params)), handler_(handler)
		{}

		virtual bool Query(OsiArgumentDesc & params) override;

	private:
		std::function<bool(OsiArgumentDesc &)> handler_;
	};

	class CustomEvent : public CustomFunction
	{
	public:
		inline CustomEvent(STDString const & name, std::vector<CustomFunctionParam> params)
			: CustomFunction(name, std::move(params))
		{}

		inline virtual FunctionType GetType() override
		{
			return FunctionType::Event;
		}
	};

	class CustomFunctionManager
	{
	public:
		// Arbitrary ID for custom extension functions
		static constexpr uint32_t CallClassId = 1;
		static constexpr uint32_t QueryClassId = 2;
		static constexpr uint32_t EventClassId = 3;

		void BeginStaticRegistrationPhase();
		void EndStaticRegistrationPhase();
		void ClearDynamicEntries();

		FunctionHandle Register(std::unique_ptr<CustomCallBase> call);
		FunctionHandle Register(std::unique_ptr<CustomQueryBase> qry);
		FunctionHandle Register(std::unique_ptr<CustomEvent> event);

		std::optional<FunctionHandle> RegisterDynamic(std::unique_ptr<CustomCallBase> call);
		std::optional<FunctionHandle> RegisterDynamic(std::unique_ptr<CustomQueryBase> qry);
		std::optional<FunctionHandle> RegisterDynamic(std::unique_ptr<CustomEvent> event);

		CustomFunction * Find(FunctionNameAndArity const & signature);

		bool Call(FunctionHandle handle, OsiArgumentDesc const & params);
		bool Query(FunctionHandle handle, OsiArgumentDesc & params);

		STDString GenerateHeaders() const;
		void PreProcessStory(wchar_t const * path);
		void PreProcessStory(STDString const & original, STDString & postProcessed);

	private:
		struct DynamicFunctionBindingInfo
		{
			FunctionType Type; 
			uint32_t Index;
			CustomFunction * Function;
		};

		std::unordered_map<FunctionNameAndArity, CustomFunction *> signatures_;
		std::unordered_map<FunctionNameAndArity, DynamicFunctionBindingInfo> dynamicSignatures_;

		std::vector<std::unique_ptr<CustomCallBase>> calls_;
		std::vector<std::unique_ptr<CustomQueryBase>> queries_;
		std::vector<std::unique_ptr<CustomEvent>> events_;

		std::size_t numStaticCalls_{ 0 };
		std::size_t numStaticQueries_{ 0 };
		std::size_t numStaticEvents_{ 0 };
		bool staticRegistrationDone_{ false };

		void RegisterSignature(CustomFunction * func);
		bool RegisterDynamicSignature(CustomFunction * func, uint32_t & index);
	};

	struct OsiSymbolInfo
	{
		struct ParamInfo
		{
			ValueType type;
			bool output;
		};

		STDString name;
		std::vector<ParamInfo> params;
		FunctionType type{ FunctionType::Unknown };
		uint32_t nodeId{ 0 };
		uint32_t EoCFunctionId{ 0 };
	};

	class CustomFunctionInjector
	{
	public:
		CustomFunctionInjector(class OsirisWrappers & wrappers, CustomFunctionManager & functions);

		void Initialize();
		void ThrowEvent(FunctionHandle handle, OsiArgumentDesc * args) const;

		inline std::vector<OsiSymbolInfo> const & OsiSymbols() const
		{
			return osiSymbols_;
		}

		static bool StaticCallWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc* params);
		static bool StaticQueryWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc* params);

		bool CallWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc* params);
		bool QueryWrapper(DivFunctions::CallProc next, uint32_t handle, OsiArgumentDesc* params);

	private:
		OsirisWrappers & wrappers_;
		CustomFunctionManager & functions_;
		std::wstring storyHeaderPath_;
		HANDLE storyHeaderFile_{ NULL };
		bool extendingStory_{ false };
		std::unordered_map<uint32_t, FunctionHandle> osiToDivMappings_;
		std::unordered_map<FunctionHandle, uint32_t> divToOsiMappings_;
		std::vector<OsiSymbolInfo> osiSymbols_;

		void CreateOsirisSymbolMap(MappingInfo ** Mappings, uint32_t * MappingCount);
		void OnAfterGetFunctionMappings(void * Osiris, MappingInfo ** Mappings, uint32_t * MappingCount);
		void ExtendStoryHeader(std::wstring const & headerPath);
		void OnCreateFile(LPCWSTR lpFileName,
			DWORD dwDesiredAccess,
			DWORD dwShareMode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			DWORD dwCreationDisposition,
			DWORD dwFlagsAndAttributes,
			HANDLE hTemplateFile,
			HANDLE hFile);
		void OnCloseHandle(HANDLE hFile, BOOL bSucceeded);
	};
}
