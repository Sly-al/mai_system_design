#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <userver/storages/postgres/cluster.hpp>

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
  explicit BookingStore(std::shared_ptr<userver::storages::postgres::Cluster> pg);

  std::optional<BookingRecord> Create(int64_t user_id, int64_t hotel_id, std::string check_in,
                                      std::string check_out) const;
  std::vector<BookingRecord> ListByUser(int64_t user_id) const;
  std::optional<BookingRecord> FindById(int64_t id) const;
  bool CancelIfOwner(int64_t booking_id, int64_t user_id) const;

private:
  std::shared_ptr<userver::storages::postgres::Cluster> pg_;
};

}  // namespace hotel_booking::booking_service
