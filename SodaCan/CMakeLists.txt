cmake_minimum_required(VERSION 4.3)

project(SodaCan
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
set(PROJECT_PREFIX SDC)
set(PROJECT_LOG_NAME SodaCan)

set(SODA_THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/../ThirdParty)
set(SODA_ENGINE_DIR ${PROJECT_SOURCE_DIR}/../SodaEngine)

set(SODA_BINARY_DIR ${PROJECT_SOURCE_DIR}/Binary)
set(SODA_INSTALL_DIR ${PROJECT_SOURCE_DIR}/Install)
# Folder structure for builds without any multi-config generators
set(SODA_OUTPUT_FOLDER_SIGNATURE
  ${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}
)

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

# Including Stuff
include(./CMake/Files.cmake)

# Generating The Executable
add_executable(${PROJECT_NAME})

# Sources For The Executable
# All The Sources Will Be Updated In "${CMAKE_SOURCE_DIR}/Files.cmake"
target_sources(${PROJECT_NAME} PRIVATE
  ${SODA_CAN_SOURCES}
)


##############################
# Setting Build/Config Types #
###############################
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type")
  message(STATUS
    "[${PROJECT_LOG_NAME}]
    No Build Type Was Selected. Defaulting To ${CMAKE_BUILD_TYPE}"
  )
endif()


################################
# FINDING AND ADDING LIBRARIES #
################################
# Finding Libraries (Imported)
find_library(IMGUI_LIB
  NAMES imgui
  PATHS ${SODA_THIRD_PARTY_DIR}/Binary/Libs/${SODA_OUTPUT_FOLDER_SIGNATURE}
  NO_DEFAULT_PATH
  REQUIRED
)

# Adding Libraries (Imported)
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${IMGUI_LIB}
)

# Separate Case For SodaEngine
# This was done because I want to be able to
# Make CMake from the root file or separatly as a single project
if(TARGET SodaEngine)
  target_link_libraries(${PROJECT_NAME} PRIVATE SodaEngine)
else()
  find_library(SODA_ENGINE_LIB
    NAMES SodaEngine
    PATHS ${SODA_ENGINE_DIR}/Binary/Libs/${SODA_OUTPUT_FOLDER_SIGNATURE}
    NO_DEFAULT_PATH
    REQUIRED
  )
  target_link_libraries(${PROJECT_NAME} PRIVATE ${SODA_ENGINE_LIB})
endif()


########################
# ADDING INCLUDE PATHS #
########################
target_include_directories(${PROJECT_NAME} PRIVATE
  ${SODA_THIRD_PARTY_DIR}/imgui

  ${PROJECT_SOURCE_DIR}/Sources
  ${SODA_ENGINE_DIR}/Include
)


#######################################################
# SETTINGS COMPILE DEFINITIONS FOR BUILD/CONFIG TYPES #
#######################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:${PROJECT_PREFIX}_DEBUG>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Release>:${PROJECT_PREFIX}_RELEASE>
)

if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${PROJECT_PREFIX}_PLATFORM_WINDOWS
  )
elseif(APPLE)
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${PROJECT_PREFIX}_PLATFORM_MACOS
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${PROJECT_PREFIX}_PLATFORM_LINUX
  )
endif()


# Managing Visual Studio Bullshit
# This is so that visual studio can generate
if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  )
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}
  )
endif()
