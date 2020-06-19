//
// Created by tavo on 19.06.20.
//

#include <opencv2/opencv.hpp>

#define FRAME_BUFFER_LENGTH 5

#ifndef PICAM_FACEDETECTOR_H
#define PICAM_FACEDETECTOR_H

struct Face {
    cv::Rect bounds;
    bool isFrontal;

    Face(cv::Rect bounds, bool isFrontal);
};

struct Frame {
    std::vector<Face> faces;

    Frame(std::vector<Face> faces);
};


class FaceDetector {
public:

    std::vector<Face> detectFaces(const cv::Mat& frame);

    FaceDetector(double frameHeight, double frameWidth);

    ~FaceDetector();

private:
    cv::CascadeClassifier profileFaceClassifier;
    cv::CascadeClassifier frontalFaceClassifier;
    Frame frameBuffer[FRAME_BUFFER_LENGTH];

    uint frameBufferIndexPointer = 0;

    int faceRecognitionFrameHeight = 54 * 2;
    int faceRecognitionFrameWidth = 96 * 2;

    double frameHeight = 720;
    double frameWidth = 1280;
};


#endif //PICAM_FACEDETECTOR_H
