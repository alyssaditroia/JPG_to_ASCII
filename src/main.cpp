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
        cout << "Failed to load image!" << endl;
        return -1;
    }

    IMGProcessor proc;

    //int k = 70; // Number of singular values to keep
    //cv::Mat compressedImg;
    //proc.compressImg(img, k, compressedImg);
    //Original image
    cv::imshow("Original Image", img);

    //Black and white compressed imgae using SVD
    //cv::imshow("Compressed Image", compressedImg);
    // Image to ASCII Art
    proc.convertToASCII(img);

    cv::waitKey(0);
    return 0;
}

