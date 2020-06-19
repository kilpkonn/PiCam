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


class FaceDetector {
public:

    std::vector<Face> detectFaces();

    FaceDetector();

    ~FaceDetector();

private:
    cv::CascadeClassifier profileFaceClassifier;
    cv::CascadeClassifier frontalFaceClassifier;
    Face frameBuffer[FRAME_BUFFER_LENGTH];
};


#endif //PICAM_FACEDETECTOR_H
