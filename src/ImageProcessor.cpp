#include "../header/ImageProcessor.hpp"
#include <thread>

using namespace cv;
using namespace std;

void ImageProcessor::applyGaussianBlur(Mat &img) {
    GaussianBlur(img, img, Size(5, 5), 1.5);
}

void ImageProcessor::applyEdgeDetection(Mat &img) {
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    Canny(gray, img, 100, 200);
}

void ImageProcessor::singleThreadProcessing(Mat &img, int choice) {
    if (choice == 1) {
        applyGaussianBlur(img);
    } else if (choice == 2) {
        applyEdgeDetection(img);
    }
}

void ImageProcessor::multiThreadProcessing(Mat &img, int choice) {
    Mat topHalf = img(Rect(0, 0, img.cols, img.rows / 2));
    Mat bottomHalf = img(Rect(0, img.rows / 2, img.cols, img.rows / 2));

    thread t1((choice == 1) ? applyGaussianBlur : applyEdgeDetection, std::ref(topHalf));
    thread t2((choice == 1) ? applyGaussianBlur : applyEdgeDetection, std::ref(bottomHalf));

    t1.join();
    t2.join();
}
