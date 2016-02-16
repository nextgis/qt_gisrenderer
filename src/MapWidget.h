#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include "MapPaintSurface.h"


class MapWidget
    : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
    void scroll(
            int deltaX,
            int deltaY);

    MapPaintSurface* mPaintSurface;
};


#endif  // MAPWIDGET_H
