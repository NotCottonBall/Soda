cmake_minimum_required(VERSION 3.10)


# setting the output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin-objs/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}")


# files of the Soda project
file(GLOB_RECURSE SODA_SOURCES "src/*.cpp" "src/*.hpp" "src/*.c" "src/*.h")
file(GLOB_RECURSE STBI_SOURCES "submodules/stb_image/*.cpp" "submodules/stb_image/*.h")
file(GLOB_RECURSE GLM_SOURCES "submodules/glm/glm/*.hpp")


# creating the static lib
add_library(SodaEngine STATIC ${SODA_SOURCES} ${STBI_SOURCES} ${GLM_SOURCES})


# setting up libraries
add_subdirectory(submodules/glfw)

# imgui library
add_library(imgui STATIC
    submodules/imgui/imconfig.h
    submodules/imgui/imgui.h
    submodules/imgui/imgui.cpp
    submodules/imgui/imgui_draw.cpp
    submodules/imgui/imgui_internal.h
    submodules/imgui/imgui_widgets.cpp
    submodules/imgui/imstb_rectpack.h
    submodules/imgui/imstb_textedit.h
    submodules/imgui/imstb_truetype.h
    submodules/imgui/imgui_demo.cpp
    submodules/imgui/imgui_tables.cpp

    submodules/imgui/backends/imgui_impl_opengl3.cpp
    submodules/imgui/backends/imgui_impl_glfw.cpp
)
# adding the include dirs
target_include_directories(imgui PUBLIC submodules/imgui)


# glad library
add_library(glad STATIC submodules/glad/src/glad.c)
# setting the include dirs
target_include_directories(glad PUBLIC submodules/glad/include)


target_link_libraries(SodaEngine PRIVATE glad glfw imgui opengl32)


# Build specific stuff
target_compile_definitions(SodaEngine PUBLIC $<$<CONFIG:Debug>:SD_DEBUG>)
target_compile_definitions(SodaEngine PUBLIC $<$<CONFIG:RELEASE>:SD_RELEASE>)
target_compile_definitions(SodaEngine PUBLIC $<$<CONFIG:Dist>:SD_DIST>)
# some other defines
target_compile_definitions(SodaEngine PUBLIC
    _CRT_SECURE_NO_WARNINGS
    
    GLFW_INCLUDE_NONE
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)


# pre compiled header
set(PCH_HEADER "src/SD_PCH.h")
set(PCH_SOURCE "src/SD_PCH.cpp")

target_precompile_headers(SodaEngine PRIVATE ${PCH_HEADER})


# specifying the include directories
include_directories(src)
include_directories(submodules/glfw/include)
include_directories(submodules/glad/include)
include_directories(submodules/imgui)
include_directories(submodules/spdlog/include)
include_directories(submodules/glm)
include_directories(submodules/stb_image)
include_directories(submodules/entt)