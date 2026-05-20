#pragma once

#include <memory>

#include <hotel_booking/events/json_projection_store.hpp>

#include <userver/components/component_base.hpp>
#include <userver/kafka/consumer_component.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

class BookingEventsConsumer final : public components::ComponentBase {
public:
  static constexpr std::string_view kName = "booking-events-consumer";

  BookingEventsConsumer(const components::ComponentConfig& config, const components::ComponentContext& context);

  static yaml_config::Schema GetStaticConfigSchema();

  formats::json::Value ProjectionSnapshot() const;

private:
  void OnBatch(kafka::MessageBatchView messages);

  std::shared_ptr<events::JsonProjectionStore> store_;
  kafka::ConsumerScope consumer_;
};

}  // namespace hotel_booking::hotel_service
