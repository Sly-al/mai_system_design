#include "projection_handler.hpp"

#include <hotel_booking/http/json.hpp>

#include <userver/formats/json.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

namespace hb = hotel_booking::http;

HotelBookingProjectionHandler::HotelBookingProjectionHandler(const components::ComponentConfig& config,
                                                           const components::ComponentContext& context)
    : HttpHandlerBase(config, context), consumer_{context.FindComponent<BookingEventsConsumer>()} {}

std::string HotelBookingProjectionHandler::HandleRequest(server::http::HttpRequest& request,
                                                         server::request::RequestContext&) const {
  hb::SetJsonContentType(request);
  return formats::json::ToString(consumer_.ProjectionSnapshot());
}

}  // namespace hotel_booking::hotel_service
