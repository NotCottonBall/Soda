cmake_minimum_required(VERSION 4.3)
project(Soda
  VERSION 0.0.1
  LANGUAGES CXX
)

############
# Settings #
############
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#############
# Variables #
#############
set(PROJECT_PREFIX SD)
set(PROJECT_LOG_NAME SODA)

set(SODA_THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)

set(SODA_BINARY_DIR ${PROJECT_SOURCE_DIR}/Binary)
set(SODA_INSTALL_DIR ${PROJECT_SOURCE_DIR}/Install)
# Folder structure for builds without any multi-config generators
set(SODA_OUTPUT_FOLDER_SIGNATURE
  ${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}
)

###############
# Definitions #
###############
if(WIN32)
  add_compile_definitions(${PROJECT_PREFIX}_PLATFORM_WINDOWS)
elseif(APPLE)
  add_compile_definitions(${PROJECT_PREFIX}_PLATFORM_MACOS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_compile_definitions(${PROJECT_PREFIX}_PLATFORM_LINUX)
endif()

# Setting Build/Config Types
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type")
  message(STATUS
    "[${PROJECT_LOG_NAME}] No Build Type Was Selected. Defaulting To ${CMAKE_BUILD_TYPE}"
  )
endif()

##################################################################
# Setting Up A Folder Structure                                  #
# For All The Available Variations Of Compilers, And Build Types #
##################################################################
if(CMAKE_CONFIGURATION_TYPES)
  foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG} CONFIG_UPPER)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}
      "${SODA_BINARY_DIR}/Bin/${SODA_OUTPUT_FOLDER_SIGNATURE}"
    )
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER}
      "${SODA_BINARY_DIR}/Libs/${SODA_OUTPUT_FOLDER_SIGNATURE}"
    )
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER}
      "${SODA_BINARY_DIR}/Libs/${SODA_OUTPUT_FOLDER_SIGNATURE}"
    )
  endforeach()
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${SODA_BINARY_DIR}/Bin/${SODA_OUTPUT_FOLDER_SIGNATURE}"
  )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${SODA_BINARY_DIR}/Libs/${SODA_OUTPUT_FOLDER_SIGNATURE}"
  )
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    "${SODA_BINARY_DIR}/Libs/${SODA_OUTPUT_FOLDER_SIGNATURE}"
  )
endif()

add_subdirectory(SodaEngine)
