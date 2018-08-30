#include "common.h"

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <stdlib.h>

const static uint INPUT_MAX_NUM_KEYS = 285;
const static uint INPUT_MAX_NUM_CONTROLLERS = 4;
const static uint INPUT_MAX_NUM_TOUCHES = 5;

typedef struct {
  InputDigitalBtn keys[INPUT_MAX_NUM_KEYS]; 
  InputController controllers[INPUT_MAX_NUM_CONTROLLERS];
  InputMouse mouse;
  InputTouch touches[INPUT_MAX_NUM_TOUCHES];
  size_t num_touches;
} Input;

typedef struct {
  bool is_down;
  bool is_pressed;
  bool is_released;
} InputDigitalBtn;

typedef struct {
  InputDigitalBtn btn;
  int x_value;
  int y_value;
} InputAnalogBtn;

typedef struct {
  int32 joystick_index;
  SDL_Controller* controller;
  SDL_Haptic* haptic;
  InputDigitalBtn dpad_left;
  InputDigitalBtn dpad_up; 
  InputDigitalBtn dpad_right;
  InputDigitalBtn dpad_down; 
  InputAnalogBtn left_stick;
  InputDigitalBtn back_btn; 
  InputDigitalBtn start_btn; 
  InputAnalogBtn right_stick;
  InputDigitalBtn a_btn; 
  InputDigitalBtn b_btn; 
  InputDigitalBtn x_btn; 
  InputDigitalBtn y_btn; 
  InputDigitalBtn left_shoulder_btn;
  InputAnalogBtn left_trigger; 
  InputDigitalBtn right_shoulder_btn;
  InputAnalogBtn right_trigger;
} InputController;

typedef struct {
  InputDigitalBtn left_btn; 
  InputDigitalBtn middle_btn; 
  InputDigitalBtn right_btn; 
  int x;
  int y;
  int delta_x;
  int delta_y;
} InputMouse;

typedef struct {
  InputDigitalBtn btn;
  int64 finger_id;
  float x;
  float y;
} InputTouch;

void input_initialize(Input* input); 

void input_update(Input* input, SDL_Event* event);
