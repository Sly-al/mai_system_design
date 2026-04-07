#include "health_handler.hpp"

#include <userver/http/content_type.hpp>

#include <auth/undef_macros.hpp>

namespace hotel_booking::booking_service {

#define AUTH_STRINGIFY_HELPER(x) #x
#define AUTH_STRINGIFY(x) AUTH_STRINGIFY_HELPER(x)

std::string HealthHandler::HandleRequest(
    server::http::HttpRequest& request,
    server::request::RequestContext&) const {
  request.GetHttpResponse().SetStatusOk();
  request.GetHttpResponse().SetContentType(http::content_type::kApplicationJson);
  return std::string("{\"status\":\"ok\",\"service\":\"") +
         std::string(AUTH_STRINGIFY(SERVICE_TAG)) + "\"}";
}

}  // namespace hotel_booking::booking_service
