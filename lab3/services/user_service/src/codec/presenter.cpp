#include "presenter.hpp"

#include "fields.hpp"

#include <userver/formats/json/serialize.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::user_service {

formats::json::Value UserPublicValue(const UserRecord& u) {
  formats::json::ValueBuilder b;
  b[fields::kId] = u.id;
  b[fields::kLogin] = u.login;
  b[fields::kFirstName] = u.first_name;
  b[fields::kLastName] = u.last_name;
  return b.ExtractValue();
}

std::string UserPublicJsonString(const UserRecord& u) {
  return formats::json::ToString(UserPublicValue(u));
}

std::string LoginResponseJson(const std::string& access_token, const UserRecord& u) {
  formats::json::ValueBuilder b;
  b[fields::kAccessToken] = access_token;
  b[fields::kTokenType] = fields::kBearerTokenType;
  b[fields::kUser] = UserPublicValue(u);
  return formats::json::ToString(b.ExtractValue());
}

std::string UsersArrayJson(const std::vector<UserRecord>& rows) {
  formats::json::ValueBuilder arr(formats::json::Type::kArray);
  for (const auto& u : rows) {
    arr.PushBack(formats::json::ValueBuilder{UserPublicValue(u)});
  }
  formats::json::ValueBuilder out;
  out[fields::kUsers] = arr.ExtractValue();
  return formats::json::ToString(out.ExtractValue());
}

}  // namespace hotel_booking::user_service
