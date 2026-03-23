#include "store.hpp"

#include <algorithm>

namespace hotel_booking::hotel_service {

HotelRecord HotelStore::Create(int64_t owner_user_id, std::string name, std::string city, std::string address) {
  std::lock_guard lock(mutex_);
  const int64_t id = next_id_++;
  HotelRecord h{id, std::move(name), std::move(city), std::move(address), owner_user_id};
  by_id_[id] = h;
  return h;
}

std::vector<HotelRecord> HotelStore::ListAll() const {
  std::lock_guard lock(mutex_);
  std::vector<HotelRecord> out;
  out.reserve(by_id_.size());
  for (const auto& [_, h] : by_id_) out.push_back(h);
  std::sort(out.begin(), out.end(), [](const HotelRecord& a, const HotelRecord& b) { return a.id < b.id; });
  return out;
}

std::vector<HotelRecord> HotelStore::ByCity(const std::string& city) const {
  std::lock_guard lock(mutex_);
  std::vector<HotelRecord> out;
  for (const auto& [_, h] : by_id_) {
    if (h.city == city) out.push_back(h);
  }
  return out;
}

std::optional<HotelRecord> HotelStore::FindById(int64_t id) const {
  std::lock_guard lock(mutex_);
  auto it = by_id_.find(id);
  if (it == by_id_.end()) return std::nullopt;
  return it->second;
}

HotelStore& GetHotelStore() {
  static HotelStore s;
  return s;
}

}  // namespace hotel_booking::hotel_service
