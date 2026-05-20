#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace hotel_booking::events {

inline constexpr std::string_view kTopic = "hotel-booking-events";

inline constexpr std::string_view kBookingCreated = "BookingCreated";
inline constexpr std::string_view kBookingCancelled = "BookingCancelled";

std::string BuildBookingCreated(int64_t id, int64_t user_id, int64_t hotel_id, std::string_view check_in,
                                std::string_view check_out, std::string_view status);

std::string BuildBookingCancelled(int64_t id, int64_t user_id, int64_t hotel_id, std::string_view status);

}  // namespace hotel_booking::events
