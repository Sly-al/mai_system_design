-- Тестовые данные

INSERT INTO users (login, password_hash, first_name, last_name) VALUES
  ('u01', 'h01', 'Ivan', 'Petrov'),
  ('u02', 'h02', 'Maria', 'Sidorova'),
  ('u03', 'h03', 'Alex', 'Ivanov'),
  ('u04', 'h04', 'Elena', 'Kozlova'),
  ('u05', 'h05', 'Dmitry', 'Smirnov'),
  ('u06', 'h06', 'Olga', 'Volkova'),
  ('u07', 'h07', 'Sergey', 'Popov'),
  ('u08', 'h08', 'Anna', 'Lebedeva'),
  ('u09', 'h09', 'Pavel', 'Novikov'),
  ('u10', 'h10', 'Natalia', 'Morozova'),
  ('u11', 'h11', 'Victor', 'Fedorov'),
  ('u12', 'h12', 'Irina', 'Sokolova');

INSERT INTO hotels (owner_user_id, name, city, address) VALUES
  (1, 'Grand Moscow', 'Moscow', 'Tverskaya 1'),
  (2, 'Nevsky Inn', 'Saint Petersburg', 'Nevsky 10'),
  (3, 'Ural Lodge', 'Ekaterinburg', 'Lenina 5'),
  (1, 'Kremlin View', 'Moscow', 'Varvarka 3'),
  (4, 'Baikal Shore', 'Irkutsk', 'Gagarin 7'),
  (5, 'Volga Plaza', 'Kazan', 'Bauman 12'),
  (6, 'Siberian Star', 'Novosibirsk', 'Krasny 20'),
  (7, 'Golden Ring', 'Yaroslavl', 'Pervomayskaya 4'),
  (8, 'Black Sea', 'Sochi', 'Kurortny 9'),
  (9, 'Caspian', 'Astrakhan', 'Admiralteyskaya 2'),
  (10, 'Pacific', 'Vladivostok', 'Svetlanskaya 15'),
  (11, 'Arctic', 'Murmansk', 'Lenina 30');

INSERT INTO bookings (user_id, hotel_id, check_in, check_out, status) VALUES
  (1, 1, '2026-01-10', '2026-01-15', 'active'),
  (2, 2, '2026-02-01', '2026-02-05', 'cancelled'),
  (3, 3, '2026-03-01', '2026-03-07', 'active'),
  (4, 4, '2026-04-01', '2026-04-04', 'active'),
  (5, 5, '2026-05-10', '2026-05-12', 'active'),
  (6, 6, '2026-06-01', '2026-06-10', 'cancelled'),
  (7, 7, '2026-07-05', '2026-07-08', 'active'),
  (8, 8, '2026-08-01', '2026-08-06', 'active'),
  (9, 9, '2026-09-01', '2026-09-03', 'active'),
  (10, 10, '2026-10-01', '2026-10-05', 'active'),
  (11, 11, '2026-11-01', '2026-11-04', 'cancelled'),
  (12, 12, '2026-12-01', '2026-12-10', 'active');
