#include "handlers.hpp"

#include "fields.hpp"
#include "parse.hpp"
#include "presenter.hpp"
#include "store.hpp"
#include "user_errors.hpp"

#include <auth/jwt.hpp>
#include <auth/password.hpp>

#include <hotel_booking/http/errors.hpp>
#include <hotel_booking/http/json.hpp>
#include <hotel_booking/http/path_args.hpp>

#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace hotel_booking::user_service {

namespace hb = hotel_booking::http;

std::string UserRegisterHandler::HandleRequest(server::http::HttpRequest& request,
                                               server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  const auto body = hb::TryParseJsonBody(request.RequestBody());
  if (!body) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(hb::errors::kInvalidJson);
  }
  RegisterFields in{};
  if (const auto err = TryParseRegister(*body, in)) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(*err);
  }
  auto created = GetUserStore().Create(in.login, auth::HashPassword(in.password), in.first_name, in.last_name);
  if (!created) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kConflict);
    return hb::ErrorJson(kErrLoginExists);
  }
  request.GetHttpResponse().SetStatus(server::http::HttpStatus::kCreated);
  return UserPublicJsonString(*created);
}

UserLoginHandler::UserLoginHandler(const components::ComponentConfig& config, const components::ComponentContext& context)
    : HttpHandlerBase(config, context), jwt_secret_(config["jwt-secret"].As<std::string>()) {}

yaml_config::Schema UserLoginHandler::GetStaticConfigSchema() {
  return yaml_config::MergeSchemas<server::handlers::HttpHandlerBase>(R"(
type: object
description: login handler with JWT signing secret
additionalProperties: false
properties:
  jwt-secret:
    type: string
    description: HS256 signing secret for access tokens
)");
}

std::string UserLoginHandler::HandleRequest(server::http::HttpRequest& request,
                                            server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  const auto body = hb::TryParseJsonBody(request.RequestBody());
  if (!body) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(hb::errors::kInvalidJson);
  }
  LoginFields in{};
  if (const auto err = TryParseLogin(*body, in)) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(*err);
  }
  auto u = GetUserStore().FindByLogin(in.login);
  if (!u || u->password_hash != auth::HashPassword(in.password)) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kUnauthorized);
    return hb::ErrorJson(kErrInvalidCredentials);
  }
  const auto token = auth::JwtSignHs256(jwt_secret_, u->id, u->login);
  return LoginResponseJson(token, *u);
}

std::string UserByLoginHandler::HandleRequest(server::http::HttpRequest& request,
                                              server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  const auto& login = request.GetPathArg(fields::kPathLogin);
  if (login.empty()) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(kErrMissingLogin);
  }
  auto u = GetUserStore().FindByLogin(login);
  if (!u) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kNotFound);
    return hb::ErrorJson(hb::errors::kNotFound);
  }
  return UserPublicJsonString(*u);
}

std::string UserSearchHandler::HandleRequest(server::http::HttpRequest& request,
                                             server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  const auto fn = request.GetArg(fields::kQueryFirstName);
  const auto ln = request.GetArg(fields::kQueryLastName);
  if (fn.empty() && ln.empty()) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(kErrNeedNameQuery);
  }
  const auto rows = GetUserStore().SearchByNameMask(fn, ln);
  return UsersArrayJson(rows);
}

std::string UserInternalGetHandler::HandleRequest(server::http::HttpRequest& request,
                                                    server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  hb::PathIntError perr{};
  const auto id = hb::ParsePathInt64(request.GetPathArg(fields::kPathId), &perr);
  if (!id) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(perr == hb::PathIntError::kMissing ? hb::errors::kMissingId : hb::errors::kInvalidId);
  }
  auto u = GetUserStore().FindById(*id);
  if (!u) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kNotFound);
    return hb::ErrorJson(hb::errors::kNotFound);
  }
  return UserPublicJsonString(*u);
}

}  // namespace hotel_booking::user_service
