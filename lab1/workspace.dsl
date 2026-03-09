workspace {
    name "Online booking system"
    !identifiers hierarchical

    model {

        user = Person "Пользователь" "Человек, который ищет отели"
        admin = Person "Администратор отеля" "Человек, который добавляет отели и слоты для бронирования"

        paymentSystem = softwareSystem "Платежная система" "Внешняя система обработки платежей"
        moderationSystem = softwareSystem "Единая система отелей" "Внешняя система проверки отелей"

        bookingSystem = softwareSystem "Система бронирования" {
            -> paymentSystem "Биллинг" "HTTPS"
            -> moderationSystem "Запросы по валидации" "HTTPS"

            apiGateway = container "API Gateway / Load balancer" {
                technology "C++ / Userver"
                description "Единая точка входа, rate limiting, аутентификация запросов"
                tags "gateway"
            }

            bookingSvc = container "Сервис бронирования" {
                technology "C++ / Userver"
                description "Обработка бронирований"
                tags "service"
            }

            userSvc = container "Сервис пользователей" {
                technology "C++ / Userver"
                description "Управление учетными записями"
                tags "service"
            }

            hotelSvc = container "Сервис отелей" {
                technology "C++ / Userver"
                description "Управление каталогом отелей, номерами, ценами, наличием, circuit breaker для модерации"
                tags "service"
            }
            
            moderationSvc = container "Сервис модерации" {
                 technology "C++ / Userver"
                  description "Проверка юридических данных отеля, его местонахождения"
                  tags "service"
            }

            userDb = container "База данных пользователей" {
                technology "PostgreSQL"
                description "Хранит информацию о пользователях"
                tags "Database"
            }

            bookingDB = container "База данных бронирований" {
                technology "PostgreSQL"
                description "Хранит информацию о бронированиях, статусах, платежах"
                tags "Database"
            }

            hotelDB = container "База данных отелей" {
                technology "PostgreSQL"
                description "Хранит данные об отелях, номерах, ценах"
                tags "Database"
            }
            
            hotelCache = container "Кэш данных отелей" {
                technology "Redis"
                description "Хранит горячие данные об отелях"
                tags "Database"
            }
            
            broker = container "Брокер сообщений" {
                technology "Kafka"
                description "Шина данных для обмена событиями"
                tags "Broker"
            }
            
        }

        user -> bookingSystem.apiGateway "Бронирование отеля" "HTTPS"
        admin -> bookingSystem.apiGateway "Создание отеля" "HTTPS"

        bookingSystem.apiGateway -> bookingSystem.userSvc "Запросы пользователей, проверка JWT токенов" "HTTPS"
        bookingSystem.apiGateway -> bookingSystem.hotelSvc "Запросы отелей" "HTTPS"
        bookingSystem.apiGateway -> bookingSystem.bookingSvc "Запросы бронирований" "HTTPS"

        bookingSystem.userSvc -> bookingSystem.userDb "CRUD операции" "SQL"
        
        bookingSystem.hotelSvc -> bookingSystem.hotelDB "CRUD операции" "SQL"
        bookingSystem.hotelSvc -> bookingSystem.broker "Отправка или получение событий модерации создания отелей" "Kafka"
        bookingSystem.hotelSvc -> bookingSystem.hotelCache "Получение горячих данных из кэша" "Redis protocol"
        bookingSystem.hotelSvc -> bookingSystem.bookingSvc "Получение данных s2s по бронированию или по отелям" "gRPC"
        
        bookingSystem.bookingSvc -> bookingSystem.bookingDB "CRUD операции" "SQL"
        bookingSystem.bookingSvc -> paymentSystem "Вызов платежного API" "HTTPS"

        bookingSystem.broker -> bookingSystem.moderationSvc "Обработка событий по модерации создания отелей" "Kafka"
        bookingSystem.moderationSvc -> moderationSystem "Запросы валидации корректности данных по отелю" "HTTPS"
        
        deploymentEnvironment "Production" {
        deploymentNode "Cloud" {
            deploymentNode "Kubernetes cluster" {
                technology "Kubernetes"

                deploymentNode "Ingress / Load balancer" {
                    technology "Nginx / L7 LB"
                    description "Публичная точка входа в кластер"
                    apiGatewayInstance = containerInstance bookingSystem.apiGateway
                }

                deploymentNode "Application node" {
                    technology "C++ / Userver services"
                    description "Под каждый сервис поднимается набор pod-ов"
                    instances 3

                    containerInstance bookingSystem.bookingSvc
                    containerInstance bookingSystem.userSvc
                    containerInstance bookingSystem.hotelSvc
                    containerInstance bookingSystem.moderationSvc
                }

                deploymentNode "PostgreSQL cluster" {
                    technology "PostgreSQL HA"
                    description "Кластер БД пользователей, бронирований и отелей"

                    containerInstance bookingSystem.userDb
                    containerInstance bookingSystem.bookingDB
                    containerInstance bookingSystem.hotelDB
                }

                deploymentNode "Redis cluster" {
                    technology "Redis"
                    description "Кластер кэша данных отелей"
                    containerInstance bookingSystem.hotelCache
                }

                deploymentNode "Kafka cluster" {
                    technology "Kafka"
                    description "Кластер брокера сообщений"
                    containerInstance bookingSystem.broker
                }
            }
        }
    }
    }

    views {
        styles {
            element "Database" {
                shape cylinder
            }
        }
        
        styles {
            element "Broker" {
                shape pipe
            }
        }
    
        systemContext bookingSystem "Context" {
            include *
            autoLayout tb
        }
        
        container bookingSystem "C2" {
            include *
            autoLayout tb
        }
        
        dynamic bookingSystem "view1" "Выполнить бронирование номера" {
            autoLayout tb

            user -> bookingSystem.apiGateway "Отправить запрос на бронирование номера"
            bookingSystem.apiGateway -> bookingSystem.bookingSvc "Передать запрос бронирования"

            bookingSystem.bookingSvc -> bookingSystem.hotelSvc "Проверить доступность номера"
            bookingSystem.bookingSvc -> paymentSystem "Провести транзакцию оплаты"
            bookingSystem.bookingSvc -> bookingSystem.bookingDB "Сохранить бронь с результатом оплаты"

            bookingSystem.bookingSvc -> bookingSystem.apiGateway "Вернуть результат бронирования"
            bookingSystem.apiGateway -> user "Отправить пользователю подтверждение бронирования"
        }
        
        dynamic bookingSystem "view2" "Создать отель" {
            autoLayout tb

            admin -> bookingSystem.apiGateway "Отправить запрос на создание отеля"
            bookingSystem.apiGateway -> bookingSystem.hotelSvc "Передать запрос создания отеля"

            bookingSystem.hotelSvc -> bookingSystem.hotelDB "Создать отель со статусом"
            bookingSystem.hotelSvc -> bookingSystem.broker "Опубликовать событие HotelCreated"
            bookingSystem.hotelSvc -> bookingSystem.apiGateway "Вернуть администратору ID отеля и статус PENDING_MODERATION"


            bookingSystem.broker -> bookingSystem.moderationSvc "Обработать событие HotelCreated"
            bookingSystem.moderationSvc -> moderationSystem "Проверить корректность данных отеля"

            moderationSystem -> bookingSystem.moderationSvc "Вернуть результат модерации"
            bookingSystem.moderationSvc -> bookingSystem.broker "Опубликовать событие HotelModerationCompleted"

            bookingSystem.broker -> bookingSystem.hotelSvc "Обработать событие HotelModerationCompleted"
            bookingSystem.hotelSvc -> bookingSystem.hotelDB "Обновить статус отеля на APPROVED"

            bookingSystem.apiGateway -> admin "Отправить результат создания отеля (ожидает модерации)"
        }

        deployment bookingSystem "Production" {
            description "Deployment Системы бронирования в окружении Production"
            include *
            autoLayout tb
        }

        themes default
    }
    

}