//
// Created by tavo on 09.06.20.
//

#include "picam.h"

bool PiCam::run() {
    cv::Mat frame;
    // cap = cv::VideoCapture(0)
    if (!this->cap.isOpened()) {
        std::cout << "Unable to connect to camera!" << std::endl;
        return false;
    }
    this->mjpegWriter.write(frame);
    //frame.release();
    this->mjpegWriter.start();

    while (this->cap.isOpened()) {
        this->cap >> frame;
        this->mjpegWriter.write(frame);
        //frame.release();
    }
    this->mjpegWriter.stop();

    return false;
}

PiCam::PiCam(const int &cameraIndex, const int &port) :
        cameraIndex(cameraIndex),
        port(port),
        cap(cameraIndex),
        mjpegWriter(port) {}

PiCam::~PiCam() = default;
