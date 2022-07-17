#pragma once

#include <random>

#include <opencv2/opencv.hpp>

#include "faceDetector.h"

#include "piCam.h"

namespace picam {

class PiCamFancy : public PiCam {
public:
  PiCamFancy(const int &cameraIndex = 0, const int &port = 8080);

  bool run() override;

  void setFrameSize(const int &width, const int &height) override;

  void setBlur(bool blur);

  void setGrayscale(bool grayscale);

  void setDrawRectangle(bool drawRectangle);

private:
  void draw(cv::Mat &img);
  void rotateStand() override;

  bool isBlur = false;
  bool isGrayscale = false;
  bool isDrawRectangle = false;

  std::default_random_engine randGenerator;
  std::uniform_real_distribution<float> randDistribution;

  Vector2D<float> searchDirection;

  cv::VideoCapture cap;

  std::vector<Face> currentFaces;

  FaceDetector faceDetector;
};

} // namespace picam
