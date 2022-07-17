//
// Created by tavo on 19.06.20.
//
#pragma once

#include <opencv2/opencv.hpp>

#include "vec2.h"

#define FRAME_BUFFER_LENGTH 14
#define MERGE_OVERLAPPING_AMOUNT 0.3

namespace picam {

    struct Face {
        cv::Rect bounds;
        bool isFrontal;
        Vector2D<int> velocity;
        Vector2D<int> sizeIncreaseVelocity;

        Face(cv::Rect bounds, const bool &isFrontal);
    };

    struct Frame {
        std::vector<Face> faces;

        Frame() = default;

        explicit Frame(std::vector<Face> faces);
    };


    class FaceDetector {
    public:

        std::vector<Face> detectFaces(const cv::Mat &frame);

        std::vector<Face> predictFaces();

        bool loadClassifiers();

        void setFrameSize(const int &width, const int &height);

        FaceDetector(const double &frameWidth, const double &frameHeight);

        virtual ~FaceDetector();

    private:

        static std::vector<Face> mergeOverlapped(std::vector<Face> &faces);

        cv::CascadeClassifier profileFaceClassifier;
        cv::CascadeClassifier frontalFaceClassifier;
        Frame frameBuffer[FRAME_BUFFER_LENGTH];

        uint frameBufferIndexPointer = 0;

        int faceRecognitionFrameHeight = 54 * 2;
        int faceRecognitionFrameWidth = 96 * 2;

        double frameHeight = 720;
        double frameWidth = 1280;
    };

}
