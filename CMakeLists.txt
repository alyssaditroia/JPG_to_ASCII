cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME JPG_TO_ASCII)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(JPG_TO_ASCII ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

file(COPY resources/ DESTINATION ${CMAKE_BINARY_DIR}/resources)