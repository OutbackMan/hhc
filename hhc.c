#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


#define HHC_GLOBAL static

HHC_GLOBAL bool want_to_run = true;

int main(int argc, char** argv)
{
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL: %s", 
	  SDL_GetError()
	);  

	return EXIT_FAILURE;
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

  const uint8_t* cur_keyboard_state = SDL_GetKeyboardState(NULL);
  const uint8_t* prev_keyboard_state = cur_keyboard_state;

  int cur_mouse_x = 0;
  int cur_mouse_y = 0;
  int prev_mouse_x = 0;
  int prev_mouse_y = 0;
  uint32_t cur_mouse_state = SDL_GetMouseState(NULL, NULL);
  uint32_t prev_mouse_state = cur_mouse_state;

  // support rumble later
  SDL_GameController *controllers[4] = {0}; 
  int32_t controller_indexes[4] = {0};
  uint8_t num_controllers_connected = 0;

  while (want_to_run) {
    SDL_Event event = {0}; 	  
	while (SDL_PollEvent(&event)) {
      cur_keyboard_state = SDL_GetKeyboardState(NULL); 
	  cur_mouse_state = SDL_GetMouseState(&cur_mouse_x, &cur_mouse_y);

      switch (event.type) {
	  case SDL_QUIT:
	    want_to_run = false;
	  case SDL_CONTROLLERDEVICEEVENT:
	    switch (event.cdevice.type) {
	    case SDL_CONTROLLERDEVICEDADDED:
		  controller_indexes[num_controllers_connected++] = event.cdevice.which; 
		  controllers[num_controllers_connected++] = SDL_GameControllerOpen(event.cdevice.which); 
		case SDL_CONTROLLERDEVICEREMOVED:
		  for (size_t controller_indexes_pos = 0; controller_indexes_pos < 4; ++controller_indexes_pos) {
		    if (controller_indexes[controller_indexes_pos] == event.cdevice.which) {
		      SDL_GameControllerClose(controllers[controller_indexes_pos]);
			  --num_controllers_connected;
			}
		  }
	  case SDL_WINDOWEVENT:
	    switch (event.window.event) {
	    case SDL_WINDOWEVENT_CLOSE:
		  want_to_run = false;
		}  
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
  } 

__cleanup:
  if (window != NULL) SDL_DestroyWindow(window);  
  if (renderer != NULL) SDL_DestroyRenderer(renderer);

  SDL_Quit();	  
  return EXIT_SUCCESS; 
}
