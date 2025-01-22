#include "../header/Application.hpp"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace cv;

void Application::run() {
    string filename = "/Users/mathisserra/Desktop/Github/B2_Laplateforme/CPMULTI/assets/totorableu.png";
    Mat img = Utils::loadImage(filename);

    Mat imgSingleThread = img.clone();
    Mat imgMultiThread = img.clone();

    int choice = Utils::getUserChoice();

    auto start = high_resolution_clock::now();
    ImageProcessor::singleThreadProcessing(imgSingleThread, choice);
    auto end = high_resolution_clock::now();
    cout << "Sequential execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
    Utils::displayImage("Single-thread Processing", imgSingleThread);

    start = high_resolution_clock::now();
    ImageProcessor::multiThreadProcessing(imgMultiThread, choice);
    end = high_resolution_clock::now();
    cout << "Multi-thread execution time: " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;
    Utils::displayImage("Multi-thread Processing", imgMultiThread);

    destroyAllWindows();
}
