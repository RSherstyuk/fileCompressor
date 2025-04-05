#pragma once
#include <filesystem>

class FileProc {
public:
  FileProc(const std::filesystem::path& input_file,const std::filesystem::path& output_file);
  ~FileProc();
  void compress_file();

private:
  std::filesystem::path output_file_;
  std::filesystem::path input_file_;
};