#include "graphics.h"

#include "piCam.h"

picam::PiCam::PiCam(const int &cameraIndex, const int &port) :
        cameraIndex(cameraIndex),
        port(port),
        randDistribution(-0.01f, 0.01f),
        currentStandLocation((minXServo + maxXServo) / 2, (minYServo + maxYServo) / 2),
        searchDirection(randDistribution(randGenerator), randDistribution(randGenerator)),
        cap(cameraIndex),
        faceDetector(frameWidth, frameHeight),
        servoStand(17, 18) {
}

bool picam::PiCam::run() {
    faceDetector.loadClassifiers();

    cv::Mat frame;
    if (!cap.isOpened()) {
        std::cout << "Unable to connect to camera!" << std::endl;
        return false;
    } else {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
    }

    while (cap.isOpened()) {
        cap >> frame;

        //cv::cvtColor(frame, frame, COLOR_BGR2RGB);
        faceDetector.detectFaces(frame);
        draw(frame);
        rotateStand();

        if (mjpegWriter != nullptr) {
            mjpegWriter->write(frame);
        }
    }
    return false;
}

void picam::PiCam::draw(Mat &img) {
    currentFaces = faceDetector.predictFaces();

    std::vector<Rect> highlights;
    highlights.reserve(currentFaces.size());
    std::transform(currentFaces.begin(), currentFaces.end(), std::back_inserter(highlights), [](const Face &f) { return f.bounds; });
    if (isBlur) {
        img = graphics::blurBackground(img, highlights, 70);
    }

    if (isGrayscale) {
        img = graphics::grayscaleBackground(img, highlights, 50);
    }

    // img = graphics::drawRectangles(img, highlights);
}

void picam::PiCam::startServer() {
    std::cout << "Starting server at port: " << port << std::endl;
    mjpegWriter = new MJPEGWriter(port);
    mjpegWriter->start();
    isServerRunning = true;
    std::cout << "Server is up!" << std::endl;
}

void picam::PiCam::stopServer() {
    std::cout << "Stopping server..." << std::endl;
    if (isServerRunning) {
        mjpegWriter->stop();
        delete mjpegWriter;
        mjpegWriter = nullptr;
        isServerRunning = false;
    }
    std::cout << "Server shut down!" << std::endl;
}


picam::PiCam::~PiCam() {
    stopServer();
}

void picam::PiCam::setBlur(const bool &blur) {
    this->isBlur = blur;
}

void picam::PiCam::setGrayscale(const bool &grayscale) {
    this->isGrayscale = grayscale;
}

void picam::PiCam::setFrameSize(const int &width, const int &height) {
    frameWidth = width;
    frameHeight = height;
    faceDetector.setFrameSize(width, height);

    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
    }
}

void picam::PiCam::rotateStand() {
    float avgX = 0.0f;
    float avgY = 0.0f;
    for (const auto &face: currentFaces) {
        avgX += face.bounds.x + static_cast<float>(face.bounds.width) / 2;
        avgY += face.bounds.y + static_cast<float>(face.bounds.height) / 2;
    }
    avgX /= currentFaces.size();
    avgY /= currentFaces.size();

    if (!currentFaces.empty()) {
        currentStandLocation += Vector2D((avgX - frameWidth / 2.0f) * speedXMultiplier,
                                         (avgY - frameHeight / 2.0f) * speedYMultiplier);
    } else {
        currentStandLocation += searchDirection;
        if (currentStandLocation.x < minXServo || currentStandLocation.x > maxXServo ||
            currentStandLocation.y < minYServo || currentStandLocation.y > maxYServo) {
            searchDirection = Vector2D(randDistribution(randGenerator), randDistribution(randGenerator));
        }
    }

    currentStandLocation.x = std::clamp(currentStandLocation.x, minXServo, maxXServo);
    currentStandLocation.y = std::clamp(currentStandLocation.y, minYServo, maxYServo);

    servoStand.rotate(-currentStandLocation.x, currentStandLocation.y);
}
