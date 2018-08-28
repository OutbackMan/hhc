#include <SDL2/SDL.h>

DigitalButton touches[5];
size_t num_active_touches;

manifest.json {
  "display": "fullscreen",
  "orientation": "landscape"
}
<link rel="manifest" href="manifest.json" />

#include <stdbool.h>

#define PLATFORM_LAYER_ERROR_BUFFER_SIZE 256 
#define PLATFORM_LAYER_MAX_NUM_KEYS 256
#define PLATFORM_LAYER_MAX_NUM_CONTROLLERS 4
#define PLATFORM_LAYER_MAX_NUM_TOUCHES 5
#define PLATFORM_LAYER_INPUT_TEXT_BUFFER_SIZE 1024

typedef struct PlatformLayer {
  bool sdl_is_initialized;
  bool sdl_mixer_is_initialized;
  bool sdl_img_is_initialized;
  bool sdl_ttf_is_initialized;
  bool sdl_net_is_initialized;

  char error_buffer[PLATFORM_LAYER_ERROR_BUFFER_SIZE];

  bool are_on_mobile_device;

  PlatformLayer__Window window;
  PlatformLayer__Renderer renderer;

  PlatformLayer__DigitalButton keys[256]; 
  PlatformLayer__Controllers controllers[4];
  PlatformLayer__Mouse mouse;
  PlatformLayer__Touches;

  PlatformLayer__Time time; 

  char input_text[PLATFORM_LAYER_INPUT_TEXT_BUFFER_SIZE];
  size_t input_text_length;
} 

typedef struct PlatformLayer__DigitalButton {
};

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

// read_entire_file, write_to_entire_file
// state
