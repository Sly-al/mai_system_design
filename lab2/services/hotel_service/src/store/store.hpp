#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace hotel_booking::hotel_service {

struct HotelRecord final {
  int64_t id{};
  std::string name;
  std::string city;
  std::string address;
  int64_t owner_user_id{};
};

class HotelStore final {
public:
  HotelRecord Create(int64_t owner_user_id, std::string name, std::string city, std::string address);
  std::vector<HotelRecord> ListAll() const;
  std::vector<HotelRecord> ByCity(const std::string& city) const;
  std::optional<HotelRecord> FindById(int64_t id) const;

private:
  mutable std::mutex mutex_;
  int64_t next_id_{1};
  std::unordered_map<int64_t, HotelRecord> by_id_;
};

HotelStore& GetHotelStore();

}  // namespace hotel_booking::hotel_service
