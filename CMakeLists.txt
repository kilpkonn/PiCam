cmake_minimum_required(VERSION 3.5)

project(PiCam)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Where to find CMake modules and OpenCV
set(OpenCV_DIR "E:/OpenCV/opencv/build/x64/vc15/lib")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


find_package(OpenCV REQUIRED)
set(OpenCV_FOUND 1)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(pi_cam src/main.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(pi_cam ${OpenCV_LIBS})