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
    mjpegWriter.write(frame);
    //frame.release();
    mjpegWriter.start();

    while (this->cap.isOpened()) {
        this->cap >> frame;
        mjpegWriter.write(frame);
        //frame.release();
    }
    mjpegWriter.stop();

    return false;
}

PiCam::PiCam(const int& cameraIndex, const int& port) {
    this->cameraIndex = cameraIndex;
    this->port = port;
    this->cap = cv::VideoCapture(cameraIndex);

    mjpegWriter = MJPEGWriter(port);
}

PiCam::~PiCam() = default;
