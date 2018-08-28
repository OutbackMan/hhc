#include "common.h"
#include "platform-layer.h"

#include <SDL2/SDL.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif


STATUS platform_layer_initialize(PlatformLayer* platform_layer)
{
#if defined(WANT_DEBUG_BUILD)
#define SDL_ASSERT_LEVEL 2
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
#define SDL_ASSERT_LEVEL 1
  SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
#endif

  SDL_assert(platform_layer != NULL);

  platform_layer->sdl_is_initialized = false;
  platform_layer->sdl_mixer_is_initialized = false;
  platform_layer->sdl_mixer_is_opened = false;
  platform_layer->sdl_img_is_initialized = false;
  platform_layer->sdl_ttf_is_initialized = false;
  platform_layer->sdl_net_is_initialized = false;
  
  platform_layer->renderer.is_initialized = false;
  platform_layer->window.is_initialized = false;

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
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL mixer with OGG support: %s", 
	  Mix_GetError()
	);
	platform_layer_cleanup(platform_layer);

	return FAILURE;
  } else {
    platform_layer->sdl_mixer_is_initialized = true;	  
  }
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to open SDL mixer: %s", 
	  Mix_GetError()
	); 
	platform_layer_cleanup(platform_layer);

	return FAILURE;
  } else {
    platform_layer->sdl_mixer_is_opened = true;	  
  }

  subsystem_flags = IMG_INIT_PNG;
  if (IMG_Init(subsystem_flags) != subsystem_flags) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL image with PNG support: %s", 
	  IMG_GetError()
	);
	platform_layer_cleanup(platform_layer);

	return FAILURE;
  } else {
    platform_layer->sdl_image_is_initialized = true;	  
  }

  if (TTF_Init() < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL ttf: %s", 
	  TTF_GetError()
	);
	platform_layer_cleanup(platform_layer);

	return FAILURE;
  } else {
    platform_layer->sdl_ttf_is_initialized = true;	  
  }

  if (SDLNet_Init() < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL net: %s", 
	  SDLNet_GetError()
	);
	platform_layer_cleanup(platform_layer);

	return FAILURE;
  } else {
    platform_layer->sdl_net_is_initialized = true;	  
  }

#if defined(__EMSCRIPTEN__)
  platform_layer->are_on_mobile_device = \
    EM_ASM_INT({return typeof window.orientation !== 'undefined';});
#else
  platform_layer->are_on_mobile_device = false;
#endif

  if (platform_layer__initialize_window(platform_layer) != SUCCESS) {
    platform_layer_cleanup(platform_layer);	  
	return FAILURE;
  }

  if (platform_layer__initialize_renderer(platform_layer) != SUCCESS) {
    platform_layer_cleanup(platform_layer);	  
	return FAILURE;
  }

  

}

void platform_layer_cleanup(PlatformLayer* platform_layer)
{
  SDL_assert(platform_layer != NULL);

  if (platform_layer->renderer.is_initialized) SDL_DestroyRenderer(platform_layer->renderer.renderer);
  if (platform_layer->window.is_initialized) SDL_DestroyWindow(platform_layer->window.window);

  if (platform_layer->sdl_net_is_initialized) SDLNet_Quit();
  if (platform_layer->sdl_ttf_is_initialized) TTF_Quit();
  if (platform_layer->sdl_img_is_initialized) IMG_Quit();
  if (platform_layer->sdl_mixer_is_opened) Mix_CloseAudio();
  if (platform_layer->sdl_mixer_is_initialized) Mix_Quit();
  if (platform_layer->sdl_is_initialized) SDL_Quit();
}

STATUS platform_layer__initialize_window(PlatformLayer__Window* window)
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
	return FAILURE;
  } else {
    window->is_initialized = true;	  
	return SUCCESS;
  }
}

STATUS platform_layer__initialize_renderer(PlatformLayer__Window* window, PlatformLayer__Renderer* renderer)
{
  SDL_assert(window != NULL && renderer != NULL);

  if (!renderer->) {
	  
  }

  renderer->renderer = SDL_CreateRenderer
}

void platform_layer_update(PlatformLayer* platform_layer, SDL_Event* event)
{
  SDL_assert(platform_layer	!= NULL && event != NULL);

  switch (event->type) {
  case SDL_WINDOWEVENT:
    switch (event->window.type) {
    case SDL_WINDOWEVENT_CLOSED:
	  platform_layer->window.closed = true;
	  break;
    NO_DEFAULT_CASE
	}
  case SDL_KEYDOWN:
    break;
  case SDL_KEYUP:
    break;
		
  case SDL_TEXTEDITINGEVENT
  case SDL_FINGERMOTION:
  case SDL_FINGERDOWN:
  case SDL_FINGERUP:

  case SDL_MULTIGESTURE: {
	  
  } break;
}

/*
  platform_layer_pull(&platform_layer);
    
	while (event) {

	platform_player_update(&platform_layer, event);
	}
*/

void platform_layer_window_restore_info();
