#include "fixed_window_limiter.hpp"

#include <ctime>
#include <string>
#include <string_view>

namespace hotel_booking::hotel_service {

FixedWindowLimiter::FixedWindowLimiter(const int max_per_window, const std::chrono::seconds window)
    : max_per_window_(max_per_window), window_(window) {}

FixedWindowLimiter::Decision FixedWindowLimiter::Admit(const std::string& client_key) {
  const auto now = std::time(nullptr);
  const auto window_seconds = static_cast<std::int64_t>(window_.count());
  const auto wid = window_seconds > 0 ? now / window_seconds : 0;

  std::lock_guard lock(mu_);
  auto& b = buckets_[client_key];
  if (b.window_id != wid) {
    b.window_id = wid;
    b.count = 0;
  }
  b.count += 1;

  Decision d;
  d.limit = max_per_window_;
  d.reset_unix = (wid + 1) * window_seconds;
  d.allowed = b.count <= max_per_window_;
  d.remaining = d.allowed ? (max_per_window_ - b.count) : 0;
  return d;
}

FixedWindowLimiter& ByCitySearchLimiter() {
  static FixedWindowLimiter instance{100, std::chrono::seconds{60}};
  return instance;
}

void ApplyRateLimitHeaders(server::http::HttpRequest& request, const FixedWindowLimiter::Decision& decision) {
  auto& response = request.GetHttpResponse();
  response.SetHeader(std::string_view{"X-RateLimit-Limit"}, std::to_string(decision.limit));
  response.SetHeader(std::string_view{"X-RateLimit-Remaining"}, std::to_string(decision.remaining));
  response.SetHeader(std::string_view{"X-RateLimit-Reset"}, std::to_string(decision.reset_unix));
}

}  // namespace hotel_booking::hotel_service
