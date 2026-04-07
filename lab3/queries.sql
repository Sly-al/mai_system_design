-- Создание пользователя
INSERT INTO users (login, password_hash, first_name, last_name)
VALUES ($1, $2, $3, $4)
RETURNING id, login, first_name, last_name;

-- Поиск по логину
SELECT id, login, password_hash, first_name, last_name
FROM users
WHERE login = $1;

-- Поиск по маске имени и фамилии
SELECT id, login, first_name, last_name
FROM users
WHERE ($1 = '' OR first_name ILIKE '%' || $1 || '%')
  AND ($2 = '' OR last_name ILIKE '%' || $2 || '%')
ORDER BY id;

-- Получение пользака по id
SELECT id, login, first_name, last_name
FROM users
WHERE id = $1;

-- Создание отеля
INSERT INTO hotels (owner_user_id, name, city, address)
VALUES ($1, $2, $3, $4)
RETURNING id, name, city, address, owner_user_id;

-- Список отелей
SELECT id, name, city, address, owner_user_id
FROM hotels
ORDER BY id;

-- Отели по городу
SELECT id, name, city, address, owner_user_id
FROM hotels
WHERE city = $1
ORDER BY id;

-- Получение отеля по id
SELECT id, name, city, address, owner_user_id
FROM hotels
WHERE id = $1;

-- Проверка пересечения активных броней
SELECT EXISTS (
    SELECT 1
    FROM bookings
    WHERE hotel_id = $1
      AND status = 'active'
      AND NOT (check_out <= $2::date OR check_in >= $3::date)
) AS overlaps;

-- Создание бронирования
INSERT INTO bookings (user_id, hotel_id, check_in, check_out, status)
VALUES ($1, $2, $3::date, $4::date, 'active')
RETURNING id, user_id, hotel_id, check_in::text, check_out::text, status;

-- Бронирования пользователя
SELECT id, user_id, hotel_id, check_in::text, check_out::text, status
FROM bookings
WHERE user_id = $1
ORDER BY id;

-- Получение брони по id
SELECT id, user_id, hotel_id, check_in::text, check_out::text, status
FROM bookings
WHERE id = $1;

-- Отмена брони
UPDATE bookings
SET status = 'cancelled'
WHERE id = $1 AND user_id = $2 AND status = 'active'
RETURNING id;
