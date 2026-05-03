#pragma once

namespace hotel_booking::user_service::fields {

inline constexpr const char* kId = "id";
inline constexpr const char* kLogin = "login";
inline constexpr const char* kPassword = "password";
inline constexpr const char* kFirstName = "first_name";
inline constexpr const char* kLastName = "last_name";
inline constexpr const char* kUsers = "users";
inline constexpr const char* kAccessToken = "access_token";
inline constexpr const char* kTokenType = "token_type";
inline constexpr const char* kBearerTokenType = "Bearer";
inline constexpr const char* kUser = "user";

inline constexpr const char* kPathLogin = "login";
inline constexpr const char* kPathId = "id";

inline constexpr const char* kQueryFirstName = "first_name";
inline constexpr const char* kQueryLastName = "last_name";

}  // namespace hotel_booking::user_service::fields
