#pragma once

#include "store.hpp"

#include <string>
#include <vector>

#include <userver/formats/json.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

formats::json::Value HotelValue(const HotelRecord& h);

std::string HotelJsonString(const HotelRecord& h);

std::string HotelsListJson(const std::vector<HotelRecord>& rows);

}  // namespace hotel_booking::hotel_service
