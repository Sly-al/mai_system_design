#include "store.hpp"

#include <userver/storages/postgres/result_set.hpp>
#include <userver/storages/query.hpp>

namespace hotel_booking::hotel_service {

namespace {

const userver::storages::postgres::Query kInsertHotel{
    "INSERT INTO hotels (owner_user_id, name, city, address) VALUES ($1,$2,$3,$4) "
    "RETURNING id, name, city, address, owner_user_id",
    userver::storages::postgres::Query::Name{"hotel_insert"}};

const userver::storages::postgres::Query kListHotels{
    "SELECT id, name, city, address, owner_user_id FROM hotels ORDER BY id",
    userver::storages::postgres::Query::Name{"hotel_list"}};

const userver::storages::postgres::Query kHotelsByCity{
    "SELECT id, name, city, address, owner_user_id FROM hotels WHERE city=$1 ORDER BY id",
    userver::storages::postgres::Query::Name{"hotel_by_city"}};

const userver::storages::postgres::Query kHotelById{
    "SELECT id, name, city, address, owner_user_id FROM hotels WHERE id=$1",
    userver::storages::postgres::Query::Name{"hotel_by_id"}};

HotelRecord RowToHotel(const userver::storages::postgres::Row& row) {
  return HotelRecord{row["id"].As<int64_t>(), row["name"].As<std::string>(), row["city"].As<std::string>(),
                     row["address"].As<std::string>(), row["owner_user_id"].As<int64_t>()};
}

}  // namespace

HotelStore::HotelStore(std::shared_ptr<userver::storages::postgres::Cluster> pg) : pg_(std::move(pg)) {}

HotelRecord HotelStore::Create(int64_t owner_user_id, std::string name, std::string city, std::string address) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kInsertHotel, owner_user_id,
                                name, city, address);
  return RowToHotel(res[0]);
}

std::vector<HotelRecord> HotelStore::ListAll() const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kListHotels);
  std::vector<HotelRecord> out;
  for (const auto& row : res) {
    out.push_back(RowToHotel(row));
  }
  return out;
}

std::vector<HotelRecord> HotelStore::ByCity(const std::string& city) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kHotelsByCity, city);
  std::vector<HotelRecord> out;
  for (const auto& row : res) {
    out.push_back(RowToHotel(row));
  }
  return out;
}

std::optional<HotelRecord> HotelStore::FindById(int64_t id) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kHotelById, id);
  if (res.IsEmpty()) return std::nullopt;
  return RowToHotel(res[0]);
}

}  // namespace hotel_booking::hotel_service
