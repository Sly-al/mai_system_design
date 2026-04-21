#include "store.hpp"

#include <userver/formats/bson/inline.hpp>
#include <userver/storages/mongo/options.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

namespace {

using formats::bson::MakeDoc;

namespace mongo_opt = storages::mongo::options;

HotelRecord DocToHotel(const formats::bson::Document& doc) {
  return HotelRecord{doc["_id"].As<int64_t>(), doc["name"].As<std::string>(), doc["city"].As<std::string>(),
                       doc["address"].As<std::string>(), doc["owner_user_id"].As<int64_t>()};
}

int64_t NextHotelId(storages::mongo::Collection& counters) {
  const auto wr = counters.FindAndModify(MakeDoc("_id", "hotel_seq"), MakeDoc("$inc", MakeDoc("seq", 1)),
                                         mongo_opt::Upsert{}, mongo_opt::ReturnNew{});
  const auto doc = wr.FoundDocument();
  if (!doc) {
    return 1;
  }
  return (*doc)["seq"].As<int64_t>();
}

}  // namespace

HotelStore::HotelStore(userver::storages::mongo::PoolPtr pool) : pool_(std::move(pool)) {}

HotelRecord HotelStore::Create(int64_t owner_user_id, std::string name, std::string city, std::string address) const {
  auto counters = pool_->GetCollection("counters");
  const int64_t id = NextHotelId(counters);
  auto hotels = pool_->GetCollection("hotels");
  hotels.InsertOne(MakeDoc("_id", id, "owner_user_id", owner_user_id, "name", name, "city", city, "address", address));
  return HotelRecord{id, std::move(name), std::move(city), std::move(address), owner_user_id};
}

std::vector<HotelRecord> HotelStore::ListAll() const {
  auto hotels = pool_->GetCollection("hotels");
  auto cursor = hotels.Find(MakeDoc(), mongo_opt::Sort{std::make_pair("_id", mongo_opt::Sort::kAscending)});
  std::vector<HotelRecord> out;
  if (!cursor) {
    return out;
  }
  for (const auto& doc : cursor) {
    out.push_back(DocToHotel(doc));
  }
  return out;
}

std::vector<HotelRecord> HotelStore::ByCity(const std::string& city) const {
  auto hotels = pool_->GetCollection("hotels");
  auto cursor = hotels.Find(MakeDoc("city", city),
                            mongo_opt::Sort{std::make_pair("_id", mongo_opt::Sort::kAscending)});
  std::vector<HotelRecord> out;
  if (!cursor) {
    return out;
  }
  for (const auto& doc : cursor) {
    out.push_back(DocToHotel(doc));
  }
  return out;
}

std::optional<HotelRecord> HotelStore::FindById(int64_t id) const {
  auto hotels = pool_->GetCollection("hotels");
  const auto doc = hotels.FindOne(MakeDoc("_id", id));
  if (!doc) {
    return std::nullopt;
  }
  return DocToHotel(*doc);
}

}  // namespace hotel_booking::hotel_service
