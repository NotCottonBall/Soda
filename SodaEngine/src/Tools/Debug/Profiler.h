#pragma once

#include "SD_PCH.h"

namespace Soda
{
// the profiler infos
struct Profile
{
  std::string name;
  int64_t start, end;
  uint32_t threadID;
};

struct InstrumentProfile
{
  std::string name;
};

// visual profiler that will write everything ot a file specified
// the file can be loaded by chrome://tracing
class VisualProfiler
{
public:
  VisualProfiler() : m_ProfileCount(0), m_InstrumentProfile(nullptr) {}

  void BeginSession(const std::string &name,
                    const std::string &filepath = "ProfileResult.json");
  void EndSession();

  void WriteHeader();
  void WriteProfile(const Profile &profile);
  void WriteFooter();

  static VisualProfiler &Get()
  {
    static VisualProfiler instance;
    return instance;
  }

private:
  InstrumentProfile *m_InstrumentProfile;
  std::ofstream m_OutputFile;
  uint32_t m_ProfileCount;
};

// Profil Timer //
class Profiler
{
public:
  Profiler(const char *name) : m_name(name)
  {
    m_StartTime = std::chrono::high_resolution_clock::now();
  }

  ~Profiler()
  {
    auto endTime = std::chrono::high_resolution_clock::now();

    int64_t startPoint =
        std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime)
            .time_since_epoch()
            .count();
    int64_t endPoint =
        std::chrono::time_point_cast<std::chrono::microseconds>(endTime)
            .time_since_epoch()
            .count();

    float duration = (endPoint - startPoint) * 0.001f;

    uint32_t ThreadID =
        std::hash<std::thread::id>{}(std::this_thread::get_id());
    VisualProfiler::Get().WriteProfile(
        {m_name, startPoint, endPoint, ThreadID});
  }

private:
  std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
  const char *m_name;
};
} // namespace Soda

#ifdef SD_ENABLE_PROFILING
#define SD_START_PROFILER(name, filepath)                                      \
  ::Soda::VisualProfiler::Get().BeginSession(name, filepath);
#define SD_STOP_PROFILER() Soda::VisualProfiler::Get().EndSession();

#define SD_PROFILE() SD_PROFILE_SCOPE(__FUNCSIG__);
#define SD_PROFILE_SCOPE(name) ::Soda::Profiler profiler##__LINE__(name);
#else
#define SD_START_PROFILER(name, filepath)
#define SD_STOP_PROFILER()
#define SD_PROFILE()
#define SD_PROFILE_SCOPE(name)
#endif