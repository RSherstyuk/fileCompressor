cmake_minimum_required(VERSION 3.10)
project(fileProcTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)

add_executable(fileProcTest
    src/main.cpp
    src/file_proc.cpp
    src/file_proc.hpp
    src/task_queue.hpp
    src/task_queue.cpp
)

target_include_directories(fileProcTest PRIVATE src)

target_link_libraries(fileProcTest PRIVATE ZLIB::ZLIB)


