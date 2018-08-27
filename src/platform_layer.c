#include "common.h" // define error codes and compiler stuff 
#include "platform-layer.h"

#include <SDL2/SDL.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

STATUS platform_layer_initialize(PlatformLayer* platform_layer)
{
#if defined(__EMSCRIPTEN__)
  // on_touch
#endif
  STATUS window_init_status = platform_layer_initialize_window(
                                platform_layer->window_handle
							  );
  if (window_init_status != SUCCESS) {
	SDL_LogFatal(
      SDL_LOG_CATEGORY_APPLICATION,
	  "Unable to initialize window component of platform layer: %s",
	  status_msg(window_init_status);
	);
	return FAILURE;
  }
}


  if (platform_layer->window_handle.title) {
    platform_layer->window_handle.title = "Default Window Title";
  }
  int window_x;
  if (platform_layer->window_handle.x) {
    window_x = platform_layer->window_handle.x;
  } else {
    window_x = SDL_WINDOWPOS_CENTERED;	  
  }
  int window_y;
  if (platform_layer->window_handle.y) {
    window_y = platform_layer->window_handle.y;
  } else {
    window_y = SDL_WINDOWPOS_CENTERED;	  
  }
  int window_width;
  if (platform_layer->window_handle.width) {
    window_width = platform_layer->window_handle.width;
  } else {
    window_width = 400;	  
  }
  int window_height;
  if (platform_layer->window_handle.height) {
    window_height = platform_layer->window_handle.height;
  } else {
    window_height = 400;	  
  }
  platform_layer->window_handle.window = SDL_CreateWindow();
  if (platform_layer->window_handle == NULL) {
    return false; 	  
  }

  if (platform_layer->renderer_handle.driver) {}
  platform_layer->renderer_handle.renderer = SDL_CreateRenderer(); 

  // text, audio, net
}

/*
  platform_layer_pull(&platform_layer);
    
	while (event) {

	platform_player_update(&platform_layer, event);
	}
*/


void platform_layer_window_restore_info();

void platform_layer_update(SDL_Event* event)
{
  switch (event->type) {
  case SDL_WINDOWEVENT: {
    switch (event->window.type) {
      		
    }
  } break;
  case SDL_FINGERMOTION:
  case SDL_FINGERDOWN:
  case SDL_FINGERUP:

  case SDL_MULTIGESTURE: {
	  
  } break;
}

void platform_layer_cleanup();





void platform_layer_update_digital_button(
  PlatformLayer__DigitalButton* button
  bool is_down)
{
  	
}
