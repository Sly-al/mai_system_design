#include "response_cache.hpp"

namespace hotel_booking::hotel_service {

ResponseCache::ResponseCache(const std::chrono::seconds ttl) : ttl_(ttl) {}

std::string ResponseCache::CityKey(const std::string& city) { return std::string{"hotels:city:"} + city; }

std::optional<std::string> ResponseCache::Get(const std::string& key) const {
  const auto now = std::chrono::steady_clock::now();
  std::lock_guard lock(mu_);
  const auto it = map_.find(key);
  if (it == map_.end()) {
    return std::nullopt;
  }
  if (it->second.expires <= now) {
    map_.erase(it);
    return std::nullopt;
  }
  return it->second.body;
}

void ResponseCache::Put(const std::string& key, std::string body) const {
  const auto now = std::chrono::steady_clock::now();
  Entry e;
  e.body = std::move(body);
  e.expires = now + ttl_;
  std::lock_guard lock(mu_);
  map_.insert_or_assign(key, std::move(e));
}

void ResponseCache::InvalidateForCity(const std::string& city) const {
  std::lock_guard lock(mu_);
  map_.erase(kListKey);
  map_.erase(CityKey(city));
}

ResponseCache& HotelListCache() {
  static ResponseCache cache{std::chrono::seconds{60}};
  return cache;
}

}  // namespace hotel_booking::hotel_service
