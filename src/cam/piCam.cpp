//
// Created by tavo on 09.06.20.
//

#include "piCam.h"

#include "graphics.h"

bool picam::PiCam::run() {
    faceDetector.loadClassifiers();

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

        //cv::cvtColor(frame, frame, COLOR_BGR2RGB);
        faceDetector.detectFaces(frame);
        draw(frame);

        if (mjpegWriter != nullptr) {
            mjpegWriter->write(frame);
        }
    }
    return false;
}

void picam::PiCam::draw(Mat &img) {
    std::vector<Face> faces = faceDetector.predictFaces();

    std::vector<Rect> highlights;
    highlights.reserve(faces.size());
    std::transform(faces.begin(), faces.end(), std::back_inserter(highlights), [](const Face &f) { return f.bounds; });
    if (isBlur) {
        img = graphics::blurBackground(img, highlights, 70);
    }

    if (isGrayscale) {
        img = graphics::grayscaleBackground(img, highlights, 50);
    }

    img = graphics::drawRectangles(img, highlights);
}

picam::PiCam::PiCam(const int &cameraIndex, const int &port) :
        cameraIndex(cameraIndex),
        port(port),
        cap(cameraIndex),
        faceDetector(frameWidth, frameHeight) {
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

void picam::PiCam::setBlur(const bool &blur) {
    this->isBlur = blur;
}

void picam::PiCam::setGrayscale(const bool &grayscale) {
    this->isGrayscale = grayscale;
}

void picam::PiCam::setFrameSize(const int& width, const int& height) {
    frameWidth = width;
    frameHeight = height;
    faceDetector.setFrameSize(width, height);

    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
    }
}
