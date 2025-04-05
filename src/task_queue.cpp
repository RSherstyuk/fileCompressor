#pragma once
#include <queue>
#include <filesystem>
#include <mutex>
#include <condition_variable>


class TaskQueue {
public:
  TaskQueue();
  ~TaskQueue();
  void done();
  bool is_done() const;

private:
  std::queue<std::filesystem::path> queue_;
  mutable std::mutex mutex_;
  std::condition_variable cv_;
  bool is_done_ = false;
};