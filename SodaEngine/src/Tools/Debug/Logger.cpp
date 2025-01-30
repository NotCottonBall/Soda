#include "SD_PCH.h"

#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Soda
{
// we declare our two options
std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
std::shared_ptr<spdlog::logger> Log::s_UserLogger;

void Log::Init()
{
  // setting the format of spdlog to print
  spdlog::set_pattern("%^[%T] %n: %v%$");

  // What spdlog should call The Engine And User Logging options and the default
  // option... "TRACE"
  s_EngineLogger = spdlog::stdout_color_mt("ENGINE LOG");
  s_EngineLogger->set_level(spdlog::level::trace);

  s_UserLogger = spdlog::stdout_color_mt("LOG");
  s_UserLogger->set_level(spdlog::level::trace);
}
} // namespace Soda