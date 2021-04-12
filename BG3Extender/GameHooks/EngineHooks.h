#pragma once

#include <GameHooks/Wrappers.h>
#include <GameDefinitions/Stats/Functors.h>
#include <GameDefinitions/Misc.h>

namespace bg3se
{
	class EngineHooks
	{
	public:
#define HOOK_DEFN(name, sym, defn, hookType) enum class name##Tag {}; \
	hookType<name##Tag, defn> name;
#include <GameHooks/EngineHooks.inl>
#undef HOOK_DEFN

		void HookAll();
		void UnhookAll();
	};
}
