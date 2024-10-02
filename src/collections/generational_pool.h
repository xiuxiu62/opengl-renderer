#pragma once

#include "core/types.h"

struct GenPool { // A slot and generation tracking pool.
    u8 *data;
    u32 *generations, *free_list, stride, capacity, len, free_len, next_id;
};

struct GenHandle { // Handle to a value stored in a generational arena
    u32 id, gen;
};

GenPool gen_pool_create(u32 stride, u32 capacity);

GenPool gen_pool_create(u32 stride);

void gen_pool_destroy(GenPool &self);

bool gen_pool_contains(const GenPool &self, GenHandle handle);

GenHandle gen_pool_insert(GenPool &self, u8 *data);

bool gen_pool_remove(GenPool &self, GenHandle handle);

u8 *gen_pool_get(GenPool &self, GenHandle handle);

void gen_pool_clear(GenPool &self);
