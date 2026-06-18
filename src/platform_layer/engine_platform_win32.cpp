#pragma once
#include "platform_layer/engine_platform.hpp"
#include <cassert>
#ifdef WIN32
#include <windows.h>

HMODULE LoadLibrary( 
  LPCSTR lib_name;//16bit const char
);

//return the address of the exported function
FARPROC GetProcAddress(
  HMODULE module,
  LPCSTR  func_name
);

void dlib_loader(const char* library_path, const char* function_name){
  //returns a handle if succedeed  
func_sig;
 HANDLE lib_handle =  LoadLibrary(TEXT(library_path));
 assert(lib_handle != NULL && "Libray not found!");

  pfn = (func_sig)GetProcAddress(lib_handle, TEXT(function_name));
  pfn();

  freeLibrary(lib_handle);
}
#endif
