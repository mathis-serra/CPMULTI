#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cerr << "Hello World!\n";
    cv::Mat image = cv::imread("C:/Users/drvba/OneDrive/Bureau/test_image.png");
    if (image.empty()) {
        std::cout << "Image non trouvée !" << std::endl;
        return -1;
    }
    cv::imshow("Image", image);
    cv::waitKey(0);
    return 0;
}
