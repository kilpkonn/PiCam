//
// Created by tavo on 09.06.20.
//
#pragma once

#include <opencv2/opencv.hpp>
#include "mjpegWriter.h"
#include "faceDetector.h"


namespace picam {

class PiCam {
public:
    explicit PiCam(const int& cameraIndex = 0, const int& port = 8080);

    void startServer();

    void stopServer();

    bool run();

    void setFrameSize(const int& width, const int& height);

    void setBlur(const bool& blur);

    void setGrayscale(const bool& grayscale);

    virtual ~PiCam();

private:

    void draw(cv::Mat &img);

    int cameraIndex;
    int port;

    bool isBlur = false;
    bool isGrayscale = false;

    double frameHeight = 720;
    double frameWidth = 1280;

    bool isServerRunning = false;

    cv::VideoCapture cap;

    FaceDetector faceDetector;

    MJPEGWriter* mjpegWriter = nullptr;
};

}
