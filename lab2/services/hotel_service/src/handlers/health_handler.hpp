#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace hotel_booking::hotel_service {

class HealthHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-health";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequest(server::http::HttpRequest& request,
                             server::request::RequestContext&) const override;
};

}  // namespace hotel_booking::hotel_service
