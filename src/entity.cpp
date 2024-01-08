#include <memory>
#include <unordered_map>

struct Entity;

struct EntityManager {
private:
  std::unordered_map<unsigned int, std::unique_ptr<Entity>> entities;
  unsigned int next_id;

  Entity *get(unsigned int id) {
    auto it = this->entities.find(id);

    return (it != entities.end()) ? it->second.get() : nullptr;
  }

  unsigned int insert(std::unique_ptr<Entity> entity) {
    unsigned int id = this->next_id;
    this->next_id += 1;
    this->entities[id] = std::move(entity);

    return id;
  }

  // Entity delete(unsigned int id) {}
};

struct Entity {
  unsigned int id;

  virtual ~Entity() = default;
  virtual void update() = 0;

  // virtual
};
