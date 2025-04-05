#include "file_proc.hpp"
#include "zlib.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

FileProc::FileProc(const std::filesystem::path &input_file,
                   const std::filesystem::path &output_file)
    : output_file_(output_file), input_file_(input_file) {}

FileProc::~FileProc() {};

void FileProc::compress_file() {
  try {
    if (!fs::exists(input_file_)) {
      throw std::runtime_error("Input file does not exist");
    }

    if (!fs::exists(output_file_)) {
      fs::create_directories(output_file_);
    }

    std::ifstream input(input_file_, std::ios::binary);
    if (!input) {
      throw std::runtime_error("Failed to open input file");
    }

    std::vector<char> input_data((std::istreambuf_iterator<char>(input)),
                                 std::istreambuf_iterator<char>());

    uLongf compressed_size = compressBound(input_data.size());
    std::vector<Bytef> compressed_data(compressed_size);

    if (compress(compressed_data.data(), &compressed_size,
                 reinterpret_cast<Bytef *>(input_data.data()),
                 input_data.size()) != Z_OK) {
      throw std::runtime_error("Compression failed");
    }

    fs::path output_path = output_file_ / (input_file_.stem().string() + ".z");

    std::ofstream output(output_path, std::ios::binary);
    if (!output) {
      throw std::runtime_error("Failed to create output file");
    }

    output.write(reinterpret_cast<char *>(compressed_data.data()),
                 compressed_size);

    std::cout << "Successfully compressed: " << input_file_ << " -> "
              << output_path << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}