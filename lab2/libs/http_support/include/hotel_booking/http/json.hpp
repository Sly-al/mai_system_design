#pragma once

#include <optional>
#include <string>
#include <string_view>

#include <userver/formats/json.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/http/content_type.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::http {

inline constexpr const char* kErrorField = "error";

inline void SetJsonContentType(server::http::HttpRequest& request) {
  request.GetHttpResponse().SetContentType(::userver::http::content_type::kApplicationJson);
}

inline std::string ErrorJson(std::string_view code) {
  formats::json::ValueBuilder b;
  b[kErrorField] = std::string(code);
  return formats::json::ToString(b.ExtractValue());
}

inline std::optional<formats::json::Value> TryParseJsonBody(std::string_view body) {
  try {
    return formats::json::FromString(body);
  } catch (...) {
    return std::nullopt;
  }
}

}  // namespace hotel_booking::http
