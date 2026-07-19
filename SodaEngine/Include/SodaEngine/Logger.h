#pragma once
#include <memory>

#include <spdlog/spdlog.h>

#include <SodaEngine/SDAssert.h>

namespace SodaEngine
{
class SDLogger
{
private:
  static std::shared_ptr<spdlog::logger> m_Logger;

public:
  static void Initialize(std::string name);

  static std::shared_ptr<spdlog::logger> GetLogger() { return m_Logger; }
};
} // namespace SodaEngine

#define SD_LOG_TRACE(...)                                                      \
  ::SodaEngine::SDLogger::GetLogger()->trace(__VA_ARGS__)
#define SD_LOG_INFO(...) ::SodaEngine::SDLogger::GetLogger()->info(__VA_ARGS__)
#define SD_LOG_DEBUG(...)                                                      \
  ::SodaEngine::SDLogger::GetLogger()->debug(__VA_ARGS__)
#define SD_LOG_WARNING(...)                                                    \
  ::SodaEngine::SDLogger::GetLogger()->warn(__VA_ARGS__)
#define SD_LOG_ERROR(...)                                                      \
  ::SodaEngine::SDLogger::GetLogger()->error(__VA_ARGS__)
#define SD_LOG_CRITICAL(...)                                                   \
  ::SodaEngine::SDLogger::GetLogger()->critical(__VA_ARGS__)
