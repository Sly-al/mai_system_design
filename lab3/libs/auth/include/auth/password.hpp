#pragma once

#include <string>
#include <string_view>

namespace auth {

std::string HashPassword(std::string_view password);

}  // namespace auth
