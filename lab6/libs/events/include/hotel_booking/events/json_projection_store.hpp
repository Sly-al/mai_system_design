#pragma once

#include <mutex>
#include <string>
#include <vector>

#include <userver/formats/json/value.hpp>

namespace hotel_booking::events {

// Простое JSON-хранилище read-проекции событий (CQRS read-model на hotel-service).
class JsonProjectionStore final {
public:
  explicit JsonProjectionStore(std::string file_path);

  void Append(const formats::json::Value& event);
  formats::json::Value Snapshot() const;

private:
  void Load();
  void Save() const;

  std::string file_path_;
  mutable std::mutex mutex_;
  formats::json::Value state_;
};

}  // namespace hotel_booking::events
