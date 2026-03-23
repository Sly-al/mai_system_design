#pragma once

#include <optional>
#include <string>
#include <string_view>

#include <userver/formats/json.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::user_service {

struct RegisterFields final {
  std::string login;
  std::string password;
  std::string first_name;
  std::string last_name;
};

struct LoginFields final {
  std::string login;
  std::string password;
};

std::optional<std::string_view> TryParseRegister(const formats::json::Value& j, RegisterFields& out);

std::optional<std::string_view> TryParseLogin(const formats::json::Value& j, LoginFields& out);

}  // namespace hotel_booking::user_service
