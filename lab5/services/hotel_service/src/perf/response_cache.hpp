#pragma once

#include <chrono>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace hotel_booking::hotel_service {

class ResponseCache final {
public:
  explicit ResponseCache(std::chrono::seconds ttl);

  static constexpr const char* kListKey = "hotels:list";

  static std::string CityKey(const std::string& city);

  std::optional<std::string> Get(const std::string& key) const;

  void Put(const std::string& key, std::string body) const;

  void InvalidateForCity(const std::string& city) const;

private:
  struct Entry final {
    std::string body;
    std::chrono::steady_clock::time_point expires;
  };

  const std::chrono::seconds ttl_;
  mutable std::mutex mu_;
  mutable std::unordered_map<std::string, Entry> map_;
};

ResponseCache& HotelListCache();

}  // namespace hotel_booking::hotel_service
