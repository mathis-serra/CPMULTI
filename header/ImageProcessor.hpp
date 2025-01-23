#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <opencv2/opencv.hpp>

class ImageProcessor {
public:
    static void applyGaussianBlur(cv::Mat &img);
    static void applyEdgeDetection(cv::Mat &img);
    static void singleThreadProcessing(cv::Mat &img, char choice);
    static void multiThreadProcessing(cv::Mat &img, char choice);
};

#endif
