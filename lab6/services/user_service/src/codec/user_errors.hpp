#pragma once

#include <string_view>

namespace hotel_booking::user_service {

inline constexpr std::string_view kErrLoginExists = "login_exists";
inline constexpr std::string_view kErrInvalidCredentials = "invalid_credentials";
inline constexpr std::string_view kErrMissingLogin = "missing_login";
inline constexpr std::string_view kErrNeedNameQuery = "provide_first_name_and_or_last_name";

}  // namespace hotel_booking::user_service
