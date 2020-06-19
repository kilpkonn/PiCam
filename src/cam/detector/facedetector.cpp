//
// Created by tavo on 19.06.20.
//

#include "facedetector.h"

#include <utility>

#define MERGE_OVERLAPPING_AMOUNT 0.6

FaceDetector::FaceDetector(double frameWidth, double frameHeight) :
        frameWidth(frameWidth),
        frameHeight(frameHeight) {
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
        faces.emplace_back(cv::Rect(r.x / fx, r.y / fy, r.width / fx, r.height / fy), true);
    }

    for (const auto &r: flippedProfileFaces) {
        faces.emplace_back(cv::Rect((faceRecognitionFrameWidth - r.x - r.width) / fx, r.y / fy, r.width / fx, r.height / fy), false);
    }

    for (const auto &r : profileFaces) {
        faces.emplace_back(cv::Rect(r.x / fx, r.y / fy, r.width / fx, r.height / fy), false);
    }

    std::vector<cv::Rect> toMerge;
    toMerge.reserve(faces.size());

    std::transform(faces.begin(), faces.end(), std::back_inserter(toMerge), [](const Face& f) { return f.bounds; });

    frameBufferIndexPointer = (frameBufferIndexPointer + 1) % FRAME_BUFFER_LENGTH;
    frameBuffer[frameBufferIndexPointer] = Frame(faces);

    // TODO: Merge rectangles, some more statistics etc.
    mergeOverlapped(faces);

    return faces;
}

std::vector<Face> FaceDetector::mergeOverlapped(std::vector<Face> &faces) {
    int i = 0;
    while (i < faces.size()) {
        for (int j = i; j < faces.size(); j++) {
            if ((faces[i].bounds & faces[j].bounds).area() > std::min(faces[i].bounds.area(), faces[j].bounds.area()) * MERGE_OVERLAPPING_AMOUNT) {
                faces[i].bounds = cv::Rect(
                        std::round((faces[i].bounds.x + faces[j].bounds.x) / 2),
                        std::round((faces[i].bounds.y + faces[j].bounds.y) / 2),
                        std::max(faces[i].bounds.width, faces[j].bounds.width),
                        std::max(faces[i].bounds.height, faces[j].bounds.height)
                        );
                faces.erase(faces.begin() + j);
            }
        }
    }
    return faces;
}

FaceDetector::~FaceDetector() = default;

Face::Face(cv::Rect bounds, const bool isFrontal) : bounds(std::move(bounds)), isFrontal(isFrontal) {}

Frame::Frame(std::vector<Face> faces) : faces(std::move(faces)) {}
