#pragma once

#include <string>

#include <userver/clients/http/client.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::booking_service {

bool HttpResourceOk(clients::http::Client& client, const std::string& url);

}  // namespace hotel_booking::booking_service
