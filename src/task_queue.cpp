#include "task_queue.hpp"

void TaskQueue::push(const std::filesystem::path &file_path) {
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(file_path);
  cv_.notify_one();
}

bool TaskQueue::try_pop(std::filesystem::path &file_path) {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_.wait(lock, [this]() { return !queue_.empty() || is_done_; });

  if (queue_.empty())
    return false;

  file_path = queue_.front();
  queue_.pop();
  return true;
}

void TaskQueue::done() {
  std::lock_guard<std::mutex> lock(mutex_);
  is_done_ = true;
  cv_.notify_all();
}

bool TaskQueue::is_done() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return is_done_ && queue_.empty();
}