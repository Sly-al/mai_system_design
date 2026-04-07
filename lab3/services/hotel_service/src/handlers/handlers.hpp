#pragma once

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include "store.hpp"

#include <auth/undef_macros.hpp>

namespace hotel_booking::hotel_service {

class HotelCreateHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-create";
  HotelCreateHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext& ctx) const override;

private:
  HotelStore store_;
};

class HotelListHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-list";
  HotelListHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  HotelStore store_;
};

class HotelByCityHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-by-city";
  HotelByCityHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  HotelStore store_;
};

class HotelInternalGetHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-internal";
  HotelInternalGetHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  HotelStore store_;
};

}  // namespace hotel_booking::hotel_service
