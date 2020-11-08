cmake_minimum_required(VERSION 3.0.0)

# Set the project name
project(Project_V1)

# Set flags
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")

# Include Directory
include_directories("${PROJECT_SOURCE_DIR}/include")

# Client and server library
add_library(server_lib SHARED lib/server.c lib/chatRoom.c)
add_library(client_lib SHARED lib/client.c lib/chatRoom.c)

# Executables
add_executable(client src/client_main.c)
add_executable(server src/server_main.c)

# Tests
add_executable(server_test test/server_test.c include/server.h lib/server.c)

# Link libraries
target_link_libraries(client client_lib)
target_link_libraries(server server_lib)
target_link_libraries(server_test server_lib)