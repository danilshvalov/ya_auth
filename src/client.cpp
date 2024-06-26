#include <ya_auth/client.hpp>

#include <fmt/format.h>
#include <userver/clients/http/client.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/http/header_map.hpp>
#include <userver/http/url.hpp>
#include <userver/storages/secdist/secdist.hpp>

namespace ya_auth {

namespace {

namespace http = userver::http;
namespace json = userver::formats::json;

}  // namespace

Client::Client(
    userver::clients::http::Client& http_client, const ClientConfig& config
)
    : http_client_(http_client), config_(config) {}

UserInfo Client::FetchUserInfo(
    const userver::server::auth::UserAuthInfo::Ticket& token
) const {
  auto response = SendRequest(token);
  response->raise_for_status();
  return json::FromString(response->body()).As<UserInfo>();
}

std::shared_ptr<userver::clients::http::Response> Client::SendRequest(
    const userver::server::auth::UserAuthInfo::Ticket& token
) const {
  http::headers::HeaderMap headers = {
      {"Authorization", fmt::format("OAuth {}", token.GetUnderlying())},
  };

  return http_client_.CreateRequest()
      .get(fmt::format("{}/{}", config_.ya_auth_url, "info"))
      .headers(headers)
      .retry(config_.retries)
      .timeout(config_.timeout)
      .perform();
}

}  // namespace ya_auth
