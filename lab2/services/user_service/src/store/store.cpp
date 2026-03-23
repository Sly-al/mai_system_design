#include "store.hpp"

#include <algorithm>
#include <cctype>

namespace hotel_booking::user_service {

namespace {

bool ICaseSubstr(std::string_view hay, std::string_view needle) {
  if (needle.empty()) return true;
  std::string h(hay);
  std::string n(needle);
  std::transform(h.begin(), h.end(), h.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  std::transform(n.begin(), n.end(), n.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return h.find(n) != std::string::npos;
}

}  // namespace

std::optional<UserRecord> UserStore::Create(std::string login, std::string password_hash, std::string first_name,
                                              std::string last_name) {
  std::lock_guard lock(mutex_);
  if (by_login_.count(login)) return std::nullopt;
  const int64_t id = next_id_++;
  UserRecord u{id, std::move(login), std::move(password_hash), std::move(first_name), std::move(last_name)};
  by_login_[u.login] = id;
  by_id_[id] = u;
  return u;
}

std::optional<UserRecord> UserStore::FindByLogin(const std::string& login) const {
  std::lock_guard lock(mutex_);
  auto it = by_login_.find(login);
  if (it == by_login_.end()) return std::nullopt;
  auto u = by_id_.find(it->second);
  if (u == by_id_.end()) return std::nullopt;
  return u->second;
}

std::optional<UserRecord> UserStore::FindById(int64_t id) const {
  std::lock_guard lock(mutex_);
  auto it = by_id_.find(id);
  if (it == by_id_.end()) return std::nullopt;
  return it->second;
}

std::vector<UserRecord> UserStore::SearchByNameMask(const std::string& first_mask,
                                                    const std::string& last_mask) const {
  std::lock_guard lock(mutex_);
  std::vector<UserRecord> out;
  for (const auto& [_, u] : by_id_) {
    if (ICaseSubstr(u.first_name, first_mask) && ICaseSubstr(u.last_name, last_mask)) out.push_back(u);
  }
  return out;
}

UserStore& GetUserStore() {
  static UserStore s;
  return s;
}

}  // namespace hotel_booking::user_service
