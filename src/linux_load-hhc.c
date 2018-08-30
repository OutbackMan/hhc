#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>

bool has_copied()
{
// various FS.calls()
  bool has_copied = false;
  EM_ASM()
  return has_copied;
}


#endif
