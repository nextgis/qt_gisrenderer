#include "MapPaintSurface.h"

#include <QPainter>

MapPaintSurface::MapPaintSurface(
        QScreen*     targetScreen,
        const QSize& size)
    : OpenGlOffscreenSurface(targetScreen, size)
{
//    m_image = QImage("E:/path/to/image/proba.png");
    m_image = QImage("2.png");
}


MapPaintSurface::~MapPaintSurface() {}


void MapPaintSurface::initializeGL() {}


void MapPaintSurface::resizeGL(
        int width,
        int height)            {}


void MapPaintSurface::paintGL()
{
    QPainter painter(getPaintDevice());
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.drawImage(0, 0, m_image);
    painter.drawImage(bufferSize().width() - m_image.width(),
            bufferSize().height() - m_image.height(), m_image);

    painter.end();
}  // MapPaintSurface::paintGL
