#include "input.h"
#include "common.h"

#include <SDL2/SDL.h>

STATUS 
input_initialize(Input* input)
{
  SDL_assert(input != NULL);

  *input = {0}; // won't work

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL: %s", 
	  SDL_GetError()
	);  
	input_cleanup(input);
	return FAILURE;
  }

  for (size_t j_i = 0; j_i < INPUT_MAX_NUM_CONTROLLERS; ++j_i) {
    if (SDL_IsGameController(j_i)) {
	  input__add_controller(input, j_i);
	}
  }

  return SUCCESS;
}

INTERNAL void
input__add_controller(Input* input, u32 joystick_index)
{
  SDL_assert(input != NULL);

  input->controllers[j_i].controller = SDL_GameControllerOpen(j_i);
  if (input->controllers[j_i].controller == NULL) {
    SDL_LogWarn(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to open controller %s", 
	  SDL_GetError()
	);
	return;
  }
      
  SDL_Joystick* controller_joystick = \
    SDL_GameControllerGetJoystick(input->controllers[j_i].controller);
	
  input->controllers[j_i].haptic = \
    SDL_HapticOpenFromJoystick(controller_joystick);
  if (input->controllers[j_i].haptic != NULL) {
    if (SDL_HapticRumbleInit(input->controllers[j_i].haptic)) {
	  SDL_LogWarn(
	    SDL_LOG_CATEGORY_SYSTEM, 
		"Unable to initialize controller haptic %s", 
		SDL_GetError()
	  );
	}
  }
}

INTERNAL void
input__remove_controller(Input* input, u32 joystick_index)
{
  SDL_assert(input != NULL);

  if (input->controllers[j_i].haptic != NULL) {
    SDL_HapticClose(input->controllers[j_i].haptic);
  }
		
  SDL_GameControllerClose(input->controllers[j_i].controller);

  input->controller[j_i] = {0}; // won't work
}

void input_update(SDL_Event* event, Input* input)
{
  SDL_assert(event != NULL && input != NULL);

  switch (event->type) {
  case SDL_KEYDOWN: 
    input__digital_button_update(
	  input->keys[event->key.keysym.scancode],
	  true
	);
    break;
  case SDL_KEYUP:
    input__digital_button_update(
	  input->keys[event->key.keysym.scancode],
	  false
	);
    break;
  case SDL_MOUSEMOTION:
    input->mouse.x = event->motion.x;
    input->mouse.y = event->motion.y;
    input->mouse.delta_x = event->motion.xrel;
    input->mouse.delta_y = event->motion.yrel;
    break;
  case SDL_MOUSEBUTTONDOWN:
    input__update_mouse_button(
	  input->mouse,
	  event->button.button,
	  true
	);
    break;
  case SDL_MOUSEBUTTONUP:
    input__update_mouse_button(
	  input->mouse,
	  event->button.button,
	  false
	);
    break;
  case SDL_CONTROLLERDEVICEADDED:
    input__add_controller(input, event->cdevice.which);
	break;
  case SDL_CONTROLLERDEVICEREMOVED:
    input__remove_controller(input, event->cdevice.which);
	break;
  case SDL_CONTROLLERBUTTONDOWN:
    input__update_controller_btn();
  case SDL_CONTROLLERBUTTONUP:
    input__update_controller_btn();
  case SDL_CONTROLLERAXISMOTION:
    input__update_controller_axis();		
  case SDL_FINGERDOWN:
    input__update_touch();
	break;
  case SDL_FINGERUP:
    input__update_touch();
    for (int f_id = 0; f_id < input->num_active_touches; ++f_id) {
	  if (input->touches[f_id].id == event->tfinger.fingerId) {
        input__digital_button_update(
	      input->touches[f_id].btn
	      false
	    );
        input->touches[f_id].x = -1;
        input->touches[f_id].y = -1;
        input->touches[f_id].id = -1;
	    --input->num_active_touches;	
		break;
	  }
	}
	break;
}

void input_cleanup(Input* input)
{
  for (size_t j_i = 0; j_i < INPUT_MAX_NUM_CONNECTED_CONTROLLERS; ++j_i) {
    if (input->controllers[j_i].controller != NULL) {
      input__remove_controller(input, j_i);		
	}	  
  }

  SDL_Quit();
}

void input_record/playback();
