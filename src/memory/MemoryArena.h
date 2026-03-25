#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>


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


struct MemoryArena {
    u64 reserve_size;
    u64 commit_size;

    u64 pos;
    u64 commit_pos;
};

MemoryArena* arena_create(u64 reserve_size, u64 commit_size);
void arena_destroy(MemoryArena* arena);
void* arena_push(MemoryArena* arena, u64 size, b32 non_zero);
void arena_pop(MemoryArena* arena, u64 size);
void arena_pop_to(MemoryArena* arena, u64 pos);
void arena_clear(MemoryArena* arena);


u32 plat_get_pagesize(void);
void* plat_mem_reserve(u64 size);
b32 plat_mem_commit(void* ptr, u64 size);
b32 plat_mem_decommit(void* ptr, u64 size);
b32 plat_mem_release(void* ptr, u64 size);


