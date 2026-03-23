#include "handlers.hpp"

#include "fields.hpp"
#include "hotel_errors.hpp"
#include "parse.hpp"
#include "presenter.hpp"
#include "store.hpp"

#include <hotel_booking/http/errors.hpp>
#include <hotel_booking/http/json.hpp>
#include <hotel_booking/http/path_args.hpp>
#include <hotel_booking/http/principal.hpp>

#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace hotel_booking::hotel_service {

namespace hb = hotel_booking::http;

std::string HotelCreateHandler::HandleRequest(server::http::HttpRequest& request,
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
  HotelCreateFields in{};
  if (const auto err = TryParseHotelCreate(*body, in)) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(*err);
  }
  const auto h = GetHotelStore().Create(*principal, std::move(in.name), std::move(in.city), std::move(in.address));
  request.GetHttpResponse().SetStatus(server::http::HttpStatus::kCreated);
  return HotelJsonString(h);
}

std::string HotelListHandler::HandleRequest(server::http::HttpRequest& request,
                                            server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  return HotelsListJson(GetHotelStore().ListAll());
}

std::string HotelByCityHandler::HandleRequest(server::http::HttpRequest& request,
                                              server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  const auto city = request.GetArg(fields::kQueryCity);
  if (city.empty()) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(kErrMissingCity);
  }
  return HotelsListJson(GetHotelStore().ByCity(city));
}

std::string HotelInternalGetHandler::HandleRequest(server::http::HttpRequest& request,
                                                     server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  hb::PathIntError perr{};
  const auto id = hb::ParsePathInt64(request.GetPathArg(fields::kPathId), &perr);
  if (!id) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kBadRequest);
    return hb::ErrorJson(perr == hb::PathIntError::kMissing ? hb::errors::kMissingId : hb::errors::kInvalidId);
  }
  auto h = GetHotelStore().FindById(*id);
  if (!h) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kNotFound);
    return hb::ErrorJson(hb::errors::kNotFound);
  }
  return HotelJsonString(*h);
}

}  // namespace hotel_booking::hotel_service
