cmake_minimum_required(VERSION 3.0.0)

# Set the project name
project(Project_V2)

# Set flags
# set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror")
# set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror")
set(CMAKE_C_FLAGS "-std=c11")
set(CMAKE_CXX_FLAGS "-std=c++11")

# Set debug
set(CMAKE_BUILD_TYPE debug)

# Pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Include Directory
include_directories("${PROJECT_SOURCE_DIR}/include")

# Client and server library
add_library(server_lib SHARED lib/server.c)
add_library(client_lib SHARED lib/client.c)

# Executables
add_executable(client src/client_main.c)
add_executable(server src/server_main.c)

# Tests
# add_executable(client_test test/client_test.c lib/client.c)

# Link libraries
target_link_libraries(client client_lib Threads::Threads)
target_link_libraries(server server_lib Threads::Threads)