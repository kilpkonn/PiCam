//
// Created by tavo on 09.06.20.
//

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "server/MJPEGWriter.h"
#include "detector/facedetector.h"

#ifndef PICAM_PICAM_H
#define PICAM_PICAM_H


class PiCam {
public:
    explicit PiCam(const int& cameraIndex = 0, const int& port = 8080);

    void startServer();

    void stopServer();

    bool run();

    void draw(cv::Mat &img);

    ~PiCam();

private:
    int cameraIndex;
    int port;

    double frameHeight = 720; // 1080;
    double frameWidth = 1280; //1920;

    bool isServerRunning = false;

    cv::VideoCapture cap;

    FaceDetector faceDetector;

    MJPEGWriter* mjpegWriter = nullptr;
};


#endif //PICAM_PICAM_H
