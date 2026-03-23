#!/usr/bin/env bash
set -euo pipefail
U="${USER_SERVICE:-http://127.0.0.1:8081}"
H="${HOTEL_SERVICE:-http://127.0.0.1:8082}"
B="${BOOKING_SERVICE:-http://127.0.0.1:8083}"

curl -sf "$U/health" | grep -q '"service":"user"'
curl -sf "$H/health" | grep -q '"service":"hotel"'
curl -sf "$B/health" | grep -q '"service":"booking"'

curl -sf -X POST "$U/v1/users" \
  -H 'Content-Type: application/json' \
  -d '{"login":"alice","password":"secret","first_name":"Ali","last_name":"Ce"}' | grep -q '"id"'

code=$(curl -s -o /dev/null -w '%{http_code}' "$U/v1/users/by-login/nobody")
test "$code" = "404"

curl -sf "$U/v1/users/search?first_name=al&last_name=c" | grep -q '"users"'

TOKEN=$(curl -sf -X POST "$U/v1/auth/login" \
  -H 'Content-Type: application/json' \
  -d '{"login":"alice","password":"secret"}' | python3 -c "import sys,json; print(json.load(sys.stdin)['access_token'])")

code=$(curl -s -o /dev/null -w '%{http_code}' -X POST "$H/v1/hotels" \
  -H 'Content-Type: application/json' -d '{"name":"Grand","city":"Moscow","address":"Tverskaya 1"}')
test "$code" = "401"

curl -sf -X POST "$H/v1/hotels" \
  -H "Authorization: Bearer $TOKEN" \
  -H 'Content-Type: application/json' \
  -d '{"name":"Grand","city":"Moscow","address":"Tverskaya 1"}' | grep -q '"id"'

HID=$(curl -sf "$H/v1/hotels/by-city?city=Moscow" | python3 -c "import sys,json; d=json.load(sys.stdin); print(d['hotels'][0]['id'])")

curl -sf -X POST "$B/v1/bookings" \
  -H "Authorization: Bearer $TOKEN" \
  -H 'Content-Type: application/json' \
  -d "{\"hotel_id\":$HID,\"check_in\":\"2026-04-01\",\"check_out\":\"2026-04-05\"}" | grep -q '"status":"active"'

curl -sf "$B/v1/users/me/bookings" -H "Authorization: Bearer $TOKEN" | grep -q '"bookings"'

BID=$(curl -sf "$B/v1/users/me/bookings" -H "Authorization: Bearer $TOKEN" | python3 -c "import sys,json; d=json.load(sys.stdin); print(d['bookings'][0]['id'])")

curl -sf -X DELETE "$B/v1/bookings/$BID" -H "Authorization: Bearer $TOKEN" | grep -q '"cancelled"'

echo OK
