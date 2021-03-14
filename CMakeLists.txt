cmake_minimum_required(VERSION 3.17)
project(rcon_client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(rcon_client
        src/rcon_client.cpp
        src/rcon_client.h)
