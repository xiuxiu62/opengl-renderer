#pragma once

#include "collections/generational_arena.h"

void entity_manager_init();

void entity_manager_deinit();

GenHandle entity_manager_spawn();

bool entity_manager_despawn(GenHandle handle);

const u8 *entity_manager_get(GenHandle handle);

u8 *entity_manager_get_mut(GenHandle handle);
