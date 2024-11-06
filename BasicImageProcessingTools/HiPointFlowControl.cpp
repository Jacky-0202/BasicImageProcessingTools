#include "HiPointFlowControl.h"

HiPointFlowControl::HiPointFlowControl(QObject* parent)
    : QObject(parent)
{
}

bool HiPointFlowControl::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (fileName.isEmpty()) {
        return false;
    }

    m_originalImage = cv::imread(fileName.toStdString());
    if (m_originalImage.empty()) {
        QMessageBox::warning(nullptr, tr("Open Image"), tr("Failed to load image."));
        return false;
    }

    m_processedImage = m_originalImage.clone();
    isImageLoaded = true;

    QPixmap pixmap = m_imageProcessor.matToQPixmap(m_processedImage);
    emit imageLoaded(pixmap);
    return true;
}

bool HiPointFlowControl::saveImage()
{
    if (m_processedImage.empty()) {
        QMessageBox::warning(nullptr, tr("Save Image"), tr("No processed image to save."));
        return false;
    }

    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("Save Image"), "", tr("PNG Files (*.png);;JPG Files (*.jpg);;BMP Files (*.bmp)"));
    if (fileName.isEmpty()) {
        return false;
    }


    if (!cv::imwrite(fileName.toStdString(), m_processedImage)) {
        QMessageBox::warning(nullptr, tr("Save Image"), tr("Failed to save image."));
        return false;
    }

    QMessageBox::information(nullptr, tr("Save Image"), tr("Image saved successfully."));
    return true;
}

void HiPointFlowControl::processImage(ProcessingAction action, int binarizationThreshold, int cannyLower, int cannyUpper)
{
    if (!isImageLoaded) {
        QMessageBox::warning(nullptr, tr("Process Image"), tr("No image loaded."));
        return;
    }

    switch (action) {
    case ProcessingAction::Grayscale:
        m_processedImage = m_imageProcessor.applyGrayscale(m_originalImage);
        break;

    case ProcessingAction::HSV:
        m_processedImage = m_imageProcessor.applyHSV(m_originalImage);
        break;

    case ProcessingAction::Binarization:
        m_processedImage = m_imageProcessor.applyBinarization(m_originalImage, binarizationThreshold);
        break;

    case ProcessingAction::CannyEdge:
        m_processedImage = m_imageProcessor.applyCanny(m_originalImage, cannyLower, cannyUpper);
        break;

    default:
        m_processedImage = m_originalImage.clone();
        break;
    }

    QPixmap pixmap = m_imageProcessor.matToQPixmap(m_processedImage);
    emit imageProcessed(pixmap);
}
