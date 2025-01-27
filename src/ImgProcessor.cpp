/**
 * @file ImgProcessor.cpp
 * @brief Implementation of image processing functions for converting images to ASCII art and compressing images using SVD.
 */

#include "ImgProcessor.h"

/**
 * @brief Converts a given image to ASCII art and displays it.
 * 
 * This function takes an input image, converts it to grayscale if necessary, and then generates an ASCII art representation
 * of the image. The ASCII art is displayed in a window and also printed to the console.
 * 
 * @param image The input image to be converted to ASCII art.
 */
void IMGProcessor::convertToASCII(const cv::Mat &image) {
    const string grayScaleChars = " `.-':_,=+!*s#$%@";
    cv::Mat gray;
    ostringstream asciiArt;

    if(image.channels() == 1){
        gray = image;
    } else {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }

    int scale = 5; // Block size for original image

    // fontScale to make characters fit scale x scale
    double fontScale = 1.0;
    int fontThickness = 1;
    cv::Size textSize;
    int baseline;

    // Measure the size of a sample char
    textSize = cv::getTextSize("W", cv::FONT_HERSHEY_SIMPLEX, fontScale, fontThickness, &baseline);
    int currentCharWidth = textSize.width;
    cout << "Current Char Width:" << currentCharWidth << "\n";
    int currentCharHeight = textSize.height + baseline;
    cout << "Current Char Height:" << currentCharHeight << "\n";

    // Avoid dividing by zero
    if (currentCharWidth == 0) currentCharWidth = 1;
    if (currentCharHeight == 0) currentCharHeight = 1;

    // Compute fontScale to fit scale x scale
    double widthRatio = static_cast<double>(scale) / currentCharWidth;
    cout << "width Ratio:" << widthRatio << "\n";
    double heightRatio = static_cast<double>(scale) / currentCharHeight;
    cout << "height Ratio:" << heightRatio << "\n";
    fontScale *= std::min(widthRatio, heightRatio);
    cout << "Font Scale: " << fontScale << "\n";

    // Re-measure with adjusted fontScale
    textSize = cv::getTextSize("W", cv::FONT_HERSHEY_SIMPLEX, fontScale, fontThickness, &baseline);
    int charWidth = textSize.width;
    cout << "New Char Width:" << currentCharWidth << "\n";
    int charHeight = textSize.height + baseline;
    cout << "New Char Height:" << currentCharHeight << "\n";

    // ASCII image dimensions
    int asciiCols = (gray.cols + scale - 1) / scale;
    int asciiRows = (gray.rows + scale - 1) / scale;

    // Create canvas
    cv::Mat asciiImage(asciiRows * charHeight, asciiCols * charWidth, CV_8UC3, cv::Scalar(0, 0, 0));

    // cout << "ascii Cols: " << asciiCols << "\n";
    // cout << "Gray.cols: " << gray.cols << "\n";
    // cout << "ascii Rows: " << asciiRows << "\n";
    // cout << "Gray.rows: " << gray.rows << "\n";

    // ASCII art generation
    for (int i = 0; i < (gray.rows); i += scale) {
        for (int j = 0; j < (gray.cols); j += scale) {
            int pixelValue = calcAverage(gray, i, j, scale);
            char asciiChar = grayScaleChars[ceil((grayScaleChars.length() - 1) * pixelValue / 255)];
            asciiArt << asciiChar;
            string text(1, asciiChar);
            int x = (j / scale) * charWidth;
            int y = (i / scale + 1) * charHeight;
            cv::putText(asciiImage, text, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, 
                        fontScale, cv::Scalar(255, 255, 255), fontThickness);
        }
        asciiArt << "\n";
    }

    // Resize ASCII image to match original dimensions
    if (asciiImage.cols != gray.cols || asciiImage.rows != gray.rows) {
        cv::resize(asciiImage, asciiImage, cv::Size(gray.cols, gray.rows), 0, 0, cv::INTER_NEAREST);
    }

    cv::imshow("ASCII Art", asciiImage);
    cout << asciiArt.str() << endl;
}
/**
 * @brief Calculates the average pixel value in a block of the image.
 * 
 * This helper function computes the average pixel value within a specified block of the image. It is used to determine
 * the corresponding ASCII character for that block.
 * 
 * @param image The input image.
 * @param row The starting row of the block.
 * @param col The starting column of the block.
 * @param scale The size of the block.
 * @return The average pixel value of the block.
 */
int IMGProcessor::calcAverage(const cv::Mat &image, int row, int col, int scale) {
    int sum = 0, pixelCount = 0;
    for (int r = row; r < row + scale && r < image.rows; ++r) {
        for (int c = col; c < col + scale && c < image.cols; ++c) {
            sum += static_cast<int>(image.at<uchar>(r, c));
            pixelCount++;
        }
    }
    return pixelCount > 0 ? sum / pixelCount : 0;
}
/**
 * @brief Compresses a given image using Singular Value Decomposition (SVD).
 * 
 * This function compresses the input image by retaining only the top-k singular values from its SVD decomposition.
 * The compressed image is reconstructed and normalized for display.
 * 
 * @param image The input image to be compressed.
 * @param k The number of singular values to retain.
 * @param compressedImage The output compressed image.
 */
void IMGProcessor::compressImg(const cv::Mat &image, int k, cv::Mat &compressedImage) {
    if (image.empty()) {
        cout << "Image is empty or could not be loaded!" << endl;
        return;
    }

    // Convert to grayscale and float
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat grayFloat;
    gray.convertTo(grayFloat, CV_32F);

    // Perform SVD
    cv::Mat U, S, Vt;
    cv::SVDecomp(grayFloat, S, U, Vt);

    // Retain top-k singular values
    cv::Mat S_k = cv::Mat::zeros(U.cols, Vt.rows, CV_32F);
    for (int i = 0; i < k; i++) {
        S_k.at<float>(i, i) = S.at<float>(i, 0);
    }

    // Reconstruct the compressed image
    cv::Mat compressedFloat = U * S_k * Vt;

    // Normalize and convert to 8-bit for display
    cv::normalize(compressedFloat, compressedFloat, 0, 255, cv::NORM_MINMAX);
    compressedFloat.convertTo(compressedImage, CV_8U);
}


