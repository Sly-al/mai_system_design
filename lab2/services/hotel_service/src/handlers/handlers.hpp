#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace hotel_booking::hotel_service {

class HotelCreateHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-create";
  using HttpHandlerBase::HttpHandlerBase;
  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext& ctx) const override;
};

class HotelListHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-list";
  using HttpHandlerBase::HttpHandlerBase;
  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;
};

class HotelByCityHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-by-city";
  using HttpHandlerBase::HttpHandlerBase;
  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;
};

class HotelInternalGetHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-internal";
  using HttpHandlerBase::HttpHandlerBase;
  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;
};

}  // namespace hotel_booking::hotel_service
