#include "fs.hpp"

#include <fstream>
#include <iostream>

namespace core::file {
std::optional<std::string> read_to_string(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << path << std::endl;

    return std::nullopt;
  }

  std::string data((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  file.close();

  return data;
}
} // namespace core::file
