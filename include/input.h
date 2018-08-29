#include "common.h"

#include <SDL2/SDL.h>

#include <stdbool.h>

#define INPUT_MAX_NUM_KEYS 285
#define INPUT_MAX_NUM_CONTROLLERS 4
#define INPUT_MAX_NUM_TOUCHES 5

typedef struct {
  Input__DigitalBtn keys[INPUT_MAX_NUM_KEYS]; 
  Input__Controller controllers[INPUT_MAX_NUM_CONTROLLERS];
  size_t num_active_controllers;
  Input__Mouse mouse;
  Input__Touch touches[INPUT_MAX_NUM_TOUCHES];
  size_t num_active_touches;
} Input; // correct

#define EXPAND_AS_STRUCT(type, name) type name;
#define ZERO_STRUCT(struc, value) struc->value = {0};

#define INPUT_DIGITAL_BTN_FIELDS(X_ENTRY) \
  X_ENTRY(bool, is_down) \
  X_ENTRY(bool, is_pressed) \
  X_ENTRY(bool, is_released)
typedef struct InputDigitalBtn {
  INPUT_DIGITAL_BTN_FIELDS(EXPAND_AS_STRUCT)
};
extern const struct INPUT_DIGITAL_BTN_DEFAULT;

typedef struct Input__Touch {
  Input__DigitalBtn btn;
  int x;
  int y;
}

typedef struct Input__AnalogBtn {
  InputDigitalBtn btn;
  int x_value;
  int y_value;
};

#define INPUT_CONTROLLER_FIELDS(X_ENTRY) \
  X_ENTRY(int32, joystick_index) \
  X_ENTRY(SDL_Controller*, controller) \
  X_ENTRY(SDL_Haptic*, haptic) \
  X_ENTRY(InputDigitalBtn, dpad_left) \ 
  X_ENTRY(InputDigitalBtn, dpad_up) \ 
  X_ENTRY(InputDigitalBtn, dpad_right) \ 
  X_ENTRY(InputDigitalBtn, dpad_down) \ 
  X_ENTRY(InputAnalogBtn, left_stick) \
  X_ENTRY(InputDigitalBtn, back_btn) \ 
  X_ENTRY(InputDigitalBtn, start_btn) \ 
  X_ENTRY(InputAnalogBtn, right_stick) \
  X_ENTRY(InputDigitalBtn, a_btn) \ 
  X_ENTRY(InputDigitalBtn, b_btn) \ 
  X_ENTRY(InputDigitalBtn, x_btn) \ 
  X_ENTRY(InputDigitalBtn, y_btn) \ 
  X_ENTRY(InputDigitalBtn, left_shoulder_btn) \ 
  X_ENTRY(InputAnalogBtn, left_trigger) \ 
  X_ENTRY(InputDigitalBtn, right_shoulder_btn) \ 
  X_ENTRY(InputAnalogBtn, right_trigger)
typedef struct Input__Controller {
  INPUT_CONTROLLER_FIELDS(EXPAND_AS_STRUCT)
};
extern const struct INPUT_CONTROLLER_DEFAULT;

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
