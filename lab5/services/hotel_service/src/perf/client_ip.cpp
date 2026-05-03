#include "client_ip.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace hotel_booking::hotel_service {

namespace {

void TrimInPlace(std::string& s) {
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
    s.erase(s.begin());
  }
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
    s.pop_back();
  }
}

}  // namespace

std::string ClientIp(const server::http::HttpRequest& request) {
  auto xf = std::string{request.GetHeader("X-Forwarded-For")};
  if (!xf.empty()) {
    const auto comma = xf.find(',');
    auto part = comma == std::string::npos ? xf : xf.substr(0, comma);
    TrimInPlace(part);
    if (!part.empty()) {
      return part;
    }
  }
  const auto rip = std::string{request.GetHeader("X-Real-IP")};
  if (!rip.empty()) {
    return rip;
  }
  return "unknown";
}

}  // namespace hotel_booking::hotel_service
