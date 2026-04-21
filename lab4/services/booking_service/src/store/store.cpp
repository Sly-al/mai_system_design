#include "store.hpp"

#include <algorithm>

#include <userver/storages/postgres/result_set.hpp>
#include <userver/storages/postgres/transaction.hpp>
#include <userver/storages/query.hpp>

namespace hotel_booking::booking_service {

namespace {

bool DateLess(const std::string& a, const std::string& b) { return a < b; }

const userver::storages::postgres::Query kOverlap{
    "SELECT EXISTS (SELECT 1 FROM bookings WHERE hotel_id=$1 AND status='active' "
    "AND NOT (check_out <= $2::date OR check_in >= $3::date))",
    userver::storages::postgres::Query::Name{"booking_overlap"}};

const userver::storages::postgres::Query kInsertBooking{
    "INSERT INTO bookings (user_id, hotel_id, check_in, check_out, status) "
    "VALUES ($1,$2,$3::date,$4::date,'active') "
    "RETURNING id, user_id, hotel_id, check_in::text, check_out::text, status",
    userver::storages::postgres::Query::Name{"booking_insert"}};

const userver::storages::postgres::Query kListByUser{
    "SELECT id, user_id, hotel_id, check_in::text, check_out::text, status FROM bookings WHERE user_id=$1 ORDER BY id",
    userver::storages::postgres::Query::Name{"booking_list_user"}};

const userver::storages::postgres::Query kFindById{
    "SELECT id, user_id, hotel_id, check_in::text, check_out::text, status FROM bookings WHERE id=$1",
    userver::storages::postgres::Query::Name{"booking_by_id"}};

const userver::storages::postgres::Query kCancel{
    "UPDATE bookings SET status='cancelled' WHERE id=$1 AND user_id=$2 AND status='active' RETURNING id",
    userver::storages::postgres::Query::Name{"booking_cancel"}};

BookingRecord RowToBooking(const userver::storages::postgres::Row& row) {
  return BookingRecord{row["id"].As<int64_t>(),         row["user_id"].As<int64_t>(),
                       row["hotel_id"].As<int64_t>(),     row["check_in"].As<std::string>(),
                       row["check_out"].As<std::string>(), row["status"].As<std::string>()};
}

}  // namespace

BookingStore::BookingStore(std::shared_ptr<userver::storages::postgres::Cluster> pg) : pg_(std::move(pg)) {}

std::optional<BookingRecord> BookingStore::Create(int64_t user_id, int64_t hotel_id, std::string check_in,
                                                  std::string check_out) const {
  if (!DateLess(check_in, check_out)) return std::nullopt;

  userver::storages::postgres::TransactionOptions opts;
  auto trx = pg_->Begin(opts);

  const auto ores = trx.Execute(kOverlap, hotel_id, check_in, check_out);
  const bool overlaps = ores[0][0].As<bool>();
  if (overlaps) {
    trx.Rollback();
    return std::nullopt;
  }

  const auto ires = trx.Execute(kInsertBooking, user_id, hotel_id, check_in, check_out);
  trx.Commit();
  if (ires.IsEmpty()) return std::nullopt;
  return RowToBooking(ires[0]);
}

std::vector<BookingRecord> BookingStore::ListByUser(int64_t user_id) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kListByUser, user_id);
  std::vector<BookingRecord> out;
  for (const auto& row : res) {
    out.push_back(RowToBooking(row));
  }
  return out;
}

std::optional<BookingRecord> BookingStore::FindById(int64_t id) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kFindById, id);
  if (res.IsEmpty()) return std::nullopt;
  return RowToBooking(res[0]);
}

bool BookingStore::CancelIfOwner(int64_t booking_id, int64_t user_id) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kCancel, booking_id, user_id);
  return !res.IsEmpty();
}

}  // namespace hotel_booking::booking_service
