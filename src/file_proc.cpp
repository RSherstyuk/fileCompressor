#include "file_proc.hpp"
#include <fstream>
#include <iostream>
#include <zlib.h>

FileProcessor::FileProcessor(TaskQueue &queue,
                             const std::filesystem::path &output_dir)
    : queue_(queue), output_dir_(output_dir) {}

FileProcessor::~FileProcessor() {
  stop();
  wait();
}

void FileProcessor::start(size_t thread_count) {
  stop_flag_ = false;
  for (size_t i = 0; i < thread_count; ++i) {
    threads_.emplace_back(&FileProcessor::worker_thread, this);
  }
}

void FileProcessor::stop() {
  stop_flag_ = true;
  queue_.done();
}

void FileProcessor::wait() {
  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
  threads_.clear();
}

void FileProcessor::worker_thread() {
  std::filesystem::path file_path;

  while (!stop_flag_ && queue_.try_pop(file_path)) {
    try {
      std::ifstream input(file_path, std::ios::binary);
      if (!input)
        continue;

      std::vector<char> input_data((std::istreambuf_iterator<char>(input)),
                                   std::istreambuf_iterator<char>());

      uLongf compressed_size = compressBound(input_data.size());
      std::vector<Bytef> compressed_data(compressed_size);

      if (compress(compressed_data.data(), &compressed_size,
                   reinterpret_cast<Bytef *>(input_data.data()),
                   input_data.size()) != Z_OK) {
        std::cerr << "Compression failed for: " << file_path << std::endl;
        continue;
      }

      std::filesystem::path output_path =
          output_dir_ / (file_path.filename().string() + ".z");
      std::ofstream output(output_path, std::ios::binary);
      if (!output)
        continue;

      output.write(reinterpret_cast<char *>(compressed_data.data()),
                   compressed_size);
      std::cout << "Compressed: " << file_path << " -> " << output_path
                << std::endl;

    } catch (const std::exception &e) {
      std::cerr << "Error processing " << file_path << ": " << e.what()
                << std::endl;
    }
  }
}