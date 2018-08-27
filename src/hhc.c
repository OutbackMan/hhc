#include "hhc.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stdlib.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

INTERNAL loop(void)
{

#if defined(__EMSCRIPTEN__)
  if (!global_want_to_run) {
    emscripten_cancel_main_loop();	  
  }
#endif
}

int main(int argc, char** argv)
{
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
  int exit_status;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL: %s", 
	  SDL_GetError()
	);  

	exit_status = EXIT_FAILURE;
	goto __exit;
  } 

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL mixer: %s", 
	  Mix_GetError()
	);  

	exit_status = EXIT_FAILURE;
	goto __exit;
  }
  
  if (IMG_Init()) {
	  
	SDL_Quit();
	return EXIT_FAILURE;
  }

  if (TTF_Init()) {
	  
  }

  if ()

  global_input = {
    .cur_keyboard_state = SDL_GetKeyboardState(NULL),
    .prev_keyboard_state = SDL_GetKeyboardState(NULL),
    .cur_mouse_x = 0,
    .cur_mouse_y = 0,
    .prev_mouse_x = 0,
    .prev_mouse_y = 0,
	.cur_mouse_state = SDL_GetMouseState(NULL, NULL),
	.prev_mouse_state = SDL_GetMouseState(NULL, NULL), 
    .controllers = {NULL, NULL, NULL, NULL},
	.num_controllers_connected = 0,
	.controllers_haptic_support = {false, false, false, false},
	._controller_indexes = {0, 0, 0, 0},
  };

  global_want_to_run = true;

#if defined(__EMSCRIPTEN__)
  emscripten_set_main_loop(loop, 0, 1);
#else
  while (global_want_to_run) {
    loop();	  
  }
#endif

  exit_status = EXIT_SUCCESS;

__cleanup:
  if (SDL_WasInit(SDL_INIT_EVERYTHING) != 0) SDL_Quit(); 


  return exit_status;	
}

  SDL_Window* window = NULL; 
  window = SDL_CreateWindow(
             "HHC", 
			 SDL_WINDOWPOS_CENTERED,
			 SDL_WINDOWPOS_CENTERED,
			 640,
			 480,
			 SDL_WINDOW_RESIZABLE
		   );
  if (window == NULL) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to create SDL window: %s", 
	  SDL_GetError()
	); 

    goto __cleanup;
  }

  // sethint texture
  SDL_Renderer* renderer = NULL;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to create SDL renderer: %s", 
	  SDL_GetError()
	); 

    goto __cleanup;
  }

  uint64_t perf_counter_freq = SDL_GetPeformanceFrequency(); // units per second
  uint64_t prev_counter = SDL_GetPerformanceCounter(); // units

  while (want_to_run) {
    SDL_Event event = {0}; 	  
	while (SDL_PollEvent(&event)) {
      cur_keyboard_state = SDL_GetKeyboardState(NULL); 
	  cur_mouse_state = SDL_GetMouseState(&cur_mouse_x, &cur_mouse_y);

      switch (event.type) {
	  case SDL_QUIT:
	    want_to_run = false;
		break;
	  case SDL_CONTROLLERDEVICEADDED:
	    if (input.num_controllers_connected != MAX_NUM_CONTROLLERS) {
		  _InputController* controller = input.controllers[input.num_controllers_connected];

		  controller->joystick_handle = SDL_JoystickOpen(event.cdevice.which);
		  controller->haptic_handle = SDL_HapticOpenFromJoystick(controller->joystick_handle);
		  SDL_HapticRumbleInit(controller->haptic_handle);
          SDL_GameControllerOpen(event.cdevice.which); 
		}
		break;
	  case SDL_CONTROLLERDEVICEREMOVED:
		for (size_t controller_indexes_pos = 0; controller_indexes_pos < 4; ++controller_indexes_pos) {
		  if (controller_indexes[controller_indexes_pos] == event.cdevice.which) {
		    SDL_GameControllerClose(controllers[controller_indexes_pos]);
		    --num_controllers_connected;
		  }
		}
		break;
	  case SDL_WINDOWEVENT:
	    switch (event.window.event) {
	    case SDL_WINDOWEVENT_CLOSE:
		  want_to_run = false;
		  break;
		}  
	    break;
	  }

     prev_keyboard_state = cur_keyboard_state;
	 prev_mouse_state = cur_mouse_state; 
	 prev_mouse_x = cur_mouse_x;
	 prev_mouse_y = cur_mouse_y;
	}

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
  } 

__cleanup:
  if (window != NULL) SDL_DestroyWindow(window);  
  if (renderer != NULL) SDL_DestroyRenderer(renderer);

  Mix_Quit();
  SDL_Quit();	  
  return EXIT_SUCCESS; 
}
