#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;

void applyGaussianBlur(Mat &img) {
    GaussianBlur(img, img, Size(15, 15), 5);
}

void applyEdgeDetection(Mat &img) {
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    Canny(gray, img, 100, 200);
}

void singleThreadProcessing(Mat &img, int choice) {
    if (choice == 1) {
        applyGaussianBlur(img);
    } else if (choice == 2) {
        applyEdgeDetection(img);
    }
}

void multiThreadProcessing(Mat &img, int choice) {
    Mat topHalf = img(Rect(0, 0, img.cols, img.rows / 2));
    Mat bottomHalf = img(Rect(0, img.rows / 2, img.cols, img.rows / 2));

    thread t1((choice == 1) ? applyGaussianBlur : applyEdgeDetection, std::ref(topHalf));
    thread t2((choice == 1) ? applyGaussianBlur : applyEdgeDetection, std::ref(bottomHalf));

    t1.join();
    t2.join();
}

int main() {
    string filename = "totorableu.png";
    Mat img = imread(filename);

    if (img.empty()) {
        cerr << "Error: could not open image file" << endl;
        return -1;
    }

    cout << "Choose processing method: \n1 - Gaussian Blur\n2 - Edge Detection\nEnter choice: ";
    int choice;
    cin >> choice;

    if (choice != 1 && choice != 2) {
        cerr << "Invalid choice!" << endl;
        return -1;
    }

    auto start = high_resolution_clock::now();
    singleThreadProcessing(img, choice);
    auto end = high_resolution_clock::now();
    cout << "Single-thread execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    namedWindow("Single-thread Processing", WINDOW_NORMAL);
    imshow("Single-thread Processing", img);
    waitKey(0);

    img = imread(filename);

    start = high_resolution_clock::now();
    multiThreadProcessing(img, choice);
    end = high_resolution_clock::now();
    cout << "Multi-thread execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    namedWindow("Multi-thread Processing", WINDOW_NORMAL);
    imshow("Multi-thread Processing", img);
    waitKey(0);

    destroyAllWindows();
    return 0;
}




// /Users/mathisserra/Downloads/totoro blanc.png