#include "presenter.hpp"

#include "fields.hpp"

#include <hotel_booking/http/collections.hpp>

#include <userver/formats/json/serialize.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::booking_service {

formats::json::Value BookingValue(const BookingRecord& b) {
  formats::json::ValueBuilder o;
  o[fields::kId] = b.id;
  o[fields::kUserId] = b.user_id;
  o[fields::kHotelId] = b.hotel_id;
  o[fields::kCheckIn] = b.check_in;
  o[fields::kCheckOut] = b.check_out;
  o[fields::kStatus] = b.status;
  return o.ExtractValue();
}

std::string BookingJsonString(const BookingRecord& b) {
  return formats::json::ToString(BookingValue(b));
}

std::string BookingsListJson(const std::vector<BookingRecord>& rows) {
  return hotel_booking::http::JsonArrayEnvelope(fields::kBookings, rows,
                                                 [](const BookingRecord& b) { return BookingValue(b); });
}

std::string CancelledBookingJson(int64_t booking_id) {
  formats::json::ValueBuilder o;
  o[fields::kId] = booking_id;
  o[fields::kStatus] = fields::kStatusCancelled;
  return formats::json::ToString(o.ExtractValue());
}

}  // namespace hotel_booking::booking_service
