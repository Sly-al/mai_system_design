# Лабораторная №4 — MongoDB

Рокотянский Александр, группа М8О‑102СВ‑25.

## 1. Задание

Вариант 13. Система бронирования

В рамках задания переделал работу с hotel-service, он теперь работает с mongoDB.

## Сервисы

| Сервис              | Порт | Назначение                                     |
| ------------------- | ---- | ---------------------------------------------- |
| **postgres**        | 5432 | База данных|
| **mongo**           | 27017 | База данных|
| **user-service**    | 8081 | Регистрация, логин, JWT, поиск пользователей   |
| **hotel-service**   | 8082 | Список отелей, поиск по городу, создание отеля |
| **booking-service** | 8083 | Создание брони, мои брони, отмена брони        |

## Запуск

```bash
cd lab4
docker compose build
docker compose up -d
```

Проверка API:

```bash
chmod +x tests/smoke.sh
./tests/smoke.sh
```

## Файлы по заданию

| Файл | Содержание |
|------|------------|
| `schema_design.md` | Модель коллекций |
| `data.json` | Пример структуры тестовых данных |
| `queries.md` | Примеры CRUD и агрегации в mongosh |
| `validation.js` | Валидация схемы|

## Ручные запросы к Mongo

```bash
docker compose exec mongo mongo --host 127.0.0.1 hotel_booking --eval 'db.hotels.count()'
mongosh mongodb://127.0.0.1:27017/hotel_booking
mongosh mongodb://127.0.0.1:27017/hotel_booking validation.js
```
