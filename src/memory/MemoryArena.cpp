#include "MemoryArena.h"

#if defined(_WIN32)
#include <Windows.h>
#endif

#define NOOP(x) (void)x

u32 plat_get_pagesize(void) {
#if defined(_WIN32)
	// Código de Windows
	// sdmfskdfjhsdjfh
	// skjdfhskdfjhj
	return 0xFF;
#else
	printf("Platform Not Yet Implemented!\n");
	return 0;
#endif
}

void* plat_mem_reserve(u64 size) {
#if defined(_WIN32)
	// Código de Windows
	// sdmfskdfjhsdjfh
	// skjdfhskdfjhj
	size += 2;
	return nullptr;
#else
	printf("Platform Not Yet Implemented!\n");
	NOOP(size); // NO-OP
	return nullptr;
#endif
	
}

b32 plat_mem_commit(void* ptr, u64 size) {
	
}

b32 plat_mem_decommit(void* ptr, u64 size) {
	
}

b32 plat_mem_release(void* ptr, u64 size) {
	
}
