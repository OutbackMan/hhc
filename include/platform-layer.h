#include <SDL2/SDL.h>

#include <stdbool.h>

typedef struct PlatformLayer {
  bool are_on_touch_platform;

  PlatformLayer__Window* window;
  PlatformLayer__Renderer* renderer;

  PlatformLayer__DigitalButton keys[256]; 
  PlatformLayer__Controllers controllers[4];
  PlatformLayer__Mouse mouse;

  PlatformLayer__Time time; 

  PlatformLayer__Touches; // refer to unity api

  char text[PLATFORM_LAYER_MAX_TEXT];
  size_t text_length;
} 

typedef struct PlatformLayer__DigitalButton {
};

#define MAX_NUM_CONTROLLERS 4
typedef struct Input {
  const u8* cur_keyboard_state;
  const u8* prev_keyboard_state;
  int cur_mouse_x;
  int cur_mouse_y;
  int prev_mouse_x;
  int prev_mouse_y;
  u32 cur_mouse_state;
  u32 prev_mouse_state;
  _InputControllers controllers[4];
  int num_controllers_connected;
};
extern struct Input global_input;

extern bool global_want_to_run;

#define PLATFORM_LAYER_MAX_CONTROLLERS 4
#define PLATFORM_LAYER_MAX_KEYS 256

// read_entire_file, write_to_entire_file
// state
