#ifndef MAPPAINTSURFACE_H
#define MAPPAINTSURFACE_H

#include <QImage>
#include "OpenGlOffscreenSurface.h"


class MapPaintSurface
    : public OpenGlOffscreenSurface
{
    Q_OBJECT

public:
    explicit MapPaintSurface(
            QScreen* targetScreen = nullptr,
            const QSize& size = QSize (1, 1));

    virtual ~MapPaintSurface() override;

protected:
    virtual void initializeGL() override;

    virtual void resizeGL(
            int width,
            int height) override;

    virtual void paintGL() override;

private:
    QImage m_image;
};


#endif  // MAPPAINTSURFACE_H
