cmake_minimum_required(VERSION 3.1)

project(PiCam)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Where to find CMake modules and OpenCV
SET(OpenCV_DIR /opt/opencv-4.3.0/lib/cmake/opencv4)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


find_package(OpenCV REQUIRED)
set(OpenCV_FOUND 1)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(pi_cam src/main.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(pi_cam ${OpenCV_LIBS})