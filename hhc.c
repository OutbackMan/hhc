#include "hhc.h"
#include "common.h"

#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char** argv)
{
#if defined(WANT_DEBUG_BUILD)
#define SDL_ASSERT_LEVEL 2
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
#define SDL_ASSERT_LEVEL 1
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
#endif

  HHC hhc_instance;
  if (hhc_initialize(&hhc_instance) != SUCCESS) {
    SDL_Log();
	hhc_cleanup();
  }

#if defined(__EMSCRIPTEN__)
  const int REQUEST_ANIMATION_FRAME_RATE = 0;
  const int SIMLUATE_INFINITE_LOOP = 1;
  emscripten_set_main_loop_arg(
    hhc_update_and_render, 
	&hhc_instance, 
	REQUEST_ANIMATION_FRAME_RATE, 
	SIMULATE_INFINITE_LOOP
  );
#else
  int monitor_refresh_rate = 60; // actually compute()
  int game_refresh_rate = monitor_refresh_rate; // may have to limit
  float seconds_per_frame = 1000.0f / (float)game_refresh_rate;

  while (hhc_instance.want_to_run) {
    hhc_update_and_render(&hhc_instance);
  }
#endif

  hhc_cleanup(&hhc_instance);

  return EXIT_SUCCESS;
}

void hhc_cleanup(HHC* hhc)
{
  input_cleanup(&hhc->input);
  output_cleanup(&hhc->output);
}

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
