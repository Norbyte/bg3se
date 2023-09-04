#pragma once

#include <functional>
#include <unordered_set>
#include <detours.h>

namespace bg3se {

	void const* ResolveRealFunctionAddress(void const* ptr);

	extern std::unordered_set<void const *> gRegisteredTrampolines;

	template <typename T>
	class WrappedFunction;

	template <typename R, typename... Params>
	class WrappedFunction<R(Params...)>
	{
	public:
		typedef R(*FuncType)(Params...);

		bool IsWrapped() const
		{
			return TrampolineFunc != nullptr;
		}

		void Wrap(HMODULE Module, char const * ProcName, FuncType NewFunction)
		{
			// FIXME - move to BinaryMappings
			FARPROC ExportProc = GetProcAddress(Module, ProcName);
			if (ExportProc == NULL) {
				std::string errmsg("Could not locate export ");
				errmsg += ProcName;
				Fail(errmsg.c_str());
			}

			Wrap(ExportProc, NewFunction);
		}

		void Wrap(void * Function, FuncType NewFunction)
		{
			if (IsWrapped()) {
				throw std::runtime_error("Tried to wrap function multiple times");
			}

			OriginalFunc = Function;
			TrampolineFunc = Function;
			NewFunc = NewFunction;
			gRegisteredTrampolines.insert(NewFunction);
			auto status = DetourAttachEx((PVOID *)&TrampolineFunc, (PVOID)NewFunc, (PDETOUR_TRAMPOLINE *)&FuncTrampoline, NULL, NULL);
			if (status != NO_ERROR) {
				Fail("Detour attach failed");
			}
		}

		void Unwrap()
		{
			if (IsWrapped()) {
				DWORD result = DetourDetach((PVOID *)&TrampolineFunc, (PVOID)NewFunc);
				if (result != NO_ERROR) {
					Fail("DetourDetach failed!");
				}

				OriginalFunc = nullptr;
				TrampolineFunc = nullptr;
				NewFunc = nullptr;
				FuncTrampoline = nullptr;
			}
		}

		__forceinline R operator ()(Params... Args) const
		{
			return FuncTrampoline(std::forward<Params>(Args)...);
		}

		__forceinline FuncType GetTrampoline() const
		{
			return FuncTrampoline;
		}

	private:
		void * OriginalFunc{ nullptr };
		void * TrampolineFunc{ nullptr };
		FuncType NewFunc{ nullptr };
		FuncType FuncTrampoline{ nullptr };
	};

	template <class TObject, class TFunc>
	struct MethodPtrHelpers;

	template <class TObject, class R, class... Params>
	struct MethodPtrHelpers<TObject, R (Params...)>
	{
		using MethodType = R (TObject::*)(Params...);
		using FunctionType = R (*)(TObject*, Params...);
		using BaseFunctionType = R (*)(Params...);
		
		static_assert(sizeof(MethodType) == sizeof(FunctionType));

		inline static FunctionType ToFunction(MethodType fun)
		{
			return *reinterpret_cast<FunctionType*>(&fun);
		}

		inline static MethodType ToMethod(FunctionType fun)
		{
			return *reinterpret_cast<MethodType*>(&fun);
		}
	};

	template <class Tag, class T>
	class WrappableFunction;

	template <class Tag, class R, class... Params>
	class WrappableFunction<Tag, R(Params...)>
	{
	public:
		template <class TReturn, class... TParams>
		struct PostHookSignature
		{
			using Type = void (void*, Params..., R);
		};
		
		template <class... TParams>
		struct PostHookSignature<void, TParams...>
		{
			using Type = void (void*, Params...);
		};
		
		template <class TContext, class TReturn, class... TParams>
		struct PostHookCallbackSignature
		{
			using Type = void (TContext::*)(Params..., TReturn);
		};
		
		template <class TContext, class... TParams>
		struct PostHookCallbackSignature<TContext, void, TParams...>
		{
			using Type = void (TContext::*)(Params...);
		};

		using BaseFuncType = R (Params...);
		using HookFuncType = R (void*, BaseFuncType*, Params...);
		using PreHookFuncType = void (void*, Params...);
		using PostHookFuncType = PostHookSignature<R, Params...>::Type;
		using NoContextHookFuncType = R (BaseFuncType*, Params...);

		void Wrap(HMODULE Module, char const * ProcName)
		{
			this->wrapped_.Wrap(Module, ProcName, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Wrap(void * Function)
		{
			this->wrapped_.Wrap(Function, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Unwrap()
		{
			this->wrapped_.Unwrap();
			gHook = nullptr;
		}

		bool IsWrapped() const
		{
			return wrapped_.IsWrapped();
		}

		void SetWrapper(NoContextHookFuncType* wrapper)
		{
			if (hook_ != nullptr) {
				throw std::runtime_error("Function already wrapped");
			}

			gRegisteredTrampolines.insert(ResolveRealFunctionAddress(&NoContextHook));
			gRegisteredTrampolines.insert(ResolveRealFunctionAddress(wrapper));
			hook_ = &NoContextHook;
			func_ = wrapper;
			context_ = nullptr;
		}

		template <class TContext>
		void SetWrapper(R (* wrapper)(TContext*, BaseFuncType*, Params...), TContext* context)
		{
			if (hook_ != nullptr) {
				throw std::runtime_error("Function already wrapped");
			}

			gRegisteredTrampolines.insert(ResolveRealFunctionAddress(wrapper));
			hook_ = reinterpret_cast<HookFuncType*>(wrapper);
			func_ = nullptr;
			context_ = context;
		}

		template <class TContext>
		void SetWrapper(R (TContext::* wrapper)(BaseFuncType*, Params...), TContext* context)
		{
			if (hook_ != nullptr) {
				throw std::runtime_error("Function already wrapped");
			}

			auto fun = MethodPtrHelpers<TContext, R (BaseFuncType*, Params...)>::ToFunction(wrapper);
			gRegisteredTrampolines.insert(ResolveRealFunctionAddress(fun));
			hook_ = reinterpret_cast<HookFuncType*>(fun);
			func_ = nullptr;
			context_ = context;
		}

		template <class TContext>
		void SetPreHook(void (TContext::* wrapper)(Params...), TContext* context)
		{
			if (hook_ != nullptr) {
				throw std::runtime_error("Function already wrapped");
			}

			gRegisteredTrampolines.insert(ResolveRealFunctionAddress(&StaticPreHook));
			hook_ = &StaticPreHook;
			func_ = MethodPtrHelpers<TContext, void (Params...)>::ToFunction(wrapper);
			context_ = context;
		}

		template <class TContext>
		void SetPostHook(PostHookCallbackSignature<TContext, R, Params...>::Type wrapper, TContext* context)
		{
			if (hook_ != nullptr) {
				throw std::runtime_error("Function already wrapped");
			}

			gRegisteredTrampolines.insert(ResolveRealFunctionAddress(&StaticPostHook));
			hook_ = &StaticPostHook;
			if constexpr (std::is_same_v<R, void>) {
				func_ = MethodPtrHelpers<TContext, void(Params...)>::ToFunction(wrapper);
			} else {
				func_ = MethodPtrHelpers<TContext, void(Params..., R)>::ToFunction(wrapper);
			}
			context_ = context;
		}

		void ClearHook()
		{
			hook_ = nullptr;
			func_ = nullptr;
			context_ = nullptr;
		}

		inline bool IsHooked() const
		{
			return hook_ != nullptr;
		}

		__forceinline R CallOriginal(Params... Args) const
		{
			return wrapped_(std::forward<Params>(Args)...);
		}

		__forceinline R CallWithHooks(Params... Args) const
		{
			if (hook_) {
				return hook_(context_, this->wrapped_.GetTrampoline(), std::forward<Params>(Args)...);
			} else {
				return this->CallOriginal(std::forward<Params>(Args)...);
			}
		}

	private:
		WrappedFunction<R(Params...)> wrapped_;
		HookFuncType* hook_;
		void* func_;
		void* context_;

		static R CallToTrampoline(Params... Args)
		{
			return gHook->CallWithHooks(std::forward<Params>(Args)...);
		}

		static R NoContextHook(void* ctx, BaseFuncType* fun, Params... args)
		{
			auto hook = reinterpret_cast<NoContextHookFuncType*>(gHook->func_);
			return hook(fun, std::forward<Params>(args)...);
		}

		static R StaticPreHook(void* ctx, BaseFuncType* fun, Params... args)
		{
			auto hook = reinterpret_cast<PreHookFuncType*>(gHook->func_);
			hook(ctx, std::forward<Params>(args)...);
			return gHook->CallOriginal(std::forward<Params>(args)...);
		}

		static R StaticPostHook(void* ctx, BaseFuncType* fun, Params... args)
		{
			auto hook = reinterpret_cast<PostHookFuncType*>(gHook->func_);
			if constexpr (std::is_same_v<R, void>) {
				gHook->CallOriginal(std::forward<Params>(args)...);
				hook(ctx, std::forward<Params>(args)...);
			} else {
				auto retval = gHook->CallOriginal(std::forward<Params>(args)...);
				hook(ctx, std::forward<Params>(args)..., retval);
				return retval;
			}
		}

		static WrappableFunction<Tag, R(Params...)> * gHook;
	};
}
