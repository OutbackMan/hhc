#include <SDL2/SDL.h>

manifest.json {
  "display": "fullscreen",
  "orientation": "landscape"
}
<link rel="manifest" href="manifest.json" />
/*
  perhaps use configure_file() to get the path of the js binary
*/

#include <stdbool.h>

#define PLATFORM_LAYER_MAX_NUM_KEYS 256
#define PLATFORM_LAYER_MAX_NUM_CONTROLLERS 4
#define PLATFORM_LAYER_MAX_NUM_TOUCHES 5
#define PLATFORM_LAYER_INPUT_TEXT_BUFFER_SIZE 1024
#define PLATFORM_LAYER_THRESHOLD 1000

typedef struct PlatformLayer {
  bool sdl_is_initialized;
  bool sdl_mixer_is_initialized;
  bool sdl_mixer_is_opened;
  bool sdl_img_is_initialized;
  bool sdl_ttf_is_initialized;
  bool sdl_net_is_initialized;

  bool are_on_mobile_device;

  PlatformLayer__Window window;
  PlatformLayer__Renderer renderer;

  PlatformLayer__DigitalButton keys[PLATFORM_LAYER_MAX_NUM_KEYS]; 
  PlatformLayer__Controller controllers[PLATFORM_LAYER_MAX_NUM_CONTROLLERS];
  PlatformLayer__Mouse mouse;
  PlatformLayer__DigitalButton touches[PLATFORM_LAYER_MAX_NUM_TOUCHES];

  PlatformLayer__Time time; 

  char input_text[PLATFORM_LAYER_INPUT_TEXT_BUFFER_SIZE];
  size_t input_text_length;
} 

typedef struct PlatformLayer__Window {
  const char* title;
  int x;
  int y;
  int width;
  int height;
  // add more when event handling
  u32 flags;
};

typedef struct PlatformLayer__Renderer {
  SDL_Window* window;
  int driver_index;
  u32 flags;
};

typedef struct PlatformLayer__DigitalButton {
  bool is_down;
  bool is_pressed;
  bool is_released;
};

typedef struct PlatformLayer__AnalogButton {
  float threshold;
  float value;
  bool is_down;
  bool is_pressed;
  bool is_released;
};

typedef struct PlatformLayer__Stick {
  float threshold;
  float x;
  float y;
};

typedef struct PlatformLayer__Controller {
  PlatformLayer__DigitalButton left_btn; 
  PlatformLayer__DigitalButton up_btn; 
  PlatformLayer__DigitalButton right_btn; 
  PlatformLayer__DigitalButton down_btn; 
  PlatformLayer__Stick left_thumb_stick;
  PlatformLayer__DigitalButton back_btn; 
  PlatformLayer__DigitalButton start_btn; 
  PlatformLayer__Stick right_thumb_stick;
  PlatformLayer__DigitalButton a_btn; 
  PlatformLayer__DigitalButton b_btn; 
  PlatformLayer__DigitalButton x_btn; 
  PlatformLayer__DigitalButton y_btn; 
  PlatformLayer__DigitalButton left_shoulder_btn; 
  PlatformLayer__AnalogButton left_trigger; 
  PlatformLayer__DigitalButton right_shoulder_btn; 
  PlatformLayer__AnalogButton right_trigger; 
};

typedef struct PlatformLayer__Mouse {
  PlatformLayer__DigitalButton left_btn; 
  int wheel;
  int wheel_delta;
  PlatformLayer__DigitalButton right_btn; 
  int x;
  int y;
  int delta_x;
  int delta_y;
};

typedef struct PlatformLayer__Time {
  u64 ticks;
};


// read_entire_file, write_to_entire_file
// state
