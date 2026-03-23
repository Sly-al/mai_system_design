#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/bearer_jwt_middleware.hpp>

#include "handlers.hpp"
#include "health_handler.hpp"

int main(int argc, char* argv[]) {
  auto component_list = components::MinimalServerComponentList()
                            .Append<clients::dns::Component>()
                            .AppendComponentList(clients::http::ComponentList())
                            .Append<hotel_booking::booking_service::HealthHandler>()
                            .Append<auth::BearerJwtMiddlewareFactory>()
                            .Append<auth::BearerJwtHandlerPipelineBuilder>()
                            .Append<hotel_booking::booking_service::BookingCreateHandler>()
                            .Append<hotel_booking::booking_service::BookingListHandler>()
                            .Append<hotel_booking::booking_service::BookingCancelHandler>();
  return utils::DaemonMain(argc, argv, component_list);
}
