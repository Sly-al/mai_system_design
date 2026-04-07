#pragma once

#include <string>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/server/middlewares/configuration.hpp>
#include <userver/server/middlewares/http_middleware_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <auth/undef_macros.hpp>

namespace auth {

class BearerJwtMiddleware final : public server::middlewares::HttpMiddlewareBase {
public:
  explicit BearerJwtMiddleware(std::string secret);

private:
  void HandleRequest(server::http::HttpRequest& request, server::request::RequestContext& context) const override;

  std::string secret_;
};

class BearerJwtMiddlewareFactory final : public server::middlewares::HttpMiddlewareFactoryBase {
public:
  static constexpr std::string_view kName{"jwt-bearer-middleware"};

  BearerJwtMiddlewareFactory(const components::ComponentConfig& config, const components::ComponentContext& context);

  static yaml_config::Schema GetStaticConfigSchema();

private:
  std::unique_ptr<server::middlewares::HttpMiddlewareBase> Create(
      const server::handlers::HttpHandlerBase& handler,
      yaml_config::YamlConfig middleware_config
  ) const override;

  std::string secret_;
};

class BearerJwtHandlerPipelineBuilder final : public server::middlewares::HandlerPipelineBuilder {
public:
  static constexpr std::string_view kName{"jwt-bearer-pipeline"};

  using HandlerPipelineBuilder::HandlerPipelineBuilder;

  server::middlewares::MiddlewaresList BuildPipeline(server::middlewares::MiddlewaresList server_pipeline
  ) const override;
};

}  // namespace auth
