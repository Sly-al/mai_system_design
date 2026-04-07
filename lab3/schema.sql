-- Схема

CREATE TABLE users (
    id BIGSERIAL PRIMARY KEY,
    login TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    CONSTRAINT users_login_len CHECK (char_length(login) >= 1)
);

CREATE TABLE hotels (
    id BIGSERIAL PRIMARY KEY,
    owner_user_id BIGINT NOT NULL REFERENCES users (id) ON DELETE RESTRICT,
    name TEXT NOT NULL,
    city TEXT NOT NULL,
    address TEXT NOT NULL,
    CONSTRAINT hotels_name_len CHECK (char_length(name) >= 1)
);

CREATE TABLE bookings (
    id BIGSERIAL PRIMARY KEY,
    user_id BIGINT NOT NULL REFERENCES users (id) ON DELETE RESTRICT,
    hotel_id BIGINT NOT NULL REFERENCES hotels (id) ON DELETE RESTRICT,
    check_in DATE NOT NULL,
    check_out DATE NOT NULL,
    status TEXT NOT NULL,
    CONSTRAINT bookings_status_ok CHECK (status IN ('active', 'cancelled')),
    CONSTRAINT bookings_dates_ok CHECK (check_in < check_out)
);

CREATE INDEX idx_hotels_owner_user_id ON hotels (owner_user_id);
CREATE INDEX idx_hotels_city ON hotels (city);

CREATE INDEX idx_bookings_user_id ON bookings (user_id);
CREATE INDEX idx_bookings_hotel_id ON bookings (hotel_id);
CREATE INDEX idx_bookings_hotel_active_dates ON bookings (hotel_id, check_in, check_out)
    WHERE status = 'active';

CREATE INDEX idx_users_name_search ON users (lower(first_name), lower(last_name));
