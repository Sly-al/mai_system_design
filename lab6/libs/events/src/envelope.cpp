#include <hotel_booking/events/envelope.hpp>

#include <chrono>
#include <random>

#include <userver/formats/json/value_builder.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::events {

namespace {

std::string NewEventId() {
  static thread_local std::mt19937_64 rng{std::random_device{}()};
  return std::to_string(rng());
}

formats::json::ValueBuilder BaseEnvelope(std::string_view event_type) {
  formats::json::ValueBuilder root;
  root["event_id"] = NewEventId();
  root["event_type"] = std::string(event_type);
  const auto now = std::chrono::system_clock::now();
  const auto sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
  root["occurred_at"] = std::to_string(sec);
  return root;
}

}  // namespace

std::string BuildBookingCreated(const int64_t id, const int64_t user_id, const int64_t hotel_id,
                                const std::string_view check_in, const std::string_view check_out,
                                const std::string_view status) {
  auto root = BaseEnvelope(kBookingCreated);
  formats::json::ValueBuilder payload;
  payload["id"] = id;
  payload["user_id"] = user_id;
  payload["hotel_id"] = hotel_id;
  payload["check_in"] = std::string(check_in);
  payload["check_out"] = std::string(check_out);
  payload["status"] = std::string(status);
  root["payload"] = payload.ExtractValue();
  return formats::json::ToString(root.ExtractValue());
}

std::string BuildBookingCancelled(const int64_t id, const int64_t user_id, const int64_t hotel_id,
                                  const std::string_view status) {
  auto root = BaseEnvelope(kBookingCancelled);
  formats::json::ValueBuilder payload;
  payload["id"] = id;
  payload["user_id"] = user_id;
  payload["hotel_id"] = hotel_id;
  payload["status"] = std::string(status);
  root["payload"] = payload.ExtractValue();
  return formats::json::ToString(root.ExtractValue());
}

}  // namespace hotel_booking::events
