//
// Created by tavo on 09.06.20.
//

#include "picam.h"

bool PiCam::run() {
    cv::Mat frame;
    // cap = cv::VideoCapture(0)
    if (!cap.isOpened()) {
        std::cout << "Unable to connect to camera!" << std::endl;
        return false;
    } else {
        cap.set(CAP_PROP_FRAME_WIDTH, 1920);
        cap.set(CAP_PROP_FRAME_HEIGHT, 1080);
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

PiCam::PiCam(const int &cameraIndex, const int &port) :
        cameraIndex(cameraIndex),
        port(port),
        cap(cameraIndex),
        mjpegWriter(port) {}

PiCam::~PiCam() = default;
