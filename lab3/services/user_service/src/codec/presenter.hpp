#pragma once

#include "store.hpp"

#include <string>

#include <userver/formats/json.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::user_service {

formats::json::Value UserPublicValue(const UserRecord& u);

std::string UserPublicJsonString(const UserRecord& u);

std::string LoginResponseJson(const std::string& access_token, const UserRecord& u);

std::string UsersArrayJson(const std::vector<UserRecord>& rows);

}  // namespace hotel_booking::user_service
