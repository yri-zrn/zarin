#pragma once
#include "Log.hpp"

#include <memory>

#define ZRN_ENABLE_ASSERTS

#ifdef ZRN_ENABLE_ASSERTS
#   define ZRN_ASSERT(x, ...) { if(!(x)) { ZRN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#   define ZRN_CORE_ASSERT(x, ...) { if(!(x)) { ZRN_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
#   define ZRN_ASSERT(x, ...)
#   define ZRN_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ZRN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace zrn {

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

} // namespace zrn