cmake_minimum_required(VERSION 3.10)
project(coreapp)

set(CMAKE_CXX_STANDARD 20)

find_package(jsoncpp REQUIRED)
find_package(serial REQUIRED)

add_subdirectory(common)

add_executable(uart-mqtt-app main.cpp)

target_link_libraries(uart-mqtt-app
    PRIVATE
    common
    jsoncpp_lib
    serial
    paho-mqttpp3
    paho-mqtt3as
)