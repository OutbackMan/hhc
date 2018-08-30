#include "output.h"
#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_img.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif


STATUS 
output_initialize(Output* output)
{
  output->mixer_is_initialized = false;
  output->mixer_is_opened = false;
  output->img_is_initialized = false;
  output->ttf_is_initialized = false;
  output->net_is_initialized = false;
  output->renderer.is_initialized = false;
  output->window.is_initialized = false;
	
  int subsystem_flags = MIX_INIT_OGG;
  if (Mix_Init(subsystem_flags) != subsystem_flags) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL mixer with OGG support: %s", 
	  Mix_GetError()
	);
	return FAILURE;
  } else {
    output->mixer_is_initialized = true;	  
  }
  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to open SDL mixer: %s", 
	  Mix_GetError()
	); 
	output_cleanup(output);
	return FAILURE;
  } else {
    output->mixer_is_opened = true;	  
  }

  subsystem_flags = IMG_INIT_PNG;
  if (IMG_Init(subsystem_flags) != subsystem_flags) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL image with PNG support: %s", 
	  IMG_GetError()
	);
	output_cleanup(output);
	return FAILURE;
  } else {
    output->img_is_initialized = true;	  
  }

  if (TTF_Init() < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL ttf: %s", 
	  TTF_GetError()
	);
	output_cleanup(output);
	return FAILURE;
  } else {
    output->ttf_is_initialized = true;	  
  }

  if (SDLNet_Init() < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL net: %s", 
	  SDLNet_GetError()
	);
	output_cleanup(output);
	return FAILURE;
  } else {
    output->net_is_initialized = true;	  
  }

#if defined(__EMSCRIPTEN__)
  output->are_on_mobile_device = \
    EM_ASM_INT({return typeof window.orientation !== 'undefined';});
#else
  output->are_on_mobile_device = false;
#endif

   output->window.title = "Default Window Title";
   output->window.x = SDL_WINDOWPOS_CENTERED;	  
   output->window.y = SDL_WINDOWPOS_CENTERED;	  
   output->window.width = 400;	  
   output->window.height = 400;	  
   output->window.flags = SDL_WINDOW_RESIZABLE; 	  
   output->window.is_shown = false;
   output->window.is_minimized = false;
   output->window.has_focus = false;
   output->window.is_closed = false;
   output->window.window = SDL_CreateWindow(
                            output->window.title, 
					        output->window.x, 
					        output->window.y,
					        output->window.width,
					        output->window.height,
					        output->window.flags
				          );
  if (output->window.window == NULL) {
    SDL_LogFatal(
      SDL_LOG_CATEGORY_SYSTEM,
      "Unable to create SDL window: %s",
	  SDL_GetError()
	);
	output_cleanup(output);
	return FAILURE;
  } else {
    output->window.is_initialized = true;	  
  }

  output->renderer.driver_index = -1;
  output->renderer.flags = SDL_RENDERER_ACCELERATED;
  output->renderer.window = output->window.window;
  output->renderer.renderer = SDL_CreateRenderer(
                                output->renderer.window,
								output->renderer.driver_index,
								output->renderer.flags
                              );
  if (output->renderer.renderer == NULL) {
    SDL_LogFatal(
      SDL_LOG_CATEGORY_SYSTEM,
      "Unable to create SDL renderer: %s",
	  SDL_GetError()
	);
	output_cleanup(output);
	return FAILURE;
  } else {
    output->renderer.is_initialized = true;	  
  }

  return SUCCESS;
}

void
output_update(Output* output, SDL_Event* event)
{
  SDL_assert(output	!= NULL && event != NULL);

  switch (event->type) {
  case SDL_WINDOWEVENT:
    switch (event->window.event) {
    case SDL_WINDOWEVENT_SHOWN:
	  output->window.is_shown = true;
	  break;
	case SDL_WINDOWEVENT_HIDDEN:
	  output->window.is_shown = false;
	  break;
	case SDL_WINDOWEVENT_MINIMIZED:
	  output->window.is_minimized = true;
	  break;
	case SDL_WINDOWEVENT_RESTORED:
	  output->window.is_minimized = false;
	  break;
	case SDL_WINDOWEVENT_MOVED:
	  output->window.x = event->window.data1;
	  output->window.y = event->window.data1;
	  break;
	case SDL_WINDOWEVENT_SIZE_CHANGED:
	case SDL_WINDOWEVENT_RESIZED:
	  output->window.width = event->window.data1;
	  output->window.height = event->window.data2;
	  break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
	  output->window.has_focus = true;
      break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
	  output->window.has_focus = false;
      break;
    case SDL_WINDOWEVENT_CLOSED:
	  output->window.is_closed = true;
	  break;
    NO_DEFAULT_CASE
	}
  NO_DEFAULT_CASE
  }
}

void
output_cleanup(Output* output)
{
  if (input->renderer.is_initialized) {
	SDL_DestroyRenderer(input->renderer.renderer);  
  }
  if (input->window.is_initialized) {
	SDL_DestroyWindow(input->window.window);
  }

  if (input->net_is_initialized) SDLNet_Quit();
  if (input->ttf_is_initialized) TTF_Quit();
  if (input->img_is_initialized) IMG_Quit();
  if (input->mixer_is_opened) Mix_CloseAudio();
  if (input->mixer_is_initialized) Mix_Quit();
}
