//
// Created by tavo on 20.06.20.
//

#include "graphics.h"

cv::Mat Graphics::grayscaleBackground(const cv::Mat &img, const std::vector<cv::Rect>& highlights) {
    cv::Mat grayscale;
    cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayscale, grayscale, cv::COLOR_GRAY2BGR);

    std::cout << highlights.size() << std::endl;

    for (const cv::Rect& rect : highlights) {
        cv::rectangle(
                mask,
                cv::Point(rect.x, rect.y),
                cv::Point(rect.x + rect.width - 1, rect.y + rect.height - 1),
                cv::Scalar(255, 255, 255),
                cv::FILLED,
                cv::LINE_8
                );
    }

    cv::GaussianBlur(mask, mask, cv::Size(21, 21), 11);
    cv::Mat blended;
    alphaBlend(img, grayscale, 255 - mask, blended);

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

