#pragma once

#include <random>

#include <opencv2/opencv.hpp>

#include "mjpegWriter.h"
#include "faceDetector.h"
#include "servoStand.h"


namespace picam {

class PiCam {
public:
    explicit PiCam(const int& cameraIndex = 0, const int& port = 8080);

    void startServer();

    void stopServer();

    bool run();

    void setFrameSize(const int& width, const int& height);

    void setBlur(const bool& blur);

    void setGrayscale(const bool& grayscale);

    virtual ~PiCam();

private:

    void draw(cv::Mat &img);

    void rotateStand();

    int cameraIndex;
    int port;

    bool isBlur = false;
    bool isGrayscale = false;

    float frameHeight = 720;
    float frameWidth = 1280;

    float minXServo = -M_PI_2f32 / 6;
    float maxXServo = M_PI_2f32 / 6;
    float minYServo = -0.2;
    float maxYServo = M_PI_2f32 / 6;
    float speedXMultiplier = 0.00006f;
    float speedYMultiplier = 0.00005f;

    bool isServerRunning = false;

    std::default_random_engine randGenerator;
    std::uniform_real_distribution<float> randDistribution;

    Vector2D<float> currentStandLocation;
    Vector2D<float> searchDirection;

    cv::VideoCapture cap;

    std::vector<Face> currentFaces;

    FaceDetector faceDetector;
    ServoStand servoStand;

    MJPEGWriter* mjpegWriter = nullptr;
};

}
