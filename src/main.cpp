#include "file_proc.hpp"
#include "task_queue.hpp"
#include <filesystem>
#include <iostream>
#include <thread>

namespace fs = std::filesystem;

void collect_files(const fs::path &dir_path, TaskQueue &queue) {
  try {
    for (const auto &entry : fs::recursive_directory_iterator(dir_path)) {
      if (entry.is_regular_file()) {
        queue.push(entry.path());
      }
    }
    queue.done();
  } catch (const std::exception &e) {
    std::cerr << "Error scanning directory: " << e.what() << std::endl;
    queue.done();
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
    return 1;
  }

  fs::path dir_path(argv[1]);
  if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
    std::cerr << "Invalid directory path: " << dir_path << std::endl;
    return 1;
  }

  TaskQueue queue;
  FileProcessor processor(queue, dir_path);

  std::thread collector(collect_files, dir_path, std::ref(queue));

  size_t thread_count = std::thread::hardware_concurrency();
  if (thread_count == 0)
    thread_count = 2;

  std::cout << "Starting compression with " << thread_count << " threads"
            << std::endl;
  processor.start(thread_count);

  collector.join();
  processor.wait();

  std::cout << "Compression completed" << std::endl;
  return 0;
}