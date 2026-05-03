#include "parse.hpp"

#include "booking_errors.hpp"
#include "fields.hpp"

#include <hotel_booking/http/errors.hpp>

namespace hotel_booking::booking_service {

std::optional<std::string_view> TryParseBookingCreate(const formats::json::Value& j, BookingCreateFields& out) {
  try {
    out.hotel_id = j[fields::kHotelId].As<int64_t>();
    out.check_in = j[fields::kCheckIn].As<std::string>();
    out.check_out = j[fields::kCheckOut].As<std::string>();
  } catch (...) {
    return hotel_booking::http::errors::kInvalidBody;
  }
  if (out.check_in.empty() || out.check_out.empty() || out.hotel_id <= 0) {
    return kErrInvalidFields;
  }
  return std::nullopt;
}

}  // namespace hotel_booking::booking_service
