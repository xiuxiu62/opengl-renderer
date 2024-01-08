#pragma once

#include <optional>
#include <string>

namespace core::file {
std::optional<std::string> read_to_string(const std::string &path);
}
