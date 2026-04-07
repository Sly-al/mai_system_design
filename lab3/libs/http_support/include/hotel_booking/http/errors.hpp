#pragma once

#include <string_view>

namespace hotel_booking::http::errors {

inline constexpr std::string_view kInvalidJson = "invalid_json";
inline constexpr std::string_view kInvalidBody = "invalid_body";
inline constexpr std::string_view kNotFound = "not_found";
inline constexpr std::string_view kMissingId = "missing_id";
inline constexpr std::string_view kInvalidId = "invalid_id";
inline constexpr std::string_view kEmptyField = "empty_field";
inline constexpr std::string_view kPrincipalMissing = "principal_missing";

}  // namespace hotel_booking::http::errors
