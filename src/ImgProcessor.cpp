#include "ImgProcessor.h"

void IMGProcessor::convertToASCII(const cv::Mat &image) {
    const string grayScaleChars = " .:-=+*#%@";
    //" .'`^\",:;I!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdkbhao*#MW&8%B@$";

    //Image to GrayScale
    cv::Mat gray;

    if(image.channels() == 1){
        gray = image;
    }else{
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }

    // Pixel dimension is scale x scale
    int scale = 4;

    // Font scale and thickness
    double fontScale = max(1.0, 100.0 / gray.cols); // Adjust font scale based on image width
    int fontThickness = 1;

    // ASCII image dimensions
    int charWidth = 4 * fontScale;
    int charHeight = 5 * fontScale;
    int asciiWidth = gray.cols / scale * charWidth;
    int asciiHeight = gray.rows / scale * charHeight;

    // Blank image for ASCII art
    cv::Mat asciiImage(asciiHeight, asciiWidth, CV_8UC3, cv::Scalar(0, 0, 0));

    // ASCII art generation
    for (int i = 0; i < gray.rows; i += scale) {
        for (int j = 0; j < gray.cols; j += scale) {
            // Ave pixel intensity for the block
            int pixelValue = calcAverage(gray, i, j, scale);
            // Intensity is mapped to a character
            char asciiChar = grayScaleChars[ceil((grayScaleChars.length() - 1) * pixelValue / 255)];

            // Character gets rendered on the blank image canvas
            string text(1, asciiChar);
            int x = (j / scale) * charWidth;
            int y = (i / scale + 1) * charHeight;
            cv::putText(asciiImage, text, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar(255, 255, 255), fontThickness);
        }
    }

    // Displays the ASCII image
    cv::imshow("ASCII Art", asciiImage);
}

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

void IMGProcessor::compressImg(const cv::Mat &image, int k, cv::Mat &compressedImage) {
    if (image.empty()) {
        cout << "Image is empty or could not be loaded!" << endl;
        return;
    }

    // Convert to grayscale and floating-point
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


