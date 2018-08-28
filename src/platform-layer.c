#include "common.h"
#include "platform-layer.h"

#include <SDL2/SDL.h>

#include <stdbool.h>

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

// logical_size() perhaps
  renderer->renderer = SDL_CreateRenderer
}

void platform_layer_update(PlatformLayer* platform_layer, SDL_Event* event)
{
  SDL_assert(platform_layer	!= NULL && event != NULL);

  switch (event->type) {
  case SDL_WINDOWEVENT:
    switch (event->window.event) {
    case SDL_WINDOWEVENT_SHOWN:
	  platform_layer->window.is_shown = true;
	  break;
	case SDL_WINDOWEVENT_HIDDEN:
	  platform_layer->window.is_shown = false;
	  break;
	case SDL_WINDOWEVENT_MINIMIZED:
	  platform_layer->window.is_minimized = true;
	  break;
	case SDL_WINDOWEVENT_RESTORED:
	  platform_layer->window.is_minimized = false;
	  break;
	case SDL_WINDOWEVENT_MOVED:
	  platform_layer->window.x = event->window.data1;
	  platform_layer->window.y = event->window.data1;
	  break;
	case SDL_WINDOWEVENT_SIZE_CHANGED:
	case SDL_WINDOWEVENT_RESIZED:
	  platform_layer->window.width = event->window.data1;
	  platform_layer->window.height = event->window.data2;
	  break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
	  platform_layer->window.has_focus = true;
      break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
	  platform_layer->window.has_focus = false;
      break;
    case SDL_WINDOWEVENT_CLOSED:
	  platform_layer->window.is_closed = true;
	  break;
    NO_DEFAULT_CASE
	}
  case SDL_KEYDOWN: 
    platform_layer__digital_button_update(
	  platform_layer->keys[event->key.keysym.scancode],
	  true
	);
    break;
  case SDL_KEYUP:
    platform_layer__digital_button_update(
	  platform_layer->keys[event->key.keysym.scancode],
	  false
	);
    break;
  case SDL_MOUSEMOTION:
    platform_layer->mouse.x = event->motion.x;
    platform_layer->mouse.y = event->motion.y;
    platform_layer->mouse.delta_x = event->motion.xrel;
    platform_layer->mouse.delta_y = event->motion.yrel;
    break;
  case SDL_MOUSEBUTTONDOWN:
    if (event->button.button == SDL_BUTTON_LEFT) {
      platform_layer__digital_button_update(
	    platform_layer->mouse.left_btn,
	    true
	  );
	}
    if (event->button.button == SDL_BUTTON_MIDDLE) {
      platform_layer__digital_button_update(
	    platform_layer->mouse.middle_btn,
	    true
	  );
	}
    if (event->button.button == SDL_BUTTON_RIGHT) {
      platform_layer__digital_button_update(
	    platform_layer->mouse.right_btn,
	    true
	  );
	}
    break;
  case SDL_MOUSEBUTTONUP:
    if (event->button.button == SDL_BUTTON_LEFT) {
      platform_layer__digital_button_update(
	    platform_layer->mouse.left_btn,
	    false
	  );
	}
    if (event->button.button == SDL_BUTTON_MIDDLE) {
      platform_layer__digital_button_update(
	    platform_layer->mouse.middle_btn,
	    false
	  );
	}
    if (event->button.button == SDL_BUTTON_RIGHT) {
      platform_layer__digital_button_update(
	    platform_layer->mouse.right_btn,
	    false
	  );
	}
    break;
  case SDL_MOUSEWHEEL:
   platform_layer->mouse.scrolled_vertically = event->wheel.y;
   break;
  case SDL_CONTROLLERDEVICEADDED:
    if (platform_layer->num_active_controllers != 
	  PLATFORM_LAYER_MAX_NUM_ACTIVE_CONTROLLERS) {
      platform_layer->controllers[platform_layer.num_active_controllers] = \
	    SDL_GameControllerOpen(event->cdevice.which);
		  if (platform_layer->controllers[platform_layer.num_active_controllers] == NULL) {
		    SDL_LogWarn(
			  SDL_LOG_CATEGORY_SYSTEM, 
			  "Unable to open controller %s", 
			  SDL_GetError()
			);
		    break;	  
		  }

          SDL_Joystick* controller_joystick = SDL_GameControllerGetJoystick(controller->controller);
		  controller->haptic_handle = SDL_HapticOpenFromJoystick(controller_joystick);
		  if (controller->haptic_handle != NULL) {
		    if (SDL_HapticRumbleInit(controller->haptic_handle)) {
			  SDL_LogWarn(
			    SDL_LOG_CATEGORY_SYSTEM, 
				"Unable to initialize controller haptic handle %s", 
				SDL_GetError()
			  );
			}
		  }

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

  case SDL_CONTROLLERBUTTONDOWN:
  case SDL_CONTROLLERBUTTONUP:
  case SDL_FINGERDOWN: {
    bool is_existing_touch = false;	
    for (size_t f_id = 0; f_id < platform_layer->num_active_touches; ++f_id) {
	  if (platform_layer->touches[f_id].id == event->tfinger.fingerId) {
        platform_layer__digital_button_update(
	      platform_layer->touches[f_id].btn
	      true
	    );
        platform_layer->touches[f_id].x = event->tfinger.x;
        platform_layer->touches[f_id].y = event->tfinger.y;
		is_existing_touch = true;
		break;
	  }
	}
	if (!is_existing_touch && 
	  platform_layer->num_active_touches < PLATFORM_LAYER_MAX_ACTIVE_TOUCHES) {
      platform_layer__digital_button_update(
	    platform_layer->touches[platform_layer->num_active_touches].btn
	    true
	  );
      platform_layer->touches[platform_layer->num_active_touches].x = \
	    event->tfinger.x;
      platform_layer->touches[platform_layer->num_active_touches].y = \
	    event->tfinger.y;
      platform_layer->touches[platform_layer->num_active_touches++].id = \
	    event->tfinger.fingerId;
	}
  } break;
  case SDL_FINGERUP:
    for (int f_id = 0; f_id < platform_layer->num_active_touches; ++f_id) {
	  if (platform_layer->touches[f_id].id == event->tfinger.fingerId) {
        platform_layer__digital_button_update(
	      platform_layer->touches[f_id].btn
	      false
	    );
        platform_layer->touches[f_id].x = -1;
        platform_layer->touches[f_id].y = -1;
        platform_layer->touches[f_id].id = -1;
	    --platform_layer->num_active_touches;	
		break;
	  }
	}
	break;
}

void platform_layer__handle_controller_btn() {
  if (event->cbutton.button == SDL_CONTROLLER_BUTTON_A) {
    platform_layer__digital_button_update(
	  platform_layer->controllers[i].a_btn
	  is_down
    );
  }
}

void platform_layer__handle_controller_btn_down() {
  platform_layer__handle_controller_btn(true);
}
void platform_layer__handle_controller_btn_up() {
  platform_layer__handle_controller_btn(true);
}

void platform_layer__handle_finger_down() {}
void platform_layer__handle_finger_up() {}

void platform_layer__digital_button_update()
{
  bool was_down = btn->is_down;
  btn->is_down = is_down;
  btn->is_pressed = !btn->was_down && is_down;
  btn->is_released = btn->was_down && !is_down;
}

void platform_layer_restore(PlatformLayer* platform_layer)
{
  platform_layer->mouse.scrolled_vertically = 0;
}

/*
  platform_layer_intialize(&platform_layer);
    
	while (event) {

	platform_player_update(&platform_layer, event);
	}

	platform_layer_restore();
*/
