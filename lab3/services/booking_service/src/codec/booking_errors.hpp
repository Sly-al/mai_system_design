#pragma once

#include <string_view>

namespace hotel_booking::booking_service {

inline constexpr std::string_view kErrInvalidFields = "invalid_fields";
inline constexpr std::string_view kErrUserUnreachable = "user_service_unreachable_or_user_missing";
inline constexpr std::string_view kErrHotelNotFound = "hotel_not_found";
inline constexpr std::string_view kErrDownstreamFailed = "downstream_request_failed";
inline constexpr std::string_view kErrOverlapOrDates = "invalid_dates_or_overlap";
inline constexpr std::string_view kErrForbidden = "forbidden";
inline constexpr std::string_view kErrCannotCancel = "cannot_cancel";

}  // namespace hotel_booking::booking_service
