cmake_minimum_required(VERSION 3.25)
project(lunam LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

add_executable(${PROJECT_NAME} src/main.cpp src/lunam/Lexer.cpp src/lunam/Token.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(thirdparty)