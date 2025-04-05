#include "zlib.h"
#include <filesystem>
#include <iostream>
#include "file_proc.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_directory>" << std::endl;
        return 1;
    }

    fs::path input_file = argv[1];
    fs::path output_dir = argv[2];

    FileProc proc(input_file, output_dir);
    proc.compress_file();

    return 0;
}