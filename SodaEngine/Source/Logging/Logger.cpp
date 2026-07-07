#include <filesystem>
#include <memory>
#include <string>

#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "EngineLogger.h"
#include <SodaEngine/Logger.h>

namespace SodaEngine
{
std::shared_ptr<spdlog::logger> SDELogger::m_Logger = nullptr;
std::shared_ptr<spdlog::logger> SDLogger::m_Logger = nullptr;

void SDELogger::Initialize()
{
  // Filepath for Logs to dump into
  std::filesystem::path logDir = std::filesystem::current_path();
  logDir /= "LogDump";
  std::string engineInfoLogLvlFile =
      (logDir / "EngineLogger_LogLevelInfo.log").string();
  std::string engineErrLogLvlFile =
      (logDir / "EngineLogger_LogLevelError.log").string();

  // Creating Sinks
  auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto engineInfoLvlFileSink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(engineInfoLogLvlFile);
  auto engineErrLvlFileSink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(engineErrLogLvlFile);

  // Settings Up Logger
  m_Logger = std::make_shared<spdlog::logger>("Engine");
  m_Logger->sinks().push_back(consoleSink);
  m_Logger->sinks().push_back(engineInfoLvlFileSink);
  m_Logger->sinks().push_back(engineErrLvlFileSink);
  // Logger Design
  m_Logger->set_level(spdlog::level::trace);

  m_Logger->info("Initialized Soda Engine Logger...");
}

void SDLogger::Initialize(std::string name)
{
  // Filepath for Logs to dump into
  std::filesystem::path logDir = std::filesystem::current_path() / "LogDump";
  std::string engineInfoLogLvlFile =
      (logDir / "ClientLogger_LogLevelInfo.log").string();
  std::string engineErrLogLvlFile =
      (logDir / "ClientLogger_LogLevelError.log").string();

  // Creating Sinks
  auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  auto engineInfoLvlFileSink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(engineInfoLogLvlFile);
  auto engineErrLvlFileSink =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(engineErrLogLvlFile);

  // Settings Up Logger
  m_Logger = std::make_shared<spdlog::logger>(name);
  m_Logger->sinks().push_back(consoleSink);
  m_Logger->sinks().push_back(engineInfoLvlFileSink);
  m_Logger->sinks().push_back(engineErrLvlFileSink);
  // Logger Design
  m_Logger->set_level(spdlog::level::trace);

  m_Logger->info("Initialized Soda Client Logger...");
}
} // namespace SodaEngine
