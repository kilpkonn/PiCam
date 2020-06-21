//
// Created by tavo on 20.06.20.
//

#include <opencv2/opencv.hpp>

#ifndef PICAM_GRAPHICS_H
#define PICAM_GRAPHICS_H


class Graphics {
public:
    static cv::Mat grayscaleBackground(const cv::Mat& img, const std::vector<cv::Rect>& highlights, const int& radius);
    static void alphaBlend(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& mask, cv::Mat& blended);

};


#endif //PICAM_GRAPHICS_H
