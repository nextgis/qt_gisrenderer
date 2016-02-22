#include "RenderThread.h"

#include <QGuiApplication>
#include <QTimer>


RenderThread::RenderThread(RenderedImage renderedImage)
    : mRenderedImage(renderedImage)
{
    restart = false;
    abort = false;
}


RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.notify_one();
    mutex.unlock();

    if (mThread.joinable()) {
        qDebug() << "mThread.join()";
        mThread.join();
    }
}


void RenderThread::render(
      double centerX,
      double centerY,
      int    width,
      int    height,
      double scaleFactor)
{
    std::lock_guard <std::mutex> locker(mutex);

    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->width = width;
    this->height = height;

    qDebug() << "render()";

    if (!mThread.joinable()) {
        qDebug() << "running thread";
        std::thread thread(&RenderThread::run, this);
        mThread = std::move(thread);
    }
    else {
        restart = true;
        condition.notify_one();
    }
}  // RenderThread::render


void RenderThread::run()
{
    qDebug() << "guiApp starting";
    std::string     test = "test";
    int             argc = 1;
    char*           argv[] = { const_cast <char*>(test.c_str()) };
    QGuiApplication a(argc, argv);
    qDebug() << "guiApp started";

/*
    QEventLoop loop;
    QTimer     timer;
    timer.setSingleShot(true);
    loop.connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(1000);  // your predefined timeout
    loop.exec();
    qDebug() << "QEventLoop exited in thread";
*/

    MapPaintSurface paintSurface;

    double scaleFactor = 1;

    for (int i = 0; i < 10; ++i) {
        mutex.lock();
        int width = this->width;
        int height = this->height;
//        double scaleFactor = this->scaleFactor;
        double        centerX = this->centerX;
        double        centerY = this->centerY;
        RenderedImage renderedImage = this->mRenderedImage;
        mutex.unlock();

        width += scaleFactor * 10;
        qDebug() << "width: " << width << ", height: " << height;
        paintSurface.resize(width, height);
        paintSurface.render();
        QImage image = paintSurface.grabFramebuffer();
        renderedImage(image, scaleFactor);

        ++scaleFactor;
    }
}  // RenderThread::run
