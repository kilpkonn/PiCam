//
// Created by tavo on 09.06.20.
//

#include "picam.h"

bool PiCam::run() {
    cv::Mat frame;
    std::cout << cap.getBackendName() << std:: endl;
    if (!cap.isOpened()) {
        std::cout << "Unable to connect to camera!" << std::endl;
        return false;
    }
    mjpegWriter.write(frame);
    //frame.release();
    mjpegWriter.start();

    while (cap.isOpened()) {
        cap >> frame;
        mjpegWriter.write(frame);
        //frame.release();
    }
    mjpegWriter.stop();

    return false;
}

PiCam::PiCam(const int& cameraIndex, const int& port) {
    this->cameraIndex = cameraIndex;
    this->port = port;
    cap = cv::VideoCapture(0);

    mjpegWriter = MJPEGWriter(port);
}

PiCam::~PiCam() {
}
