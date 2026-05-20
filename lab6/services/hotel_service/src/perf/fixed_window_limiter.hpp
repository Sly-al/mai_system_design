#pragma once

#include <chrono>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>

#include <userver/server/http/http_request.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

class FixedWindowLimiter final {
public:
  struct Decision final {
    bool allowed{};
    int limit{};
    int remaining{};
    std::int64_t reset_unix{};
  };

  FixedWindowLimiter(int max_per_window, std::chrono::seconds window);

  Decision Admit(const std::string& client_key);

private:
  struct Bucket final {
    std::int64_t window_id{};
    int count{};
  };

  const int max_per_window_;
  const std::chrono::seconds window_;
  std::mutex mu_;
  std::unordered_map<std::string, Bucket> buckets_;
};

FixedWindowLimiter& ByCitySearchLimiter();

void ApplyRateLimitHeaders(server::http::HttpRequest& request, const FixedWindowLimiter::Decision& decision);

}  // namespace hotel_booking::hotel_service
