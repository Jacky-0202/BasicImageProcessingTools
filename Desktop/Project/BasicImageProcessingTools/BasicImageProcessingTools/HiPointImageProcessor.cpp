#include "HiPointImageProcessor.h"

cv::Mat HiPointImageProcessor::applyGrayscale(const cv::Mat& image) {
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    return grayImage;
}

cv::Mat HiPointImageProcessor::applyBinarization(const cv::Mat& image, int threshold) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    }
    else {
        grayImage = image;
    }

    cv::Mat binaryImage;
    cv::threshold(grayImage, binaryImage, threshold, 255, cv::THRESH_BINARY);
    return binaryImage;
}

cv::Mat HiPointImageProcessor::applyHSV(const cv::Mat& image) {
    cv::Mat colorImage;
    if (image.channels() == 1) {
        cv::cvtColor(image, colorImage, cv::COLOR_GRAY2BGR);
    }
    else {
        colorImage = image;
    }

    cv::Mat hsvImage;
    cv::cvtColor(colorImage, hsvImage, cv::COLOR_BGR2HSV);
    return hsvImage;
}

cv::Mat HiPointImageProcessor::applyCanny(const cv::Mat& image, int lowThreshold, int highThreshold) {
    cv::Mat grayImage;
    if (image.channels() == 3) {
        cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
    }
    else {
        grayImage = image;
    }

    cv::Mat cannyImage;
    cv::Canny(grayImage, cannyImage, lowThreshold, highThreshold);
    return cannyImage;
}

QPixmap HiPointImageProcessor::matToQPixmap(const cv::Mat& mat) {
    QImage::Format format;

    if (mat.type() == CV_8UC1) {
        format = QImage::Format_Grayscale8;
    }
    else if (mat.type() == CV_8UC3) {
        format = QImage::Format_RGB888;
    }
    else {
        throw std::invalid_argument("Unsupported cv::Mat format");
    }

    QImage image(mat.data, mat.cols, mat.rows, mat.step, format);

    if (mat.type() == CV_8UC3) {
        image = image.rgbSwapped();
    }

    return QPixmap::fromImage(image.copy());
}
