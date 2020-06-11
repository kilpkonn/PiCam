//
// Created by tavo on 09.06.20.
//

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "server/MJPEGWriter.h"

#ifndef PICAM_PICAM_H
#define PICAM_PICAM_H


class PiCam {
public:
    explicit PiCam(const int& cameraIndex = 0, const int& port = 8080);

    bool startServer();

    bool stopServer();

    bool run();

    void detectAndDraw(cv::Mat &img);

    virtual ~PiCam();

private:
    int cameraIndex;
    int port;
    double frameHeight = 720; // 1080;
    double frameWidth = 1280; //1920;
    int faceRecognitionFrameHeight = 54;
    int faceRecognitionFrameWidth = 96;

    bool isServerRunning = false;

    cv::VideoCapture cap;
    MJPEGWriter* mjpegWriter = nullptr;
    cv::CascadeClassifier faceClassifier;
};


#endif //PICAM_PICAM_H
