# Socket Project - Version 2

This project is written in C using unix libraries. Please make sure the following tools are installed on your system:
- Linux or Mac OS X
- CMake (version > 3.0.0)
- GCC

## To use this program
1. Create a directory `build` under the root directory of this project.
2. In the build directory, run `cmake ..`
3. In the build directory, run `make`.  Two executable files will be created under the build directory.  One is for client `client`, one is for server `server`.
4. Start the server.
5. Start the client. The server can handle up to 3 clients at a time.