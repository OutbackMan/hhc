#include "common.h"
#include "platform-layer.h"

#include <SDL2/SDL.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#include <string.h>


STATUS platform_layer_initialize(PlatformLayer* platform_layer)
{
#if defined(WANT_DEBUG_BUILD)
#define SDL_ASSERT_LEVEL 2
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
#define SDL_ASSERT_LEVEL 1
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
#endif

  platform_layer->sdl_is_initialized = false;
  platform_layer->sdl_mixer_is_initialized = false;
  platform_layer->sdl_img_is_initialized = false;
  platform_layer->sdl_ttf_is_initialized = false;
  platform_layer->sdl_net_is_initialized = false;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL: %s", 
	  SDL_GetError()
	);  
	platform_layer_cleanup(platform_layer);

	return FAILURE;
  } else {
    platform_layer->sdl_is_initialized = true;
  }

  int subsystem_flags = MIX_INIT_OGG;
  if (Mix_Init(subsystem_flags) != subsystem_flags) {
	  
  }
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL mixer: %s", 
	  Mix_GetError()
	);  
  }

  subsystem_flags = IMG_INIT_PNG;
  if (IMG_Init(subsystem_flags) != subsystem_flags) {
	  
  }

  if (TTF_Init() < 0) {
	  
  }

  if (SDLNet_Init() < 0) {
	  
  }

  platform_layer->error_buffer = {0};

#if defined(__EMSCRIPTEN__)
  platform_layer->are_on_mobile_device = \
    EM_ASM_INT({return typeof window.orientation !== 'undefined';});
#else
  platform_layer->are_on_mobile_device = false;
#endif

  if (platform_layer__initialize_window(platform_layer) != SUCCESS) {
	SDL_LogFatal(
      SDL_LOG_CATEGORY_APPLICATION,
	  "Unable to initialize window component of platform layer: %s",
	  platform_layer->error_buffer
	);
	return FAILURE;
  }
  

}

void platform_layer_cleanup(PlatformLayer* platform_layer)
{
  if (platform_layer->sdl_net_is_initialized) SDLNet_Quit();
  if (platform_layer->sdl_ttf_is_initialized) TTF_Quit();
  if (platform_layer->sdl_img_is_initialized) IMG_Quit();
  if (platform_layer->sdl_mixer_is_initialized) Mix_Quit();
  if (platform_layer->sdl_is_initialized) SDL_Quit();
}

STATUS platform_layer__initialize_window(PlatformLayer* platform_layer)
{
  if (!window->title) {
    window->title = "Default Window Title";
  }
  if (!window->x) {
    window->x = SDL_WINDOWPOS_CENTERED;	  
  }
  if (!window->y) {
    window->y = SDL_WINDOWPOS_CENTERED;	  
  }
  if (!window->width) {
    window->width = 400;	  
  }
  if (!window->height) {
    window->height = 400;	  
  }
  if (!window->flags) {
    window->flags = SDL_WINDOW_RESIZABLE; 	  
  }

  window->window = SDL_CreateWindow(
                     window->title, 
					 window->x, 
					 window->y,
					 window->width,
					 window->height,
					 window->flags
				   );
  if (window->window == NULL) {
    SDL_LogFatal(
      SDL_LOG_CATEGORY_SYSTEM,
      "Unable to create SDL window: %s",
	  SDL_GetError()
	);
    strcpy(platform_layer->error_buffer, "SDL_CreateWindow() failed");
	return FAILURE;
  }

  return SUCCESS;
}
  // text, audio, net

  if (platform_layer->renderer_handle.driver) {}
  platform_layer->renderer_handle.renderer = SDL_CreateRenderer(); 

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
