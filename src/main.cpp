/**
 * @file main.cpp
 * @brief Main entry point for the JPG to ASCII converter application.
 *
 * This file contains the main function which loads an image, processes it,
 * and displays the original image and its ASCII art representation.
 *
 * Usage:
 * 1. Navigate to the build directory:
 *    @code
 *    cd build
 *    @endcode
 * 2. Generate build files using CMake:
 *    @code
 *    cmake ..
 *    @endcode
 * 3. Build the project:
 *    @code
 *    cmake --build .
 *    @endcode
 * 4. Run the executable:
 *    @code
 *    ./JPG_TO_ASCII
 *    @endcode
 *
 * The program performs the following steps:
 * 1. Loads an image from the specified file path.
 * 2. Checks if the image is loaded successfully.
 * 3. Creates an instance of the IMGProcessor class.
 * 4. Displays the original image.
 * 5. Converts the image to ASCII art and displays it.
 *
 * @note Ensure that the OpenCV library is properly installed and linked.
 * @note The image file path should be correct and the image should exist at the specified location.
 *
 * @author Alyssa DiTroia
 * @date January 2026
 */
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

