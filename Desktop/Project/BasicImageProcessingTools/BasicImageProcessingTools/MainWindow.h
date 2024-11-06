#pragma once

#include "ui_MainWindow.h"
#include "HiPointFlowControl.h"
#include "HiPointImageProcessor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onImageLoaded(const QPixmap& pixmap);
    void updateImageDisplay(const QPixmap& pixmap);

private:
    Ui::ImgProcessingUI ui;

    QGraphicsScene* m_scene;
    HiPointFlowControl* m_flowControl;

    void setupConnections();
    void setProcessingButtonsEnabled(bool enabled);
};