cmake_minimum_required(VERSION 3.10)
project(coreapp)

set(CMAKE_CXX_STANDARD 14)

find_package(jsoncpp REQUIRED)
find_package(serial REQUIRED)

add_subdirectory(common)

add_executable(coreapp main.cpp)

target_link_libraries(coreapp
    PRIVATE
    common
    jsoncpp_lib
    serial
)