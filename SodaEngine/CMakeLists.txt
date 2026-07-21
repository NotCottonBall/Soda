cmake_minimum_required(VERSION 4.3)
project(SodaEngine
  VERSION 0.0.1
  LANGUAGES CXX
)

############################
# Configuring Version File #
############################
configure_file(
  ${PROJECT_SOURCE_DIR}/Version.h.in
  ${CMAKE_BINARY_DIR}/Generated/SodaEngine/Version.h
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
set(PROJECT_LOG_NAME Soda)

set(SODA_THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/../ThirdParty)

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

# Generating The Library
add_library(${PROJECT_NAME} STATIC)

# Sources For The Library SodaEngine.
# All The Sources Will Be Updated In "${CMAKE_SOURCE_DIR}/Files.cmake"
target_sources(${PROJECT_NAME} PRIVATE
  ${SODA_ENGINE_SOURCES}
)


##############################
# Setting Build/Config Types #
##############################
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
find_library(SDL_LIB
  NAMES SDL3
  PATHS ${SODA_THIRD_PARTY_DIR}/Binary/Libs/${SODA_OUTPUT_FOLDER_SIGNATURE}
  NO_DEFAULT_PATH
  REQUIRED
)

# Adding Libraries (Imported)
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${SDL_LIB}
)

# Finding Libraries (System)
find_package(Vulkan REQUIRED COMPONENTS volk shaderc_combined)

# Adding Libraries (System)
target_link_libraries(${PROJECT_NAME} PRIVATE
  # Vulkan Related Packages
  Vulkan::Vulkan
  Vulkan::volk
  Vulkan::shaderc_combined
)


########################
# ADDING INCLUDE PATHS #
########################
target_include_directories(${PROJECT_NAME} PRIVATE
  ${SODA_THIRD_PARTY_DIR}/SDL/include

  ${PROJECT_SOURCE_DIR}/Source
  ${CMAKE_BINARY_DIR}/Generated
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${SODA_THIRD_PARTY_DIR}/glm
  ${SODA_THIRD_PARTY_DIR}/spdlog/include

  ${PROJECT_SOURCE_DIR}/Include
)


##############
# ADDING PCH #
##############
set(PCH_HEADER "${PROJECT_SOURCE_DIR}/Source/SodaEnginePCH.h")
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})


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


# Some Other Definitions and Settings
target_compile_definitions(${PROJECT_NAME} PUBLIC
  GLM_ENABLE_EXPERIMENTAL
)


# Managing Visual Studio Bullshit
# This is so that visual studio can generate
if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  )
endif()
