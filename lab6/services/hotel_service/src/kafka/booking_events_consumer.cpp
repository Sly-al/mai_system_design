#include "booking_events_consumer.hpp"

#include <userver/formats/json.hpp>
#include <userver/logging/log.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::hotel_service {

yaml_config::Schema BookingEventsConsumer::GetStaticConfigSchema() {
  return yaml_config::MergeSchemas<ComponentBase>(R"(
type: object
description: consumes booking events into JSON projection file
additionalProperties: false
properties:
  projection-file:
    type: string
)");
}

BookingEventsConsumer::BookingEventsConsumer(const components::ComponentConfig& config,
                                             const components::ComponentContext& context)
    : ComponentBase(config, context),
      store_{std::make_shared<events::JsonProjectionStore>(config["projection-file"].As<std::string>())},
      consumer_{context.FindComponent<kafka::ConsumerComponent>("kafka-consumer").GetConsumer()} {
  consumer_.Start([this](kafka::MessageBatchView messages) {
    // at-most-once: фиксируем offset до обработки
    consumer_.AsyncCommit();
    OnBatch(messages);
  });
}

void BookingEventsConsumer::OnBatch(kafka::MessageBatchView messages) {
  for (const auto& message : messages) {
    try {
      const auto event = formats::json::FromString(message.GetPayload());
      store_->Append(event);
    } catch (const std::exception& ex) {
      LOG_WARNING() << "skip invalid kafka payload: " << ex.what();
    }
  }
}

formats::json::Value BookingEventsConsumer::ProjectionSnapshot() const { return store_->Snapshot(); }

}  // namespace hotel_booking::hotel_service
