cmake_minimum_required(VERSION 3.10)

project(SodaEngine)

# Setting the build type for libraries to fetch from
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DebRelease")
  set(LIB_BUILD_TYPE "Debug")
else()
  set(LIB_BUILD_TYPE "Release")
endif()
# Finding Libraries
## GLFW
find_library(GLFW_LIB
    NAMES glfw3
    PATHS "${CMAKE_SOURCE_DIR}/Libraries/Libs/${CMAKE_SYSTEM_NAME}-${LIB_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}"
    NO_DEFAULT_PATH
)
if(NOT GLFW_LIB)
  message("ERROR: Required Library glfw Was Not Found! Did You Build 'Libraries' Before Building SodaEngine?")
endif()
## GLAD
find_library(GLAD_LIB
    NAMES glad
    PATHS "${CMAKE_SOURCE_DIR}/Libraries/Libs/${CMAKE_SYSTEM_NAME}-${LIB_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}"
    NO_DEFAULT_PATH
)
if(NOT GLAD_LIB)
  message("ERROR: Required Library glad Was Not Found! Did You Build 'Libraries' Before Building SodaEngine?")
endif()
## IMGUI
find_library(IMGUI_LIB
    NAMES imgui
    PATHS "${CMAKE_SOURCE_DIR}/Libraries/Libs/${CMAKE_SYSTEM_NAME}-${LIB_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}"
    NO_DEFAULT_PATH
)
if(NOT IMGUI_LIB)
  message("ERROR: Required Library imgui Was Not Found! Did You Build 'Libraries' Before Building SodaEngine?")
endif()
## YAML-CPP
find_library(YAML_CPP_LIB
    NAMES yaml-cppd yaml-cpp
    PATHS "${CMAKE_SOURCE_DIR}/Libraries/Libs/${CMAKE_SYSTEM_NAME}-${LIB_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}"
    NO_DEFAULT_PATH
)
if(NOT YAML_CPP_LIB)
  message("ERROR: Required Library yaml-cpp Was Not Found! Did You Build 'Libraries' Before Building SodaEngine?")
endif()

# files of the Soda project
file(GLOB_RECURSE SODA_SOURCES "src/*.cpp" "src/*.hpp" "src/*.c" "src/*.h")
file(GLOB_RECURSE STBI_SOURCES "${CMAKE_SOURCE_DIR}/Libraries/stb/*.cpp" "${CMAKE_SOURCE_DIR}/Libraries/stb/*.h")
file(GLOB_RECURSE GLM_SOURCES "${CMAKE_SOURCE_DIR}/Headers/glm/glm/*.hpp")

# creating the static lib
add_library(${PROJECT_NAME} STATIC ${SODA_SOURCES} ${STBI_SOURCES} ${GLM_SOURCES})

# Linking Libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${GLAD_LIB}
    ${GLFW_LIB}
    ${IMGUI_LIB}
    ${YAML_CPP_LIB}
    opengl32
)

# pre compiled header
set(PCH_HEADER "src/SD_PCH.h")
set(PCH_SOURCE "src/SD_PCH.cpp")
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADER})

# specifying the include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Libraries/glfw/include
    ${CMAKE_SOURCE_DIR}/Libraries/glad/include
    ${CMAKE_SOURCE_DIR}/Libraries/imgui
    ${CMAKE_SOURCE_DIR}/Headers/spdlog/include
    ${CMAKE_SOURCE_DIR}/Headers/glm
    ${CMAKE_SOURCE_DIR}/Libraries/stb
    ${CMAKE_SOURCE_DIR}/Headers/entt
    ${CMAKE_SOURCE_DIR}/Libraries/yaml_cpp/include

    ${CMAKE_SOURCE_DIR}/SodaEngine/src
)

# Settings compile type based on build type
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Debug>: -g -O0 -Wall -Wextra -Wpedantic -Wno-unused-parameter>
      $<$<CONFIG:DebRelease>: -g -O2 -Wall -Wextra -Wpedantic -Wno-unused-parameter>
      $<$<CONFIG:Release>: -O3 -DNDEBUG -Wall -Wextra -Wpedantic -Wno-unused-parameter>
      $<$<CONFIG:RelExport>: -O2 -DNDEBUG -Wall -Wextra -Wpedantic -Wno-unused-parameter>
      $<$<CONFIG:Export>: -O3 -DNDEBUG -Wall -Wextra -Wpedantic -Wno-unused-parameter>
  )
elseif(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
      $<$<CONFIG:Debug>: /Zi /Od /W4 /RTC1>
      $<$<CONFIG:DebRelease>: /O2 /W4>
      $<$<CONFIG:Release>: /O2 /DNDEBUG /W4>
      $<$<CONFIG:RelExport>: /O2 /DNDEBUG /W4>
      $<$<CONFIG:Export>: /O2 /DNDEBUG /W4>
  )
  # Specially For Visual Studio
  set_target_properties(${PROJECT_NAME} PROPERTIES
      VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  )
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# Settings compile definitions for build type
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:SD_Debug>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:DebRelease>:SD_DebRelease>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:SD_Release>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:RelExport>:SD_RelExport>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Export>:SD_Export>)

if(CMAKE_BUILD_TYPE MATCHES "RelExport|Export")
  target_compile_definitions(${PROJECT_NAME} PUBLIC SD_ExportBuild)
endif()

# some other defines
target_compile_definitions(${PROJECT_NAME} PUBLIC
    _CRT_SECURE_NO_WARNINGS

    GLFW_INCLUDE_NONE
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
    GLM_ENABLE_EXPERIMENTAL
    YAML_CPP_STATIC_DEFINE
)

# Copying libaries into the Bin folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Libraries/Bin/"
    "${CMAKE_SOURCE_DIR}/Bin/"
)