#include "store.hpp"

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/result_set.hpp>
#include <userver/storages/postgres/exceptions.hpp>
#include <userver/storages/query.hpp>

namespace hotel_booking::user_service {

namespace {

const userver::storages::postgres::Query kInsertUser{
    "INSERT INTO users (login, password_hash, first_name, last_name) VALUES ($1,$2,$3,$4) "
    "RETURNING id, login, password_hash, first_name, last_name",
    userver::storages::postgres::Query::Name{"user_insert"}};

const userver::storages::postgres::Query kSelectByLogin{
    "SELECT id, login, password_hash, first_name, last_name FROM users WHERE login=$1",
    userver::storages::postgres::Query::Name{"user_by_login"}};

const userver::storages::postgres::Query kSelectById{
    "SELECT id, login, password_hash, first_name, last_name FROM users WHERE id=$1",
    userver::storages::postgres::Query::Name{"user_by_id"}};

const userver::storages::postgres::Query kSearchByMask{
    "SELECT id, login, password_hash, first_name, last_name FROM users "
    "WHERE ($1::text = '' OR first_name ILIKE ('%' || $1 || '%')) "
    "AND ($2::text = '' OR last_name ILIKE ('%' || $2 || '%')) ORDER BY id",
    userver::storages::postgres::Query::Name{"user_search_mask"}};

UserRecord RowToUser(const userver::storages::postgres::Row& row) {
  return UserRecord{row["id"].As<int64_t>(),       row["login"].As<std::string>(),
                     row["password_hash"].As<std::string>(), row["first_name"].As<std::string>(),
                     row["last_name"].As<std::string>()};
}

}  // namespace

UserStore::UserStore(std::shared_ptr<userver::storages::postgres::Cluster> pg) : pg_(std::move(pg)) {}

std::optional<UserRecord> UserStore::Create(std::string login, std::string password_hash, std::string first_name,
                                            std::string last_name) const {
  try {
    const auto res =
        pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kInsertUser, login, password_hash,
                     first_name, last_name);
    if (res.IsEmpty()) return std::nullopt;
    return RowToUser(res[0]);
  } catch (const userver::storages::postgres::UniqueViolation&) {
    return std::nullopt;
  }
}

std::optional<UserRecord> UserStore::FindByLogin(const std::string& login) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kSelectByLogin, login);
  if (res.IsEmpty()) return std::nullopt;
  return RowToUser(res[0]);
}

std::optional<UserRecord> UserStore::FindById(int64_t id) const {
  const auto res = pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kSelectById, id);
  if (res.IsEmpty()) return std::nullopt;
  return RowToUser(res[0]);
}

std::vector<UserRecord> UserStore::SearchByNameMask(const std::string& first_mask,
                                                    const std::string& last_mask) const {
  const auto res =
      pg_->Execute(userver::storages::postgres::ClusterHostType::kMaster, kSearchByMask, first_mask, last_mask);
  std::vector<UserRecord> out;
  for (const auto& row : res) {
    out.push_back(RowToUser(row));
  }
  return out;
}

}  // namespace hotel_booking::user_service
