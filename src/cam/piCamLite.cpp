#include "piCamLite.h"

picam::PiCamLite::PiCamLite(const int &cameraIndex, const int &port) :
        PiCam(cameraIndex, port) {
}

bool picam::PiCamLite::run() {
    cv::Mat frame;
    if (!cap.isOpened()) {
        std::cout << "Unable to connect to camera!" << std::endl;
        return false;
    } else {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
    }

    while (cap.isOpened()) {
        cap >> frame;

        rotateStand();

        if (mjpegWriter != nullptr) {
            mjpegWriter->write(frame);
        }
    }
    return false;
}

