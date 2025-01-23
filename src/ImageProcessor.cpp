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

void ImageProcessor::singleThreadProcessing(Mat &img, char choice) {
    if (choice == 'b') {
        applyGaussianBlur(img);
    } else if (choice == 'e') {
        applyEdgeDetection(img);
    }
}

void ImageProcessor::multiThreadProcessing(Mat &img, char choice) {
    Mat topHalf = img(Rect(0, 0, img.cols, img.rows / 2)).clone();
    Mat bottomHalf = img(Rect(0, img.rows / 2, img.cols, img.rows / 2)).clone();

    thread t1((choice == 'b') ? applyGaussianBlur : applyEdgeDetection, std::ref(topHalf));
    thread t2((choice == 'b') ? applyGaussianBlur : applyEdgeDetection, std::ref(bottomHalf));

    t1.join();
    t2.join();

    // Vérifier si l'image originale est en couleur et reconvertir si nécessaire
    if (img.channels() == 3 && topHalf.channels() == 1) {
        cvtColor(topHalf, topHalf, COLOR_GRAY2BGR);
        cvtColor(bottomHalf, bottomHalf, COLOR_GRAY2BGR);
    }

    topHalf.copyTo(img(Rect(0, 0, img.cols, img.rows / 2)));
    bottomHalf.copyTo(img(Rect(0, img.rows / 2, img.cols, img.rows / 2)));

}
