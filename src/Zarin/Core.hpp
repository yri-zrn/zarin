#pragma once
#include "Log.hpp"

#define ZRN_ENABLE_ASSERTS

#ifdef ZRN_ENABLE_ASSERTS
#   define ZRN_ASSERT(x, ...) { if(!x(x)) { ZRN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#   define ZRN_CORE_ASSERT(x, ...) { if(!(x)) { ZRN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
#   define ZRN_ASSERT(x, ...)
#   define ZRN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ZRN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
