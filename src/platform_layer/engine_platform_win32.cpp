#include <cassert>
#include <libloaderapi.h>
#include <minwindef.h>
#ifdef _WIN32
#include <windows.h>

void* _internal_dlib_open(const char *library_path) {
  // returns a handle if succedeed
  HANDLE lib_handle = LoadLibrary(library_path);
  assert(lib_handle != nullptr && "Libray not found!");

  return reinterpret_cast<void*>(lib_handle);
}

void* _internal_dlib_load_symbol(void* libHandle, const char* symbolName) {
  HINSTANCE handleInstance = static_cast<HINSTANCE>(libHandle);
  FARPROC processAddress = GetProcAddress(handleInstance, symbolName);
  return reinterpret_cast<void *>(processAddress);
}

void _internal_dlib_close(void* handle) {
  FreeLibrary(static_cast<HMODULE>(handle));
}

#else

#error "Windows API cannot be included in compilation unit for another platform"

#endif
