#include "downstream.hpp"

#include <chrono>

namespace hotel_booking::booking_service {

bool HttpResourceOk(clients::http::Client& client, const std::string& url) {
  auto req = client.CreateRequest();
  req.get(url).timeout(std::chrono::milliseconds{3000});
  const auto res = req.perform();
  return res && res->IsOk();
}

}  // namespace hotel_booking::booking_service
