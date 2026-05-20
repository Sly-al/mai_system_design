#pragma once

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include "booking_events_consumer.hpp"

#include <auth/undef_macros.hpp>

namespace hotel_booking::hotel_service {

class HotelBookingProjectionHandler final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hotel-booking-projection";
  HotelBookingProjectionHandler(const components::ComponentConfig& config, const components::ComponentContext& context);

  std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
  const BookingEventsConsumer& consumer_;
};

}  // namespace hotel_booking::hotel_service
