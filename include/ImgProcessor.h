#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <string>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <vector>
using namespace std;

class IMGProcessor {
    public:
        void convertToASCII(const cv::Mat &image);
        int calcAverage(const cv::Mat &image, int row, int col, int scale);
        void compressImg(const cv::Mat &image, int k, cv::Mat &compressedImage);

};

#endif // IMGPROCESSOR_H