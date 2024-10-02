#include "entity.h"

#include "core/logger.h"
#include "core/types.h"
#include "graphics/light/point.h"
#include "math/transform.h"
#include "physics/body.h"
#include "sprite/sprite.h"

#include <cstring>

#define bytes(val) reinterpret_cast<u8 *>(val)

#define MAX_LABEL_LENGTH 64

static struct {
    GenPool components[COMPONENT_COUNT];
    GenPool entities;
} storage;

struct a {
    ComponentKind aa;
};

struct Entity {
    const char *label;
    u32 mask;
    GenHandle component_handles[COMPONENT_COUNT];
};

void entity_manager_init() {
    storage.components[TRANSFORM] = gen_pool_create(sizeof(Transform));
    storage.components[SPRITE] = gen_pool_create(sizeof(Sprite));
    storage.components[RIGID_BODY] = gen_pool_create(sizeof(RigidBody));
    storage.components[POINT_LIGHT] = gen_pool_create(sizeof(PointLight));
    storage.entities = gen_pool_create(sizeof(Entity));
}

void entity_manager_deinit() {
    gen_pool_destroy(storage.entities);
    for (usize i = 0; i < COMPONENT_COUNT; i++)
        gen_pool_destroy(storage.components[i]);
}

GenHandle entity_spawn(const char label[MAX_LABEL_LENGTH]) {
    Entity entity{0};
    memset(&entity, 0, sizeof(Entity));
    entity.label = label;

    return gen_pool_insert(storage.entities, bytes(&entity));
}

void entity_spawn_batch(u32 count, GenHandle *out_handles) {
    for (usize i = 0; i < count; i++)
        out_handles[i] = entity_spawn("\0");
}

void entity_despawn(GenHandle handle) {
    Entity *entity = reinterpret_cast<Entity *>(gen_pool_get(storage.entities, handle));
    if (!entity) return; // Skip if the entity doesn't exist

    for (usize i = 0; i < COMPONENT_COUNT; i++)
        if (entity->mask & (1 << i)) // Deinitialize bound components
            gen_pool_remove(storage.components[i], entity->component_handles[i]);

    gen_pool_remove(storage.entities, handle); // Deinitialize entity;
}

void entity_set_label(GenHandle handle, const char label[MAX_LABEL_LENGTH]) {
    Entity *entity = reinterpret_cast<Entity *>(gen_pool_get(storage.entities, handle));
    if (!entity) return; // Skip if the entity doesn't exist
    memcpy(&entity->label, label, MAX_LABEL_LENGTH);
}

u8 *entity_get(GenHandle handle, ComponentKind kind) {
    Entity *entity = reinterpret_cast<Entity *>(gen_pool_get(storage.entities, handle));
    if (!entity || !(entity->mask & (1 << kind))) return nullptr;

    GenHandle component_handle = entity->component_handles[kind];
    return gen_pool_get(storage.components[kind], component_handle);
}

void entity_insert(GenHandle handle, ComponentKind kind, u8 *data) {
    Entity *entity = reinterpret_cast<Entity *>(gen_pool_get(storage.entities, handle));
    if (!entity) return; // Ensure the entity exists

    // Insert a new component
    GenHandle component_handle = gen_pool_insert(storage.components[kind], data);

    // Update entity with component info
    entity->mask |= (1 << kind);
    entity->component_handles[kind] = component_handle;
}

void entity_update(GenHandle handle, ComponentKind kind, u8 *data) {
    Entity *entity = reinterpret_cast<Entity *>(gen_pool_get(storage.entities, handle));
    if (!entity) return; // Ensure the entity exists

    u32 stride = storage.components[kind].stride;
    u8 *component = gen_pool_get(storage.components[kind], entity->component_handles[kind]);
    memcpy(component, data, stride);
}
