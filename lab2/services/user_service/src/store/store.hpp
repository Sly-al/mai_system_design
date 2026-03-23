#pragma once

#include <cstdint>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

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
  std::optional<UserRecord> Create(std::string login, std::string password_hash, std::string first_name,
                                     std::string last_name);
  std::optional<UserRecord> FindByLogin(const std::string& login) const;
  std::optional<UserRecord> FindById(int64_t id) const;
  std::vector<UserRecord> SearchByNameMask(const std::string& first_mask, const std::string& last_mask) const;

private:
  mutable std::mutex mutex_;
  int64_t next_id_{1};
  std::unordered_map<int64_t, UserRecord> by_id_;
  std::unordered_map<std::string, int64_t> by_login_;
};

UserStore& GetUserStore();

}  // namespace hotel_booking::user_service
