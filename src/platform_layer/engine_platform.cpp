#include "engine_platform.hpp"

#ifdef _WIN32
#include "engine_platform_win32.cpp"
#else
#include "engine_platform_linux.cpp"
#endif


void* os::dlib_open(const char *library_path) {
	return _internal_dlib_open(library_path);
}

void os::dlib_close(void* handle) {
	return _internal_dlib_close(handle);
}

void* os::dlib_load_symbol(void* libHandle, const char* symbolName) {
	return _internal_dlib_load_symbol(libHandle, symbolName);
}

