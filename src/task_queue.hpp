#pragma once
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <queue>

class TaskQueue {
public:
  void push(const std::filesystem::path &file_path);
  bool try_pop(std::filesystem::path &file_path);
  void done();
  bool is_done() const;

private:
  std::queue<std::filesystem::path> queue_;
  mutable std::mutex mutex_;
  std::condition_variable cv_;
  bool is_done_ = false;
};