//
// Created by tavo on 19.06.20.
//

#include "facedetector.h"

#include <utility>

FaceDetector::FaceDetector(double frameHeight, double frameWidth) :
        frameHeight(frameHeight),
        frameWidth(frameWidth) {
    if (!frontalFaceClassifier.load("./data/haarcascades/haarcascade_frontalface_default.xml")) {
        std::cout << "Unable to load classifier data for frontal face!" << std::endl;
    }
    if (!profileFaceClassifier.load("./data/haarcascades/haarcascade_profileface.xml")) {
        std::cout << "Unable to load classifier data for profile face!" << std::endl;
    }
}

std::vector<Face> FaceDetector::detectFaces(const cv::Mat &frame) {
    std::vector<Face> faces;

    std::vector<cv::Rect> frontalFaces, profileFaces, flippedProfileFaces;
    cv::Mat gray, smallImg, flippedSmallImg;

    // Convert to grayscale
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Resize
    double fx = faceRecognitionFrameWidth / frameWidth;
    double fy = faceRecognitionFrameHeight / frameHeight;
    cv::resize(gray, smallImg, cv::Size(), fx, fy, cv::INTER_LINEAR);
    cv::equalizeHist(smallImg, smallImg);
    cv::flip(smallImg, flippedSmallImg, 1);

    // Detect faces of different sizes using cascade classifier
    frontalFaceClassifier.detectMultiScale(
            smallImg,
            frontalFaces,
            1.1,
            2,
            (uint) 0 | cv::CASCADE_SCALE_IMAGE,
            cv::Size(10, 10)
    );

    profileFaceClassifier.detectMultiScale(
            smallImg,
            profileFaces,
            1.1,
            2,
            (uint) 0 | cv::CASCADE_SCALE_IMAGE,
            cv::Size(10, 10)
    );

    profileFaceClassifier.detectMultiScale(
            flippedSmallImg,
            flippedProfileFaces,
            1.1,
            2, (uint) 0 | cv::CASCADE_SCALE_IMAGE,
            cv::Size(10, 10)
    );

    for (const auto &r : frontalFaces) {
        faces.emplace_back(r, true);
    }

    for (const auto &r: flippedProfileFaces) {
        faces.emplace_back(cv::Rect(faceRecognitionFrameWidth - r.x - r.width, r.y, r.width, r.height), false);
    }

    for (const auto &r : profileFaces) {
        faces.emplace_back(r, false);
    }

    frameBufferIndexPointer = (frameBufferIndexPointer + 1) % FRAME_BUFFER_LENGTH;
    frameBuffer[frameBufferIndexPointer] = Frame(faces);

    // TODO: Merge rectangles, some more statistics etc.

    return faces;
}

FaceDetector::~FaceDetector() = default;

Face::Face(cv::Rect bounds, const bool isFrontal) : bounds(std::move(bounds)), isFrontal(isFrontal) {}

Frame::Frame(std::vector<Face> faces) : faces(std::move(faces)) {}
