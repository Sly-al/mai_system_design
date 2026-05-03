#!/usr/bin/env bash
set -euo pipefail
H="${HOTEL_SERVICE:-http://127.0.0.1:8082}"

pass=0
blocked=0
for i in $(seq 1 105); do
  code=$(curl -s -o /dev/null -w '%{http_code}' "$H/v1/hotels/by-city?city=Moscow")
  if [[ "$code" == "200" ]]; then
    pass=$((pass + 1))
  elif [[ "$code" == "429" ]]; then
    blocked=$((blocked + 1))
  else
    echo "unexpected HTTP $code on request $i" >&2
    exit 1
  fi
done

if [[ "$pass" -lt 100 ]] || [[ "$blocked" -lt 1 ]]; then
  echo "expected at least 100 successes and 1 rate-limit response, got ok=$pass blocked=$blocked" >&2
  exit 1
fi

echo "rate_limit OK (200 x $pass, 429 x $blocked)"
