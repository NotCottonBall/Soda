project(SodaEngine)

# Including Stuff
include(./CMake/Files.cmake)

# Generating The Library
add_library(${PROJECT_NAME} STATIC)

# Sources For The Library SodaEngine.
# All The Sources Will Be Updated In "./CMake/Files.cmake"
target_sources(${PROJECT_NAME} PRIVATE
  ${SODA_ENGINE_SOURCES}
)


################################
# FINDING AND ADDING LIBRARIES #
################################
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
find_package(Vulkan REQUIRED)

# Adding Libraries (System)
target_link_libraries(${PROJECT_NAME} PRIVATE
  Vulkan::Vulkan
)


########################
# ADDING INCLUDE PATHS #
########################
target_include_directories(${PROJECT_NAME} PRIVATE
  ${SODA_THIRD_PARTY_DIR}/SDL/include
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${SODA_THIRD_PARTY_DIR}/glm
)


#######################################################
# SETTINGS COMPILE DEFINITIONS FOR BUILD/CONFIG TYPES #
#######################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Debug>:SD_Debug>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  $<$<CONFIG:Release>:SD_Release>
)


# Some Other Definitions and Settings
target_compile_definitions(${PROJECT_NAME} PRIVATE
  GLM_ENABLE_EXPERIMENTAL
)
