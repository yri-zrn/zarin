#pragma once

#include "Core/Log.hpp"

#include <memory>
#include <stdint.h>

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

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

using RendererID = uint32_t;

} // namespace zrn