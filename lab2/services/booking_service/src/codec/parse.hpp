#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include <userver/formats/json.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::booking_service {

struct BookingCreateFields final {
  int64_t hotel_id{};
  std::string check_in;
  std::string check_out;
};

std::optional<std::string_view> TryParseBookingCreate(const formats::json::Value& j, BookingCreateFields& out);

}  // namespace hotel_booking::booking_service
