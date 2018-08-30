#if !defined(OUTPUT_H)
#define OUTPUT_H

#include "common.h"

#include <SDL2/SDL.h>

#include <stdbool.h>


typedef struct {
  bool mixer_is_initialized;
  bool mixer_is_opened;
  bool img_is_initialized;
  bool ttf_is_initialized;
  bool net_is_initialized;

  bool are_on_mobile_device;

  OutputWindow window;
  OutputRenderer renderer;
} Output;

typedef struct {
  bool is_initialized;
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
} OutputWindow;

typedef struct {
  bool is_initialized;
  SDL_Window* window;
  SDL_Renderer* renderer;
  int driver_index;
  u32 flags;
} OutputRenderer;

STATUS 
output_initialize(Output* output);

void
output_update(Output* output, SDL_Event* event);

void
output_cleanup(Output* output);


#endif
