#include <auth/bearer_jwt_middleware.hpp>
#include <auth/jwt.hpp>
#include <auth/request_context.hpp>

#include <userver/components/component_base.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/server/http/http_response.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/http/content_type.hpp>
#include <userver/utils/text_light.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace auth {

BearerJwtMiddleware::BearerJwtMiddleware(std::string secret) : secret_(std::move(secret)) {}

void BearerJwtMiddleware::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext& context
                                      ) const {
  const auto& auth_header = request.GetHeader(http::headers::kAuthorization);
  if (auth_header.empty() || !utils::text::StartsWith(auth_header, "Bearer ")) {
    auto& resp = request.GetHttpResponse();
    resp.SetStatus(server::http::HttpStatus::kUnauthorized);
    resp.SetContentType(http::content_type::kApplicationJson);
    resp.SetData(R"({"error":"missing_or_invalid_authorization"})");
    return;
  }
  const std::string_view tok_sv{auth_header.data() + 7, auth_header.size() - 7};
  const auto claims = JwtVerifyHs256(secret_, tok_sv);
  if (!claims.valid || !claims.sub) {
    auto& resp = request.GetHttpResponse();
    resp.SetStatus(server::http::HttpStatus::kUnauthorized);
    resp.SetContentType(http::content_type::kApplicationJson);
    resp.SetData(R"({"error":"invalid_token"})");
    return;
  }
  context.SetData(kPrincipalIdKey, *claims.sub);
  Next(request, context);
}

BearerJwtMiddlewareFactory::BearerJwtMiddlewareFactory(const components::ComponentConfig& config,
                                                       const components::ComponentContext& context)
    : HttpMiddlewareFactoryBase(config, context), secret_(config["secret"].As<std::string>()) {}

yaml_config::Schema BearerJwtMiddlewareFactory::GetStaticConfigSchema() {
  return yaml_config::MergeSchemas<components::ComponentBase>(R"(
type: object
description: Bearer JWT (HS256) verification middleware factory
additionalProperties: false
properties:
  secret:
    type: string
    description: HMAC key; must match the signing secret used when tokens are issued
)");
}

std::unique_ptr<server::middlewares::HttpMiddlewareBase> BearerJwtMiddlewareFactory::Create(
    const server::handlers::HttpHandlerBase&,
    yaml_config::YamlConfig
) const {
  return std::make_unique<BearerJwtMiddleware>(secret_);
}

server::middlewares::MiddlewaresList BearerJwtHandlerPipelineBuilder::BuildPipeline(
    server::middlewares::MiddlewaresList server_pipeline
) const {
  server_pipeline.emplace_back(std::string{BearerJwtMiddlewareFactory::kName});
  return server_pipeline;
}

}  // namespace auth
