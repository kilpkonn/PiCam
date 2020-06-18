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
        detectAndDraw(frame);

        if (mjpegWriter != nullptr) {
            std::cout << "Writing frame" << std::endl;
            mjpegWriter->write(frame);
        }
        //frame.release();
    }
    return false;
}

void PiCam::detectAndDraw(Mat &img) {
    std::vector<Rect> faces, faces2;
    cv::Mat gray, smallImg;

    // Convert to grayscale
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Resize
    double fx = faceRecognitionFrameWidth / frameWidth;
    double fy = faceRecognitionFrameHeight / frameHeight;
    cv::resize(gray, smallImg, Size(), fx, fy, cv::INTER_LINEAR);
    cv::equalizeHist(smallImg, smallImg);

    // Detect faces of different sizes using cascade classifier
    faceClassifier.detectMultiScale(
            smallImg,
            faces,
            1.1,
            2,
            (uint) 0 | CASCADE_SCALE_IMAGE,
            Size(30, 30));

    // Draw circles around the faces
    for (const auto &r : faces) {
        cv::Mat smallImgROI;
        std::vector<cv::Rect> nestedObjects;
        cv::Point center;
        cv::Scalar color = cv::Scalar(255, 0, 0); // Color for Drawing tool
        int radius;

        double aspect_ratio = (double) r.width / r.height;
        if (0.75 < aspect_ratio && aspect_ratio < 1.3) {
            center.x = cvRound((r.x + r.width * 0.5) / fx);
            center.y = cvRound((r.y + r.height * 0.5) / fy);
            radius = cvRound((r.width + r.height) * 0.25 / (fx + fy));
            cv::circle(img, center, radius, color, 3, 8, 0);
        } else
            cv::rectangle(img,
                          cv::Point(cvRound(r.x / fx), cvRound(r.y / fy)),
                          cv::Point(cvRound((r.x + r.width - 1) / fx), cvRound((r.y + r.height - 1) / fy)),
                          color,
                          3,
                          8,
                          0);

    }
}

PiCam::PiCam(const int &cameraIndex, const int &port) :
        cameraIndex(cameraIndex),
        port(port),
        cap(cameraIndex) {
    if (!faceClassifier.load("./data/haarcascades/haarcascade_frontalcatface.xml")) {
        std::cout << "Unable to load classifier data!" << std::endl;
    }
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
