#pragma once

#include <Logging/EngineLogger.h>

#ifdef SD_DEBUG
#define SDE_ASSERT(x, ...)                                                     \
  do                                                                           \
  {                                                                            \
    if(!(x))                                                                   \
    {                                                                          \
      SDE_LOG_CRITICAL(__VA_ARGS__);                                           \
      std::abort();                                                            \
    }                                                                          \
  } while(0)

#elif SD_REL_DEB
#define SDE_ASSERT(x, ...)                                                     \
  do                                                                           \
  {                                                                            \
    if(!(x))                                                                   \
    {                                                                          \
      SDE_LOG_CRITICAL(__VA_ARGS__);                                           \
    }                                                                          \
  } while(0)

#elif SD_RELEASE
#define SDE_ASSERT(x, ...)                                                     \
  do                                                                           \
  {                                                                            \
    if(!(x))                                                                   \
    {                                                                          \
      SDE_LOG_CRITICAL(__VA_ARGS__);                                           \
    }                                                                          \
  } while(0)

#else
#define SDE_ASSERT(x, ...) ((void)0)
#endif
