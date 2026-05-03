#pragma once

#include <string>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include "store.hpp"

#include <auth/undef_macros.hpp>

namespace hotel_booking::user_service {

class UserRegisterHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-register";
  UserRegisterHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  UserStore store_;
};

class UserLoginHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-auth-login";
  UserLoginHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  static yaml_config::Schema GetStaticConfigSchema();

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  std::string jwt_secret_;
  UserStore store_;
};

class UserByLoginHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-by-login";
  UserByLoginHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  UserStore store_;
};

class UserSearchHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-search";
  UserSearchHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  UserStore store_;
};

class UserInternalGetHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-internal";
  UserInternalGetHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  UserStore store_;
};

}  // namespace hotel_booking::user_service
