#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace hotel_booking::http {

enum class PathIntError { kMissing, kInvalid };

inline std::optional<int64_t> ParsePathInt64(const std::string& s, PathIntError* err_out = nullptr) {
  if (s.empty()) {
    if (err_out) *err_out = PathIntError::kMissing;
    return std::nullopt;
  }
  try {
    return std::stoll(s);
  } catch (...) {
    if (err_out) *err_out = PathIntError::kInvalid;
    return std::nullopt;
  }
}

}  // namespace hotel_booking::http
