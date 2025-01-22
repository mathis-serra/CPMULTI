#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <string>

class Utils {
public:
    static cv::Mat loadImage(const std::string &filename);
    static int getUserChoice();
    static void displayImage(const std::string &windowName, const cv::Mat &img);
};

#endif
