cmake_minimum_required(VERSION 3.21)
project(hotel_booking CXX)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

find_package(userver COMPONENTS core postgresql QUIET)
if(NOT userver_FOUND)
  set(_userver_vendor_dir "${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver")
  if(EXISTS "${_userver_vendor_dir}/CMakeLists.txt")
    download_userver(TRY_DIR "${_userver_vendor_dir}" VERSION 2.8)
  else()
    download_userver(VERSION 2.8)
  endif()
endif()

userver_setup_environment()

add_library(
  auth STATIC
  libs/auth/src/jwt.cpp
  libs/auth/src/password.cpp
  libs/auth/src/bearer_jwt_middleware.cpp
)
target_include_directories(auth PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libs/auth/include")
target_link_libraries(auth PUBLIC userver::core)

add_library(hotel_booking_http INTERFACE)
target_include_directories(hotel_booking_http INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/http_support/include")
target_link_libraries(hotel_booking_http INTERFACE auth userver::core userver::postgresql)

function(hotel_booking_add_http_service target_name service_path service_tag)
  set(_objs ${target_name}_objs)
  file(
    GLOB_RECURSE _service_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/src/*/*.cpp"
  )
  add_library(${_objs} OBJECT ${_service_sources})
  set(_incdirs
      "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/src/handlers"
      "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/src/codec"
      "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/src/store")
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/src/integration")
    list(APPEND _incdirs "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/src/integration")
  endif()
  target_include_directories(${_objs} PUBLIC ${_incdirs})
  target_link_libraries(${_objs} PUBLIC hotel_booking_http)
  target_compile_definitions(${_objs} PRIVATE SERVICE_TAG=${service_tag})

  add_executable(${target_name} "${CMAKE_CURRENT_SOURCE_DIR}/${service_path}/main.cpp")
  target_link_libraries(${target_name} PRIVATE ${_objs})
endfunction()

hotel_booking_add_http_service(user-service services/user_service user)
hotel_booking_add_http_service(hotel-service services/hotel_service hotel)
hotel_booking_add_http_service(booking-service services/booking_service booking)
