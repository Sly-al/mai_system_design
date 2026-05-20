#include <userver/clients/dns/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/kafka/consumer_component.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/bearer_jwt_middleware.hpp>

#include "booking_events_consumer.hpp"
#include "handlers.hpp"
#include "health_handler.hpp"
#include "projection_handler.hpp"

int main(int argc, char* argv[]) {
  auto component_list = components::MinimalServerComponentList()
                            .Append<clients::dns::Component>()
                            .Append<components::TestsuiteSupport>()
                            .Append<components::Mongo>("mongo-db")
                            .Append<kafka::ConsumerComponent>("kafka-consumer")
                            .Append<hotel_booking::hotel_service::BookingEventsConsumer>()
                            .Append<hotel_booking::hotel_service::HealthHandler>()
                            .Append<auth::BearerJwtMiddlewareFactory>()
                            .Append<auth::BearerJwtHandlerPipelineBuilder>()
                            .Append<hotel_booking::hotel_service::HotelCreateHandler>()
                            .Append<hotel_booking::hotel_service::HotelListHandler>()
                            .Append<hotel_booking::hotel_service::HotelByCityHandler>()
                            .Append<hotel_booking::hotel_service::HotelInternalGetHandler>()
                            .Append<hotel_booking::hotel_service::HotelBookingProjectionHandler>();
  return utils::DaemonMain(argc, argv, component_list);
}
