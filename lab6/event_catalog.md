# Каталог

Топик: hotel-booking-events
Формат: JSON  
Гарантия доставки: at-most-once


Пример заполнения события для bookingCreated

```json
{
    "event_id": "123456789",
    "event_type": "BookingCreated",
    "occurred_at": "1716140000",
    "payload": {
        "id": 201,
        "user_id": 101,
        "hotel_id": 1,
        "check_in": "2026-04-01",
        "check_out": "2026-04-05",
        "status": "active"
    }
}
```

Пример заполнения события для bookingCancelled

```json
{
    "event_id": "123456789",
    "event_type": "BookingCancelled",
    "occurred_at": "1716140000",
    "payload": {
        "id": 201,
        "user_id": 101,
        "hotel_id": 1,
        "status": "cancelled"
    }
}
```
