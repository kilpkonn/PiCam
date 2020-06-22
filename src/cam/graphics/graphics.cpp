//
// Created by tavo on 20.06.20.
//

#include "graphics.h"

cv::Mat Graphics::grayscaleBackground(const cv::Mat &img, const std::vector<cv::Rect>& highlights, const int& radius) {
    cv::Mat grayscale;
    cv::Mat mask = cv::Mat::zeros(img.rows / GAUSSIAN_STEP, img.cols / GAUSSIAN_STEP, CV_8UC1);
    cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayscale, grayscale, cv::COLOR_GRAY2BGR);

    for (const cv::Rect& rect : highlights) {
        cv::rectangle(
                mask,
                cv::Point( (rect.x - radius) / GAUSSIAN_STEP , (rect.y - radius) / GAUSSIAN_STEP),
                cv::Point((rect.x + rect.width + radius) / GAUSSIAN_STEP - 1 , (rect.y + rect.height + radius) / GAUSSIAN_STEP - 1),
                cv::Scalar(255, 255, 255),
                cv::FILLED,
                cv::LINE_8
                );
    }

    cv::GaussianBlur(mask, mask, cv::Size(MASK_BLUR_SIZE, MASK_BLUR_SIZE), MASK_SIG_X);
    cv::resize(mask, mask, cv::Size(img.cols, img.rows)); // <- OpenCV fucked up x and y?
    cv::Mat blended;
    alphaBlend(img, grayscale, mask, blended);

    return blended;
}

cv::Mat Graphics::blurBackground(const cv::Mat &img, const std::vector<cv::Rect> &highlights, const int &radius) {
    cv::Mat blurred;
    cv::GaussianBlur(img, blurred, cv::Size(GAUSSIAN_BLUR_RADIUS, GAUSSIAN_BLUR_RADIUS), GAUSSIAN_BLUR_SIG_X);
    cv::Mat mask = cv::Mat::zeros(img.rows / GAUSSIAN_STEP, img.cols / GAUSSIAN_STEP, CV_8UC1);

    for (const cv::Rect& rect : highlights) {
        cv::rectangle(
                mask,
                cv::Point( (rect.x - radius) / GAUSSIAN_STEP , (rect.y - radius) / GAUSSIAN_STEP),
                cv::Point((rect.x + rect.width + radius) / GAUSSIAN_STEP - 1 , (rect.y + rect.height + radius) / GAUSSIAN_STEP - 1),
                cv::Scalar(255, 255, 255),
                cv::FILLED,
                cv::LINE_8
        );
    }

    cv::GaussianBlur(mask, mask, cv::Size(7, 7), 5);
    cv::resize(mask, mask, cv::Size(img.cols, img.rows)); // <- OpenCV fucked up x and y?
    cv::Mat blended;
    alphaBlend(img, blurred, mask, blended);

    return blended;
}

void Graphics::alphaBlend(const cv::Mat &img1, const cv::Mat &img2, const cv::Mat &mask, cv::Mat &blended) {
    blended = cv::Mat(img1.rows, img1.cols, img1.type());
    for (int y = 0; y < blended.rows; y++) {
        for (int x = 0; x < blended.cols; x++) {
            float alpha = (float) mask.at<unsigned char>(y, x) / 255.0f;
            blended.at<cv::Vec3b>(y, x) = alpha * img1.at<cv::Vec3b>(y, x) + (1 - alpha) * img2.at<cv::Vec3b>(y, x);
        }
    }
}

cv::Mat Graphics::drawRectangles(const cv::Mat &img, const std::vector<cv::Rect> &rectangles) {
    cv::Mat result = img.clone();
    for (const cv::Rect &r : rectangles) {
        cv::Scalar color = cv::Scalar(0, 0, 255); // Color for Drawing tool
        cv::rectangle(result,
                      cv::Point(cvRound(r.x), cvRound(r.y)),
                      cv::Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
                      color,
                      3,
                      cv::LINE_8,
                      0);

    }
    return result;
}
