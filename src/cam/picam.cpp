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
        //frame.release();
    }
    return false;
}

void PiCam::draw(Mat &img) {
    std::vector<Face> faces = faceDetector.predictFaces();

    for (const auto &r : faces) {
        cv::Point center;
        cv::Scalar color = cv::Scalar(0, 0, 255); // Color for Drawing tool
        int radius;

        double aspect_ratio = (double) r.bounds.width / r.bounds.height;
        if (0.75 < aspect_ratio && aspect_ratio < 1.3 && false) {
            center.x = cvRound((r.bounds.x + r.bounds.width * 0.5));
            center.y = cvRound((r.bounds.y + r.bounds.height * 0.5));
            radius = cvRound((r.bounds.width + r.bounds.height) * 0.75);
            cv::circle(img, center, radius, color, 3, 8, 0);
        } else
            cv::rectangle(img,
                          cv::Point(cvRound(r.bounds.x), cvRound(r.bounds.y)),
                          cv::Point(cvRound((r.bounds.x + r.bounds.width - 1)), cvRound((r.bounds.y + r.bounds.height - 1))),
                          color,
                          3,
                          8,
                          0);

    }
}

PiCam::PiCam(const int &cameraIndex, const int &port) :
        cameraIndex(cameraIndex),
        port(port),
        cap(cameraIndex),
        faceDetector(frameWidth, frameHeight) {
}

void PiCam::startServer() {
    std::cout << "Starting server at port: " << port << std::endl;
    mjpegWriter = new MJPEGWriter(port);
    mjpegWriter->start();
    isServerRunning = true;
    std::cout << "Server is up!" << std::endl;
}

void PiCam::stopServer() {
    std::cout << "Stopping server..." << std::endl;
    if (isServerRunning) {
        mjpegWriter->stop();
        delete mjpegWriter;
        mjpegWriter = nullptr;
        isServerRunning = false;
    }
    std::cout << "Server shut down!" << std::endl;
}


PiCam::~PiCam() {
    stopServer();
}
