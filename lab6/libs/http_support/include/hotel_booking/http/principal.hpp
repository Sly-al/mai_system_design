#pragma once

#include <cstdint>
#include <optional>

#include <auth/request_context.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::http {

inline std::optional<int64_t> PrincipalIdOr500(server::http::HttpRequest& request,
                                                 const server::request::RequestContext& ctx) {
  const auto* id = ctx.GetDataOptional<int64_t>(auth::kPrincipalIdKey);
  if (!id) {
    request.GetHttpResponse().SetStatus(server::http::HttpStatus::kInternalServerError);
    return std::nullopt;
  }
  return *id;
}

}  // namespace hotel_booking::http
