project (program3)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(lib vehiclecounter.h vehiclecounter.cpp videofileinfo.h videofileinfo.cpp imageprocessor.h imageprocessor.cpp)

# create create individual projects
add_executable(program3 program3.cpp)
target_link_libraries(program3 ${OpenCV_LIBS} lib)