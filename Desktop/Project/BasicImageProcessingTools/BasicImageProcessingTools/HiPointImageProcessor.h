#pragma once

#include <opencv2/opencv.hpp>
#include <QGraphicsPixmapItem>

class HiPointImageProcessor
{
public:
    cv::Mat applyGrayscale(const cv::Mat& image);
    cv::Mat applyBinarization(const cv::Mat& image, int threshold);
    cv::Mat applyHSV(const cv::Mat& image);
    cv::Mat applyCanny(const cv::Mat& image, int lowThreshold, int highThreshold);

    QPixmap matToQPixmap(const cv::Mat& mat);
};

