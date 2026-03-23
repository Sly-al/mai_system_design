#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace hotel_booking::booking_service {

struct BookingRecord final {
  int64_t id{};
  int64_t user_id{};
  int64_t hotel_id{};
  std::string check_in;
  std::string check_out;
  std::string status;
};

class BookingStore final {
public:
  std::optional<BookingRecord> Create(int64_t user_id, int64_t hotel_id, std::string check_in, std::string check_out);
  std::vector<BookingRecord> ListByUser(int64_t user_id) const;
  std::optional<BookingRecord> FindById(int64_t id) const;
  bool CancelIfOwner(int64_t booking_id, int64_t user_id);

private:
  bool OverlapsActiveLocked(int64_t hotel_id, const std::string& in, const std::string& out) const;

  mutable std::mutex mutex_;
  int64_t next_id_{1};
  std::unordered_map<int64_t, BookingRecord> by_id_;
};

BookingStore& GetBookingStore();

}  // namespace hotel_booking::booking_service
