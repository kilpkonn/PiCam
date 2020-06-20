//
// Created by tavo on 19.06.20.
//

#include <opencv2/opencv.hpp>

#define FRAME_BUFFER_LENGTH 7
#define MERGE_OVERLAPPING_AMOUNT 0.4

#ifndef PICAM_FACEDETECTOR_H
#define PICAM_FACEDETECTOR_H

struct Face {
    cv::Rect bounds;
    bool isFrontal;

    Face(cv::Rect bounds, bool isFrontal);
};

struct Frame {
    std::vector<Face> faces;

    Frame() = default;
    explicit Frame(std::vector<Face> faces);
};


class FaceDetector {
public:

    std::vector<Face> detectFaces(const cv::Mat& frame);

    std::vector<Face> predictFaces();

    FaceDetector(double frameWidth, double frameHeight);

    ~FaceDetector();

private:

    static std::vector<Face> mergeOverlapped(std::vector<Face>& faces);

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
