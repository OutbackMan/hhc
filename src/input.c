#include "input.h"
#include "common.h"

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdlib.h>

INTERNAL void
input_set_sedentary_digital_btn(InputDigitalBtn* btn)
{
  SDL_assert(btn != NULL);

  btn->is_down = false;	
  btn->is_pressed = false;	
  btn->is_released = false;	
}

INTERNAL void
input_digital_btn_update(InputDigitalBtn* btn, bool is_down)
{
  SDL_assert(btn != NULL);

  bool was_down = btn->is_down;	
  btn->is_pressed = !was_down && is_down;
  btn->is_down = is_down;
  btn->is_released = !is_down && was_down;
}

INTERNAL void
input_set_sedentary_analog_btn(InputAnalogBtn* btn)
{
  SDL_assert(btn != NULL);

  input_set_sedentary_digital_btn(&btn->btn);
  btn->x_value = 0;
  btn->y_value = 0;
}

INTERNAL void
input_set_sedentary_controller(InputController* controller)
{
  SDL_assert(controller != NULL);

  controller->joystick_index = -1;	
  controller->controller = NULL;
  controller->haptic = NULL;
  input_set_sedentary_digital_btn(&controller->dpad_left);
  input_set_sedentary_digital_btn(&controller->dpad_up);
  input_set_sedentary_digital_btn(&controller->dpad_right);
  input_set_sedentary_digital_btn(&controller->dpad_down);
  input_set_sedentary_analog_btn(&controller->left_stick);
  input_set_sedentary_digital_btn(&controller->back_btn);
  input_set_sedentary_digital_btn(&controller->start_btn);
  input_set_sedentary_analog_btn(&controller->right_stick);
  input_set_sedentary_digital_btn(&controller->a_btn); 
  input_set_sedentary_digital_btn(&controller->b_btn); 
  input_set_sedentary_digital_btn(&controller->x_btn); 
  input_set_sedentary_digital_btn(&controller->y_btn); 
  input_set_sedentary_digital_btn(&controller->left_shoulder_btn);
  input_set_sedentary_analog_btn(&controller->left_trigger);
  input_set_sedentary_digital_btn(&controller->right_shoulder_btn;
  input_set_sedentary_analog_btn(&controller->right_trigger);
}

INTERNAL void
input_controller_add(Input* input, u32 joystick_index)
{
  SDL_assert(input != NULL && joystick_index >= 0);

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
input_controller_remove(Input* input, u32 joystick_index)
{
  SDL_assert(input != NULL);

  if (input->controllers[j_i].haptic != NULL) {
    SDL_HapticClose(input->controllers[j_i].haptic);
  }
		
  SDL_GameControllerClose(input->controllers[j_i].controller);

  input_set_sedentary_controller(&input->controllers[j_i]);
}

INTERNAL void
input_controller_digital_btn_update(
  Input* input, 
  u32 joystick_index, 
  u8 btn_id, 
  bool is_down
)
{
  SDL_assert(input != NULL);

  InputController controller = input->controllers[joystick_index];

  switch (btn_id) {
  case SDL_CONTROLLER_BUTTON_DPAD_LEFT:	  
    input_digital_btn_update(&controller.dpad_left, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_DPAD_UP:
    input_digital_btn_update(&controller.dpad_up, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
    input_digital_btn_update(&controller.dpad_right, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
    input_digital_btn_update(&controller.dpad_down, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_LEFTSTICK:
    input_digital_btn_update(&controller.left_stick.btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_BACK:
    input_digital_btn_update(&controller.back_btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_START:
    input_digital_btn_update(&controller.start_btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
    input_digital_btn_update(&controller->right_stick.btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_A:
    input_digital_btn_update(&controller.a_btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_B:
    input_digital_btn_update(&controller.b_btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_X:
    input_digital_btn_update(&controller->x_btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_Y:
    input_digital_btn_update(&controller->y_btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_LEFT_SHOULDER:
    input_digital_btn_update(&controller->left_shoulder_btn, is_down);
	break;
  case SDL_CONTROLLER_BUTTON_RIGHT_SHOULDER:
    input_digital_btn_update(&controller->right_shoulder_btn, is_down);
	break;
  NO_DEFAULT_CASE
  }
}

INTERNAL void
input_controller_analog_btn_update(
  Input* input, 
  u32 joystick_index,
  u8 axis_id,
  int16 axis_value
)
{
  SDL_assert(input != NULL);

  InputController controller = input->controllers[joystick_index];

  switch (axis_id) {
  case SDL_CONTROLLER_LEFTX:
    controller.left_stick.x_value = axis_value;
    break;
  case SDL_CONTROLLER_LEFTY:
    controller.left_stick.y_value = axis_value;
    break;
  case SDL_CONTROLLER_RIGHTX:
    controller.right_stick.x_value = axis_value;
    break;
  case SDL_CONTROLLER_RIGHTY:
    controller.right_stick.y_value = axis_value;
    break;
  case SDL_CONTROLLER_TRIGGERLEFT:
    controller.left_trigger = axis_value;
    break;
  case SDL_CONTROLLER_TRIGGERRIGHT:
    controller.right_trigger = axis_value;
    break;
  NO_DEFAULT_CASE
  }
}

INTERNAL void
input_mouse_digital_btn_update(InputMouse* mouse, u8 btn_id, bool is_down)
{
  SDL_assert(mouse != NULL);

  switch (btn_id) {
  case SDL_BUTTON_LEFT:
    input_digital_btn_update(&mouse->left_btn, is_down);
	break;
  case SDL_BUTTON_MIDDLE:
    input_digital_btn_update(&mouse->middle_btn, is_down);
	break;
  case SDL_BUTTON_RIGHT:
    input_digital_btn_update(&mouse->right_btn, is_down);
	break;
  NO_DEFAULT_CASE
  }
}

INTERNAL void
input_touch_update(
  Input* input, 
  int64 finger_id, 
  float x, 
  float y, 
  bool is_down
)
{
  SDL_assert(input != NULL);

  bool is_existing_touch = false;
  for (size_t t_i = 0; t_i < INPUT_MAX_NUM_TOUCHES; ++t_i) {
    if (input->touches[t_i].finger_id == finger_id) {
      is_existing_touch = true;

	  input_digital_btn_update(&input->touches[t_i].btn, is_down);
	  if (is_down) {
	    input->touches[t_i].x = x;
	    input->touches[t_i].y = y;
	  } else {
		input->touches[t_i].finger_id = 0;
	    input->touches[t_i].x = 0.0f;
	    input->touches[t_i].y = 0.0f;
		--input->num_touches;
	  }
	} 	  
  }

  if (!is_existing_touch && input->num_touches < INPUT_MAX_NUM_TOUCHES) {
    input_digital_btn_update(&input->touches[input->num_touches].btn, is_down);
	input->touches[input->num_touches].x = x;
    input->touches[input->num_touches].y = y;
    input->touches[input->num_touches++].finger_id = finger_id;
  }
}

STATUS 
input_initialize(Input* input)
{
  SDL_assert(input != NULL);

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogCritical(
	  SDL_LOG_CATEGORY_SYSTEM, 
	  "Unable to initialize SDL: %s", 
	  SDL_GetError()
	);  
	return FAILURE;
  }

  for (size_t k_i = 0; k_i < INPUT_MAX_NUM_KEYS; ++k_i) {
    input_set_sedentary_digital_btn(&input->keys[k_i]);
  }

  for (size_t j_i = 0; j_i < INPUT_MAX_NUM_CONTROLLERS; ++j_i) {
    if (SDL_IsGameController(j_i)) {
	  input_add_controller(input, j_i);
	} else {
      input_set_sedentary_controller(&input->controller[j_i]);		
	}
  }
  
  input_set_sedentary_digital_btn(&input->mouse.left_btn);
  input_set_sedentary_digital_btn(&input->mouse.middle_btn);
  input_set_sedentary_digital_btn(&input->mouse.right_btn);
  input->mouse.x = 0;
  input->mouse.y = 0;
  input->mouse.delta_x = 0;
  input->mouse.delta_y = 0;

  for (size_t t_i = 0; t_i < INPUT_MAX_NUM_TOUCHES; ++t_i) {
    input_set_sedentary_digital_btn(&input->touches[t_i].btn);
	input->touches[t_i].finger_id = 0;
	input->touches[t_i].x = 0.0f;
	input->touches[t_i].y = 0.0f;
  }  

  return SUCCESS;
}


void input_update(Input* input, SDL_Event* event)
{
  SDL_assert(input != NULL && event != NULL);

  switch (event->type) {
  case SDL_KEYDOWN: 
    input_digital_btn_update(input->keys[event->key.keysym.scancode], true);
    break;
  case SDL_KEYUP:
    input_digital_btn_update(input->keys[event->key.keysym.scancode], false);
    break;
  case SDL_CONTROLLERDEVICEADDED:
    input_controller_add(input, event->cdevice.which);
	break;
  case SDL_CONTROLLERDEVICEREMOVED:
    input_controller_remove(input, event->cdevice.which);
	break;
  case SDL_CONTROLLERBUTTONDOWN:
    input_controller_digital_btn_update(
	  input, 
	  event->cbutton.which, 
	  event->cbutton.button, 
	  true
	);
	break;
  case SDL_CONTROLLERBUTTONUP:
    input_controller_digital_btn_update(
	  input, 
	  event->cbutton.which, 
	  event->cbutton.button, 
	  false
	);
	break;
  case SDL_CONTROLLERAXISMOTION:
    input_controller_analog_btn_update(
      input,
	  event->caxis.which,
	  event->caxis.axis,
	  event->caxis.value
	);
  case SDL_MOUSEMOTION:
    input->mouse.x = event->motion.x;
    input->mouse.y = event->motion.y;
    input->mouse.delta_x = event->motion.xrel;
    input->mouse.delta_y = event->motion.yrel;
    break;
  case SDL_MOUSEBUTTONDOWN:
    input_mouse_digital_btn_update(input->mouse, event->button.button, true);
    break;
  case SDL_MOUSEBUTTONUP:
    input_mouse_digital_btn_update(input->mouse, event->button.button, false);
    break;
  case SDL_FINGERDOWN:
    input_update_touch(
      input,
	  event->tfinger.fingerId,
	  event->tfinger.x,
	  event->tfinger.y,
	  true
	);
	break;
  case SDL_FINGERUP:
    input_update_touch(
      input,
	  event->tfinger.fingerId,
	  event->tfinger.x,
	  event->tfinger.y,
	  false
	);
	break;
  NO_DEFAULT_CASE
  }
}

void input_cleanup(Input* input)
{
  SDL_assert(input != NULL);

  for (size_t j_i = 0; j_i < INPUT_MAX_NUM_CONTROLLERS; ++j_i) {
    if (input->controllers[j_i].controller != NULL) {
      input_remove_controller(input, j_i);		
	}	  
  }

  SDL_Quit();
}
