cmake_minimum_required(VERSION 3.31)
project(SodaLibraries)

# Now, this is useful for me because I want seperate output folders for seperate platforms and compilers.
foreach(CONFIG "Debug" "Release")
    string(TOUPPER ${CONFIG} CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_SOURCE_DIR}/Bin/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_SOURCE_DIR}/Libs/Libraries${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER}
        "${CMAKE_SOURCE_DIR}/Libs/${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}")
endforeach()

# Adding Outside Stuff
add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/yaml_cpp)

# Adding Libraries
# IMGUI
add_library(imgui STATIC
    ${CMAKE_SOURCE_DIR}/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/imgui/imgui.h
    ${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/imgui/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/imgui/imstb_truetype.h
    ${CMAKE_SOURCE_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp

    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
)
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/imgui ${CMAKE_SOURCE_DIR}/glfw/include)

# No need to do anything special for yaml_cpp

# GALD
add_library(glad STATIC ${CMAKE_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/glad/include)