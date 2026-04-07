# syntax=docker/dockerfile:1

FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest AS builder

WORKDIR /app

COPY CMakeLists.txt ./
COPY cmake ./cmake/
COPY libs/auth ./libs/auth/
COPY libs/http_support ./libs/http_support/
COPY services ./services/

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja \
    && cmake --build build -j"$(nproc)"

FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest AS user-service
COPY --from=builder /app/build/user-service /usr/local/bin/user-service
COPY services/user_service/configs/static_config.yaml /etc/hotel_booking/static_config.yaml
COPY services/user_service/configs/config_vars.yaml /etc/hotel_booking/config_vars.yaml
EXPOSE 8081
ENTRYPOINT ["/usr/local/bin/user-service", "--config", "/etc/hotel_booking/static_config.yaml", "--config_vars", "/etc/hotel_booking/config_vars.yaml"]

FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest AS hotel-service
COPY --from=builder /app/build/hotel-service /usr/local/bin/hotel-service
COPY services/hotel_service/configs/static_config.yaml /etc/hotel_booking/static_config.yaml
COPY services/hotel_service/configs/config_vars.yaml /etc/hotel_booking/config_vars.yaml
EXPOSE 8082
ENTRYPOINT ["/usr/local/bin/hotel-service", "--config", "/etc/hotel_booking/static_config.yaml", "--config_vars", "/etc/hotel_booking/config_vars.yaml"]

FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest AS booking-service
COPY --from=builder /app/build/booking-service /usr/local/bin/booking-service
COPY services/booking_service/configs/static_config.yaml /etc/hotel_booking/static_config.yaml
COPY services/booking_service/configs/config_vars.yaml /etc/hotel_booking/config_vars.yaml
EXPOSE 8083
ENTRYPOINT ["/usr/local/bin/booking-service", "--config", "/etc/hotel_booking/static_config.yaml", "--config_vars", "/etc/hotel_booking/config_vars.yaml"]
