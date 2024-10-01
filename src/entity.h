#pragma once

#include "collections/generational_arena.h"

enum ComponentKind {
    TRANSFORM = 0,
    SPRITE,
    RIGID_BODY,
    POINT_LIGHT,
    COMPONENT_COUNT = 4,
};

void entity_manager_init();
void entity_manager_deinit();

GenHandle entity_spawn(const char *label);
void entity_spawn_batch(u32 count, GenHandle *out_handles);
void entity_despawn(GenHandle handle);

void entity_set_label(GenHandle handle, const char *label);

u8 *entity_get(GenHandle handle, ComponentKind kind);

void entity_insert(GenHandle handle, ComponentKind kind, u8 *data);
void entity_update(GenHandle handle, ComponentKind kind, u8 *data);
