//
// Created by tavo on 20.06.20.
//
#pragma once

#include <opencv2/opencv.hpp>

#define GAUSSIAN_STEP 10
#define GAUSSIAN_BLUR_RADIUS 21
#define GAUSSIAN_BLUR_SIG_X 11

#define MASK_BLUR_SIZE 7
#define MASK_SIG_X 5


namespace graphics {
    /**
     * Grayscale image background whilst keeping highlights colorful.
     * Uses GaussianBlur for smoother edges, so that definitely has impact on performance.
     *
     * @param img - image to grayscale background, use BGR just in case
     * @param highlights - cv::Rect objects that represent areas, which are kept colorful
     * @param radius - extra radius around rectangles to keep colorful
     * @return new cv::Mat image with grayscale background
     */
    cv::Mat grayscaleBackground(const cv::Mat &img, const std::vector<cv::Rect> &highlights, const int &radius);


    /**
     * Blur background of an image using gaussian blur whilst keeping highlights sharp.
     * Note that gaussian blur might have effect on performance!!
     *
     * @param img - cv::Mat image to blur
     * @param highlights - cv::Rect object that represent highlights, that are kept sharp
     * @param radius - extra radius around rectangles
     * @return new cv::Mat image with grayscale background
     */
    cv::Mat blurBackground(const cv::Mat& img, const std::vector<cv::Rect>& highlights, const int& radius);


    /**
     * Draw rectangles onto image
     *
     * @param img - cv::Mat image to use as base (note that this image will not be modified)
     * @param rectangles - cv::Rect object to draw as rectangles
     * @return new cv::Mat image with rectangles on it
     */
    cv::Mat drawRectangles(const cv::Mat& img, const std::vector<cv::Rect>& rectangles);


    /**
     * Alpha blend 2 cv::Mat images
     *
     * @param img1 - image 1, aka where mask is 255
     * @param img2 - image 2, aka where mask is 0
     * @param mask - cv::Mat grayscale image with weights to use while blending
     * @param blended - output image (result of blending)
     */
    void alphaBlend(const cv::Mat& img1, const cv::Mat& img2, const cv::Mat& mask, cv::Mat& blended);

}
