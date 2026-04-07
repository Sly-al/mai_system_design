#include "url_paths.hpp"

namespace hotel_booking::booking_service {

std::string InternalUserUrl(const std::string& base_url, int64_t user_id) {
  return base_url + kInternalUsersPrefix + std::to_string(user_id);
}

std::string InternalHotelUrl(const std::string& base_url, int64_t hotel_id) {
  return base_url + kInternalHotelsPrefix + std::to_string(hotel_id);
}

}  // namespace hotel_booking::booking_service
