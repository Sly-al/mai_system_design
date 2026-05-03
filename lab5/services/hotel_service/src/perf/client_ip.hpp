#pragma once

#include <string>

#include <userver/server/http/http_request.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

std::string ClientIp(const server::http::HttpRequest& request);

}  // namespace hotel_booking::hotel_service
