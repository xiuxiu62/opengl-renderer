#include "collections/generational_arena.h"
#include "entity.h"
#include "math/transform.h"
#include "sprite/sprite.h"

#define MAX_NAME_LENGTH 32

// NOTES:
// decide between unique entity buffers with an enum switch, sequential component arrays,
// or a mix of both where component arrays are used for systems that need to iterate over those components
//
// really this boils down to array-of-structs
// vs struct-of-arrays

static GenArena character_manager;

struct Character {
    Sprite sprite;
    Transform transform;
};

struct Entity {
    char name[MAX_NAME_LENGTH];
};

void entity_manager_init() {
    character_manager = gen_arena_create(sizeof(Character));
}

void entity_manager_deinit() {
    gen_arena_destroy(character_manager);
}

GenHandle entity_manager_spawn(Character character) {
    return gen_arena_insert(character_manager, reinterpret_cast<u8 *>(&character));
}

bool entity_manager_despawn(GenHandle handle) {
    return gen_arena_remove(character_manager, handle);
}

const Character &entity_manager_get_character(GenHandle handle) {
    return reinterpret_cast<Character &>(*gen_arena_get(character_manager, handle));
}

const u8 *entity_manager_get(GenHandle handle) {
    return gen_arena_get(character_manager, handle);
}

u8 *entity_manager_get_mut(GenHandle handle) {
    return gen_arena_get(character_manager, handle);
}
