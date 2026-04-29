#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <cstdio.h>
#include <string.h>

using  i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using b8  = i8;
using b32 = i32;

#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ALIGN_UP_POW2(n, p) (((u64)(n) + ((u64)(p) - 1))& (~((u64)(p) - 1)))

 
#define ARENA_BASE_POS (sizeof(mem_arena))
#define ARENA_ALIGN (sizeof(void*))


struct mem_arena {
    u64 reserve_size;
    u64 commit_size;

    u64 pos;
    u64 commit_pos;
};

struct mem_arena_temp{
    mem_arena*  arena;
    u64 start_pos;  
};

mem_arena* arena_create(u64 reserve_size, u64 commit_size);
void arena_destroy(mem_arena* arena);
void* arena_push(mem_arena* arena, u64 size, b32 non_zero);
void arena_pop(mem_arena* arena, u64 size);
void arena_pop_to(mem_arena* arena, u64 pos);
void arena_clear(mem_arena* arena);

mem_arena_temp arena_scratch_get(mem_arena* arena);
void arena_scratch_release(mem_arena_temp scratch);

#define PUSH_STRUCT(ARENA, T) (T*)arena_push((arena), sizeof(T), false)
#define PUSH_STRUCT_NZ(ARENA, T) (T*)arena_push((arena), sizeof(T), true)
#define PUSH_ARRAY(arena, T, n (T*)arena_push((arena), size of (T) * (n), false)
#define PUSH_ARRAY(arena, T, n (T*)arena_push((arena), size of (T) * (n), true)

u32 plat_get_pagesize(void);

void* plat_mem_reserve(u64 size);
b32 plat_mem_commit(void* ptr, u64 size);
b32 plat_mem_decommit(void* ptr, u64 size);
b32 plat_mem_release(void* ptr, u64 size);


