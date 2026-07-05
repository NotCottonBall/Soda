cmake_minimum_required(VERSION 4.3)

project(SodaThirdParty)

#############
# Variables #
#############
set(SODA_OUTPUT_FOLDER_SIGNATURE
  ${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}
)

set(SODA_BINARY_DIR
  ${CMAKE_SOURCE_DIR}/Binary
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


####################
# Settings For SDL #
####################
set(SDL_X11_XSCRNSAVER OFF CACHE BOOL "" FORCE)
set(SDL_X11_XTEST OFF CACHE BOOL "" FORCE)


#########################
# Adding Subdirectories #
#########################
add_subdirectory(${CMAKE_SOURCE_DIR}/SDL)


################
# Adding ImGui #
################
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

  ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_vulkan.cpp
  ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp
)
target_include_directories(imgui PRIVATE
  ${CMAKE_SOURCE_DIR}/imgui ${CMAKE_SOURCE_DIR}/sdl/include
)
find_package(Vulkan REQUIRED)
target_link_libraries(imgui PRIVATE
  Vulkan::Vulkan
)
