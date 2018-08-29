#include <SDL2/SDL.h>

#include <stdbool.h>

#define INPUT_MAX_NUM_KEYS 285 // refer to SDL_Scancode lookup table
#define INPUT_MAX_NUM_CONTROLLERS 4
#define INPUT_MAX_NUM_TOUCHES 5

typedef struct Input {
  Input__DigitalBtn keys[INPUT_MAX_NUM_KEYS]; 
  Input__Controller controllers[INPUT_MAX_NUM_CONTROLLERS];
  size_t num_active_controllers;
  Input__Mouse mouse;
  Input__Touch touches[INPUT_MAX_NUM_TOUCHES];
  size_t num_active_touches;
};

typedef struct Input__DigitalBtn {
  bool is_down;
  bool is_pressed;
  bool is_released;
};

typedef struct Input__Touch {
  Input__DigitalBtn btn;
  int x;
  int y;
}

typedef struct Input__AnalogBtn {
  Input__DigitalBtn btn;
  int x_value;
  int y_value;
};

typedef struct Input__Controller {
  int32 joystick_index;
  SDL_Controller* controller ;
  SDL_Haptic* haptic;
  Input__DigitalBtn dpad_left; 
  Input__DigitalBtn dpad_up; 
  Input__DigitalBtn dpad_right; 
  Input__DigitalBtn dpad_down; 
  Input__AnalogBtn left_stick;
  Input__DigitalBtn back_btn; 
  Input__DigitalBtn start_btn; 
  Input__AnalogBtn right_stick;
  Input__DigitalBtn a_btn; 
  Input__DigitalBtn b_btn; 
  Input__DigitalBtn x_btn; 
  Input__DigitalBtn y_btn; 
  Input__DigitalBtn left_shoulder_btn; 
  Input__AnalogBtn left_trigger; 
  Input__DigitalBtn right_shoulder_btn; 
  Input__AnalogBtn right_trigger; 
};

typedef struct Input__Mouse {
  Input__DigitalBtn left_btn; 
  Input__DigitalBtn middle_btn; 
  Input__DigitalBtn right_btn; 
  int x;
  int y;
  int delta_x;
  int delta_y;
};

void input_initialize(Input* input); 
void input_update(Input* input);
void input_record/playback();
