#include "handlers.hpp"

#include "booking_errors.hpp"
#include "downstream.hpp"
#include "fields.hpp"
#include "parse.hpp"
#include "presenter.hpp"
#include "url_paths.hpp"

#include <hotel_booking/http/errors.hpp>
#include <hotel_booking/http/json.hpp>
#include <hotel_booking/http/path_args.hpp>
#include <hotel_booking/http/principal.hpp>

#include <userver/clients/http/component.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace hotel_booking::booking_service {

namespace hb = hotel_booking::http;

BookingCreateHandler::BookingCreateHandler(const components::ComponentConfig& config,
                                           const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      http_(context.FindComponent<components::HttpClient>().GetHttpClient()),
      user_base_(config[fields::kCfgUserServiceBase].As<std::string>()),
      hotel_base_(config[fields::kCfgHotelServiceBase].As<std::string>()),
      store_(context.FindComponent<components::Postgres>("postgres-db").GetCluster()) {}

yaml_config::Schema BookingCreateHandler::GetStaticConfigSchema() {
  return yaml_config::MergeSchemas<server::handlers::HttpHandlerBase>(R"(
type: object
description: create booking with downstream service URLs
additionalProperties: false
properties:
  user-service-base:
    type: string
    description: base URL of user service for internal checks
  hotel-service-base:
    type: string
    description: base URL of hotel service for internal checks
)");
}

std::string BookingCreateHandler::HandleRequest(server::http::HttpRequest& request,
                                                server::request::RequestContext& ctx) const {
  hb::SetJsonContentType(request);
  const auto principal = hb::PrincipalIdOr500(request, ctx);
  if (!principal) {
    return hb::ErrorJson(hb::errors::kPrincipalMissing);
  }
  const auto body = hb::TryParseJsonBody(request.RequestBody());
  if (!body) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(hb::errors::kInvalidJson);
  }
  BookingCreateFields in{};
  if (const auto err = TryParseBookingCreate(*body, in)) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(*err);
  }
  try {
    if (!HttpResourceOk(http_, InternalUserUrl(user_base_, *principal))) {
      request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadGateway);
      return hb::ErrorJson(kErrUserUnreachable);
    }
    if (!HttpResourceOk(http_, InternalHotelUrl(hotel_base_, in.hotel_id))) {
      request.GetHttpResponse().SetStatus(server::http::HttpStatus::kNotFound);
      return hb::ErrorJson(kErrHotelNotFound);
    }
  } catch (...) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadGateway);
    return hb::ErrorJson(kErrDownstreamFailed);
  }
  auto created = store_.Create(*principal, in.hotel_id, std::move(in.check_in), std::move(in.check_out));
  if (!created) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kConflict);
    return hb::ErrorJson(kErrOverlapOrDates);
  }
  request.GetHttpResponse().SetStatus(server::http::HttpStatus::kCreated);
  return BookingJsonString(*created);
}

BookingListHandler::BookingListHandler(const components::ComponentConfig& config,
                                       const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      store_(context.FindComponent<components::Postgres>("postgres-db").GetCluster()) {}

std::string BookingListHandler::HandleRequest(server::http::HttpRequest& request,
                                              server::request::RequestContext& ctx) const {
  hb::SetJsonContentType(request);
  const auto principal = hb::PrincipalIdOr500(request, ctx);
  if (!principal) {
    return hb::ErrorJson(hb::errors::kPrincipalMissing);
  }
  return BookingsListJson(store_.ListByUser(*principal));
}

BookingCancelHandler::BookingCancelHandler(const components::ComponentConfig& config,
                                           const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      store_(context.FindComponent<components::Postgres>("postgres-db").GetCluster()) {}

std::string BookingCancelHandler::HandleRequest(server::http::HttpRequest& request,
                                                server::request::RequestContext& ctx) const {
  hb::SetJsonContentType(request);
  const auto principal = hb::PrincipalIdOr500(request, ctx);
  if (!principal) {
    return hb::ErrorJson(hb::errors::kPrincipalMissing);
  }
  hb::PathIntError perr{};
  const auto bid = hb::ParsePathInt64(request.GetPathArg(fields::kPathBookingId), &perr);
  if (!bid) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(perr == hb::PathIntError::kMissing ? hb::errors::kMissingId : hb::errors::kInvalidId);
  }
  auto existing = store_.FindById(*bid);
  if (!existing) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kNotFound);
    return hb::ErrorJson(hb::errors::kNotFound);
  }
  if (existing->user_id != *principal) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kForbidden);
    return hb::ErrorJson(kErrForbidden);
  }
  if (!store_.CancelIfOwner(*bid, *principal)) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kConflict);
    return hb::ErrorJson(kErrCannotCancel);
  }
  return CancelledBookingJson(*bid);
}

}  // namespace hotel_booking::booking_service
