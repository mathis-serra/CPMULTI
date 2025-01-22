#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
using namespace std::chrono;

void applyGaussianBlur(Mat &img) {

   // GaussianBlur(img, img, Size(15, 15), 5);  This operation is very expensive in calcul for the machine 
    GaussianBlur(img, img, Size(5, 5), 1.5); // For optimisation I reduced the cells of the blur to be faster 

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


// MultiThreads could be pricier for your machine 
void multiThreadProcessing(Mat &img, int choice) {
    Mat topHalf = img(Rect(0, 0, img.cols, img.rows / 2));
    Mat bottomHalf = img(Rect(0, img.rows / 2, img.cols, img.rows / 2));

    thread t1((choice == 1) ? applyGaussianBlur : applyEdgeDetection, std::ref(topHalf));
    thread t2((choice == 1) ? applyGaussianBlur : applyEdgeDetection, std::ref(bottomHalf));

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

    // Create a Copie of the image for the Single-thread and the multi-thread
    Mat imgSingleThread = img.clone();  // avoide to load again and again the image and free some memory 
    Mat imgMultiThread = img.clone();  
    

    cout << "Choose processing method: \n1 - Gaussian Blur\n2 - Edge Detection\nEnter choice: ";
    int choice;
    cin >> choice;

    if (choice != 1 && choice != 2) {
        cerr << "Invalid choice!" << endl;
        return -1;
    }

    // Version Sequenciel
    auto start = high_resolution_clock::now();
    singleThreadProcessing(imgSingleThread, choice);
    auto end = high_resolution_clock::now();
    cout << "Sequential execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    namedWindow("Single-thread Processing", WINDOW_NORMAL); 
    imshow("Single-thread Processing", imgSingleThread);
    waitKey(0);

    // Version Multi-Thread
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




