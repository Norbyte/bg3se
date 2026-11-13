// Hack to ensure IntelliSense navigates to the original definition for enums, and not the generated ones.
#if defined(__INTELLISENSE__)
#include <GameDefinitions/Enumerations/ECS.inl>
#include <GameDefinitions/Enumerations/Engine.inl>
#include <GameDefinitions/Enumerations/IMGUI.inl>
#include <GameDefinitions/Enumerations/Stats.inl>
#else
#include <GameDefinitions/Generated/Enumerations.inl>
#endif
