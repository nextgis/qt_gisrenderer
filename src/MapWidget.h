#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include "RenderThread.h"


class MapWidget
    : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private slots:
    void updatePixmap(
            const QImage& image,
            double        scaleFactor);

    void zoom(double zoomFactor);

private:
    void scroll(
            int deltaX,
            int deltaY);

    RenderThread thread;
    QPixmap pixmap;
    QPoint pixmapOffset;
    QPoint lastDragPos;
    double centerX;
    double centerY;
    double pixmapScale;
    double curScale;
};


#endif  // MAPWIDGET_H
