#pragma once

#include <userver/components/component_base.hpp>
#include <userver/kafka/producer_component.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include "store.hpp"

namespace hotel_booking::booking_service {

class BookingEventPublisher final : public components::ComponentBase {
public:
  static constexpr std::string_view kName = "booking-event-publisher";

  BookingEventPublisher(const components::ComponentConfig& config, const components::ComponentContext& context);

  void PublishCreated(const BookingRecord& booking) const;
  void PublishCancelled(const BookingRecord& booking) const;

private:
  const kafka::Producer& producer_;
};

}  // namespace hotel_booking::booking_service
