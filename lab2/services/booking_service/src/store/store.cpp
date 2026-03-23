#include "store.hpp"

#include <algorithm>

namespace hotel_booking::booking_service {

namespace {

bool DateLess(const std::string& a, const std::string& b) { return a < b; }

bool IntervalsOverlap(const std::string& a_in, const std::string& a_out, const std::string& b_in,
                      const std::string& b_out) {
  return !(DateLess(a_out, b_in) || DateLess(b_out, a_in));
}

}  // namespace

bool BookingStore::OverlapsActiveLocked(int64_t hotel_id, const std::string& in, const std::string& out) const {
  for (const auto& [_, b] : by_id_) {
    if (b.hotel_id != hotel_id) continue;
    if (b.status != "active") continue;
    if (IntervalsOverlap(in, out, b.check_in, b.check_out)) return true;
  }
  return false;
}

std::optional<BookingRecord> BookingStore::Create(int64_t user_id, int64_t hotel_id, std::string check_in,
                                                  std::string check_out) {
  if (!DateLess(check_in, check_out)) return std::nullopt;
  std::lock_guard lock(mutex_);
  if (OverlapsActiveLocked(hotel_id, check_in, check_out)) return std::nullopt;
  const int64_t id = next_id_++;
  BookingRecord b{id, user_id, hotel_id, std::move(check_in), std::move(check_out), "active"};
  by_id_[id] = b;
  return b;
}

std::vector<BookingRecord> BookingStore::ListByUser(int64_t user_id) const {
  std::lock_guard lock(mutex_);
  std::vector<BookingRecord> out;
  for (const auto& [_, b] : by_id_) {
    if (b.user_id == user_id) out.push_back(b);
  }
  std::sort(out.begin(), out.end(), [](const BookingRecord& a, const BookingRecord& b) { return a.id < b.id; });
  return out;
}

std::optional<BookingRecord> BookingStore::FindById(int64_t id) const {
  std::lock_guard lock(mutex_);
  auto it = by_id_.find(id);
  if (it == by_id_.end()) return std::nullopt;
  return it->second;
}

bool BookingStore::CancelIfOwner(int64_t booking_id, int64_t user_id) {
  std::lock_guard lock(mutex_);
  auto it = by_id_.find(booking_id);
  if (it == by_id_.end()) return false;
  if (it->second.user_id != user_id) return false;
  if (it->second.status != "active") return false;
  it->second.status = "cancelled";
  return true;
}

BookingStore& GetBookingStore() {
  static BookingStore s;
  return s;
}

}  // namespace hotel_booking::booking_service
