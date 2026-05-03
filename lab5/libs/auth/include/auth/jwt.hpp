#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace auth {

struct JwtClaims final {
  bool valid{false};
  std::optional<int64_t> sub;
  std::string login;
};

std::string JwtSignHs256(std::string_view secret, int64_t sub, std::string_view login);

JwtClaims JwtVerifyHs256(std::string_view secret, std::string_view token);

}  // namespace auth
