#include "../header/Utils.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat Utils::loadImage(const string &filename) {
    Mat img = imread(filename);
    if (img.empty()) {
        cerr << "Error: could not open image file" << endl;
        exit(-1);
    }
    return img;
}

int Utils::getUserChoice() {
    cout << "Choose processing method: \n1 - Gaussian Blur\n2 - Edge Detection\nEnter choice: ";
    int choice;
    cin >> choice;
    if (choice != 1 && choice != 2) {
        cerr << "Invalid choice!" << endl;
        exit(-1);
    }
    return choice;
}

void Utils::displayImage(const string &windowName, const Mat &img) {
    namedWindow(windowName, WINDOW_NORMAL);
    imshow(windowName, img);
    waitKey(0);
}
