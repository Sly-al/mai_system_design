#include "presenter.hpp"

#include "fields.hpp"

#include <hotel_booking/http/collections.hpp>

#include <userver/formats/json/serialize.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

formats::json::Value HotelValue(const HotelRecord& h) {
  formats::json::ValueBuilder b;
  b[fields::kId] = h.id;
  b[fields::kName] = h.name;
  b[fields::kCity] = h.city;
  b[fields::kAddress] = h.address;
  b[fields::kOwnerUserId] = h.owner_user_id;
  return b.ExtractValue();
}

std::string HotelJsonString(const HotelRecord& h) {
  return formats::json::ToString(HotelValue(h));
}

std::string HotelsListJson(const std::vector<HotelRecord>& rows) {
  return hotel_booking::http::JsonArrayEnvelope(fields::kHotels, rows, [](const HotelRecord& h) { return HotelValue(h); });
}

}  // namespace hotel_booking::hotel_service
