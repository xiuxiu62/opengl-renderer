#include "collections/generational_pool.h"

#include <stdlib.h>
#include <string.h>

static const u32 DEFUALT_CAPACITY = 64;

GenPool gen_pool_create(u32 stride, u32 capacity) {
    usize data_size = stride * capacity;
    usize aux_stride = sizeof(u32);
    usize aux_size = stride * capacity;

    GenPool self = {
        .data = static_cast<u8 *>(malloc(data_size)),
        .generations = static_cast<u32 *>(calloc(capacity, aux_stride)),
        .free_list = static_cast<u32 *>(calloc(capacity, aux_stride)),
        .stride = stride,
        .capacity = capacity,
        .len = 0,
        .free_len = 0,
        .next_id = 0,
    };

    return self;
}

GenPool gen_pool_create(u32 stride) {
    return gen_pool_create(stride, DEFUALT_CAPACITY);
}

void gen_pool_destroy(GenPool &self) {
    free(self.data);
    free(self.generations);
    free(self.free_list);
}

void gen_pool_resize(GenPool &self) {
    u32 new_capacity = self.capacity * 2;
    u32 new_data_size = self.stride * new_capacity;
    u32 new_aux_size = sizeof(u32) * new_capacity;
    u32 old_data_size = self.stride * self.capacity;
    u32 old_aux_size = sizeof(u32) * self.capacity;

    self.data = static_cast<u8 *>(realloc(self.data, new_data_size));
    self.generations = static_cast<u32 *>(realloc(self.generations, new_aux_size));
    self.free_list = static_cast<u32 *>(realloc(self.free_list, new_aux_size));

    // Zero new memory regions
    memset(self.data + old_data_size, 0, old_data_size);
    memset(self.generations + old_aux_size, 0, old_aux_size);

    self.capacity = new_capacity;
}

bool gen_pool_contains(const GenPool &self, GenHandle handle) {
    return handle.id < self.capacity && self.generations[handle.id] == handle.gen;
}

GenHandle gen_pool_insert(GenPool &self, u8 *data) {
    u32 id;
    if (self.free_len > 0) { // Try taking from free list first
        id = self.free_list[self.free_len - 1];
        self.generations[id] += 1;
        self.free_len -= 1;
    } else { // otherwise push back to data list if no slots are free
        if (self.next_id >= self.capacity) gen_pool_resize(self); // resizing lists if we're at capacity
        id = self.next_id++;
    }

    self.generations[id]++;
    if (data) { // copy data if it was provided
        memcpy(self.data + self.stride * id, data, self.stride);
    } else { // zero initializing if a nullptr was provided
        memset(self.data + self.stride * id, 0, self.stride);
    }
    self.len++;

    return {
        .id = id,
        .gen = self.generations[id],
    };
}

bool gen_pool_remove(GenPool &self, GenHandle handle) {
    if (!gen_pool_contains(self, handle)) return false;

    self.free_list[self.free_len++] = handle.id;
    self.len--;

    return true;
}

u8 *gen_pool_get(GenPool &self, GenHandle handle) {
    if (!gen_pool_contains(self, handle)) return nullptr;
    return self.data + self.stride * handle.id;
}

void gen_pool_clear(GenPool &self) {
    memset(self.data, 0, self.stride * self.capacity);
    memset(self.generations, 0, sizeof(u32) * self.capacity);
    self.len = 0;
    self.free_len = 0;
    self.next_id = 0;
}
