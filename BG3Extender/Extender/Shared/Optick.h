#pragma once

#if defined(SE_IS_DEVELOPER_BUILD)
#define USE_OPTICK 1
#define OPTICK_ENABLE_GPU 0
#else
#define USE_OPTICK 0
#endif

#include <optick.h>

#if USE_OPTICK
#define OPTICK_DESC(name, filename, line, category) \
    ::Optick::CreateDescription(name, filename, line, nullptr, Optick::Category::category, \
        (Optick::EventDescription::COPY_NAME_STRING | Optick::EventDescription::COPY_FILENAME_STRING | Optick::EventDescription::IS_CUSTOM_NAME))
#define OPTICK_SCRIPT_EVENT(name, filename, line) \
    ::Optick::Event OPTICK_CONCAT(autogen_event_, __LINE__)(*OPTICK_DESC(name, filename, line, IO));
#define OPTICK_SCRIPT_CALL_EVENT(name, L, index) \
    ::Optick::Event OPTICK_CONCAT(autogen_event_, __LINE__)(*lua_get_function_optick_desc(L, index));
#define OPTICK_D_EVENT(desc) \
    ::Optick::Event OPTICK_CONCAT(autogen_event_, __LINE__)(desc);
#else
#define OPTICK_DESC(name, filename, line, category)
#define OPTICK_SCRIPT_EVENT(name, filename, line)
#define OPTICK_SCRIPT_CALL_EVENT(name, L, index)
#define OPTICK_D_EVENT(desc)
#endif
