#include "RenderThread.h"


RenderThread::RenderThread(QObject* parent)
    : QThread(parent)
{
    restart = false;
    abort = false;
    mPaintSurface = new MapPaintSurface();
}


RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}


void RenderThread::render(
      double centerX,
      double centerY,
      double scaleFactor,
      QSize  resultSize)
{
    QMutexLocker locker(&mutex);

    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->resultSize = resultSize;

    if (!isRunning()) {
        mPaintSurface->doneCurrent();
        mPaintSurface->moveToThread(this);
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}  // RenderThread::render


void RenderThread::run()
{
    forever {
        mutex.lock();
        QSize resultSize = this->resultSize;
        double scaleFactor = this->scaleFactor;
        double centerX = this->centerX;
        double centerY = this->centerY;
        bool aborted = abort;
        mutex.unlock();

        if (aborted) {
            break;
        }

        mPaintSurface->resize(resultSize.width(), resultSize.height());
        mPaintSurface->render();
        QImage image = mPaintSurface->grabFramebuffer();
        if (!restart) {
            emit renderedImage(image, scaleFactor);
        }

        mutex.lock();
        if (!restart) {
            condition.wait(&mutex);
        }
        restart = false;
        mutex.unlock();
    }
}  // RenderThread::run
