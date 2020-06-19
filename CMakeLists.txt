cmake_minimum_required(VERSION 3.1)

project(PiCam)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

# Where to find CMake modules and OpenCV
SET(OpenCV_DIR /opt/opencv-4.3.0/lib/cmake/opencv4)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


find_package(OpenCV REQUIRED)
# set(OpenCV_FOUND 1)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB SOURCE_FILES
        src/main.cpp
        src/cam/*.cpp
        src/cam/*.h
        src/cam/server/*.h
        src/cam/server/*.cpp
        src/cam/detector/*.h
        src/cam/detector/*.cpp
        )
add_executable(pi_cam ${SOURCE_FILES})

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_objdetect)

# linking
target_link_libraries(pi_cam ${OpenCV_LIBS})