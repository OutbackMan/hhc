cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

project(HHC 0.0.1 C)

set(_hhc_sources
  hhc.c
)

add_executable(hhc ${_hhc_sources})

# handlebuildtype

# PRIVATE only affects this library not programs that depend on it
target_include_directories(hhc PRIVATE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(hhc PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

find_package(SDL2 REQUIRED)
target_include_directories(hhc PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(hhc PRIVATE ${SDL2_LIBRARIES})

find_package(SDL2_mixer REQUIRED)
target_include_directories(hhc PRIVATE ${SDL2_mixer_INCLUDE_DIRS})
target_link_libraries(hhc PRIVATE ${SDL2_mixer_LIBRARIES})

find_package(SDL2_image REQUIRED)
target_include_directories(hhc PRIVATE ${SDL2_image_INCLUDE_DIRS})
target_link_libraries(hhc PRIVATE ${SDL2_image_LIBRARIES})

find_package(SDL2_ttf REQUIRED)
target_include_directories(hhc PRIVATE ${SDL2_ttf_INCLUDE_DIRS})
target_link_libraries(hhc PRIVATE ${SDL2_ttf_LIBRARIES})

configure_file(
  ${CMAKE_SOURCE_DIR}/include/common.h.in
  ${CMAKE_SOURCE_DIR}/build/include/common.h
)
