#pragma once

#include <SodaEngine/Logger.h>

#define SD_ASSERT(x, ...)                                                      \
  do                                                                           \
  {                                                                            \
    if(!(x))                                                                   \
    {                                                                          \
      SD_LOG_CRITICAL(__VA_ARGS__);                                            \
      std::abort();                                                            \
    }                                                                          \
  } while(0)
