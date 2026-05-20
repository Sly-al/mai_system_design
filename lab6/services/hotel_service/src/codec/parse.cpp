#include "parse.hpp"

#include "fields.hpp"

#include <hotel_booking/http/errors.hpp>

namespace hotel_booking::hotel_service {

std::optional<std::string_view> TryParseHotelCreate(const formats::json::Value& j, HotelCreateFields& out) {
  try {
    out.name = j[fields::kName].As<std::string>();
    out.city = j[fields::kCity].As<std::string>();
    if (!j[fields::kAddress].IsMissing()) {
      out.address = j[fields::kAddress].As<std::string>();
    }
  } catch (...) {
    return hotel_booking::http::errors::kInvalidBody;
  }
  if (out.name.empty() || out.city.empty()) {
    return hotel_booking::http::errors::kEmptyField;
  }
  return std::nullopt;
}

}  // namespace hotel_booking::hotel_service
