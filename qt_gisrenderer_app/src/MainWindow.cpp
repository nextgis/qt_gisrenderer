#include "MainWindow.h"
#include <QtWidgets>

MainWindow::MainWindow()
{
    mMapWidget = new MapWidget();
    mMapWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setCentralWidget(mMapWidget);

//    setMinimumSize(200, 200);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}
