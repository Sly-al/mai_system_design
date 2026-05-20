# Лабораторная работа №6 Event-Driven архитектура

Рокотянский Александр, группа М8О‑102СВ‑25.

Вариант 13: система онлайн-бронирования отелей.

## Сервисы

| Сервис | Порт | Kafka |
|--------|------|-------|
| **user-service** | 8081 | — |
| **hotel-service** | 8082 | вытсупает в роль consumer |
| **booking-service** | 8083 | выступает в роли producer |

booking-service публикует события в топик `hotel-booking-events`, а hotel-service потребляет эти события. Гарантия доставки используется при этом at-most-once.


## Запуск

```bash
cd lab6
docker compose build
docker compose up -d
```


## Тесты

```bash
chmod +x tests/*.sh
./tests/smoke.sh
./tests/kafka_events.sh
```