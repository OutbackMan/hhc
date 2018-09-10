cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

if(NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}/"build")
  message(FATAL_ERROR "Must perform an out of tree build inside 'build'") 
  return()
endif(NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}/"build")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
project(hhc VERSION 0.0.1 LANGUAGES C)

add_library(hhc-lib MODULE src/hhc.c)

add_executable(hhc src/input.c src/output.c src/state.c)
# add_dependencies(hcc ${hhc_lib_name})

target_include_directories(hhc PRIVATE ${CMAKE_SOURCE_DIR}/include)

# handlebuildtype

make_minimum_required(VERSION 3.12.0 FATAL_ERROR)                              
                                                                                
# use clang 6.0.1                                                               
# __COUNTER__, vectors?, float16,                                               
# __attribute__((deprecated("msg"))                                             
# enum { name __attribute__((deprecated("msg")) }                               
# __builtins                                                                    
                                                                                
                                                                                
project(Example VERSION 0.0.1 LANGUAGES C)                                      
                                                                                
# LLVM developers YouTube                                                       
#if(${CMAKE_C_COMPILER} STREQUAL "Clang")                                       
  string(REPLACE "." "" CLANG_VERSION ${CMAKE_C_COMPILER_VERSION})              
  if(${CLANG_VERSION} LESS "600")                                               
 # -Wall --> everything up until 1980                                           
  "-Wformat=2 -Wall -Wextra -Wpedantic -Wfloat-equal -Wshadow -Wunreachable-code"
  ""                                                                            
  endif(${CLANG_VERSION} LESS "600")                                            
                                                                                
  target_compile_options()                                                      
# also check for emscripten                                                     
#endif(${CMAKE_C_COMPILER} STREQUAL "Clang")                                    
                                                                                
# set(SDL_SHARED OFF CACHE BOOL "Disable building sdl2 as a shared library")       
# set(SDL_STATIC ON CACHE BOOL "Enable building sdl2 as a static library")         
# add_subdirectory(${Example_SOURCE_DIR}/lib/SDL2)                              
                                                                                
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Example_SOURCE_DIR}) # binary and dynamic libraries will reference files via assets/
add_executable(Example src/example.c)                                           
# pass ${CMAKE_SOURCE_DIR} to configure file.                                   
                                                                                
# target_link_libraries(Example SDL2)                                           
# target_include_directories(Example PRIVATE ${SDL2_SOURCE_DIR}/include)        
                                                                                
~                                                                       





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
