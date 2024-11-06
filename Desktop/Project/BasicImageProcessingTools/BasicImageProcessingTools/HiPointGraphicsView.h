#pragma once

#include <QGraphicsView>
#include <QWheelEvent>

class HiPointGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit HiPointGraphicsView(QWidget* parent = nullptr);

    void setShowInitialText(bool show);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    qreal scaleFactor = 1.15;
    qreal zoomLevel = 0;
    qreal zoomLevelMin = -10;
    qreal zoomLevelMax = 10;

    bool showInitialText;
};
