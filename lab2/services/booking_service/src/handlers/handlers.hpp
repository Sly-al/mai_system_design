#pragma once

#include <string>

#include <userver/clients/http/client.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace hotel_booking::booking_service {

class BookingCreateHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-booking-create";
  BookingCreateHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  static yaml_config::Schema GetStaticConfigSchema();

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext& ctx) const override;

private:
  clients::http::Client& http_;
  std::string user_base_;
  std::string hotel_base_;
};

class BookingListHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-booking-list";
  using HttpHandlerBase::HttpHandlerBase;
  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext& ctx) const override;
};

class BookingCancelHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-booking-cancel";
  using HttpHandlerBase::HttpHandlerBase;
  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext& ctx) const override;
};

}  // namespace hotel_booking::booking_service
