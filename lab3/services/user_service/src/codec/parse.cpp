#include "parse.hpp"

#include "fields.hpp"

#include <hotel_booking/http/errors.hpp>

namespace hotel_booking::user_service {

std::optional<std::string_view> TryParseRegister(const formats::json::Value& j, RegisterFields& out) {
  try {
    out.login = j[fields::kLogin].As<std::string>();
    out.password = j[fields::kPassword].As<std::string>();
    out.first_name = j[fields::kFirstName].As<std::string>();
    out.last_name = j[fields::kLastName].As<std::string>();
  } catch (...) {
    return hotel_booking::http::errors::kInvalidBody;
  }
  if (out.login.empty() || out.password.empty() || out.first_name.empty() || out.last_name.empty()) {
    return hotel_booking::http::errors::kEmptyField;
  }
  return std::nullopt;
}

std::optional<std::string_view> TryParseLogin(const formats::json::Value& j, LoginFields& out) {
  try {
    out.login = j[fields::kLogin].As<std::string>();
    out.password = j[fields::kPassword].As<std::string>();
  } catch (...) {
    return hotel_booking::http::errors::kInvalidBody;
  }
  return std::nullopt;
}

}  // namespace hotel_booking::user_service
