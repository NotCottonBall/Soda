cmake_minimum_required(VERSION 3.31)

project(Soda)

# CMAKE Variables
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# defining the system specific tasks
if(WIN32)
  add_compile_definitions(SD_PLATFORM_WINDOWS)
elseif(APPLE)
  add_compile_definitions(SD_PLATFORM_APPLE)
elseif(UNIX)
  add_compile_definitions(SD_PLATFORM_LINUX)
endif()


# Settings up the build type
set(CMAKE_CONFIGURATION_TYPES "Debug;DebRelease;Release;RelExport;Export" CACHE STRING "Choose A Build Type (Debug | Release)" FORCE)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message("No Build Type Was Selected. So Defaulting To Release")
endif()

# Now, this is useful for me because I want separate output folders for separate platforms and compilers.
foreach(CONFIG "Debug" "DebRelease" "Release" "RelExport" "Export")
  string(TOUPPER ${CONFIG} CONFIG_UPPER)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}
      "${CMAKE_SOURCE_DIR}/Bin/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER}
      "${CMAKE_SOURCE_DIR}/Libs/Libraries${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER}
      "${CMAKE_SOURCE_DIR}/Libs/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
endforeach()

# adding the subdirectories
add_subdirectory(SodaEngine)
add_subdirectory(SodaCan)