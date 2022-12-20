cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 20)

add_executable(code
        main.cpp
        databaseplus.hpp
        user.hpp
        user.cpp
        error.hpp
        Tokenscanner.hpp
        )
