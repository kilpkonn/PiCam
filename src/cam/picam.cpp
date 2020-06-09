//
// Created by tavo on 09.06.20.
//

#include "picam.h"

bool PiCam::run() {
    cv::Mat frame;
    while (cap.isOpened()) {
        cap >> frame;
    }
    return false;
}

PiCam::PiCam(const int& cameraIndex, const int& port) {
    this->cameraIndex = cameraIndex;
    this->port = port;
    this->cap = cv::VideoCapture(cameraIndex);
}

PiCam::~PiCam() {

}
