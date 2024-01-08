#include "result.hpp"
#include <optional>
#include <string>

struct Texture {
private:
  unsigned int id;

public:
  Texture(unsigned char *data, int width, int height, int channels);
  ~Texture();

  static std::optional<Texture> load(const char *path);
  void bind() const;
  void unbind() const;
};
