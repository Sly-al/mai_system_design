#pragma once

namespace hotel_booking::booking_service::fields {

inline constexpr const char* kId = "id";
inline constexpr const char* kUserId = "user_id";
inline constexpr const char* kHotelId = "hotel_id";
inline constexpr const char* kCheckIn = "check_in";
inline constexpr const char* kCheckOut = "check_out";
inline constexpr const char* kStatus = "status";
inline constexpr const char* kBookings = "bookings";

inline constexpr const char* kPathBookingId = "id";

inline constexpr const char* kStatusCancelled = "cancelled";

inline constexpr const char* kCfgUserServiceBase = "user-service-base";
inline constexpr const char* kCfgHotelServiceBase = "hotel-service-base";

}  // namespace hotel_booking::booking_service::fields
