#pragma once

#include "core/types.h"

struct GenArena {
    u8 *data;
    u32 *generations, *free_list, stride, capacity, len, free_len, next_id;
};

struct GenHandle {
    u32 id, gen;
};

GenArena gen_arena_create(u32 stride, u32 capacity);

GenArena gen_arena_create(u32 stride);

void gen_arena_destroy(GenArena *self);

bool gen_arena_contains(const GenArena *self, GenHandle handle);

GenHandle gen_arena_insert(GenArena *self, u8 *data);

bool gen_arena_remove(GenArena *self, GenHandle handle);

u8 *gen_arena_get(GenArena *self, GenHandle handle);

void gen_arena_clear(GenArena *self);
