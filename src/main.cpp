//
// Created by Tavo Annus on 28/05/2020.
//

#include <iostream>

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    std::cout << "Starting PiCam..." << std::endl;

    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cout << "Unable to connect to camera!" << std::endl;
        return 1;
    }

    cv::Mat frame;
    while (cap.isOpened()) {
        cap >> frame;

        std::vector<uchar> buf;
        cv::imencode(".jpg", frame, buf, std::vector<int>());
        std::string content(buf.begin(), buf.end());

        std::cout << content << std::endl;
    }
    return 0;
}

