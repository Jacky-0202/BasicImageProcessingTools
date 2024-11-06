#include "HiPointGraphicsView.h"

HiPointGraphicsView::HiPointGraphicsView(QWidget* parent)
    : QGraphicsView(parent)
    , showInitialText(true)
{
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setDragMode(QGraphicsView::ScrollHandDrag);

    setBackgroundBrush(QBrush(Qt::black));
}

void HiPointGraphicsView::setShowInitialText(bool show)
{
    showInitialText = show;
    update();
}

void HiPointGraphicsView::paintEvent(QPaintEvent* event)
{
    QGraphicsView::paintEvent(event);

    if (showInitialText) {
        QPainter painter(viewport());
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Waiting for Image...");
    }
}

void HiPointGraphicsView::wheelEvent(QWheelEvent* event)
{
    qreal factor;
    if (event->angleDelta().y() > 0) {
        if (zoomLevel >= zoomLevelMax) return;
        factor = scaleFactor;
        zoomLevel++;
    }
    else {
        if (zoomLevel <= zoomLevelMin) return;
        factor = 1.0 / scaleFactor;
        zoomLevel--;
    }

    QPointF cursorPos = mapToScene(event->position().toPoint());

    scale(factor, factor);

    QPointF newCursorPos = mapToScene(event->position().toPoint());
    QPointF delta = newCursorPos - cursorPos;
    translate(delta.x(), delta.y());
}