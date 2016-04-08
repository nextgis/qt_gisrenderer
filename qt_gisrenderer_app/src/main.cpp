#include <string>
#include <thread>
#include <functional>

#include <QString>
#include <QImage>
#include <QTimer>
#include "RenderThread.h"


void saveImage(
      const QImage& image,
      double        scaleFactor)
{
//    QString name = "E:/testImage-" + QString::number(scaleFactor) + ".png";
    QString name = "testImage-" + QString::number(scaleFactor) + ".png";
    image.save(QString(name));
}


int main(
      int   argc,
      char* argv[])
{
    qDebug() << "start without guiApp";
    RenderThread thread(std::bind(saveImage, std::placeholders::_1, std::placeholders::_2));
    thread.render(0, 0, 500, 300, 1);
    return (0);
}  // main
