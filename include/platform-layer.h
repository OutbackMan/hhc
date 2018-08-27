typedef struct PlatformLayer {
  bool is_initialized;
  bool want_to_quit;

  // error
  SDL_Window* window;
  SDL_Renderer* renderer;

  PlatformLayer__DigitalButton keys[256]; 
  PlatformLayer__Controllers controllers[4];
  PlatformLayer__Mouse mouse;

  PlatformLayer__Time time; 
} 

typedef struct PlatformLayer__DigitalButton {
  bool is_connected;
  int joystick_index;	
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
