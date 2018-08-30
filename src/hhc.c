#include "hhc.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
#if defined(WANT_DEBUG_BUILD)
#define SDL_ASSERT_LEVEL 2
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
#define SDL_ASSERT_LEVEL 1
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
#endif

  Input input;
  input_initialize(&input);

  Output output;
  output_initialize(&output);

  State state;
  state_initialize(&state);
	
#if defined(__EMSCRIPTEN__)
  const int FPS = 60;
  const int SIMLUATE_INFINITE_LOOP = 1;
  emscripten_set_main_loop_arg(
    hhc_update_and_render, 
	&hhc_instance, 
	FPS, 
	SIMULATE_INFINITE_LOOP
  );
#else
  int monitor_refresh_rate = 60; // actually compute()
  float seconds_per_frame = 1000.0f / (float)game_refresh_rate;

  HHC hhc;

  while (state.want_to_run) {
    if (copy_lib_if_changed()) {
	  LoadLibrary();	

      // function pointers 
  	}

	hhc.update_and_render(&hhc, &input, &output, &state);
  }
#endif

  return EXIT_SUCCESS;
}

#if defined(__unix__)
#include <sys/stat.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif
INTERNAL void
load_latest_hhc(HHC* hhc)
{
#if defined(_MSC_VER)
  const char* hhc_lib_name = "build\\hhc_lib.dll";
  const char* hhc_lib_temp_name = "build\\hhc_lib_temp.dll";
#elif defined(__APPLE__)
  const char* hhc_lib_name = "build\\hhc_lib.dylib";
  const char* hhc_lib_temp_name = "build\\hhc_lib_temp.dylib";
#else
  const char* hhc_lib_name = "build\\hhc_lib.so";
  const char* hhc_lib_temp_name = "build\\hhc_lib_temp.so";
#endif

  copy_file(hhc_lib_name, hhc_lib_temp_name);
  load(hhc_lib_temp_name);
}

INTERNAL void
unload_hhc();

void hhc_update_and_render(HHC* hhc)
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event->type == SDL_QUIT) {
      hhc->want_to_run = false;		
	}
	input_update(&event, &hhc->input);
	output_update(&event, &hhc->output);
  }

  // place directly into input file
  if (hhc->input.recording_index) {
    input_record();	  
  }
  if (hhc->input.playing_index) {
    input_playback();
  } 

#if defined(__EMSCRIPTEN__)
  if (!hhc->want_to_run) {
    emscripten_cancel_main_loop();	  
  }
#endif

INTERNAL loop(void)
{
  uint64_t perf_counter_freq = SDL_GetPeformanceFrequency(); // units per second
  uint64_t prev_counter = SDL_GetPerformanceCounter(); // units

    SDL_Event event = {0}; 	  
	while (SDL_PollEvent(&event)) {

    if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE) < 0) {
	  SDL_LogWarn(
	    SDL_LOG_CATEGORY_SYSTEM, 
		"Unable to set SDL renderer draw color %s", 
		SDL_GetError()
	  );
	}
    if (SDL_RenderClear(renderer) < 0) {
	  SDL_LogWarn(
	    SDL_LOG_CATEGORY_SYSTEM, 
		"Unable to clear SDL renderer %s", 
		SDL_GetError()
	  );
	}
    SDL_RenderPresent(renderer);

    // play sound here 

    double ms_per_frame = 1000.0f * (counter / perf_count_frequency);
	int fps = perf_count_frequency / counter;
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%.02f ms/f, %.02f/s");
#if defined(__EMSCRIPTEN__)
  if (!global_want_to_run) {
    emscripten_cancel_main_loop();	  
  }
#endif
}

