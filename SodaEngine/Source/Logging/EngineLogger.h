#pragma once
#include <memory>

#include <spdlog/spdlog.h>

namespace SodaEngine
{
class SDELogger
{
private:
  static std::shared_ptr<spdlog::logger> m_Logger;

public:
  static void Initialize();

  static std::shared_ptr<spdlog::logger> GetLogger() { return m_Logger; }
};
} // namespace SodaEngine

#define SDE_LOG_TRACE(...)                                                     \
  ::SodaEngine::SDELogger::GetLogger()->trace(__VA_ARGS__)
#define SDE_LOG_INFO(...)                                                      \
  ::SodaEngine::SDELogger::GetLogger()->info(__VA_ARGS__)
#define SDE_LOG_DEBUG(...)                                                     \
  ::SodaEngine::SDELogger::GetLogger()->debug(__VA_ARGS__)
#define SDE_LOG_WARNING(...)                                                   \
  ::SodaEngine::SDELogger::GetLogger()->warn(__VA_ARGS__)
#define SDE_LOG_ERROR(...)                                                     \
  ::SodaEngine::SDELogger::GetLogger()->error(__VA_ARGS__)
#define SDE_LOG_CRITICAL(...)                                                  \
  ::SodaEngine::SDELogger::GetLogger()->critical(__VA_ARGS__)
