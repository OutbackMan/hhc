#include "copy-if-file-changed.h"
#include "hhc.h"

#include <Windows.h>

bool copy_if_file_changed(const char* src_file, const char* dst_file)
{
  const size_t CUR_DIR_BUF_SIZE = 50;
  char cur_dir_buf[CUR_DIR_BUF_SIZE];

  uint cur_dir_length = GetCurrentDirectory(CUR_DIR_BUF_SIZE, cur_dir_buf);
  if (cur_dir_length == 0) {
wchar_t buf[256];
FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
               NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
               buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
    FormatMessage();	  
	SDL_LogWarn(
      SDL_LOG_CATEGORY_APPLICATION,
	  "Unable to get current directory" 
	);
	return false;
  }

  const size_t LIB_DIR_BUF_SIZE = CUR_DIR_BUF_SIZE + 6;
  char lib_dir_buf[LIB_DIR_BUF_SIZE];
  strcpy(lib_dir_buf, cur_dir_buf, cur_dir_length);
  strcat(lib_dir_buf, "\\build");

  HANDLE file_change_object = FindFirstChangeNotificationA(
                                cur_dir_buf[cur_dir_length],
                                false,
                                FILE_NOTIFY_CHANGE_LAST_WRITE
                              );



}

/*
if (copy_if_lib_changed()) {
  load_library()	
}
*/
