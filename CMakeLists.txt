cmake_minimum_required(VERSION 3.15)
project(linalg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(linalg
        source/matrix.cpp
        source/arithmetic.cpp
        source/gauss.cpp
        source/functions.cpp
        source/print.cpp
)

# Заголовки лежат в include/linalg, поэтому подключаем саму папку include.
target_include_directories(linalg PUBLIC include)
