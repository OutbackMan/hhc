cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

if(NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}/"build")
  message(FATAL_ERROR "Must perform an out of tree build inside 'build'") 
  return()
endif(NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}/"build")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
project(hhc VERSION 0.0.1 LANGUAGES C)

set(hhc_lib_name "hhc_lib" CACHE STRING "Latest hhc dynamic library name")
add_library(${hhc_lib_name} MODULE src/hhc.c)
target_include_directories(${hhc_lib_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
if(${hhc_lib_name} STREQUAL "hhc_lib")
  set(hhc_lib_name "hhc_lib_temp" 
    CACHE STRING "Latest hhc dynamic library name" FORCE
  )
else(${hhc_lib_name} STREQUAL "hhc_lib")
  set(hhc_lib_name "hhc_lib"
    CACHE STRING "Latest hhc dynamic library name" FORCE
  )
endif(${hhc_lib_name} STREQUAL "hhc_lib")

add_executable(hhc src/input.c src/output.c src/state.c)
# add_dependencies(hcc ${hhc_lib_name})

target_include_directories(hhc PRIVATE ${CMAKE_SOURCE_DIR}/include)

# handlebuildtype

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

find_package(SDL2_net REQUIRED)
target_include_directories(hhc PRIVATE ${SDL2_net_INCLUDE_DIRS})
target_link_libraries(hhc PRIVATE ${SDL2_net_LIBRARIES})




void* game_code_dll = SDL_LoadObject("game.so"); // we don't want any local_persists in this library

// windows exported functions .h files --> __declspec(dllimport) void func_name();

void load_game_code_dll(GameCode* game_code)
{
  game_code->render_fnc = (void (*)(void))SDL_LoadFunction(game_code_dll, "render");
}

void unload_game_code();

while (is_running) {
  if (load_counter++ > 120) {
    unload_game_code();
    load_game_code();
  }
}



=================
build command (cmake . and cmake --build) opens terminal buffer in new vertical split
command to run under debugger

=================
typedef struct State {
  FILE* recording_handle;
  int input_recording_index = 0;
  FILE* playback_handle;
  int input_playing_index = 0;
};

typedef struct recorded_input {
  int input_count;
  PlatformLayerInput input;
}

if (input->keys[SDL_SCANCODE_L].is_pressed) {
  if (input_recording_index == 0) {
    input_recording_index = 1;
  } else {
    input_recording_index = 0;
  }
  
}


if (state->input_recording_index) {
  record_input(state, input);
}
if (state->input_playing_index) {
  playback_input(state, input);
}
game_update_and_render(input, output (window and renderer))




manifest.json {
  "display": "fullscreen",
  "orientation": "landscape"
}
<link rel="manifest" href="manifest.json" />
/*
  perhaps use configure_file() to get the path of the js binary
*/
