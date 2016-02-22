#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include <QImage>
#include <QEventLoop>

#include "MapPaintSurface.h"

#if defined (QT_BUILD_GISRENDERER_LIB)
  #define Q_GISRENDERER_EXPORT Q_DECL_EXPORT
#else
  #define Q_GISRENDERER_EXPORT Q_DECL_IMPORT
#endif


class Q_GISRENDERER_EXPORT RenderThread
{
    using RenderedImage = std::function <void (const QImage& image, double scaleFactor)>;

  public:
    explicit RenderThread(RenderedImage renderedImage);
    ~RenderThread();

    void render(
          double centerX,
          double centerY,
          int    width,
          int    height,
          double scaleFactor);

  protected:
    void run();

  private:
    int runQGuiApplication(
          int&   argc,
          char** argv);

    std::thread             mThread;
    std::mutex              mutex;
    std::timed_mutex        timedMutex;
    std::condition_variable condition;

    double centerX;
    double centerY;
    int    width;
    int    height;
    double scaleFactor;

    RenderedImage mRenderedImage;

    bool restart;  // TODO: use atomic (?)
    bool abort;  // TODO: use atomic (?)
};


#endif  // RENDERTHREAD_H
