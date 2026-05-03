#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <userver/storages/postgres/cluster.hpp>

namespace hotel_booking::user_service {

struct UserRecord final {
  int64_t id{};
  std::string login;
  std::string password_hash;
  std::string first_name;
  std::string last_name;
};

class UserStore final {
public:
  explicit UserStore(std::shared_ptr<userver::storages::postgres::Cluster> pg);

  std::optional<UserRecord> Create(std::string login, std::string password_hash, std::string first_name,
                                   std::string last_name) const;
  std::optional<UserRecord> FindByLogin(const std::string& login) const;
  std::optional<UserRecord> FindById(int64_t id) const;
  std::vector<UserRecord> SearchByNameMask(const std::string& first_mask, const std::string& last_mask) const;

private:
  std::shared_ptr<userver::storages::postgres::Cluster> pg_;
};

}  // namespace hotel_booking::user_service
