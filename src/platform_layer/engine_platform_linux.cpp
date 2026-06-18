#pragma once
#ifdef __linux__

#include <cassert>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>

void* _internal_dlib_open(const char *library_path) {
  void *libhandle = dlopen(library_path, RTLD_NOW);

  assert(libhandle != nullptr && "Library not found!");

  return libHandle;
}

#else

#error "Linux API cannot be included in compilation unit for another platform"

#endif
