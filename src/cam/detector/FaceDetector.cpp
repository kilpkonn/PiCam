//
// Created by tavo on 19.06.20.
//

#include "../../../include/FaceDetector.h"

#include <utility>

FaceDetector::FaceDetector(const double &frameWidth, const double &frameHeight) :
        frameWidth(frameWidth),
        frameHeight(frameHeight) {
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
        faces.emplace_back(
                cv::Rect((faceRecognitionFrameWidth - r.x - r.width) / fx, r.y / fy, r.width / fx, r.height / fy),
                false);
    }

    for (const auto &r : profileFaces) {
        faces.emplace_back(cv::Rect(r.x / fx, r.y / fy, r.width / fx, r.height / fy), false);
    }

    mergeOverlapped(faces);

    frameBufferIndexPointer = (frameBufferIndexPointer + 1) % FRAME_BUFFER_LENGTH;
    frameBuffer[frameBufferIndexPointer] = Frame(faces);

    return faces;
}

std::vector<Face> FaceDetector::mergeOverlapped(std::vector<Face> &faces) {
    for (int i = 0; i < faces.size(); i++) {
        for (int j = i + 1; j < faces.size(); j++) {
            if ((faces[i].bounds & faces[j].bounds).area() >
                std::min(faces[i].bounds.area(), faces[j].bounds.area()) * MERGE_OVERLAPPING_AMOUNT) {
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

std::vector<Face> FaceDetector::predictFaces() {
    std::vector<Face> predictedFaces;

    for (uint i = frameBufferIndexPointer + 1; i < frameBufferIndexPointer + FRAME_BUFFER_LENGTH; i++) {
        const Frame &frame = frameBuffer[i % FRAME_BUFFER_LENGTH];

        for (const auto &face : frame.faces) {
            bool isMerged = false;
            for (auto &predictedFace : predictedFaces) {
                if ((predictedFace.bounds & face.bounds).area() >
                    std::min(predictedFace.bounds.area(), face.bounds.area()) *
                    MERGE_OVERLAPPING_AMOUNT) { // Weighted towards never

                    predictedFace.velocity += Vector2D(
                            (face.bounds.x + face.bounds.width / 2) -
                            (predictedFace.bounds.x + predictedFace.bounds.width / 2),
                            (face.bounds.y + face.bounds.height / 2) -
                            (predictedFace.bounds.y + predictedFace.bounds.height / 2)
                    );
                    predictedFace.sizeIncreaseVelocity += Vector2D(
                            face.bounds.width - predictedFace.bounds.width,
                            face.bounds.height - predictedFace.bounds.height
                    );

                    predictedFace.bounds = cv::Rect(
                            std::round((predictedFace.bounds.x + predictedFace.velocity.x + face.bounds.x) / 2),
                            std::round((predictedFace.bounds.y + predictedFace.velocity.y + face.bounds.y) / 2),
                            (predictedFace.bounds.width + predictedFace.sizeIncreaseVelocity.x + face.bounds.width) / 2,
                            (predictedFace.bounds.height + predictedFace.sizeIncreaseVelocity.y + face.bounds.height) /
                            2
                    );
                    isMerged = true;
                }
            }
            if (!isMerged) {
                predictedFaces.push_back(face);
            }
        }

    }

    return predictedFaces;
}

bool FaceDetector::loadClassifiers() {
    if (!frontalFaceClassifier.load("./data/haarcascades/haarcascade_frontalface_default.xml")) {
        std::cout << "Unable to load classifier data for frontal face!" << std::endl;
        return false;
    }
    if (!profileFaceClassifier.load("./data/haarcascades/haarcascade_profileface.xml")) {
        std::cout << "Unable to load classifier data for profile face!" << std::endl;
        return false;
    }
    return true;
}

void FaceDetector::setFrameSize(const int &width, const int &height) {
    this->frameWidth = width;
    this->frameHeight = height;
}

FaceDetector::~FaceDetector() = default;

Face::Face(cv::Rect bounds, const bool &isFrontal) :
        bounds(std::move(bounds)),
        isFrontal(isFrontal),
        velocity(0, 0),
        sizeIncreaseVelocity(0, 0) {

}

Frame::Frame(std::vector<Face> faces) : faces(std::move(faces)) {}
