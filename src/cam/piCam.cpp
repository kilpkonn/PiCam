#include "piCam.h"

picam::PiCam::PiCam(const int &cameraIndex, const int &port) :
        cameraIndex(cameraIndex),
        port(port),
        currentStandLocation((minXServo + maxXServo) / 2, (minYServo + maxYServo) / 2),
        cap(cameraIndex),
        servoStand(17, 18) {
}

bool picam::PiCam::run() {
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

void picam::PiCam::startServer() {
    std::cout << "Starting server at port: " << port << std::endl;
    mjpegWriter = new MJPEGWriter(port);
    mjpegWriter->start();
    isServerRunning = true;
    std::cout << "Server is up!" << std::endl;
}

void picam::PiCam::stopServer() {
    std::cout << "Stopping server..." << std::endl;
    if (isServerRunning) {
        mjpegWriter->stop();
        delete mjpegWriter;
        mjpegWriter = nullptr;
        isServerRunning = false;
    }
    std::cout << "Server shut down!" << std::endl;
}


picam::PiCam::~PiCam() {
    stopServer();
}

void picam::PiCam::setFrameSize(const int &width, const int &height) {
    frameWidth = width;
    frameHeight = height;

    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
    }
}

void picam::PiCam::rotateStand() {
    currentStandLocation.x = std::clamp(currentStandLocation.x, minXServo, maxXServo);
    currentStandLocation.y = std::clamp(currentStandLocation.y, minYServo, maxYServo);

    servoStand.rotate(-currentStandLocation.x, currentStandLocation.y);
}
