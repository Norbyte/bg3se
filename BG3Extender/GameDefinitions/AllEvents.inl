// Hack to ensure IntelliSense navigates to the original definition for events, and not the generated ones.
#if defined(__INTELLISENSE__)
#define EVENT_EXPAND(x) x
#define EVENT(...) EVENT_EXPAND(DECLARE_EVENT(0, __VA_ARGS__))
#define EVENT_NS(...) EVENT_EXPAND(DECLARE_EVENT_NS(0, __VA_ARGS__))
#include <GameDefinitions/Events.inl>
#undef EVENT
#undef EVENT_NS
#undef EVENT_EXPAND
#else
#include <GameDefinitions/Generated/Events.inl>
#endif
