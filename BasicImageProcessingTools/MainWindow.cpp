#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_scene(new QGraphicsScene(this))
    , m_flowControl(new HiPointFlowControl(this))
{
    ui.setupUi(this);
    setupConnections();
    ui.imageDisplay->setScene(m_scene);
    setProcessingButtonsEnabled(false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupConnections()
{
    // signals and slots
    connect(m_flowControl, &HiPointFlowControl::imageLoaded, this, &MainWindow::onImageLoaded);
    connect(m_flowControl, &HiPointFlowControl::imageProcessed, this, &MainWindow::updateImageDisplay);

    connect(ui.loadImageButton, &QPushButton::clicked, this, [this]() {
        if (m_flowControl->loadImage()) {
            setProcessingButtonsEnabled(true);
            }
        });

    connect(ui.saveImageButton, &QPushButton::clicked, this, [this]() {
        m_flowControl->saveImage();
        });

    connect(ui.grayscaleButton, &QPushButton::clicked, this, [this]() {
        m_flowControl->processImage(ProcessingAction::Grayscale, 0, 0, 0);
        });

    connect(ui.hsvButton, &QPushButton::clicked, this, [this]() {
        m_flowControl->processImage(ProcessingAction::HSV, 0, 0, 0);
        });

    connect(ui.binarizationThresholdSlider, &QSlider::valueChanged, this, [this](int value) {
        ui.binarizationThresholdLabel->setText("Values: " + QString::number(value));
        m_flowControl->processImage(ProcessingAction::Binarization, value, 0, 0);
        });

    connect(ui.cannyLowerThresholdSlider, &QSlider::valueChanged, this, [this](int value) {
        ui.cannyLowerThresholdLabel->setText("Lower Value: " + QString::number(value));
        int upper = ui.cannyUpperThresholdSlider->value();
        m_flowControl->processImage(ProcessingAction::CannyEdge, 0, value, upper);
        });

    connect(ui.cannyUpperThresholdSlider, &QSlider::valueChanged, this, [this](int value) {
        ui.cannyUpperThresholdLabel->setText("Upper Value: " + QString::number(value));
        int lower = ui.cannyLowerThresholdSlider->value();
        m_flowControl->processImage(ProcessingAction::CannyEdge, 0, lower, value);
        });
}

void MainWindow::onImageLoaded(const QPixmap& pixmap)
{
    updateImageDisplay(pixmap);
    setProcessingButtonsEnabled(true);
}

void MainWindow::updateImageDisplay(const QPixmap& pixmap)
{
    m_scene->clear();
    m_scene->addPixmap(pixmap);
    ui.imageDisplay->setShowInitialText(false);
}

void MainWindow::setProcessingButtonsEnabled(bool enabled)
{
    ui.grayscaleButton->setEnabled(enabled);
    ui.saveImageButton->setEnabled(enabled);
    ui.hsvButton->setEnabled(enabled);
    ui.binarizationThresholdSlider->setEnabled(enabled);
    ui.cannyUpperThresholdSlider->setEnabled(enabled);
    ui.cannyLowerThresholdSlider->setEnabled(enabled);
}
