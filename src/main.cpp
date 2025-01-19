#include "ImgProcessor.h"

/*
 * To Run:
 *
 * cd build
 * cmake ..
 * cmake --build .
 * ./JPG_TO_ASCII
 * 
 */

int main() {
    cv::Mat img = cv::imread("resources/zippy.jpg");
    if (img.empty()) {
        std::cerr << "Failed to load image!" << std::endl;
        return -1;
    }

    std::cout << "Original image dimensions: " << img.cols << " x " << img.rows << std::endl;

    IMGProcessor proc;

    // SVD Compression
    int k = 70; // Number of singular values to keep
    cv::Mat compressedImg;
    proc.compressImg(img, k, compressedImg);
    std::cout << "Compressed image dimensions: " << compressedImg.cols << " x " << compressedImg.rows << std::endl;

    cv::imshow("Original Image", img);
    cv::imshow("Compressed Image", compressedImg);
    // Image to ASCII Art
    proc.convertToASCII(compressedImg);

    cv::waitKey(0);
    return 0;
}

