#include <userver/components/minimal_server_component_list.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include "handlers.hpp"
#include "health_handler.hpp"

int main(int argc, char* argv[]) {
  auto component_list = components::MinimalServerComponentList()
                            .Append<hotel_booking::user_service::HealthHandler>()
                            .Append<hotel_booking::user_service::UserRegisterHandler>()
                            .Append<hotel_booking::user_service::UserLoginHandler>()
                            .Append<hotel_booking::user_service::UserByLoginHandler>()
                            .Append<hotel_booking::user_service::UserSearchHandler>()
                            .Append<hotel_booking::user_service::UserInternalGetHandler>();
  return utils::DaemonMain(argc, argv, component_list);
}
