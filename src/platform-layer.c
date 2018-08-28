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

  platform_layer_initialize_input(platform_layer);

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

  if (platform_layer__initialize_window(platform_layer->window) != SUCCESS) {
    platform_layer_cleanup(platform_layer);	  
	return FAILURE;
  }

  if (platform_layer__initialize_renderer(platform_layer->renderer) != SUCCESS) {
    platform_layer_cleanup(platform_layer);	  
	return FAILURE;
  }

  return SUCCESS;
}

INTERNAL void platform_layer__create_default_digital_btn(
  PlatformLayer__DigitalButton* btn
)
{
  btn->is_down = false;
  btn->is_pressed = false;
  btn->is_released = false;
}

INTERNAL void platform_layer__initialize_controller(
  PlatformLayer__Controller* controller
)
{
  controller->joystick_index = -1;
  controller->controller = NULL;
  controller->haptic = NULL;
  // replace with x-macros --> just use {0} ??
  controller->dpad_left = PLATFORM_LAYER_DEFAULT_DIGITAL_BTN;
  platform_layer__create_default_digital_btn(&controller->dpad_up); 
  platform_layer__create_default_digital_btn(&controller->dpad_right); 
  platform_layer__create_default_digital_btn(&controller->dpad_down); 
  platform_layer__create_default_digital_btn(&controller->left_stick_btn);

  platform_layer__create_default_digital_btn(&controller->left_stick;

  platform_layer__create_default_digital_btn(&controller->back_btn); 
  platform_layer__create_default_digital_btn(&controller->start_btn); 
  platform_layer__create_default_digital_btn(&controller->right_stick_btn);

  platform_layer__create_default_digital_btn(&controller->right_stick;

  platform_layer__create_default_digital_btn(&controller->a_btn); 
  platform_layer__create_default_digital_btn(&controller->b_btn); 
  platform_layer__create_default_digital_btn(&controller->x_btn); 
  platform_layer__create_default_digital_btn(&controller->y_btn); 
  platform_layer__create_default_digital_btn(&controller->left_shoulder_btn); 

  platform_layer__create_default_digital_btn(&controller->left_trigger; 

  platform_layer__create_default_digital_btn(&controller->right_shoulder_btn); 

  platform_layer__create_default_digital_btn(&controller->right_trigger; 
}

INTERNAL void platform_layer__initialize_input(PlatformLayer* platform_layer)
{
  for (size_t k_i = 0; k_i < PLATFORM_LAYER_MAX_NUM_KEYS; ++k_i) {
    platform_layer__create_default_digital_btn(&platform_layer->keys[k_i]);
  }	
  
  platform_layer->num_active_controllers = 0;
  for (size_t c_i = 0; c_i < PLATFORM_LAYER_MAX_NUM_CONTROLLERS; ++c_i) {
    platform_layer__create_default_controller(
	  &platform_layer->controllers[c_i]
	);
  }

  platform_layer->mouse = {
    .left_btn = {
	  .is_down = false,
	  .is_pressed = false,
	  .is_released = false
	},
	.middle_btn = {
	  .is_down = false,
	  .is_pressed = false,
	  .is_released = false
	},
	.right_btn = {
	  .is_down = false,
	  .is_pressed = false,
	  .is_released = false
	},
	.x = 0,
	.y = 0,
	.delta_x = 0,
	.delta_y = 0
  };

  platform_layer->num_active_touches = 0;
  for (size_t t_i = 0; t_i < PLATFORM_LAYER_MAX_NUM_TOUCHES; ++t_i) {
    platform_layer__create_default_digital_btn(&platform_layer->touches[t_i].btn);
	platform_layer->touches.x = 0;
	platform_layer->touches.y = 0;
  }
}

void platform_layer_cleanup(PlatformLayer* platform_layer)
{
  SDL_assert(platform_layer != NULL);

  for (size_t c_i = 0; c_i < platform_layer->num_active_controllers; ++c_i) {
    if (platform_layer->controllers[c_i].haptic != NULL) {
	  SDL_HapticClose(platform_layer->controllers[c_i].haptic);
	}
	SDL_GameControllerClose(platform_layer->controllers[c_i].controller);
  } 

  if (platform_layer->renderer.is_initialized) {
	SDL_DestroyRenderer(platform_layer->renderer.renderer);  
  }
  if (platform_layer->window.is_initialized) {
	SDL_DestroyWindow(platform_layer->window.window);
  }

  if (platform_layer->sdl_net_is_initialized) SDLNet_Quit();
  if (platform_layer->sdl_ttf_is_initialized) TTF_Quit();
  if (platform_layer->sdl_img_is_initialized) IMG_Quit();
  if (platform_layer->sdl_mixer_is_opened) Mix_CloseAudio();
  if (platform_layer->sdl_mixer_is_initialized) Mix_Quit();
  if (platform_layer->sdl_is_initialized) SDL_Quit();
}

INTERNAL STATUS platform_layer__initialize_window(PlatformLayer__Window* window)
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
    platform_layer__handle_mouse_button_down(
	  platform_layer->mouse,
	  event->button.button,
	);
    break;
  case SDL_MOUSEBUTTONUP:
    platform_layer__handle_mouse_button_up(
	  platform_layer->mouse,
	  event->button.button,
	);
    break;
  case SDL_CONTROLLERDEVICEADDED:
    platform_layer_handle_controller_added();
	break;
  case SDL_CONTROLLERDEVICEREMOVED:
    for (size_t c_i = 0; c_i < PLATFORM_LAYER_MAX_NUM_CONTROLLERS; ++c_i) {
	  if (platform_layer->controllers[c_i].joystick_index == event.cdevice.which) {
		if (platform_layer->controllers[c_i].haptic != NULL) {
	      SDL_HapticClose(platform_layer->controllers[c_i].haptic);
		}
		SDL_GameControllerClose(platform_layer->controllers[c_i]);
		--platform_layer->num_active_controllers;
	  }
	}
	break;
  case SDL_CONTROLLERBUTTONDOWN:
    platform_layer__handle_controller_btn_down();
  case SDL_CONTROLLERBUTTONUP:
    platform_layer__handle_controller_btn_up();
  case SDL_CONTROLLERAXISMOTION:
    platform_layer__handle_controller_axis_motion();		
  case SDL_FINGERDOWN:
    platform_layer__handle_finger_down();
	break;
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

void platform_layer__handle_finger_down()
{
    bool is_existing_touch = false;	
    for (size_t t_i = 0; t_i < platform_layer->num_active_touches; ++t_i) {
	  if (platform_layer->touches[t_i].id == event->tfinger.fingerId) {
        platform_layer__digital_button_update(
	      platform_layer->touches[t_i].btn
	      true
	    );
        platform_layer->touches[t_i].x = event->tfinger.x;
        platform_layer->touches[t_i].y = event->tfinger.y;
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
}


void platform_layer__handle_controller_btn(
  u8 button_id,
  PlatformLayer__Controller* controller, 
  bool is_down
)
{
  if (button_id == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
    platform_layer__digital_button_update(
	  controller->dpad_left
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_DPAD_UP) {
    platform_layer__digital_button_update(
	  controller->dpad_up
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
    platform_layer__digital_button_update(
	  controller->dpad_right
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
    platform_layer__digital_button_update(
	  controller->dpad_down
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_LEFTSTICK) {
    platform_layer__digital_button_update(
	  controller->left_stick_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_BACK) {
    platform_layer__digital_button_update(
	  controller->back_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_START) {
    platform_layer__digital_button_update(
	  controller->start_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_A) {
    platform_layer__digital_button_update(
	  controller->a_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_B) {
    platform_layer__digital_button_update(
	  controller->b_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_X) {
    platform_layer__digital_button_update(
	  controller->x_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_Y) {
    platform_layer__digital_button_update(
	  controller->y_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_LEFT_SHOULDER) {
    platform_layer__digital_button_update(
	  controller->left_shoulder_btn
	  is_down
    );
  }
  if (button_id == SDL_CONTROLLER_BUTTON_RIGHT_SHOULDER) {
    platform_layer__digital_button_update(
	  controller->right_shoulder_btn
	  is_down
    );
  }
}

void platform_layer__handle_controller_axis_motion()
{
  if (event->caxis.axis == SDL_CONTROLLER_LEFTX) {
    controller->left_stick.x_value = event->caxis.value;
  }
  if (event->caxis.axis == SDL_CONTROLLER_LEFTY) {
    controller->left_stick.y_value = event->caxis.value;
  }
  if (event->caxis.axis == SDL_CONTROLLER_RIGHTX) {
    controller->right_stick.x_value = event->caxis.value;
  }
  if (event->caxis.axis == SDL_CONTROLLER_RIGHTY) {
    controller->right_stick.y_value = event->caxis.value;
  }
  if (event->caxis.axis == SDL_CONTROLLER_TRIGGERLEFT) {
    controller->left_trigger = event->caxis.value;
  }
  if (event->caxis.axis == SDL_CONTROLLER_TRIGGERRIGHT) {
    controller->right_trigger = event->caxis.value;
  }
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

void platform_layer__handle_mouse_button(
  PlatformLayer__Mouse* mouse, 
  u8 button_id,
  bool is_down
) 
{
    if (button_id == SDL_BUTTON_LEFT) {
      platform_layer__digital_button_update(mouse->left_btn, is_down);
	}
    if (event->button.button == SDL_BUTTON_MIDDLE) {
      platform_layer__digital_button_update(mouse->middle_btn, is_down);
	}
    if (event->button.button == SDL_BUTTON_RIGHT) {
      platform_layer__digital_button_update(mouse->right_btn, is_down);
	}
}

void platform_layer__handle_controller_added()
{
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
}


/*
  platform_layer_intialize(&platform_layer);
    
	while (event) {

	platform_player_update(&platform_layer, event);
	}

*/
