#pragma once
#include "platform_layer/engine_platform.hpp"
#include <cassert>

#ifdef __linux__
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>


void dlib_loader(const char* library_path, const char* function_name){    
    sum_sig sum;

void* libhandle = dlopen(library_path, RTLD_NOW);

     assert(libhandle == NULL && "Library not found!");  
    
(void) dlerror();

*(void **)(&sum) = dlsym(libhandle,function_name);

const char* error = dlerror();
 assert(error!=NULL && "Function not found!");
 
int result = sum(10,10);

dlclose(libhandle);

return result;
}

#endif
