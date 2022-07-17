#pragma once

#include <cmath>

#include "mjpegWriter.h"
#include "servoStand.h"
#include "vec2.h"

namespace picam {

class PiCam {
public:
  PiCam(const int &cameraIndex = 0, const int &port = 8080);

  void startServer();

  void stopServer();

  virtual bool run();

  virtual void setFrameSize(const int &width, const int &height);

  virtual ~PiCam();

protected:
  virtual void rotateStand();

  int cameraIndex;
  int port;

  float frameHeight = 720;
  float frameWidth = 1280;

  float minXServo = -M_PI_2f32 / 6;
  float maxXServo = M_PI_2f32 / 6;
  float minYServo = -0.0;
  float maxYServo = M_PI_2f32;
  float speedXMultiplier = 0.00006f;
  float speedYMultiplier = 0.00005f;

  bool isServerRunning = false;

  Vector2D<float> currentStandLocation;

  cv::VideoCapture cap;

  ServoStand servoStand;

  MJPEGWriter *mjpegWriter = nullptr;
};

} // namespace picam
