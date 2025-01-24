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

void applyRotation(Mat &img) {
    float rows = img.rows;
    float cols = img.cols;
    Point2f center((cols - 1) / 2.0f, (rows - 1) / 2.0f);

    Mat M;
    M = cv::getRotationMatrix2D(center, 90, 1);
    Mat dst;
    cv::warpAffine(img,dst,M,Size(cols,rows));
    dst.copyTo(img);
}

void singleThreadProcessing(Mat &img, int choice) {
    if (choice == 1) {
        applyGaussianBlur(img);
    } else if (choice == 2) {
        applyEdgeDetection(img);
    } else if (choice == 3) {
        applyRotation(img);
    }
}


// MultiThreads could be pricier for your machine 
void multiThreadProcessing(Mat &img, int choice) {
    Mat topHalf = img(Rect(0, 0, img.cols, img.rows / 2));
    Mat bottomHalf = img(Rect(0, img.rows / 2, img.cols, img.rows / 2));

    thread t1;
    thread t2;

    if (choice == 1) {
        t1 = thread(applyGaussianBlur, std::ref(topHalf));
        t2 = thread(applyGaussianBlur, std::ref(bottomHalf));
    } else if (choice == 2) {
        t1 = thread(applyEdgeDetection, std::ref(topHalf));
        t2 = thread(applyEdgeDetection, std::ref(bottomHalf));
    } else if (choice == 3) {
        t1 = thread(applyRotation, std::ref(topHalf));
        t2 = thread(applyRotation, std::ref(bottomHalf));
    }
    t1.join();
    t2.join();
}

void showIntroImage(const string &introImagePath) {
    Mat introImage = imread(introImagePath);
    if (introImage.empty()) {
        cerr << "Error: could not open intro image file." << endl;
        return;
    }

    namedWindow("Introduction", WINDOW_NORMAL);
    cv::setWindowProperty("Introduction", WND_PROP_TOPMOST, 1);

    while (true) {
        imshow("Introduction", introImage);
        if (waitKey(10) == 27) { // ferme la fenêtre si on appuie sur échap
            break;
        }
    }
}

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);


    string introImagePath = "C:/Users/drvba/OneDrive/Bureau/infoIntro.png";
    thread introThread(showIntroImage, introImagePath);

    string filename = "C:/Users/drvba/OneDrive/Bureau/test_image.png";
    Mat img = imread(filename);
    if (img.empty()) {
        cerr << "Error: could not open image file." << endl;
        return -1;
    }

    Mat imgSingleThread = img.clone();
    Mat imgMultiThread = img.clone();

    cout << "Choose processing method: \n1 - Gaussian Blur\n2 - Edge Detection\n3 - Rotation\nEnter choice: ";
    int choice;
    cin >> choice;

    if (choice != 1 && choice != 2 && choice != 3) {
        cerr << "Invalid choice!" << endl;
        return -1;
    }

    // Traitement séquentiel
    auto start = high_resolution_clock::now();
    singleThreadProcessing(imgSingleThread, choice);
    auto end = high_resolution_clock::now();
    cout << "Sequential execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    namedWindow("Single-thread Processing", WINDOW_NORMAL);
    imshow("Single-thread Processing", imgSingleThread);
    cv::setWindowProperty("Single-thread Processing", WND_PROP_TOPMOST, 1);

    // Traitement multi-thread
    start = high_resolution_clock::now();
    multiThreadProcessing(imgMultiThread, choice);
    end = high_resolution_clock::now();
    cout << "Multi-thread execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    namedWindow("Multi-thread Processing", WINDOW_NORMAL);
    imshow("Multi-thread Processing", imgMultiThread);
    cv::setWindowProperty("Multi-thread Processing", WND_PROP_TOPMOST, 1);

    waitKey(0);


    introThread.detach();
    destroyAllWindows();
    return 0;
}




