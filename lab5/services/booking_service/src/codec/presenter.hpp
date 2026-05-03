#pragma once

#include "store.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include <userver/formats/json.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::booking_service {

formats::json::Value BookingValue(const BookingRecord& b);

std::string BookingJsonString(const BookingRecord& b);

std::string BookingsListJson(const std::vector<BookingRecord>& rows);

std::string CancelledBookingJson(int64_t booking_id);

}  // namespace hotel_booking::booking_service
