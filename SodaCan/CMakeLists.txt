cmake_minimum_required(VERSION 3.10)
project(SodaCan)

# adding files
file(GLOB_RECURSE SodaCan_SOURCES "src/*.cpp" "src/*.hpp" "src/*.c" "src/*.h")

# creating the exec
add_executable(${PROJECT_NAME} ${SodaCan_SOURCES})

# linking the Soda lib
target_link_libraries(${PROJECT_NAME} PRIVATE SodaEngine)

# include directories that includes the SodaEngine
include_directories(${CMAKE_SOURCE_DIR}/Headers/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR}/SodaEngine/include)
include_directories(${CMAKE_SOURCE_DIR}/Headers/glm)
include_directories(${CMAKE_SOURCE_DIR}/Libraries/imgui)
include_directories(${CMAKE_SOURCE_DIR}/Headers/entt)

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

# Post Build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/SodaCan/Assets"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}}/SodaCan/Assets"
)