#pragma once

#define RAPIDJSON_WRITE_DEFAULT_FLAGS kWriteNanAndInfFlag | kWriteNanAndInfNullFlag
#define RAPIDJSON_SIMD
#define RAPIDJSON_SSE42
#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
