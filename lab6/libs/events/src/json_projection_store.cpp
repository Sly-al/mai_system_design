#include <hotel_booking/events/json_projection_store.hpp>

#include <fstream>

#include <userver/formats/json/value_builder.hpp>
#include <userver/logging/log.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace hotel_booking::events {

JsonProjectionStore::JsonProjectionStore(std::string file_path) : file_path_(std::move(file_path)) { Load(); }

void JsonProjectionStore::Load() {
  std::ifstream in(file_path_);
  if (!in.good()) {
    formats::json::ValueBuilder b;
    b["events"] = formats::json::MakeArray();
    state_ = b.ExtractValue();
    return;
  }
  try {
    state_ = formats::json::FromString(std::string{std::istreambuf_iterator<char>(in), {}});
  } catch (const std::exception& ex) {
    LOG_WARNING() << "projection load failed, reset: " << ex.what();
    formats::json::ValueBuilder b;
    b["events"] = formats::json::MakeArray();
    state_ = b.ExtractValue();
  }
}

void JsonProjectionStore::Save() const {
  std::ofstream out(file_path_ + ".tmp", std::ios::trunc);
  out << formats::json::ToString(state_);
  out.close();
  std::rename((file_path_ + ".tmp").c_str(), file_path_.c_str());
}

void JsonProjectionStore::Append(const formats::json::Value& event) {
  std::lock_guard lock(mutex_);
  formats::json::ValueBuilder b(state_);
  b["events"].PushBack(event);
  state_ = b.ExtractValue();
  Save();
}

formats::json::Value JsonProjectionStore::Snapshot() const {
  std::lock_guard lock(mutex_);
  return state_;
}

}  // namespace hotel_booking::events
