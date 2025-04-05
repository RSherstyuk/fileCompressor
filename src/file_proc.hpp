#pragma once
#include "task_queue.hpp"
#include <atomic>
#include <filesystem>
#include <thread>
#include <vector>

class FileProcessor {
public:
  FileProcessor(TaskQueue &queue, const std::filesystem::path &output_dir);
  ~FileProcessor();

  void start(size_t thread_count);
  void stop();
  void wait();

private:
  void worker_thread();

  TaskQueue &queue_;
  std::filesystem::path output_dir_;
  std::vector<std::thread> threads_;
  std::atomic<bool> stop_flag_{false};
};