#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace cv;
using namespace std::chrono;

mutex mtx;
condition_variable cv;
bool topHalfProcessed = false;
bool bottomHalfProcessed = false;

void applyGaussianBlur(Mat &img) {
    GaussianBlur(img, img, Size(5, 5), 1.5); // Optimized blur
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

// Function to handle the processing of each half of the image with synchronization
void processTopHalf(Mat &topHalf, int choice) {
    if (choice == 1) {
        applyGaussianBlur(topHalf);
    } else if (choice == 2) {
        applyEdgeDetection(topHalf);
    }
    // Notify that the top half is processed
    {
        lock_guard<mutex> lock(mtx);
        topHalfProcessed = true;
    }
    cv.notify_all();
}

void processBottomHalf(Mat &bottomHalf, int choice) {
    if (choice == 1) {
        applyGaussianBlur(bottomHalf);
    } else if (choice == 2) {
        applyEdgeDetection(bottomHalf);
    }
    // Notify that the bottom half is processed
    {
        lock_guard<mutex> lock(mtx);
        bottomHalfProcessed = true;
    }
    cv.notify_all();
}

void multiThreadProcessing(Mat &img, int choice) {
    Mat topHalf = img(Rect(0, 0, img.cols, img.rows / 2));
    Mat bottomHalf = img(Rect(0, img.rows / 2, img.cols, img.rows / 2));

    thread t1(processTopHalf, std::ref(topHalf), choice);
    thread t2(processBottomHalf, std::ref(bottomHalf), choice);

    // Wait for both halves to be processed
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []{ return topHalfProcessed && bottomHalfProcessed; });
    }

    // Combine both halves back into the original image
    topHalf.copyTo(img(Rect(0, 0, img.cols, img.rows / 2)));
    bottomHalf.copyTo(img(Rect(0, img.rows / 2, img.cols, img.rows / 2)));

    // Join the threads after processing
    t1.join();
    t2.join();
}

int main() {
    string filename = "/Users/mathisserra/Desktop/Github/B2_Laplateforme/CPMULTI/assets/totorableu.png";
    Mat img = imread(filename);

    if (img.empty()) {
        cerr << "Error: could not open image file" << endl;
        return -1;
    }

    // Create a copy of the image for the Single-thread and the Multi-thread
    Mat imgSingleThread = img.clone();
    Mat imgMultiThread = img.clone();

    cout << "Choose processing method: \n1 - Gaussian Blur\n2 - Edge Detection\nEnter choice: ";
    int choice;
    cin >> choice;

    if (choice != 1 && choice != 2) {
        cerr << "Invalid choice!" << endl;
        return -1;
    }

    // Version Sequential (Single-threaded)
    auto start = high_resolution_clock::now();
    singleThreadProcessing(imgSingleThread, choice);
    auto end = high_resolution_clock::now();
    cout << "Sequential execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    namedWindow("Single-thread Processing", WINDOW_NORMAL);
    imshow("Single-thread Processing", imgSingleThread);
    waitKey(0);

    // Version Multi-threaded
    start = high_resolution_clock::now();
    multiThreadProcessing(imgMultiThread, choice);
    end = high_resolution_clock::now();
    cout << "Multi-thread execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    namedWindow("Multi-thread Processing", WINDOW_NORMAL);
    imshow("Multi-thread Processing", imgMultiThread);
    waitKey(0);

    destroyAllWindows();
    return 0;
}
