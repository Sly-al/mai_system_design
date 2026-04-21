#pragma once

#include <optional>
#include <string>
#include <string_view>

#include <userver/formats/json.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

struct HotelCreateFields final {
  std::string name;
  std::string city;
  std::string address;
};

std::optional<std::string_view> TryParseHotelCreate(const formats::json::Value& j, HotelCreateFields& out);

}  // namespace hotel_booking::hotel_service
