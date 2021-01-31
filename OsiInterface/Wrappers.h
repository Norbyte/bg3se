#pragma once

#include <functional>

namespace dse {

	template <typename T>
	class WrappedFunction;

	template <typename R, typename... Params>
	class WrappedFunction<R(Params...)>
	{
	public:
		typedef R(*FuncType)(Params...);

		bool IsWrapped() const
		{
			return OriginalFunc != nullptr;
		}

		void Wrap(HMODULE Module, char * ProcName, FuncType NewFunction)
		{
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
			NewFunc = NewFunction;
			auto status = DetourAttachEx((PVOID *)&OriginalFunc, (PVOID)NewFunc, (PDETOUR_TRAMPOLINE *)&FuncTrampoline, NULL, NULL);
			if (status != NO_ERROR) {
				Fail("Detour attach failed");
			}
		}

		void Unwrap()
		{
			if (IsWrapped()) {
				DWORD result = DetourDetach((PVOID *)&OriginalFunc, (PVOID)NewFunc);
				if (result != NO_ERROR) {
					Fail("DetourDetach failed!");
				}
			}
		}

		inline R operator ()(Params... Args) const
		{
			return FuncTrampoline(std::forward<Params>(Args)...);
		}

		inline FuncType GetTrampoline() const
		{
			return FuncTrampoline;
		}

	private:
		void * OriginalFunc{ nullptr };
		FuncType NewFunc{ nullptr };
		FuncType FuncTrampoline{ nullptr };
	};

	template <typename R, typename... Params>
	struct GetHookSignature
	{
		using Type = void(Params..., R);
	};

	template <typename... Params>
	struct GetHookSignature<void, Params...>
	{
		using Type = void(Params...);
	};

	template <class Tag, class T>
	class BaseWrappableFunction;

	template <class Tag, class R, class... Params>
	class BaseWrappableFunction<Tag, R(Params...)>
	{
	public:
		using FuncType = R(Params...);

		bool IsWrapped() const
		{
			return wrapped_.IsWrapped();
		}

		inline R CallOriginal(Params... Args) const
		{
			return wrapped_(std::forward<Params>(Args)...);
		}

	protected:
		WrappedFunction<R(Params...)> wrapped_;
	};

	template <class Tag, class T>
	class PreHookableFunction;

	template <class Tag, class R, class... Params>
	class PreHookableFunction<Tag, R(Params...)> : public BaseWrappableFunction<Tag, R(Params...)>
	{
	public:
		using PreHookType = void(Params...);
		using PreHookFuncType = std::function<PreHookType>;

		void Wrap(HMODULE Module, char * ProcName)
		{
			wrapped_.Wrap(Module, ProcName, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Wrap(void * Function)
		{
			wrapped_.Wrap(Function, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Unwrap()
		{
			wrapped_.Unwrap();
			gHook = nullptr;
		}

		void ClearHooks()
		{
			preHook_ = PreHookFuncType();
		}

		void SetPreHook(PreHookFuncType hook)
		{
			preHook_ = hook;
		}

		inline R CallWithHooks(Params... Args) const
		{
			if (preHook_) {
				preHook_(std::forward<Params>(Args)...);
			}

			return CallOriginal(std::forward<Params>(Args)...);
		}

		static R CallToTrampoline(Params... Args)
		{
			return gHook->CallWithHooks(std::forward<Params>(Args)...);
		}

	private:
		PreHookFuncType preHook_;

		static PreHookableFunction<Tag, R(Params...)> * gHook;
	};

	template <class Tag, class T>
	class PostHookableFunction;

	template <class Tag, class R, class... Params>
	class PostHookableFunction<Tag, R(Params...)> : public BaseWrappableFunction<Tag, R(Params...)>
	{
	public:
		using PostHookType = typename GetHookSignature<R, Params...>::Type;
		using PostHookFuncType = std::function<PostHookType>;

		void Wrap(HMODULE Module, char * ProcName)
		{
			wrapped_.Wrap(Module, ProcName, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Wrap(void * Function)
		{
			wrapped_.Wrap(Function, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Unwrap()
		{
			wrapped_.Unwrap();
			gHook = nullptr;
		}

		void ClearHooks()
		{
			postHook_ = PostHookFuncType();
		}

		void SetPostHook(PostHookFuncType hook)
		{
			postHook_ = hook;
		}

		inline R CallWithHooks(Params... Args) const
		{
			if constexpr (std::is_same<void, R>::value) {
				CallOriginal(std::forward<Params>(Args)...);

				if (postHook_) {
					postHook_(std::forward<Params>(Args)...);
				}
			}
			else {
				auto retval = CallOriginal(std::forward<Params>(Args)...);

				if (postHook_) {
					postHook_(std::forward<Params>(Args)..., std::forward<decltype(retval)>(retval));
				}

				return retval;
			}
		}

		static R CallToTrampoline(Params... Args)
		{
			return gHook->CallWithHooks(std::forward<Params>(Args)...);
		}

	private:
		PostHookFuncType postHook_;

		static PostHookableFunction<Tag, R(Params...)> * gHook;
	};

	template <class Tag, class T>
	class HookableFunction;

	template <class Tag, class R, class... Params>
	class HookableFunction<Tag, R(Params...)> : public BaseWrappableFunction<Tag, R(Params...)>
	{
	public:
		using PreHookType = void(Params...);
		using PreHookFuncType = std::function<PreHookType>;

		using PostHookType = typename GetHookSignature<R, Params...>::Type;
		using PostHookFuncType = std::function<PostHookType>;

		void Wrap(HMODULE Module, char * ProcName)
		{
			wrapped_.Wrap(Module, ProcName, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Wrap(void * Function)
		{
			wrapped_.Wrap(Function, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Unwrap()
		{
			wrapped_.Unwrap();
			gHook = nullptr;
		}

		void ClearHooks()
		{
			preHooks_.clear();
			postHooks_.clear();
		}

		void AddPreHook(PreHookFuncType hook)
		{
			preHooks_.push_back(hook);
		}

		void AddPostHook(PostHookFuncType hook)
		{
			postHooks_.push_back(hook);
		}

		inline R CallWithHooks(Params... Args) const
		{
			for (auto const & hook : preHooks_) {
				hook(std::forward<Params>(Args)...);
			}

			if constexpr (std::is_same<void, R>::value) {
				CallOriginal(std::forward<Params>(Args)...);

				for (auto const & hook : postHooks_) {
					hook(std::forward<Params>(Args)...);
				}
			}
			else {
				auto retval = CallOriginal(std::forward<Params>(Args)...);

				for (auto const & hook : postHooks_) {
					hook(std::forward<Params>(Args)..., std::forward<decltype(retval)>(retval));
				}

				return retval;
			}
		}

		static R CallToTrampoline(Params... Args)
		{
			return gHook->CallWithHooks(std::forward<Params>(Args)...);
		}

	private:
		std::vector<PreHookFuncType> preHooks_;
		std::vector<PostHookFuncType> postHooks_;

		static HookableFunction<Tag, R(Params...)> * gHook;
	};

	template <class Tag, class T>
	class FastWrappableFunction;

	template <class Tag, class R, class... Params>
	class FastWrappableFunction<Tag, R(Params...)> : public BaseWrappableFunction<Tag, R(Params...)>
	{
	public:
		using WrapperHookType = R(FuncType *, Params...);
		using WrapperHookFuncType = WrapperHookType*;

		void Wrap(HMODULE Module, char * ProcName)
		{
			wrapped_.Wrap(Module, ProcName, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Wrap(void * Function)
		{
			wrapped_.Wrap(Function, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Unwrap()
		{
			wrapped_.Unwrap();
			gHook = nullptr;
		}

		void SetWrapper(WrapperHookFuncType wrapper)
		{
			if (wrapperHook_ != nullptr) {
				throw std::runtime_error("Function already wrapped");
			}

			wrapperHook_ = wrapper;
		}

		void ClearHook()
		{
			wrapperHook_ = nullptr;
		}

		inline bool IsHooked() const
		{
			return wrapperHook_ != nullptr;
		}

		inline R CallWithHooks(Params... Args) const
		{
			if (wrapperHook_) {
				return wrapperHook_(wrapped_.GetTrampoline(), std::forward<Params>(Args)...);
			} else {
				return CallOriginal(std::forward<Params>(Args)...);
			}
		}

		static R CallToTrampoline(Params... Args)
		{
			return gHook->CallWithHooks(std::forward<Params>(Args)...);
		}

	private:
		WrapperHookFuncType wrapperHook_;

		static FastWrappableFunction<Tag, R(Params...)> * gHook;
	};

	template <class Tag, class T>
	class WrappableFunction;

	template <class Tag, class R, class... Params>
	class WrappableFunction<Tag, R(Params...)> : public BaseWrappableFunction<Tag, R(Params...)>
	{
	public:
		using WrapperHookType = R(FuncType *, Params...);
		using WrapperHookFuncType = std::function<WrapperHookType>;

		void Wrap(HMODULE Module, char * ProcName)
		{
			wrapped_.Wrap(Module, ProcName, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Wrap(void * Function)
		{
			wrapped_.Wrap(Function, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Unwrap()
		{
			wrapped_.Unwrap();
			gHook = nullptr;
		}

		void SetWrapper(WrapperHookFuncType wrapper)
		{
			if (wrapperHook_ != nullptr) {
				throw std::runtime_error("Function already wrapped");
			}

			wrapperHook_ = wrapper;
		}

		void ClearHook()
		{
			wrapperHook_ = decltype(wrapperHook_)();
		}

		inline bool IsHooked() const
		{
			return (bool)wrapperHook_;
		}

		inline R CallWithHooks(Params... Args) const
		{
			if (wrapperHook_) {
				return wrapperHook_(wrapped_.GetTrampoline(), std::forward<Params>(Args)...);
			} else {
				return CallOriginal(std::forward<Params>(Args)...);
			}
		}

		static R CallToTrampoline(Params... Args)
		{
			return gHook->CallWithHooks(std::forward<Params>(Args)...);
		}

	private:
		WrapperHookFuncType wrapperHook_;

		static WrappableFunction<Tag, R(Params...)> * gHook;
	};

	template <class Tag, class T>
	class MultiWrappableFunction;

	template <class Tag, class R, class... Params>
	class MultiWrappableFunction<Tag, R(Params...)> : public BaseWrappableFunction<Tag, R(Params...)>
	{
	public:
		using WrapperHookType = R(std::function<FuncType> const &, Params...);
		using WrapperHookFuncType = std::function<WrapperHookType>;

		void Wrap(HMODULE Module, char * ProcName)
		{
			wrapped_.Wrap(Module, ProcName, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Wrap(void * Function)
		{
			wrapped_.Wrap(Function, &CallToTrampoline);

			if (gHook != nullptr) {
				Fail("Hook already registered");
			}

			gHook = this;
		}

		void Unwrap()
		{
			wrapped_.Unwrap();
			gHook = nullptr;
		}

		void AddWrapper(WrapperHookFuncType wrapper)
		{
			auto nextHookIndex = (unsigned)wrapperHooks_.size() + 1;
			wrapperHooks_.push_back({
				wrapper,
				[=](Params... Args) {
					return this->CallWrapper(nextHookIndex, std::forward<Params>(Args)...);
				}
			});
		}

		inline R CallWithHooks(Params... Args) const
		{
			return CallWrapper(0, std::forward<Params>(Args)...);
		}

		inline R CallOriginal(Params... Args) const
		{
			return wrapped_(std::forward<Params>(Args)...);
		}

	private:
		struct WrapperHookInfo
		{
			WrapperHookFuncType hook;
			std::function<FuncType> forwarder;
		};

		std::vector<WrapperHookInfo> wrapperHooks_;

		static MultiWrappableFunction<Tag, R(Params...)> * gHook;

		inline R CallWrapper(unsigned WrapperHookNum, Params... Args) const
		{
			if (WrapperHookNum < wrapperHooks_.size()) {
				auto const & hook = wrapperHooks_[WrapperHookNum];
				return hook.hook(std::ref(hook.forwarder), std::forward<Params>(Args)...);
			} else {
				return wrapped_(std::forward<Params>(Args)...);
			}
		}

		static R CallToTrampoline(Params... Args)
		{
			return gHook->CallWithHooks(std::forward<Params>(Args)...);
		}
	};
}