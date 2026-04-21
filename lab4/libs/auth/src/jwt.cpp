#include <auth/jwt.hpp>

#include <ctime>
#include <vector>

#include <userver/crypto/base64.hpp>
#include <userver/crypto/hash.hpp>
#include <userver/formats/json.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace auth {

namespace {

std::string B64UrlEncode(std::string_view data) {
  return crypto::base64::Base64UrlEncode(data, crypto::base64::Pad::kWithout);
}

std::string B64UrlDecode(std::string_view data) {
  std::string padded(data);
  while (padded.size() % 4 != 0) padded.push_back('=');
  return crypto::base64::Base64UrlDecode(padded);
}

std::vector<std::string> SplitDot(std::string_view s) {
  std::vector<std::string> out;
  std::size_t start = 0;
  while (start <= s.size()) {
    auto pos = s.find('.', start);
    if (pos == std::string_view::npos) {
      out.emplace_back(s.substr(start));
      break;
    }
    out.emplace_back(s.substr(start, pos - start));
    start = pos + 1;
  }
  return out;
}

}  // namespace

std::string JwtSignHs256(std::string_view secret, int64_t sub, std::string_view login) {
  const std::string header_raw = R"({"alg":"HS256","typ":"JWT"})";
  formats::json::ValueBuilder pb;
  pb["sub"] = std::to_string(sub);
  pb["login"] = std::string(login);
  const auto now = std::time(nullptr);
  pb["iat"] = now;
  pb["exp"] = now + 86400 * 7;
  const auto payload = formats::json::ToString(pb.ExtractValue());
  const std::string h = B64UrlEncode(header_raw);
  const std::string p = B64UrlEncode(payload);
  const std::string signing_input = h + '.' + p;
  const auto sig_bin = crypto::hash::HmacSha256(secret, signing_input, crypto::hash::OutputEncoding::kBinary);
  const std::string sig = B64UrlEncode(sig_bin);
  return signing_input + '.' + sig;
}

JwtClaims JwtVerifyHs256(std::string_view secret, std::string_view token) {
  JwtClaims r;
  const auto parts = SplitDot(token);
  if (parts.size() != 3) return r;
  const std::string signing_input = parts[0] + '.' + parts[1];
  const auto expected_bin = crypto::hash::HmacSha256(secret, signing_input, crypto::hash::OutputEncoding::kBinary);
  const std::string expected_sig = B64UrlEncode(expected_bin);
  if (expected_sig != parts[2]) return r;
  std::string payload_json;
  try {
    payload_json = B64UrlDecode(parts[1]);
  } catch (...) {
    return r;
  }
  formats::json::Value j;
  try {
    j = formats::json::FromString(payload_json);
  } catch (...) {
    return r;
  }
  const auto now = std::time(nullptr);
  if (!j["exp"].IsMissing()) {
    try {
      if (j["exp"].As<int64_t>() < now) return r;
    } catch (...) {
      return r;
    }
  }
  if (j["sub"].IsMissing()) return r;
  std::string sub_s;
  try {
    if (j["sub"].IsString()) {
      sub_s = j["sub"].As<std::string>();
    } else {
      sub_s = std::to_string(j["sub"].As<int64_t>());
    }
  } catch (...) {
    return r;
  }
  try {
    r.sub = std::stoll(sub_s);
  } catch (...) {
    return r;
  }
  if (!j["login"].IsMissing() && j["login"].IsString()) {
    r.login = j["login"].As<std::string>();
  }
  r.valid = true;
  return r;
}

}  // namespace auth
