#include "SD_PCH.h"

#include "Profiler.h"

namespace Soda
{
void VisualProfiler::BeginSession(const std::string &name,
                                  const std::string &filepath)
{
  m_OutputFile.open(filepath);
  WriteHeader();
  m_InstrumentProfile = new InstrumentProfile{name};
}

void VisualProfiler::EndSession()
{
  WriteFooter();
  delete m_InstrumentProfile;
  m_InstrumentProfile == nullptr;
  m_OutputFile.close();

  m_ProfileCount = 0;
}

void VisualProfiler::WriteHeader()
{
  m_OutputFile << "{\"otherData\": {},\"traceEvents\":[";
  m_OutputFile.flush();
}

void VisualProfiler::WriteProfile(const Profile &profile)
{
  if(m_ProfileCount++ > 0)
    m_OutputFile << ",";

  std::string name = profile.name; // we are creating a non const copy of the
                                   // name for std::replace
  std::replace(name.begin(), name.end(), '"', '\"');

  m_OutputFile << "{";
  m_OutputFile << "\"cat\":\"function\",";
  m_OutputFile << "\"dur\":" << (profile.end - profile.start) << ',';
  m_OutputFile << "\"name\":\"" << name << "\",";
  m_OutputFile << "\"ph\":\"X\",";
  m_OutputFile << "\"pid\":0,";
  m_OutputFile << "\"tid\":" << profile.threadID << ",";
  m_OutputFile << "\"ts\":" << profile.start;
  m_OutputFile << "}";

  m_OutputFile.flush();
}

void VisualProfiler::WriteFooter()
{
  m_OutputFile << "]}";
  m_OutputFile.flush();
}
} // namespace Soda