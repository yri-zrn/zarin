#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <concepts>
#include <memory>

namespace zrn {

class Log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
        return s_CoreLogger;
    }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
        return s_ClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace zrn

// Core log macros
#define ZRN_CORE_TRACE(...)     ::zrn::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ZRN_CORE_INFO(...)      ::zrn::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ZRN_CORE_WARN(...)      ::zrn::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ZRN_CORE_ERROR(...)     ::zrn::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ZRN_CORE_FATAL(...)     ::zrn::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define ZRN_TRACE(...)          ::zrn::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ZRN_INFO(...)           ::zrn::Log::GetClientLogger()->info(__VA_ARGS__)
#define ZRN_WARN(...)           ::zrn::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ZRN_ERROR(...)          ::zrn::Log::GetClientLogger()->error(__VA_ARGS__)
#define ZRN_FATAL(...)          ::zrn::Log::GetClientLogger()->fatal(__VA_ARGS__)

// Forward declaration
namespace zrn { class Event; }

// Formatter must be provided
template<std::derived_from<zrn::Event> Derived>
struct fmt::formatter<Derived> : ostream_formatter {};
