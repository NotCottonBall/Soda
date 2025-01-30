#pragma once

#include <memory>

// NOW This is where we have our ASSERTS and... well basically the MACROS we
// want to use in our project

// this is out own scope and ref system
namespace Soda
{
template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&...args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace Soda

// We use this BIT operation to make checking easier on the compiler without
// bools to check if an IsEventHappning is true or not
#define BIT(x) (1 << x)

// this is the binding function that binds the function you specify to a certain
// lamda
#define BIND_FN(x) std::bind(&x, this, std::placeholders::_1)

#ifdef SD_DEBUG
#define SD_ENABLE_ASSERTS // enables debug breaking
#define SD_ENABLE_PROFILING
#elif SD_RELEASE
#define SD_ENABLE_PROFILING
#endif

#ifdef SD_PLATFORM_WINDOWS
  // we dont include logger.h in this file cuz these are just defines that will
  // be writen as-is in the ones you use them in
#ifdef SD_ENABLE_ASSERTS // This sets if you want ASSERT functions in the
                         // project this is for the user
#define SD_ASSERT(x, ...)                                                      \
  {                                                                            \
    if(!(x))                                                                   \
    {                                                                          \
      SD_ERROR("ASSERT ERRORRR... {0}", __VA_ARGS__);                          \
      __debugbreak();                                                          \
    }                                                                          \
  }                                                                            \
  this is
  // for the Engine
#define SD_ENGINE_ASSERT(x, ...)                                               \
  {                                                                            \
    if(!(x))                                                                   \
    {                                                                          \
      SD_ENGINE_ERROR("ASSERT ERRORRR... {0}", __VA_ARGS__);                   \
      __debugbreak();                                                          \
    }                                                                          \
  }
#else
  // well if you dont want ASSERTS (prob dont in Release or Dist) then this is
  // your part
#define SD_ASSERT(x, ...)
#define SD_ENGINE_ASSERT(x, ...)
#endif
#else
#define SD_ASSERT(x, ...)
#define SD_ENGINE_ASSERT(x, ...)
#endif