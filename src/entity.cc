#include "entity.h"
#include "collections/generational_arena.h"
#include "core/types.h"
#include "graphics/light/point.h"
#include "math/transform.h"
#include "physics/body.h"
#include "sprite/sprite.h"
#include <cstring>

#define MAX_LABEL_LENGTH 64

enum ComponentKind {
    TRANSFORM = 0,
    SPRITE,
    RIGID_BODY,
    POINT_LIGHT,
    COMPONENT_COUNT,
};

static struct {
    GenArena components[COMPONENT_COUNT];
    GenArena entities;
} storage;

struct Entity {
    const u8 label[MAX_LABEL_LENGTH];
    u32 mask;
    GenHandle component_handles[COMPONENT_COUNT];
};

void entity_manager_init() {
    storage.components[TRANSFORM] = gen_arena_create(sizeof(Transform));
    storage.components[SPRITE] = gen_arena_create(sizeof(Sprite));
    storage.components[RIGID_BODY] = gen_arena_create(sizeof(RigidBody));
    storage.components[POINT_LIGHT] = gen_arena_create(sizeof(PointLight));
}

void entity_manager_deinit() {
    for (usize i = 0; i < COMPONENT_COUNT; i++)
        gen_arena_destroy(storage.components[i]);
    gen_arena_destroy(storage.entities);
}

GenHandle entity_spawn() {
    return gen_arena_insert(storage.entities, nullptr);
}

void entity_spawn_batch(u32 count, GenHandle *out_handles) {
    for (usize i = 0; i < count; i++)
        out_handles[i] = entity_spawn();
}

void entity_despawn(GenHandle handle) {
    Entity *entity = reinterpret_cast<Entity *>(gen_arena_get(storage.entities, handle));
    if (!entity) return; // Skip if the entity doesn't exist

    for (usize i = 0; i < COMPONENT_COUNT; i++)
        if (entity->mask & (1 << i)) // Deinitialize bound components
            gen_arena_remove(storage.components[i], entity->component_handles[i]);

    gen_arena_remove(storage.entities, handle); // Deinitialize entity;
}

template <typename T> const T *entity_get(GenHandle handle, ComponentKind kind) {
    Entity *entity = reinterpret_cast<Entity *>(gen_arena_get(storage.entities, handle));
    if (!entity || !(entity->mask & (1 << kind))) return nullptr;

    GenHandle component_handle = entity->component_handles[kind];
    return reinterpret_cast<T *>(gen_arena_get(storage.components[kind], component_handle));
}

template <typename T> T *entity_get_mut(GenHandle handle, ComponentKind kind) {
    Entity *entity = reinterpret_cast<Entity *>(gen_arena_get(storage.entities, handle));
    if (!entity || !(entity->mask & (1 << kind))) return nullptr;

    GenHandle component_handle = entity->component_handles[kind];
    return gen_arena_get(storage.components[kind], component_handle);
}

template <typename T> void entity_insert(GenHandle handle, ComponentKind kind, T data) {
    Entity *entity = reinterpret_cast<Entity *>(gen_arena_get(storage.entities, handle));
    if (!entity) return; // Ensure the entity exists

    // Insert a new component
    GenHandle component_handle = gen_arena_insert(storage.components[kind], reinterpret_cast<u8 *>(&data));

    // Update entity with component info
    entity->mask |= (1 << kind);
    entity->component_handles[kind] = component_handle;
}

template <typename T> void entity_update(GenHandle handle, ComponentKind kind, T data) {
    Entity *entity = reinterpret_cast<Entity *>(gen_arena_get(storage.entities, handle));
    if (!entity) return; // Ensure the entity exists

    u32 stride = storage.components[kind].stride;
    u8 *component = gen_arena_get(storage.components[kind], entity->component_handles[kind]);
    memcpy(component, data, stride);
}

void temp() {
    GenHandle character = entity_spawn();
    Sprite sprite;
    RigidBody body;

    entity_insert(character, ComponentKind::SPRITE, sprite);
    entity_insert(character, ComponentKind::SPRITE, body);

    Sprite &temp = entity_get<Sprite>(character, ComponentKind::SPRITE);
}
