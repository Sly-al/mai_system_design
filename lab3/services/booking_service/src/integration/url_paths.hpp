#pragma once

#include <string>

namespace hotel_booking::booking_service {

inline constexpr const char* kInternalUsersPrefix = "/v1/internal/users/";
inline constexpr const char* kInternalHotelsPrefix = "/v1/internal/hotels/";

std::string InternalUserUrl(const std::string& base_url, int64_t user_id);

std::string InternalHotelUrl(const std::string& base_url, int64_t hotel_id);

}  // namespace hotel_booking::booking_service
