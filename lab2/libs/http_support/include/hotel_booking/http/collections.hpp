#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <userver/formats/json.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::http {

template <typename T, typename Fn>
std::string JsonArrayEnvelope(std::string_view array_key, const std::vector<T>& rows, Fn&& row_to_json) {
  formats::json::ValueBuilder arr(formats::json::Type::kArray);
  for (const auto& row : rows) {
    arr.PushBack(formats::json::ValueBuilder{row_to_json(row)});
  }
  formats::json::ValueBuilder out;
  out[std::string{array_key}] = arr.ExtractValue();
  return formats::json::ToString(out.ExtractValue());
}

}  // namespace hotel_booking::http
