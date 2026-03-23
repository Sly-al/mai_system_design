#include <userver/components/minimal_server_component_list.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/bearer_jwt_middleware.hpp>

#include "handlers.hpp"
#include "health_handler.hpp"

int main(int argc, char* argv[]) {
  auto component_list = components::MinimalServerComponentList()
                            .Append<hotel_booking::hotel_service::HealthHandler>()
                            .Append<auth::BearerJwtMiddlewareFactory>()
                            .Append<auth::BearerJwtHandlerPipelineBuilder>()
                            .Append<hotel_booking::hotel_service::HotelCreateHandler>()
                            .Append<hotel_booking::hotel_service::HotelListHandler>()
                            .Append<hotel_booking::hotel_service::HotelByCityHandler>()
                            .Append<hotel_booking::hotel_service::HotelInternalGetHandler>();
  return utils::DaemonMain(argc, argv, component_list);
}
