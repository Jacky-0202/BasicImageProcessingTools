#pragma once

#include <QObject>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include "HiPointImageProcessor.h"

enum class ProcessingAction {
    Grayscale,
    HSV,
    Binarization,
    CannyEdge
};

class HiPointFlowControl : public QObject
{
    Q_OBJECT

public:
    explicit HiPointFlowControl(QObject* parent = nullptr);

    bool loadImage();
    bool saveImage();
    void processImage(ProcessingAction action, int binarizationThreshold, int cannyLower, int cannyUpper);

signals:
    void imageLoaded(const QPixmap& pixmap);
    void imageProcessed(const QPixmap& pixmap);

private:
    HiPointImageProcessor m_imageProcessor;
    cv::Mat m_originalImage;
    cv::Mat m_processedImage;
    bool isImageLoaded = false;
};
