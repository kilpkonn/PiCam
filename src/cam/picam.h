//
// Created by tavo on 09.06.20.
//

#include <opencv2/opencv.hpp>
#include "server/MJPEGWriter.h"

#ifndef PICAM_PICAM_H
#define PICAM_PICAM_H


class PiCam {
public:
    explicit PiCam(const int& cameraIndex = 0, const int& port = 8080);

    bool run();

    virtual ~PiCam();

private:
    int cameraIndex;
    int port;
    cv::VideoCapture cap;
    MJPEGWriter mjpegWriter;
};


#endif //PICAM_PICAM_H
