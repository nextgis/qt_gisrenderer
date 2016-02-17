#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>

#include "MapPaintSurface.h"

#if defined (QT_BUILD_GISRENDERER_LIB)
  #define Q_GISRENDERER_EXPORT Q_DECL_EXPORT
#else
  #define Q_GISRENDERER_EXPORT Q_DECL_IMPORT
#endif


class Q_GISRENDERER_EXPORT RenderThread
    : public QThread
{
    Q_OBJECT

  public:
    explicit RenderThread(QObject* parent = 0);
    ~RenderThread();

    void render(
          double centerX,
          double centerY,
          double scaleFactor,
          QSize  resultSize);

  signals:
    void renderedImage(
          const QImage& image,
          double        scaleFactor);

  protected:
    void run() Q_DECL_OVERRIDE;

  private:
    MapPaintSurface* mPaintSurface;

    QMutex mutex;
    QWaitCondition condition;

    double centerX;
    double centerY;
    double scaleFactor;
    QSize resultSize;

    bool restart;
    bool abort;
};


#endif  // RENDERTHREAD_H
