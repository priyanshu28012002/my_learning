cmake_minimum_required(VERSION 3.12)

# Set the project name
project(LockFreeBuffer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler-specific extensions

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the executable
add_executable(LockFreeBuffer src/main.cpp src/threadPool.cpp)

# Link any required libraries (if applicable)
# target_link_libraries(LockFreeBuffer <library_name>)
