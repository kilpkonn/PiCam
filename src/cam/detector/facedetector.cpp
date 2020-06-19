//
// Created by tavo on 19.06.20.
//

#include "facedetector.h"

#include <utility>

FaceDetector::FaceDetector() {
    if (!frontalFaceClassifier.load("./data/haarcascades/haarcascade_frontalface_default.xml")) {
        std::cout << "Unable to load classifier data for frontal face!" << std::endl;
    }
    if (!profileFaceClassifier.load("./data/haarcascades/haarcascade_profileface.xml")) {
        std::cout << "Unable to load classifier data for profile face!" << std::endl;
    }
}

std::vector<Face> FaceDetector::detectFaces() {
    return std::vector<Face>();
}

FaceDetector::~FaceDetector() = default;

Face::Face(cv::Rect bounds, const bool isFrontal) : bounds(std::move(bounds)), isFrontal(isFrontal) {}
