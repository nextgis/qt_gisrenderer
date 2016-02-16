#include "MapWidget.h"

#include <QPainter>


MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent)
{
    mPaintSurface = new MapPaintSurface();
}


void MapWidget::paintEvent(QPaintEvent* event)
{
    QRect rect = event->rect();
    mPaintSurface->resize(rect.width(), rect.height());
    mPaintSurface->render();
    QImage image = mPaintSurface->grabFramebuffer();

    QPainter painter(this);
    painter.drawImage(0, 0, image);
}  // MapWidget::paintEvent
