#!/usr/bin/env bash
set -euo pipefail
H="${HOTEL_SERVICE:-http://127.0.0.1:8082}"
B="${BOOKING_SERVICE:-http://127.0.0.1:8083}"
U="${USER_SERVICE:-http://127.0.0.1:8081}"

LOGIN="kafka_$(date +%s)$$"
curl -sf -X POST "$U/v1/users" \
  -H 'Content-Type: application/json' \
  -d "{\"login\":\"$LOGIN\",\"password\":\"secret\",\"first_name\":\"Ka\",\"last_name\":\"Fka\"}" >/dev/null

TOKEN=$(curl -sf -X POST "$U/v1/auth/login" \
  -H 'Content-Type: application/json' \
  -d "{\"login\":\"$LOGIN\",\"password\":\"secret\"}" | python3 -c "import sys,json; print(json.load(sys.stdin)['access_token'])")

HID=$(curl -sf "$H/v1/hotels/by-city?city=Moscow" | python3 -c "import sys,json; d=json.load(sys.stdin); print(d['hotels'][0]['id'])")

curl -sf -X POST "$B/v1/bookings" \
  -H "Authorization: Bearer $TOKEN" \
  -H 'Content-Type: application/json' \
  -d "{\"hotel_id\":$HID,\"check_in\":\"2026-05-01\",\"check_out\":\"2026-05-03\"}" >/dev/null

for _ in $(seq 1 40); do
  if curl -sf "$H/v1/internal/kafka/booking-events" | grep -q 'BookingCreated'; then
    echo OK
    exit 0
  fi
  sleep 0.5
done

echo "hotel-service did not receive BookingCreated via Kafka" >&2
exit 1
