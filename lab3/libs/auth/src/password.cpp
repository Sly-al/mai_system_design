#include <auth/password.hpp>

#include <userver/crypto/hash.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace auth {

std::string HashPassword(std::string_view password) {
  return crypto::hash::Sha256(password, crypto::hash::OutputEncoding::kHex);
}

}  // namespace auth
