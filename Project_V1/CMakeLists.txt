cmake_minimum_required(VERSION 3.0.0)

# Set the project name
project(Project_V1)

# Set flags
# set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
# set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_C_FLAGS "-std=c11")
set(CMAKE_CXX_FLAGS "-std=c++11")

# Executables
add_executable(client src/client.c include/client.h)
add_executable(server src/server.c include/server.h)