#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <userver/storages/postgres/cluster.hpp>

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
  explicit HotelStore(std::shared_ptr<userver::storages::postgres::Cluster> pg);

  HotelRecord Create(int64_t owner_user_id, std::string name, std::string city, std::string address) const;
  std::vector<HotelRecord> ListAll() const;
  std::vector<HotelRecord> ByCity(const std::string& city) const;
  std::optional<HotelRecord> FindById(int64_t id) const;

private:
  std::shared_ptr<userver::storages::postgres::Cluster> pg_;
};

}  // namespace hotel_booking::hotel_service
