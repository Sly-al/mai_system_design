#include "booking_event_publisher.hpp"

#include <hotel_booking/events/envelope.hpp>

#include <userver/logging/log.hpp>

namespace hotel_booking::booking_service {

BookingEventPublisher::BookingEventPublisher(const components::ComponentConfig& config,
                                             const components::ComponentContext& context)
    : ComponentBase(config, context),
      producer_{context.FindComponent<kafka::ProducerComponent>("kafka-producer").GetProducer()} {}

void BookingEventPublisher::PublishCreated(const BookingRecord& booking) const {
  try {
    const auto payload = events::BuildBookingCreated(booking.id, booking.user_id, booking.hotel_id, booking.check_in,
                                                     booking.check_out, booking.status);
    producer_.Send(events::kTopic, std::to_string(booking.id), payload);
  } catch (const std::exception& ex) {
    LOG_WARNING() << "at-most-once: drop BookingCreated event: " << ex.what();
  }
}

void BookingEventPublisher::PublishCancelled(const BookingRecord& booking) const {
  try {
    const auto payload =
        events::BuildBookingCancelled(booking.id, booking.user_id, booking.hotel_id, booking.status);
    producer_.Send(events::kTopic, std::to_string(booking.id), payload);
  } catch (const std::exception& ex) {
    LOG_WARNING() << "at-most-once: drop BookingCancelled event: " << ex.what();
  }
}

}  // namespace hotel_booking::booking_service
