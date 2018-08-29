#include <SDL2/SDL.h>

#include <stdbool.h>


typedef struct Output {
  bool sdl_is_initialized;
  bool sdl_mixer_is_initialized;
  bool sdl_mixer_is_opened;
  bool sdl_img_is_initialized;
  bool sdl_ttf_is_initialized;
  bool sdl_net_is_initialized;

  bool are_on_mobile_device;

  Output__Window window;
  Output__Renderer renderer;
} 

typedef struct Output__Window {
  const char* title;
  int x;
  int y;
  int width;
  int height;
  bool is_shown;
  bool is_minimized;
  bool has_focus;
  bool is_closed;
  u32 flags;
};

typedef struct Output__Renderer {
  SDL_Window* window;
  int driver_index;
  u32 flags;
};


// read_entire_file, write_to_entire_file
// state
