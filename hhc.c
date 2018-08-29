#include "hhc.h"
#include "common.h"

#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char** argv)
{
  HHC hhc_instance;
  if (hhc_initialize(&hhc_instance) != SUCCESS) {
    SDL_Log();
	hhc_cleanup();
  }

#if defined(__EMSCRIPTEN__)
  emscripten_set_main_loop_arg(hhc_update_and_render, &hhc, 0, 1);
#else
  const int DESIRED_FPS = 30;
  const float DESIRED_FRAME_TIME_MS = 1000.0f / DESIRED_FPS;
  int update_counter = 0;

  while (hhc->want_to_run) {
    while (total_delta_time > 0.0f && update_counter < max_update_steps) {
		
	}  
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
}
